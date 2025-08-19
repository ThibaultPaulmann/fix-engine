#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

#include "engine/Session.hpp"
#include "engine/InMemoryStore.hpp"
#include "engine/ISessionHandler.hpp"
#include "core/FieldTags.hpp"
#include "helpers/FrameBuilder.hpp"

struct TestHandler : fix::engine::ISessionHandler
{
  int gaps{0}, possdups{0}, lows{0}, logons{0}, hbs{0};
  void on_bad_frame(const char *, int) override {}
  void on_header_violation(int, const char *) override {}
  void on_gap_detected(int, int) override { ++gaps; }
  void on_poss_dup(int) override { ++possdups; }
  void on_seq_too_low(int, int) override { ++lows; }
  void on_logon(const fix::protocol::fixt11::LogonView &) override { ++logons; }
  void on_logout(const fix::protocol::fixt11::LogoutView &) override {}
  void on_heartbeat(const fix::protocol::fixt11::HeartbeatView &) override { ++hbs; }
  void on_test_request(const fix::protocol::fixt11::TestRequestView &) override {}
  void on_resend_request(const fix::protocol::fixt11::ResendRequestView &) override {}
  void on_reject(const fix::protocol::fixt11::RejectView &) override {}
  void on_sequence_reset(const fix::protocol::fixt11::SequenceResetView &) override {}
  void on_application_message(std::string_view, const fix::core::Message &) override {}
};

int main()
{
  using namespace fix::core::tags;

  // Config & session
  fix::engine::SessionConfig cfg{.sender_comp_id = "C1", .target_comp_id = "S1"};
  fix::engine::InMemoryStore store; // in=1, out=1
  TestHandler h;
  fix::engine::Session s(cfg, store, h);

  // 1) Proper increments: seq 1 then 2
  auto logon = fix::test::build_frame({{BeginString, "FIXT.1.1"}, {MsgType, "A"}, {SenderCompID, "C1"}, {TargetCompID, "S1"}, {MsgSeqNum, "1"}, {SendingTime, "20250101-12:00:00.000"}, {EncryptMethod, "0"}, {HeartBtInt, "30"}, {DefaultApplVerID, "9"}});
  s.on_frame(logon);
  auto hb2 = fix::test::build_frame({{BeginString, "FIXT.1.1"}, {MsgType, "0"}, {SenderCompID, "C1"}, {TargetCompID, "S1"}, {MsgSeqNum, "2"}, {SendingTime, "20250101-12:00:00.500"}});
  s.on_frame(hb2);
  if (store.next_incoming() != 3 || h.logons != 1 || h.hbs != 1)
  {
    std::cerr << "increment path failed\n";
    return EXIT_FAILURE;
  }

  // 2) Gap detected: seq jumps to 5 (expected 3)
  auto hb5 = fix::test::build_frame({{BeginString, "FIXT.1.1"}, {MsgType, "0"}, {SenderCompID, "C1"}, {TargetCompID, "S1"}, {MsgSeqNum, "5"}, {SendingTime, "20250101-12:00:01.000"}});
  s.on_frame(hb5);
  if (h.gaps != 1)
  {
    std::cerr << "gap detect failed\n";
    return EXIT_FAILURE;
  }

  // 3) PossDup: seq lower than expected with 43=Y
  auto hb_dup = fix::test::build_frame({{BeginString, "FIXT.1.1"}, {MsgType, "0"}, {SenderCompID, "C1"}, {TargetCompID, "S1"}, {MsgSeqNum, "2"}, {SendingTime, "20250101-12:00:01.500"}, {PossDupFlag, "Y"}});
  s.on_frame(hb_dup);
  if (h.possdups != 1)
  {
    std::cerr << "possdup failed\n";
    return EXIT_FAILURE;
  }

  // 4) Too low (without PossDup): seq lower than expected & no 43=Y
  auto hb_low = fix::test::build_frame({{BeginString, "FIXT.1.1"}, {MsgType, "0"}, {SenderCompID, "C1"}, {TargetCompID, "S1"}, {MsgSeqNum, "1"}, {SendingTime, "20250101-12:00:02.000"}});
  s.on_frame(hb_low);
  if (h.lows != 1)
  {
    std::cerr << "low seq failed\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
