#pragma once
#include <string_view>
#include "engine/SessionConfig.hpp"
#include "engine/SessionStore.hpp"
#include "engine/ISessionHandler.hpp"
#include "core/Parser.hpp"
#include "protocol/fixt11/HeaderTrailerRules.hpp"
#include "protocol/fixt11/AdminViews.hpp"

namespace fix::engine {

// Decode-only session: takes a single complete frame view and dispatches events.
// Buffering/framing is done by your network layer. Encoding comes later.
class Session {
public:
  Session(SessionConfig cfg, SessionStore& store, ISessionHandler& handler)
    : cfg_(std::move(cfg)), store_(store), h_(handler) {}

  // frame must start at 8= and contain a full FIX frame
  void on_frame(std::string_view frame) {
    fix::core::Message m;
    auto res = fix::core::Parser::parse(frame, m);
    if (res.status == fix::core::Parser::Status::NeedMore) return; // not expected here
    if (res.status == fix::core::Parser::Status::BadFrame) { h_.on_bad_frame(res.error, res.error_tag); return; }

    // header sanity (already checksum/length verified by Parser)
    for (auto& v : fix::protocol::fixt11::HeaderTrailerRules::validate(m, /*outbound_from_server*/false))
      h_.on_header_violation(v.tag, v.what);

    // sequence checks
    const int expected = store_.next_incoming();
    const int received = m.get_int(fix::core::tags::MsgSeqNum).value_or(0);
    const bool possdup = m.get_bool(fix::core::tags::PossDupFlag).value_or(false);

    if (received > 0) {
      if (received == expected) {
        store_.set_next_incoming(expected + 1);
      } else if (received > expected) {
        h_.on_gap_detected(expected, received);
        // No automatic resend logic here in decode-only phase
      } else { // received < expected
        if (possdup) {
          h_.on_poss_dup(received);
        } else {
          h_.on_seq_too_low(expected, received);
        }
      }
    }

    // dispatch by MsgType
    auto mt = m.get_sv(fix::core::tags::MsgType).value_or("");
    using namespace fix::core::msg;
    if (mt == Logon)               return h_.on_logon({m});
    if (mt == Logout)              return h_.on_logout({m});
    if (mt == Heartbeat)           return h_.on_heartbeat({m});
    if (mt == TestRequest)         return h_.on_test_request({m});
    if (mt == ResendRequest)       return h_.on_resend_request({m});
    if (mt == Reject)              return h_.on_reject({m});
    if (mt == SequenceReset)       return h_.on_sequence_reset({m});
    // otherwise application-layer (FIX 5.0 SP2 etc.)
    h_.on_application_message(mt, m);
  }

private:
  SessionConfig   cfg_;
  SessionStore&   store_;
  ISessionHandler& h_;
};

} // namespace fix::engine
