// Copyright 2026 ArcMod. Cookies, passwords and extensions from a Chromium-family profile (see header).
#include "arc/api/arc_chromium_import.h"

#import <Foundation/Foundation.h>
#import <Security/Security.h>

#include <algorithm>
#include <array>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "base/apple/bridging.h"
#include "base/apple/foundation_util.h"
#include "base/apple/scoped_cftyperef.h"
#include "base/barrier_callback.h"
#include "base/command_line.h"
#include "base/compiler_specific.h"
#include "base/containers/span.h"
#include "base/strings/string_view_util.h"
#include "base/files/file_util.h"
#include "base/functional/bind.h"
#include "base/functional/callback.h"
#include "base/memory/raw_ptr.h"
#include "base/strings/strcat.h"
#include "base/strings/string_util.h"
#include "base/strings/sys_string_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "base/time/time.h"
#include "base/version.h"
#include "base/uuid.h"
#include "chrome/browser/autofill/personal_data_manager_factory.h"
#include "chrome/browser/extensions/updater/extension_updater.h"
#include "components/autofill/core/browser/data_manager/payments/payments_data_manager.h"
#include "components/autofill/core/browser/data_manager/personal_data_manager.h"
#include "components/autofill/core/browser/field_types.h"
#include "chrome/browser/profiles/profile.h"
#include "components/crx_file/id_util.h"
#include "components/password_manager/core/browser/password_string.h"
#include "content/public/browser/storage_partition.h"
#include "crypto/aes_cbc.h"
#include "crypto/hash.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/pending_extension_manager.h"
#include "extensions/common/extension.h"
#include "extensions/common/extension_urls.h"
#include "net/cookies/canonical_cookie.h"
#include "net/cookies/cookie_access_result.h"
#include "net/cookies/cookie_constants.h"
#include "net/cookies/cookie_options.h"
#include "net/cookies/cookie_partition_key.h"
#include "net/cookies/cookie_util.h"
#include "services/network/public/mojom/cookie_manager.mojom.h"
#include "third_party/boringssl/src/include/openssl/evp.h"
#include "third_party/sqlite/sqlite3.h"
#include "url/gurl.h"
#include "url/scheme_host_port.h"

namespace arc {
namespace {

// Test-only: the source profile's keychain password, so scenarios never touch the real keychain.
constexpr char kImportTestKeychainPasswordSwitch[] = "arcmod-import-test-keychain-password";

// os_crypt's Mac scheme: "v10" + AES-128-CBC(PBKDF2-HMAC-SHA1(password, "saltysalt", 1003), IV of spaces).
constexpr char kEncryptionPrefix[] = "v10";
using Key = std::array<uint8_t, 16>;

base::Time ChromeTime(int64_t microseconds) {
  return microseconds ? base::Time::FromDeltaSinceWindowsEpoch(base::Microseconds(microseconds)) : base::Time();
}

std::optional<std::string> KeychainPassword(const std::string& service) {
  const base::CommandLine& command_line = *base::CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(kImportTestKeychainPasswordSwitch)) {
    return command_line.GetSwitchValueASCII(kImportTestKeychainPasswordSwitch);
  }
  // Any account: browsers name it after themselves ("Chrome", "Microsoft Edge", ...).
  NSDictionary* query = @{
    (__bridge id)kSecClass : (__bridge id)kSecClassGenericPassword,
    (__bridge id)kSecAttrService : base::SysUTF8ToNSString(service),
    (__bridge id)kSecMatchLimit : (__bridge id)kSecMatchLimitOne,
    (__bridge id)kSecReturnData : @YES,
  };
  CFTypeRef result = nullptr;
  if (SecItemCopyMatching((__bridge CFDictionaryRef)query, &result) != errSecSuccess || !result) {
    return std::nullopt;
  }
  base::apple::ScopedCFTypeRef<CFTypeRef> owned(result);
  CFDataRef data = base::apple::CFCast<CFDataRef>(result);
  if (!data) {
    return std::nullopt;
  }
  return std::string(base::as_string_view(base::apple::CFDataToSpan(data)));
}

std::optional<Key> DeriveKey(const std::string& password) {
  static constexpr char kSalt[] = "saltysalt";
  Key key;
  if (!PKCS5_PBKDF2_HMAC_SHA1(password.data(), password.size(), reinterpret_cast<const uint8_t*>(kSalt),
                              sizeof(kSalt) - 1, 1003, key.size(), key.data())) {
    return std::nullopt;
  }
  return key;
}

// Decrypts one "v10" OSCrypt value.
std::optional<std::string> Decrypt(const Key& key, base::span<const uint8_t> value) {
  const size_t prefix = sizeof(kEncryptionPrefix) - 1;
  if (value.size() <= prefix || base::as_string_view(value.first(prefix)) != kEncryptionPrefix) {
    return std::nullopt;
  }
  std::array<uint8_t, crypto::aes_cbc::kBlockSize> iv;
  iv.fill(' ');
  std::optional<std::vector<uint8_t>> plain = crypto::aes_cbc::Decrypt(key, iv, value.subspan(prefix));
  if (!plain) {
    return std::nullopt;
  }
  return std::string(plain->begin(), plain->end());
}

base::span<const uint8_t> Blob(sqlite3_stmt* statement, int column) {
  const void* bytes = sqlite3_column_blob(statement, column);
  const int size = sqlite3_column_bytes(statement, column);
  // SAFETY: sqlite3_column_bytes() is the size of the buffer sqlite3_column_blob() returned.
  return bytes && size > 0
             ? UNSAFE_BUFFERS(base::span(static_cast<const uint8_t*>(bytes), static_cast<size_t>(size)))
             : base::span<const uint8_t>();
}

std::string Text(sqlite3_stmt* statement, int column) {
  const unsigned char* text = sqlite3_column_text(statement, column);
  return text ? std::string(reinterpret_cast<const char*>(text)) : std::string();
}

// The source browser may hold the live database open: read a private copy (with its WAL, if any).
class DatabaseCopy {
 public:
  explicit DatabaseCopy(const base::FilePath& source) {
    if (!base::CreateNewTempDirectory("arc-import", &dir_)) {
      return;
    }
    const base::FilePath copy = dir_.Append(source.BaseName());
    if (!base::CopyFile(source, copy)) {
      return;
    }
    base::CopyFile(base::FilePath(source.value() + "-wal"), base::FilePath(copy.value() + "-wal"));
    sqlite3* opened = nullptr;
    if (sqlite3_open_v2(copy.value().c_str(), &opened, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX, nullptr) ==
        SQLITE_OK) {
      db_ = opened;
    } else if (opened) {
      sqlite3_close(opened);
    }
  }
  ~DatabaseCopy() {
    if (db_) {
      sqlite3_close(db_);
    }
    if (!dir_.empty()) {
      base::DeletePathRecursively(dir_);
    }
  }
  sqlite3* get() const { return db_; }

  std::set<std::string> Columns(const char* table) const {
    std::set<std::string> columns;
    sqlite3_stmt* statement = nullptr;
    const std::string sql = base::StrCat({"PRAGMA table_info(", table, ")"});
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK) {
      while (sqlite3_step(statement) == SQLITE_ROW) {
        columns.insert(Text(statement, 1));
      }
    }
    sqlite3_finalize(statement);
    return columns;
  }

  int MetaVersion() const {
    int version = 0;
    sqlite3_stmt* statement = nullptr;
    if (sqlite3_prepare_v2(db_, "SELECT value FROM meta WHERE key = 'version'", -1, &statement, nullptr) ==
            SQLITE_OK &&
        sqlite3_step(statement) == SQLITE_ROW) {
      version = sqlite3_column_int(statement, 0);
    }
    sqlite3_finalize(statement);
    return version;
  }

 private:
  base::FilePath dir_;
  raw_ptr<sqlite3> db_ = nullptr;
};

// Selects |wanted| columns, substituting |fallback| SQL for ones this schema version doesn't have.
std::string SelectList(const std::set<std::string>& columns,
                       std::initializer_list<std::pair<const char*, const char*>> wanted) {
  std::string list;
  for (const auto& [name, fallback] : wanted) {
    if (!list.empty()) {
      list += ", ";
    }
    list += columns.contains(name) ? name : fallback;
  }
  return list;
}

bool ReadCookies(const base::FilePath& profile_dir, const Key& key, std::vector<net::CanonicalCookie>* out) {
  base::FilePath path = profile_dir.Append("Network").Append("Cookies");
  if (!base::PathExists(path)) {
    path = profile_dir.Append("Cookies");
  }
  DatabaseCopy db(path);
  if (!db.get()) {
    return false;
  }
  const std::set<std::string> columns = db.Columns("cookies");
  if (!columns.contains("host_key") || !columns.contains("encrypted_value")) {
    return false;
  }
  // Since version 24 the encrypted value starts with SHA-256(host_key).
  const bool hashed_domain = db.MetaVersion() >= 24;
  const std::string sql = base::StrCat(
      {"SELECT ",
       SelectList(columns, {{"creation_utc", "0"},
                            {"host_key", "''"},
                            {"top_frame_site_key", "''"},
                            {"name", "''"},
                            {"value", "''"},
                            {"encrypted_value", "''"},
                            {"path", "'/'"},
                            {"expires_utc", "0"},
                            {"is_secure", "0"},
                            {"is_httponly", "0"},
                            {"last_access_utc", "0"},
                            {"has_expires", "1"},
                            {"is_persistent", "1"},
                            {"priority", "1"},
                            {"samesite", "-1"},
                            {"source_scheme", "0"},
                            {"source_port", "-1"},
                            {"last_update_utc", "0"},
                            {"source_type", "0"},
                            {"has_cross_site_ancestor", "0"}}),
       " FROM cookies"});
  sqlite3_stmt* statement = nullptr;
  if (sqlite3_prepare_v2(db.get(), sql.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
    return false;
  }
  const base::Time now = base::Time::Now();
  while (sqlite3_step(statement) == SQLITE_ROW) {
    const std::string domain = Text(statement, 1);
    std::string value = Text(statement, 4);
    const base::span<const uint8_t> encrypted = Blob(statement, 5);
    if (!encrypted.empty()) {
      std::optional<std::string> plain = Decrypt(key, encrypted);
      if (!plain) {
        continue;
      }
      if (hashed_domain) {
        const std::array<uint8_t, crypto::hash::kSha256Size> hash = crypto::hash::Sha256(domain);
        if (plain->size() < hash.size() || base::as_byte_span(*plain).first(hash.size()) != base::span(hash)) {
          continue;
        }
        plain->erase(0, hash.size());
      }
      value = std::move(*plain);
    }
    const base::Time expiration = ChromeTime(sqlite3_column_int64(statement, 7));
    const bool persistent = sqlite3_column_int(statement, 12) != 0;
    if (persistent && !expiration.is_null() && expiration < now) {
      continue;
    }
    std::optional<net::CookiePartitionKey> partition_key;
    if (const std::string top_frame_site = Text(statement, 2); !top_frame_site.empty()) {
      auto parsed = net::CookiePartitionKey::FromStorage(top_frame_site, sqlite3_column_int(statement, 19) != 0);
      if (!parsed.has_value()) {
        continue;
      }
      partition_key = std::move(parsed.value());
    }
    const int same_site = sqlite3_column_int(statement, 14);
    const int priority = sqlite3_column_int(statement, 13);
    const int source_scheme = sqlite3_column_int(statement, 15);
    const int source_type = sqlite3_column_int(statement, 18);
    std::unique_ptr<net::CanonicalCookie> cookie = net::CanonicalCookie::FromStorage(
        Text(statement, 3), std::move(value), domain, Text(statement, 6), ChromeTime(sqlite3_column_int64(statement, 0)),
        persistent ? expiration : base::Time(), ChromeTime(sqlite3_column_int64(statement, 10)),
        ChromeTime(sqlite3_column_int64(statement, 17)), sqlite3_column_int(statement, 8) != 0,
        sqlite3_column_int(statement, 9) != 0,
        same_site >= -1 && same_site <= 2 ? static_cast<net::CookieSameSite>(same_site)
                                          : net::CookieSameSite::UNSPECIFIED,
        priority >= 0 && priority <= 2 ? static_cast<net::CookiePriority>(priority) : net::COOKIE_PRIORITY_DEFAULT,
        std::move(partition_key),
        source_scheme >= 0 && source_scheme <= 2 ? static_cast<net::CookieSourceScheme>(source_scheme)
                                                 : net::CookieSourceScheme::kUnset,
        sqlite3_column_int(statement, 16),
        // Stored values: 0 unknown (read as other, like Chromium's cookie store), 1 http, 2 script, 3 other.
        source_type == 1 || source_type == 2 ? static_cast<net::CookieSourceType>(source_type)
                                             : net::CookieSourceType::kOther,
        net::CanonicalCookieFromStorageCallSite::kCookieManager);
    if (cookie) {
      out->push_back(std::move(*cookie));
    }
  }
  sqlite3_finalize(statement);
  return true;
}

// Like Arc's engine, only the autofillable logins (not the "never save" entries).
bool ReadPasswords(const base::FilePath& profile_dir,
                   const Key& key,
                   std::vector<password_manager::PasswordForm>* out) {
  DatabaseCopy db(profile_dir.Append("Login Data"));
  if (!db.get()) {
    return false;
  }
  const std::set<std::string> columns = db.Columns("logins");
  if (!columns.contains("origin_url") || !columns.contains("password_value")) {
    return false;
  }
  const std::string sql = base::StrCat(
      {"SELECT ",
       SelectList(columns, {{"origin_url", "''"},
                            {"action_url", "''"},
                            {"username_element", "''"},
                            {"username_value", "''"},
                            {"password_element", "''"},
                            {"password_value", "''"},
                            {"signon_realm", "''"},
                            {"date_created", "0"},
                            {"scheme", "0"},
                            {"times_used", "0"},
                            {"date_last_used", "0"},
                            {"date_password_modified", "0"},
                            {"display_name", "''"},
                            {"federation_url", "''"}}),
       " FROM logins WHERE blacklisted_by_user = 0"});
  sqlite3_stmt* statement = nullptr;
  if (sqlite3_prepare_v2(db.get(), sql.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
    return false;
  }
  while (sqlite3_step(statement) == SQLITE_ROW) {
    std::optional<std::string> password = Decrypt(key, Blob(statement, 5));
    const GURL url(Text(statement, 0));
    const std::string signon_realm = Text(statement, 6);
    if (!password || password->empty() || !url.is_valid() || signon_realm.empty()) {
      continue;
    }
    password_manager::PasswordForm form;
    form.url = url;
    form.action = GURL(Text(statement, 1));
    form.username_element = base::UTF8ToUTF16(Text(statement, 2));
    form.username_value = base::UTF8ToUTF16(Text(statement, 3));
    form.password_element = base::UTF8ToUTF16(Text(statement, 4));
    form.password_value = password_manager::PasswordString(base::UTF8ToUTF16(*password));
    form.signon_realm = signon_realm;
    form.date_created = ChromeTime(sqlite3_column_int64(statement, 7));
    const int scheme = sqlite3_column_int(statement, 8);
    form.scheme = scheme >= 0 && scheme <= 4 ? static_cast<password_manager::PasswordForm::Scheme>(scheme)
                                             : password_manager::PasswordForm::Scheme::kHtml;
    form.times_used_in_html_form = sqlite3_column_int(statement, 9);
    form.date_last_used = ChromeTime(sqlite3_column_int64(statement, 10));
    form.date_password_modified = ChromeTime(sqlite3_column_int64(statement, 11));
    form.display_name = base::UTF8ToUTF16(Text(statement, 12));
    if (const GURL federation(Text(statement, 13)); federation.is_valid()) {
      form.federation_origin = url::SchemeHostPort(federation);
    }
    form.in_store = password_manager::PasswordForm::Store::kProfileStore;
    out->push_back(std::move(form));
  }
  sqlite3_finalize(statement);
  return true;
}

bool ReadCards(const base::FilePath& profile_dir, const Key& key, std::vector<autofill::CreditCard>* out) {
  DatabaseCopy db(profile_dir.Append("Web Data"));
  if (!db.get()) {
    return false;
  }
  const std::set<std::string> columns = db.Columns("credit_cards");
  if (!columns.contains("card_number_encrypted")) {
    return false;
  }
  const std::string sql = base::StrCat(
      {"SELECT ",
       SelectList(columns, {{"name_on_card", "''"},
                            {"expiration_month", "0"},
                            {"expiration_year", "0"},
                            {"card_number_encrypted", "''"},
                            {"nickname", "''"}}),
       " FROM credit_cards"});
  sqlite3_stmt* statement = nullptr;
  if (sqlite3_prepare_v2(db.get(), sql.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
    return false;
  }
  while (sqlite3_step(statement) == SQLITE_ROW) {
    std::optional<std::string> number = Decrypt(key, Blob(statement, 3));
    if (!number || number->empty()) {
      continue;
    }
    autofill::CreditCard card(base::Uuid::GenerateRandomV4().AsLowercaseString());
    card.SetRawInfo(autofill::CREDIT_CARD_NAME_FULL, base::UTF8ToUTF16(Text(statement, 0)));
    card.SetExpirationMonth(sqlite3_column_int(statement, 1));
    card.SetExpirationYear(sqlite3_column_int(statement, 2));
    card.SetNumber(base::UTF8ToUTF16(*number));
    card.SetNickname(base::UTF8ToUTF16(Text(statement, 4)));
    out->push_back(std::move(card));
  }
  sqlite3_finalize(statement);
  return true;
}

using CookieKey = std::tuple<std::string, std::string, std::string, std::string>;
CookieKey KeyOf(const net::CanonicalCookie& cookie) {
  const std::optional<net::CookiePartitionKey>& partition = cookie.PartitionKey();
  return {cookie.Name(), cookie.Domain(), cookie.Path(), partition ? partition->site().Serialize() : std::string()};
}

void SetCookies(network::mojom::CookieManager* manager,
                std::vector<net::CanonicalCookie> cookies,
                base::OnceCallback<void(bool)> done) {
  if (cookies.empty()) {
    std::move(done).Run(true);
    return;
  }
  auto barrier = base::BarrierCallback<bool>(
      cookies.size(), base::BindOnce(
                          [](base::OnceCallback<void(bool)> done, std::vector<bool> results) {
                            std::move(done).Run(std::ranges::all_of(results, [](bool ok) { return ok; }));
                          },
                          std::move(done)));
  for (net::CanonicalCookie& cookie : cookies) {
    const GURL source = net::cookie_util::CookieOriginToURL(cookie.Domain(), cookie.SecureAttribute());
    manager->SetCanonicalCookie(
        cookie, source, net::CookieOptions::MakeAllInclusive(),
        base::BindOnce([](net::CookieAccessResult result) { return result.status.IsInclude(); })
            .Then(barrier));
  }
}

bool ShouldAllowImportedExtension(const extensions::Extension* extension, content::BrowserContext*) {
  return extension->is_extension();
}

}  // namespace

std::string ChromiumKeychainService(long long type) {
  // ArcImportSourceProfile.type: 2 Chrome (and Canary), 6 Edge, 7 Brave, 8 Vivaldi, 9 Opera, 10 Opera GX, 11 Arc.
  switch (type) {
    case 2: return "Chrome Safe Storage";
    case 6: return "Microsoft Edge Safe Storage";
    case 7: return "Brave Safe Storage";
    case 8: return "Vivaldi Safe Storage";
    case 9:
    case 10: return "Opera Safe Storage";
    case 11: return "Arc Safe Storage";
    default: return std::string();
  }
}

ChromiumSecrets ReadChromiumSecrets(const base::FilePath& profile_dir,
                                    const std::string& keychain_service,
                                    bool want_cookies,
                                    bool want_passwords,
                                    bool want_cards) {
  ChromiumSecrets secrets;
  if ((!want_cookies && !want_passwords && !want_cards) || keychain_service.empty()) {
    return secrets;
  }
  std::optional<std::string> password = KeychainPassword(keychain_service);
  std::optional<Key> key = password ? DeriveKey(*password) : std::nullopt;
  if (!key) {
    return secrets;
  }
  if (want_cookies) {
    secrets.cookies_ok = ReadCookies(profile_dir, *key, &secrets.cookies);
  }
  if (want_passwords) {
    secrets.passwords_ok = ReadPasswords(profile_dir, *key, &secrets.passwords);
  }
  if (want_cards) {
    secrets.cards_ok = ReadCards(profile_dir, *key, &secrets.cards);
  }
  return secrets;
}

void WriteImportedCards(Profile* profile, const std::vector<autofill::CreditCard>& cards) {
  autofill::PersonalDataManager* manager =
      profile ? autofill::PersonalDataManagerFactory::GetForBrowserContext(profile) : nullptr;
  if (!manager) {
    return;
  }
  autofill::PaymentsDataManager& payments = manager->payments_data_manager();
  for (const autofill::CreditCard& card : cards) {
    const bool known = std::ranges::any_of(payments.GetLocalCreditCards(), [&](const autofill::CreditCard* existing) {
      return existing->HasSameNumberAs(card);
    });
    if (!known) {
      payments.AddCreditCard(card);
    }
  }
}

void WriteImportedCookies(Profile* profile,
                          std::vector<net::CanonicalCookie> cookies,
                          bool skip_existing,
                          base::OnceCallback<void(bool)> done) {
  network::mojom::CookieManager* manager =
      profile ? profile->GetDefaultStoragePartition()->GetCookieManagerForBrowserProcess() : nullptr;
  if (!manager) {
    std::move(done).Run(false);
    return;
  }
  if (!skip_existing) {
    SetCookies(manager, std::move(cookies), std::move(done));
    return;
  }
  // The weak storage partition outlives this call; resolve the manager again when the list arrives.
  manager->GetAllCookies(base::BindOnce(
      [](base::WeakPtr<Profile> profile, std::vector<net::CanonicalCookie> cookies,
         base::OnceCallback<void(bool)> done, const net::CookieList& existing) {
        if (!profile) {
          std::move(done).Run(false);
          return;
        }
        std::set<CookieKey> present;
        for (const net::CanonicalCookie& cookie : existing) {
          present.insert(KeyOf(cookie));
        }
        std::erase_if(cookies, [&](const net::CanonicalCookie& cookie) { return present.contains(KeyOf(cookie)); });
        SetCookies(profile->GetDefaultStoragePartition()->GetCookieManagerForBrowserProcess(), std::move(cookies),
                   std::move(done));
      },
      profile->GetWeakPtr(), std::move(cookies), std::move(done)));
}

bool InstallImportedExtensions(Profile* profile, const std::vector<std::string>& extension_ids) {
  auto* pending = profile ? extensions::PendingExtensionManager::Get(profile) : nullptr;
  auto* registry = profile ? extensions::ExtensionRegistry::Get(profile) : nullptr;
  if (!pending || !registry) {
    return false;
  }
  bool ok = true;
  bool scheduled = false;
  for (const std::string& id : extension_ids) {
    if (!crx_file::id_util::IdIsValid(id)) {
      ok = false;
      continue;
    }
    if (registry->GetExtensionById(id, extensions::ExtensionRegistry::EVERYTHING) || pending->IsIdPending(id)) {
      continue;
    }
    if (pending->AddFromSync(id, extension_urls::GetWebstoreUpdateUrl(), base::Version(),
                             &ShouldAllowImportedExtension, /*remote_install=*/false)) {
      scheduled = true;
    } else {
      ok = false;
    }
  }
  if (scheduled) {
    if (auto* updater = extensions::ExtensionUpdater::Get(profile)) {
      updater->CheckSoon();
    }
  }
  return ok;
}

}  // namespace arc
