// Copyright 2026 ArcMod.
#include "arc/browser/ai_stream_transport.h"

#include <array>
#include <bit>
#include <cstdint>
#include <mutex>
#include <utility>
#include <vector>

#include "base/check.h"
#include "base/functional/bind.h"
#include "base/location.h"
#include "base/memory/weak_ptr.h"
#include "base/task/single_thread_task_runner.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "mojo/public/cpp/system/message_pipe.h"
#include "mojo/public/cpp/system/simple_watcher.h"

namespace arc_ai {

class StreamTransport::Impl {
 public:
  Impl(base::RepeatingCallback<void(std::string)> partial,
       base::OnceCallback<void(int)> complete)
      : runner_(content::GetUIThreadTaskRunner({})),
        partial_(std::move(partial)),
        complete_(std::move(complete)),
        listener_watcher_(FROM_HERE, mojo::SimpleWatcher::ArmingPolicy::MANUAL,
                          runner_),
        completion_watcher_(FROM_HERE, mojo::SimpleWatcher::ArmingPolicy::MANUAL,
                            runner_) {
    DCHECK(runner_->RunsTasksInCurrentSequence());
    CHECK_EQ(mojo::CreateMessagePipe(nullptr, &listener_sender_,
                                    &listener_receiver_), MOJO_RESULT_OK);
    CHECK_EQ(mojo::CreateMessagePipe(nullptr, &completion_sender_,
                                    &completion_receiver_), MOJO_RESULT_OK);
  }

  ~Impl() {
    DCHECK(runner_->RunsTasksInCurrentSequence());
    Stop();
  }

  void Start(std::weak_ptr<StreamTransport> owner) {
    DCHECK(runner_->RunsTasksInCurrentSequence());
    owner_ = std::move(owner);
    CHECK_EQ(listener_watcher_.Watch(
                 listener_receiver_.get(),
                 MOJO_HANDLE_SIGNAL_READABLE | MOJO_HANDLE_SIGNAL_PEER_CLOSED,
                 MOJO_WATCH_CONDITION_SATISFIED,
                 base::BindRepeating(&Impl::OnPartial,
                                     weak_factory_.GetWeakPtr())),
             MOJO_RESULT_OK);
    CHECK_EQ(completion_watcher_.Watch(
                 completion_receiver_.get(),
                 MOJO_HANDLE_SIGNAL_READABLE | MOJO_HANDLE_SIGNAL_PEER_CLOSED,
                 MOJO_WATCH_CONDITION_SATISFIED,
                 base::BindRepeating(&Impl::OnCompletion,
                                     weak_factory_.GetWeakPtr())),
             MOJO_RESULT_OK);
    listener_watcher_.ArmOrNotify();
    completion_watcher_.ArmOrNotify();
  }

  void SendPartial(std::string partial) {
    std::lock_guard<std::mutex> guard(senders_mutex_);
    if (listener_sender_.is_valid()) {
      mojo::WriteMessageRaw(listener_sender_.get(), partial.data(),
                            partial.size(), nullptr, 0,
                            MOJO_WRITE_MESSAGE_FLAG_NONE);
    }
  }

  void Finish(int error) {
    std::lock_guard<std::mutex> guard(senders_mutex_);
    // The original utility removes its listener remote before sending the
    // service response. These independent notifications race on the UI thread.
    listener_sender_.reset();
    if (completion_sender_.is_valid()) {
      const int32_t wire_error = error;
      mojo::WriteMessageRaw(completion_sender_.get(), &wire_error,
                            sizeof(wire_error), nullptr, 0,
                            MOJO_WRITE_MESSAGE_FLAG_NONE);
      completion_sender_.reset();
    }
  }

 private:
  void OnPartial(MojoResult result, const mojo::HandleSignalsState&) {
    auto keep_alive = owner_.lock();
    if (!keep_alive || !complete_)
      return;

    std::vector<uint8_t> payload;
    result = mojo::ReadMessageRaw(listener_receiver_.get(), &payload, nullptr,
                                  MOJO_READ_MESSAGE_FLAG_NONE);
    if (result == MOJO_RESULT_OK) {
      // Read one message per notification, allowing the other pipe's response
      // to run before this pipe's next delivery. ACKs in the original are no-op.
      partial_.Run(std::string(payload.begin(), payload.end()));
    } else if (result != MOJO_RESULT_SHOULD_WAIT) {
      Complete(10);
      return;
    }
    if (complete_)
      listener_watcher_.ArmOrNotify();
  }

  void OnCompletion(MojoResult result, const mojo::HandleSignalsState&) {
    auto keep_alive = owner_.lock();
    if (!keep_alive || !complete_)
      return;

    std::vector<uint8_t> payload;
    result = mojo::ReadMessageRaw(completion_receiver_.get(), &payload, nullptr,
                                  MOJO_READ_MESSAGE_FLAG_NONE);
    if (result == MOJO_RESULT_OK && payload.size() == sizeof(int32_t)) {
      Complete(std::bit_cast<int32_t>(std::array<uint8_t, 4>{
          payload[0], payload[1], payload[2], payload[3]}));
    } else if (result == MOJO_RESULT_SHOULD_WAIT) {
      completion_watcher_.ArmOrNotify();
    } else {
      Complete(10);
    }
  }

  void Complete(int error) {
    auto callback = std::move(complete_);
    Stop();
    partial_.Reset();
    std::move(callback).Run(error);
  }

  void Stop() {
    weak_factory_.InvalidateWeakPtrs();
    listener_watcher_.Cancel();
    completion_watcher_.Cancel();
    listener_receiver_.reset();
    completion_receiver_.reset();
    std::lock_guard<std::mutex> guard(senders_mutex_);
    listener_sender_.reset();
    completion_sender_.reset();
  }

  scoped_refptr<base::SingleThreadTaskRunner> runner_;
  base::RepeatingCallback<void(std::string)> partial_;
  base::OnceCallback<void(int)> complete_;
  std::weak_ptr<StreamTransport> owner_;
  std::mutex senders_mutex_;
  mojo::ScopedMessagePipeHandle listener_sender_;
  mojo::ScopedMessagePipeHandle completion_sender_;
  mojo::ScopedMessagePipeHandle listener_receiver_;
  mojo::ScopedMessagePipeHandle completion_receiver_;
  mojo::SimpleWatcher listener_watcher_;
  mojo::SimpleWatcher completion_watcher_;
  base::WeakPtrFactory<Impl> weak_factory_{this};
};

StreamTransport::StreamTransport(
    base::RepeatingCallback<void(std::string)> partial,
    base::OnceCallback<void(int)> complete)
    : impl_(std::make_unique<Impl>(std::move(partial), std::move(complete))) {}

StreamTransport::~StreamTransport() = default;

void StreamTransport::SendPartial(std::string partial) {
  impl_->SendPartial(std::move(partial));
}

void StreamTransport::Finish(int error) {
  impl_->Finish(error);
}

std::shared_ptr<StreamTransport> CreateStreamTransport(
    base::RepeatingCallback<void(std::string)> partial,
    base::OnceCallback<void(int)> complete) {
  auto runner = content::GetUIThreadTaskRunner({});
  auto transport = std::shared_ptr<StreamTransport>(
      new StreamTransport(std::move(partial), std::move(complete)),
      [runner](StreamTransport* transport) {
        if (runner->RunsTasksInCurrentSequence()) {
          delete transport;
        } else {
          runner->DeleteSoon(FROM_HERE, transport);
        }
      });
  transport->impl_->Start(transport->weak_from_this());
  return transport;
}

}  // namespace arc_ai
