#pragma once
#include <string_view>

namespace fix::core::msg {
using Type = std::string_view;

inline constexpr Type Logon = "A";
inline constexpr Type Logout = "5";
inline constexpr Type Heartbeat = "0";
inline constexpr Type TestRequest = "1";
inline constexpr Type ResendRequest = "2";
inline constexpr Type Reject = "3";
inline constexpr Type SequenceReset = "4";

constexpr bool is_admin(Type t) noexcept {
  return t == Logon || t == Logout || t == Heartbeat || t == TestRequest ||
         t == ResendRequest || t == Reject || t == SequenceReset;
}
}  // namespace fix::core::msg