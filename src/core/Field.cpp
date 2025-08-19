#include "core/Field.hpp"

#include <charconv>

namespace fix::core
{

  std::optional<int> sv_to_int(std::string_view sv)
  {
    int out{};
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), out);
    if (ec != std::errc{} || ptr != sv.data() + sv.size())
      return std::nullopt;
    return out;
  }

  std::optional<long long> sv_to_ll(std::string_view sv)
  {
    long long out{};
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), out);
    if (ec != std::errc{} || ptr != sv.data() + sv.size())
      return std::nullopt;
    return out;
  }

  std::optional<bool> sv_to_bool(std::string_view sv)
  {
    if (sv.size() != 1)
      return std::nullopt;
    if (sv[0] == 'Y')
      return true;
    if (sv[0] == 'N')
      return false;
    return std::nullopt;
  }

} // namespace fix::core