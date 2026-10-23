// Copyright 2026 ArcMod.
#ifndef ARC_BROWSER_AI_STREAM_TRANSPORT_H_
#define ARC_BROWSER_AI_STREAM_TRANSPORT_H_

#include <memory>
#include <string>

#include "base/functional/callback.h"

namespace arc_ai {

// Own and release on the UI sequence. Library callbacks may send from any thread.
// The listener and service reply use separate pipes, as in the original service.
class StreamTransport final
    : public std::enable_shared_from_this<StreamTransport> {
 public:
  ~StreamTransport();
  StreamTransport(const StreamTransport&) = delete;
  StreamTransport& operator=(const StreamTransport&) = delete;

  void SendPartial(std::string partial);
  void Finish(int error);

 private:
  friend std::shared_ptr<StreamTransport> CreateStreamTransport(
      base::RepeatingCallback<void(std::string)> partial,
      base::OnceCallback<void(int)> complete);

  class Impl;
  StreamTransport(base::RepeatingCallback<void(std::string)> partial,
                  base::OnceCallback<void(int)> complete);
  std::unique_ptr<Impl> impl_;
};

std::shared_ptr<StreamTransport> CreateStreamTransport(
    base::RepeatingCallback<void(std::string)> partial,
    base::OnceCallback<void(int)> complete);

}  // namespace arc_ai

#endif  // ARC_BROWSER_AI_STREAM_TRANSPORT_H_
