#pragma once
#include <string_view>

namespace fix::core::msg
{
  using Type = std::string_view;

  inline constexpr Type Logon = "A";
  inline constexpr Type Logout = "5";
  inline constexpr Type Heartbeat = "0";
  inline constexpr Type TestRequest = "1";
  inline constexpr Type ResendRequest = "2";
  inline constexpr Type Reject = "3";
  inline constexpr Type SequenceReset = "4";

  // Application messages: order handling (client-initiated)
  inline constexpr Type NewOrderSingle = "D";
  inline constexpr Type OrderCancelRequest = "F";
  inline constexpr Type OrderCancelReplaceRequest = "G";
  inline constexpr Type OrderMassCancelRequest = "q";
  inline constexpr Type NewOrderCross = "s";
  inline constexpr Type CrossOrderCancelRequest = "u";

  // Application messages: order handling (server-initiated)
  inline constexpr Type ExecutionReport = "8";
  inline constexpr Type OrderCancelReject = "9";
  inline constexpr Type OrderMassCancelReport = "r";

  // Application messages: quote handling (client-initiated)
  inline constexpr Type Quote = "S";
  inline constexpr Type QuoteCancel = "Z";
  inline constexpr Type QuoteRequest = "R";
  inline constexpr Type QuoteRequestReject = "AG";
  inline constexpr Type QuoteResponse = "AJ";

  // Application messages: quote handling (server-initiated)
  inline constexpr Type QuoteStatusReport = "AI";
  inline constexpr Type MassQuoteAcknowledgement = "b";
  inline constexpr Type QuoteAck = "CW";

  // Application messages: other
  inline constexpr Type BusinessMessageReject = "j";

  constexpr bool is_admin(Type t) noexcept
  {
    return t == Logon || t == Logout || t == Heartbeat || t == TestRequest ||
           t == ResendRequest || t == Reject || t == SequenceReset;
  }

  constexpr bool is_application(Type t) noexcept
  {
    return !is_admin(t);
  }
}
