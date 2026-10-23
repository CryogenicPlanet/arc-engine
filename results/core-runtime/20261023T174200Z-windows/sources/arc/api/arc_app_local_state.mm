// Copyright 2026 ArcMod. Browser-wide local preferences, distinct from profile preferences.
#include "arc/api/arc_internal.h"

#import <objc/runtime.h>

#include <map>
#include <memory>
#include <array>

#include "base/functional/bind.h"
#include "base/values.h"
#include "chrome/browser/browser_process.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_service.h"

namespace {
PrefService* LocalPreferences() {
  return g_browser_process ? g_browser_process->local_state() : nullptr;
}

// Arc's status constructor (0x4b37104) shifts its tagged argument by two;
// NSError conversion (0x44025f4) uses table 0001020a0304050a06070809.
void LocalStateError(NSError** error, NSInteger code, NSString* description) {
  if (error) {
    *error = [NSError errorWithDomain:@"company.thebrowser.Arc" code:code
                            userInfo:@{NSLocalizedDescriptionKey: description}];
  }
}

id PreferenceObject(const base::Value& value) {
  switch (value.type()) {
    case base::Value::Type::NONE: return [NSNull null];
    case base::Value::Type::BOOLEAN: return @(value.GetBool());
    case base::Value::Type::INTEGER: return @(value.GetInt());
    case base::Value::Type::DOUBLE: return @(value.GetDouble());
    case base::Value::Type::STRING: return ArcNSString(value.GetString());
    case base::Value::Type::DICT: {
      NSMutableDictionary* dictionary = [NSMutableDictionary dictionary];
      for (auto entry : value.GetDict()) {
        dictionary[ArcNSString(entry.first)] = PreferenceObject(entry.second);
      }
      return dictionary;
    }
    case base::Value::Type::LIST: {
      NSMutableArray* list = [NSMutableArray array];
      for (const auto& element : value.GetList()) [list addObject:PreferenceObject(element)];
      return list;
    }
    case base::Value::Type::BINARY:
      // base::Value binary data is not a registered preference value type.
      return [NSNull null];
  }
}

const base::Value* FindLocalPreference(NSString* path, base::Value::Type type,
                                      NSError** error) {
  PrefService* service = LocalPreferences();
  if (!service) {
    LocalStateError(error, 10, @"This object is detached");
    return nullptr;
  }
  const auto* preference = service->FindPreference(ArcUTF8(path));
  if (!preference) {
    LocalStateError(error, 3, @"Preference not found");
    return nullptr;
  }
  if (preference->GetType() != type) {
    LocalStateError(error, 2, @"Wrong preference type");
    return nullptr;
  }
  return preference->GetValue();
}

char kApplicationLocalStateKey;
}  // namespace

ARC_EXPORT @interface ArcLocalState : NSObject
- (instancetype)initWithLocalState:(const void*)localState;
- (NSNumber*)addPrefChangeCallback:(NSString*)path
                         callback:(void (^)(NSString*))callback error:(NSError**)error;
- (BOOL)removePrefChangeCallback:(NSNumber*)token error:(NSError**)error;
- (NSNumber*)getBoolean:(NSString*)path error:(NSError**)error;
- (NSString*)getString:(NSString*)path error:(NSError**)error;
- (NSNumber*)getInteger:(NSString*)path error:(NSError**)error;
- (NSDictionary*)getDictionary:(NSString*)path error:(NSError**)error;
- (NSArray*)getList:(NSString*)path error:(NSError**)error;
- (id)getBrowserPolicy:(NSUInteger)identifier error:(NSError**)error;
@end

@implementation ArcLocalState {
  uint32_t _nextCallbackToken;
  std::map<uint32_t, std::unique_ptr<PrefChangeRegistrar>> _callbacks;
}
- (instancetype)initWithLocalState:(const void*)localState {
  // Native C++ handle encodings are engine-internal. This wrapper resolves our
  // browser process on each use instead of interpreting Arc's shared_ptr layout.
  return [super init];
}
- (NSNumber*)getBoolean:(NSString*)path error:(NSError**)error {
  const base::Value* value = FindLocalPreference(path, base::Value::Type::BOOLEAN, error);
  return value ? @(value->GetBool()) : nil;
}
- (NSString*)getString:(NSString*)path error:(NSError**)error {
  const base::Value* value = FindLocalPreference(path, base::Value::Type::STRING, error);
  return value ? ArcNSString(value->GetString()) : nil;
}
- (NSNumber*)getInteger:(NSString*)path error:(NSError**)error {
  const base::Value* value = FindLocalPreference(path, base::Value::Type::INTEGER, error);
  return value ? @(value->GetInt()) : nil;
}
- (NSDictionary*)getDictionary:(NSString*)path error:(NSError**)error {
  const base::Value* value = FindLocalPreference(path, base::Value::Type::DICT, error);
  return value ? PreferenceObject(*value) : nil;
}
- (NSArray*)getList:(NSString*)path error:(NSError**)error {
  const base::Value* value = FindLocalPreference(path, base::Value::Type::LIST, error);
  return value ? PreferenceObject(*value) : nil;
}
- (id)getBrowserPolicy:(NSUInteger)identifier error:(NSError**)error {
  constexpr std::array<const char*, 9> policies = {
      "dia.airestricteddomainslist", "dia.airestricteddomainsexceptionslist",
      "dia.enforceautomaticupdatesbool", "dia.airestrictedfiletypeslist",
      "dia.managedupdatesdeferrallimitint", "dia.minimumversionstring",
      "dia.restrictlogindomainslist", "dia.syncescrowpublickeystring", "dia.updateafterint"};
  // Arc narrows the public NSUInteger to uint32 before its checked table lookup.
  const uint32_t index = static_cast<uint32_t>(identifier);
  CHECK(index < policies.size());
  PrefService* service = LocalPreferences();
  if (!service) { LocalStateError(error, 10, @"This object is detached"); return nil; }
  const auto* preference = service->FindPreference(policies[index]);
  if (!preference) { LocalStateError(error, 3, @"Preference not found"); return nil; }
  return PreferenceObject(*preference->GetValue());
}
- (NSNumber*)addPrefChangeCallback:(NSString*)path
                         callback:(void (^)(NSString*))callback error:(NSError**)error {
  PrefService* service = LocalPreferences();
  if (!service) {
    LocalStateError(error, 10, @"This object is detached");
    return nil;
  }
  // Arc registers one native preference observer per path and fan-outs tokens.
  // Separate registrars permit the same public path to be registered repeatedly.
  auto registrar = std::make_unique<PrefChangeRegistrar>();
  registrar->Init(service);
  registrar->Add(ArcUTF8(path), base::BindRepeating(^(const std::string& name) {
    // Retain a local copy so a callback may safely remove its own token.
    void (^invocation)(NSString*) = callback;
    if (invocation) invocation(ArcNSString(name));
  }));
  do { ++_nextCallbackToken; } while (_callbacks.contains(_nextCallbackToken));
  uint32_t token = _nextCallbackToken;
  _callbacks.emplace(token, std::move(registrar));
  return @(token);
}
- (BOOL)removePrefChangeCallback:(NSNumber*)token error:(NSError**)error {
  if (!LocalPreferences()) {
    LocalStateError(error, 10, @"This object is detached");
    return NO;
  }
  // Registrar observes OnServiceDestroyed and resets before PrefService dies.
  auto found = _callbacks.find(token.unsignedIntValue);
  if (found == _callbacks.end()) {
    LocalStateError(error, 3, @"Callback not found");
    return NO;
  }
  _callbacks.erase(found);
  return YES;
}
@end

ARC_EXPORT @interface ArcBrowserApplication : NSObject
@end
@implementation ArcBrowserApplication (AppLocalState)
- (ArcLocalState*)localState {
  id state = objc_getAssociatedObject(self, &kApplicationLocalStateKey);
  if (!state) {
    state = [[ArcLocalState alloc] initWithLocalState:nullptr];
    objc_setAssociatedObject(self, &kApplicationLocalStateKey, state, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
  }
  return state == [NSNull null] ? nil : state;
}
- (void)setLocalState:(ArcLocalState*)state {
  objc_setAssociatedObject(self, &kApplicationLocalStateKey, state ?: [NSNull null],
                           OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}
@end
