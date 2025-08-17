#pragma once
#include <optional>
#include <string_view>

namespace fix::core {

struct Field {
  int tag{};
  std::string_view value{};
};

std::optional<int> sv_to_int(std::string_view);
std::optional<long long> sv_to_ll(std::string_view);
std::optional<bool> sv_to_bool(std::string_view);

}  // namespace fix::core
