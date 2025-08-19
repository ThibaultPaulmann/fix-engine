#include "protocol/fixt11/HeaderTrailerRules.hpp"

namespace fix::protocol::fixt11 {

namespace tags = fix::core::tags;

std::vector<HeaderTrailerRules::Violation>
HeaderTrailerRules::validate(const fix::core::Message& m, bool outbound_from_server) {
  std::vector<Violation> v;

  if (!m.get_sv(tags::BeginString))  v.push_back({tags::BeginString, "missing BeginString(8)"});
  if (!m.get_int(tags::BodyLength))  v.push_back({tags::BodyLength,  "missing BodyLength(9)"});
  if (!m.get_sv(tags::MsgType))      v.push_back({tags::MsgType,      "missing MsgType(35)"});
  if (!m.get_sv(tags::SenderCompID)) v.push_back({tags::SenderCompID, "missing SenderCompID(49)"});
  if (!m.get_sv(tags::TargetCompID)) v.push_back({tags::TargetCompID, "missing TargetCompID(56)"});
  if (!m.get_int(tags::MsgSeqNum))   v.push_back({tags::MsgSeqNum,    "missing MsgSeqNum(34)"});
  if (outbound_from_server && !m.get_sv(tags::ApplVerID))
    v.push_back({tags::ApplVerID, "missing ApplVerID(1128) on server outbound"});
  if (!m.get_int(tags::CheckSum))    v.push_back({tags::CheckSum,     "missing CheckSum(10)"});
  return v;
}

} // namespace fix::protocol::fixt11
