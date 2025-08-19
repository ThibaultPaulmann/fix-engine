#pragma once
#include <string_view>
#include "core/Message.hpp"
#include "protocol/fixt11/AdminViews.hpp"

namespace fix::engine
{

  // Your app implements this to receive decoded session events.
  struct ISessionHandler
  {
    virtual ~ISessionHandler() = default;

    virtual void on_bad_frame(const char *what, int tag) = 0;
    virtual void on_header_violation(int tag, const char *what) = 0;

    virtual void on_gap_detected(int expected, int received) = 0;
    virtual void on_poss_dup(int received) = 0;
    virtual void on_seq_too_low(int expected, int received) = 0;

    virtual void on_logon(const fix::protocol::fixt11::LogonView &) = 0;
    virtual void on_logout(const fix::protocol::fixt11::LogoutView &) = 0;
    virtual void on_heartbeat(const fix::protocol::fixt11::HeartbeatView &) = 0;
    virtual void on_test_request(const fix::protocol::fixt11::TestRequestView &) = 0;
    virtual void on_resend_request(const fix::protocol::fixt11::ResendRequestView &) = 0;
    virtual void on_reject(const fix::protocol::fixt11::RejectView &) = 0;
    virtual void on_sequence_reset(const fix::protocol::fixt11::SequenceResetView &) = 0;

    // For anything not session/admin (we’ll handle later)
    virtual void on_application_message(std::string_view msg_type,
                                        const fix::core::Message &m) = 0;
  };

} // namespace fix::engine
