// Copyright 2026 ArcMod. In-process transport for the app-supplied C libraries.
#ifndef ARC_BROWSER_AI_TRANSPORT_H_
#define ARC_BROWSER_AI_TRANSPORT_H_

#import <Foundation/Foundation.h>
#include <dlfcn.h>
#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include "base/no_destructor.h"
#include "base/compiler_specific.h"
#include "base/containers/span.h"

namespace arc_ai {

// The external library owns each C result buffer and supplies its exact length.
// Bound it at this one foreign-memory boundary; all subsequent access is checked.
template <typename T>
base::span<T> Buffer(T* pointer, size_t length) {
  return UNSAFE_BUFFERS(base::span(pointer, length));
}

inline void OnMain(void (^callback)()) {
  // The original utility/Mojo reply is asynchronous, including when the C
  // library completes inline. Never call the application on a library thread.
  dispatch_async(dispatch_get_main_queue(), callback);
}

struct Lifetime {
  std::atomic<bool> alive{true};
};

struct Pending {
  id completion;
  id partial;
  std::shared_ptr<Lifetime> owner;
  std::shared_ptr<void> context;
  size_t expected_results = 0;
};

struct Requests {
  std::mutex mutex;
  uint64_t next = 1;
  std::map<uint64_t, std::shared_ptr<Pending>> pending;
};

inline Requests& Registry() {
  static base::NoDestructor<Requests> registry;
  return *registry;
}

inline void Invalidate(std::shared_ptr<Lifetime> owner) {
  owner->alive.store(false);
  OnMain(^{
    auto& registry = Registry();
    std::vector<std::shared_ptr<Pending>> abandoned;
    {
      std::lock_guard lock(registry.mutex);
      for (auto entry = registry.pending.begin(); entry != registry.pending.end();) {
        if (entry->second->owner == owner) {
          abandoned.push_back(entry->second);
          entry = registry.pending.erase(entry);
        } else {
          ++entry;
        }
      }
    }
    // Releasing a captured app object may reenter another service. Never
    // destroy application capture graphs while holding the registry lock.
    for (auto& request : abandoned) {
      request->completion = nil;
      request->partial = nil;
    }
  });
}

inline void Deliver(std::shared_ptr<Pending> pending, void (^callback)()) {
  OnMain(^{
    if (pending->owner->alive.load()) callback();
  });
}

template <typename UserData>
UserData Register(std::shared_ptr<Pending> request) {
  auto& registry = Registry();
  std::lock_guard lock(registry.mutex);
  uint64_t key = registry.next++;
  registry.pending.emplace(key, std::move(request));
  return {static_cast<uint16_t>(key), key};
}

template <typename UserData>
std::shared_ptr<Pending> Find(UserData data, bool finish = true) {
  auto& registry = Registry();
  std::lock_guard lock(registry.mutex);
  auto found = registry.pending.find(data.pointer);
  if (found == registry.pending.end() ||
      data.id != static_cast<uint16_t>(found->first)) {
    return nullptr;
  }
  auto pending = found->second;
  if (finish) {
    registry.pending.erase(found);
  }
  return pending;
}

inline std::shared_ptr<Pending> Request(id completion,
                                      id partial,
                                      std::shared_ptr<Lifetime> owner,
                                      size_t expected_results = 0) {
  auto pending = std::make_shared<Pending>();
  pending->completion = [completion copy];
  pending->partial = [partial copy];
  pending->owner = std::move(owner);
  pending->expected_results = expected_results;
  return pending;
}

inline void* OpenLibrary(const std::string& path) {
  void* module = dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);
  if (module) {
    // Neither ABI has Destroy/Cancel. Retain modules for the process lifetime:
    // unloading executable code while its external handle owns tasks is unsafe.
    struct Modules {
      std::mutex mutex;
      std::vector<void*> handles;
    };
    static base::NoDestructor<Modules> modules;
    std::lock_guard lock(modules->mutex);
    modules->handles.push_back(module);
  }
  return module;
}

template <typename Function>
Function Symbol(void* module, const char* name) {
  return reinterpret_cast<Function>(dlsym(module, name));
}
}  // namespace arc_ai
#endif
