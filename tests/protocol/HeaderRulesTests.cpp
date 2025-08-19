#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include "core/Parser.hpp"
#include "core/FieldTags.hpp"
#include "protocol/fixt11/HeaderTrailerRules.hpp"
#include "helpers/FrameBuilder.hpp"

int main() {
  using namespace fix::core;
  using namespace fix::core::tags;

  // valid admin message (Heartbeat)
  auto frame_ok = fix::test::build_frame({
    {BeginString, "FIXT.1.1"},
    {MsgType, "0"},
    {SenderCompID, "C1"},
    {TargetCompID, "S1"},
    {MsgSeqNum, "1"},
    {SendingTime, "20250101-12:00:00.000"}
  });

  Message m;
  auto res = Parser::parse(std::string_view(frame_ok), m);
  if (res.status != Parser::Status::Ok) { std::cerr<<"parse fail\n"; return EXIT_FAILURE; }

  auto v = fix::protocol::fixt11::HeaderTrailerRules::validate(m, /*outbound_from_server*/false);
  if (!v.empty()) { std::cerr<<"expected 0 violations, got "<<v.size()<<"\n"; return EXIT_FAILURE; }

  // missing SenderCompID
  auto frame_miss = fix::test::build_frame({
    {BeginString, "FIXT.1.1"},
    {MsgType, "0"},
    // {SenderCompID, "C1"}, // missing
    {TargetCompID, "S1"},
    {MsgSeqNum, "1"},
    {SendingTime, "20250101-12:00:00.000"}
  });

  res = Parser::parse(std::string_view(frame_miss), m);
  if (res.status != Parser::Status::Ok) { std::cerr<<"parse fail 2\n"; return EXIT_FAILURE; }

  v = fix::protocol::fixt11::HeaderTrailerRules::validate(m, false);
  bool found = false;
  for (auto& vi : v) if (vi.tag == SenderCompID) { found = true; break; }
  if (!found) { std::cerr<<"expected violation for SenderCompID\n"; return EXIT_FAILURE; }

  return EXIT_SUCCESS;
}
