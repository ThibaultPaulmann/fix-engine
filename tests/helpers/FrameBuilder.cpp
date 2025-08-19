#include "helpers/FrameBuilder.hpp"
#include "core/FieldTags.hpp"
#include <charconv>
#include <string_view>
#include <algorithm>
#include <stdexcept>

namespace fix::test
{
  static constexpr char SOH = '\x01';

  static int checksum(std::string_view s)
  {
    int sum = 0;
    for (unsigned char c : s)
      sum = (sum + c) & 0xFF;
    return sum;
  }

  std::string build_frame(const std::vector<std::pair<int, std::string>> &fields_in)
  {
    using namespace fix::core::tags;
    std::string out;

    // 8=...
    auto it8 = std::find_if(fields_in.begin(), fields_in.end(), [](auto &p)
                            { return p.first == BeginString; });
    if (it8 == fields_in.end())
      throw std::runtime_error("BeginString(8) missing");
    out += "8=" + it8->second + SOH;

    // 9= placeholder
    std::size_t pos9 = out.size();
    out += "9=000";
    out += SOH;

    // rest
    for (auto &[tag, val] : fields_in)
    {
      if (tag == BeginString || tag == BodyLength)
        continue;
      out += std::to_string(tag);
      out += '=';
      out += val;
      out += SOH;
    }

    // patch 9=
    auto pos9soh = out.find(SOH, pos9);
    int body_len = static_cast<int>(out.size() - (pos9soh + 1));
    {
      char buf[16];
      auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), body_len);
      if (ec != std::errc{})
        throw std::runtime_error("to_chars body_len failed");
      std::string digits(buf, ptr);
      std::string rep = "9=" + digits + std::string(1, SOH);
      out.replace(pos9, pos9soh - pos9 + 1, rep);
    }

    // append 10=
    int cs = checksum(out);
    char csbuf[4];
    auto [ptr, ec] = std::to_chars(csbuf, csbuf + sizeof(csbuf), cs);
    if (ec != std::errc{})
      throw std::runtime_error("to_chars checksum failed");
    std::string cs3(csbuf, ptr);
    if (cs3.size() < 3)
      cs3.insert(cs3.begin(), 3 - cs3.size(), '0');
    out += "10=";
    out += cs3;
    out += SOH;

    return out;
  }
} // namespace fix::test
