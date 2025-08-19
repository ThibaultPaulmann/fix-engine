#pragma once
#include <string_view>

namespace fix::protocol::fixt11
{
  inline constexpr std::string_view BeginString = "FIXT.1.1";
  inline constexpr std::string_view DefaultApplVerID_Fix50SP2 = "9";
} // namespace fix::protocol::fixt11
