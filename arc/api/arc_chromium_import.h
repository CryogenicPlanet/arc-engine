// Copyright 2026 ArcMod. Importing cookies, passwords and extensions from a Chromium-family browser profile
// (Chrome, Edge, Brave, Vivaldi, Opera, Arc), like Arc's engine: the profile's SQLite databases are read
// directly and decrypted with the source browser's "<name> Safe Storage" keychain key.
#ifndef ARC_API_ARC_CHROMIUM_IMPORT_H_
#define ARC_API_ARC_CHROMIUM_IMPORT_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/functional/callback_forward.h"
#include "components/autofill/core/browser/data_model/payments/credit_card.h"
#include "components/password_manager/core/browser/password_form.h"
#include "net/cookies/canonical_cookie.h"

class Profile;

namespace arc {

struct ChromiumSecrets {
  bool cookies_ok = false;
  std::vector<net::CanonicalCookie> cookies;
  bool passwords_ok = false;
  std::vector<password_manager::PasswordForm> passwords;
  bool cards_ok = false;
  std::vector<autofill::CreditCard> cards;
};

// The keychain service holding the source browser's cookie and password key, by Arc's public import type
// (ArcImportSourceProfile.type).
std::string ChromiumKeychainService(long long type);

// Blocking (thread pool): reads the source profile's key from the real keychain (ArcMod's --use-mock-keychain
// covers only its own data), which can show the system's access prompt, then its "Cookies", "Login Data" and
// "Web Data" (saved cards) databases. Scenarios pass the password with --arcmod-import-test-keychain-password.
ChromiumSecrets ReadChromiumSecrets(const base::FilePath& profile_dir,
                                    const std::string& keychain_service,
                                    bool want_cookies,
                                    bool want_passwords,
                                    bool want_cards);

// Adds |cards| to |profile|'s local payment methods, skipping card numbers it already has.
void WriteImportedCards(Profile* profile, const std::vector<autofill::CreditCard>& cards);

// Sets |cookies| in |profile|'s default storage partition. With |skip_existing|, a cookie the profile already
// has (same name, domain, path and partition) is kept. |done| gets whether every cookie was accepted.
void WriteImportedCookies(Profile* profile,
                          std::vector<net::CanonicalCookie> cookies,
                          bool skip_existing,
                          base::OnceCallback<void(bool)> done);

// Schedules Chrome Web Store installs of |extension_ids| into |profile| (as user installs, like Chrome sync).
// Already installed extensions are skipped. Returns false if an extension couldn't be scheduled.
bool InstallImportedExtensions(Profile* profile, const std::vector<std::string>& extension_ids);

}  // namespace arc

#endif  // ARC_API_ARC_CHROMIUM_IMPORT_H_
