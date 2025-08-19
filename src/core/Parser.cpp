#include "core/Parser.hpp"
#include "core/Field.hpp"
#include "core/FieldTags.hpp"

namespace fix::core {

Parser::Result Parser::parse(std::string_view buf, Message& out) {
  using namespace tags;

  out.clear();

  // Require start at "8=" for speed; relax if you want to resync here.
  if (buf.size() < 3 || buf.compare(0, 2, "8=") != 0) {
    return {Status::BadFrame, "buffer does not start at BeginString", BeginString, 0, {}};
  }

  // Find 9= and its SOH
  const auto pos9 = buf.find("9=", 0);
  if (pos9 == std::string_view::npos) {
    return {Status::NeedMore, nullptr, 0, 0, {}};
  }
  const auto e9 = buf.find(SOH, pos9);
  if (e9 == std::string_view::npos) {
    return {Status::NeedMore, nullptr, 0, 0, {}};
  }

  // Parse BodyLength
  int bodyLen = 0;
  {
    if (e9 < pos9 + 2) return {Status::BadFrame, "empty BodyLength", BodyLength, 0, {}};
    std::string_view digits{buf.data() + pos9 + 2, e9 - (pos9 + 2)};
    auto opt = sv_to_int(digits);
    if (!opt) return {Status::BadFrame, "invalid BodyLength", BodyLength, 0, {}};
    bodyLen = *opt;
    if (bodyLen < 0) return {Status::BadFrame, "negative BodyLength", BodyLength, 0, {}};
  }

  // Compute frame structure
  const std::size_t bodyStart = e9 + 1;
  const std::size_t bodyEnd   = bodyStart + static_cast<std::size_t>(bodyLen); // SOH before 10=
  // Need "10=ddd<SOH>" (7 bytes) after bodyEnd
  const std::size_t minFrame  = bodyEnd + 7;
  if (buf.size() < minFrame) {
    return {Status::NeedMore, nullptr, 0, 0, {}};
  }

  // Verify "10=" at expected position and that 3 digits and SOH follow
  if (!(buf.compare(bodyEnd, 3, "10=") == 0 &&
        bodyEnd + 6 < buf.size() &&
        buf[bodyEnd + 6] == SOH)) {
    return {Status::BadFrame, "framing mismatch at CheckSum", CheckSum, 0, {}};
  }

  // Compute checksum of bytes up to SOH before 10=
  int sum = 0;
  for (std::size_t i = 0; i < bodyEnd; ++i)
    sum = (sum + static_cast<unsigned char>(buf[i])) & 0xFF;

  // Parse expected checksum digits
  int expected = 0;
  {
    std::string_view cs{buf.data() + bodyEnd + 3, 3};
    auto opt = sv_to_int(cs);
    if (!opt) return {Status::BadFrame, "invalid CheckSum digits", CheckSum, 0, {}};
    expected = *opt;
  }
  if (sum != expected) {
    return {Status::BadFrame, "CheckSum mismatch", CheckSum, 0, {}};
  }

  // Tokenise fields as zero-copy views up to bodyEnd
  out.reserve(32);
  std::size_t cur = 0;
  while (cur < bodyEnd) {
    const auto eq  = buf.find('=', cur);
    if (eq == std::string_view::npos || eq >= bodyEnd) break;
    const auto soh = buf.find(SOH, eq);
    if (soh == std::string_view::npos || soh > bodyEnd) break;

    std::string_view tsv{buf.data() + cur, eq - cur};
    auto ti = sv_to_int(tsv);
    if (!ti) return {Status::BadFrame, "non-numeric tag", 0, 0, {}};
    const int tag = *ti;

    std::string_view val{buf.data() + eq + 1, soh - (eq + 1)};
    out.set_view(tag, val);

    cur = soh + 1;
  }

  // Save trailer checksum field
  out.set_view(CheckSum, std::string_view{buf.data() + bodyEnd + 3, 3});

  const std::size_t frame_size = bodyEnd + 7;
  return {Status::Ok, nullptr, 0, frame_size, std::string_view{buf.data(), frame_size}};
}

} // namespace fix::core
