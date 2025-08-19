#pragma once
#include <optional>
#include <string_view>
#include "core/Message.hpp"
#include "core/FieldTags.hpp"
#include "core/MessageType.hpp"

namespace fix::protocol::fix50
{
  // Views for common FIX 5.0 application messages.

  struct NewOrderSingleView
  {
    const fix::core::Message &m;
    static bool is(const fix::core::Message &m)
    {
      auto t = m.get_sv(fix::core::tags::MsgType);
      return t && *t == fix::core::msg::NewOrderSingle;
    }
    std::optional<std::string_view> cl_ord_id() const { return m.get_sv(fix::core::tags::ClOrdID); }
    std::optional<std::string_view> symbol() const { return m.get_sv(fix::core::tags::Symbol); }
    std::optional<std::string_view> side() const { return m.get_sv(fix::core::tags::Side); }
    std::optional<std::string_view> transact_time() const { return m.get_sv(fix::core::tags::TransactTime); }
    std::optional<std::string_view> ord_type() const { return m.get_sv(fix::core::tags::OrdType); }
    std::optional<std::string_view> order_qty() const { return m.get_sv(fix::core::tags::OrderQty); }
    std::optional<std::string_view> price() const { return m.get_sv(fix::core::tags::Price); }
  };

  struct ExecutionReportView
  {
    const fix::core::Message &m;
    static bool is(const fix::core::Message &m)
    {
      auto t = m.get_sv(fix::core::tags::MsgType);
      return t && *t == fix::core::msg::ExecutionReport;
    }
    std::optional<std::string_view> order_id() const { return m.get_sv(fix::core::tags::OrderID); }
    std::optional<std::string_view> cl_ord_id() const { return m.get_sv(fix::core::tags::ClOrdID); }
    std::optional<std::string_view> exec_id() const { return m.get_sv(fix::core::tags::ExecID); }
    std::optional<std::string_view> exec_type() const { return m.get_sv(fix::core::tags::ExecType); }
    std::optional<std::string_view> ord_status() const { return m.get_sv(fix::core::tags::OrdStatus); }
    std::optional<std::string_view> leaves_qty() const { return m.get_sv(fix::core::tags::LeavesQty); }
    std::optional<std::string_view> cum_qty() const { return m.get_sv(fix::core::tags::CumQty); }
    std::optional<std::string_view> avg_px() const { return m.get_sv(fix::core::tags::AvgPx); }
  };
}

