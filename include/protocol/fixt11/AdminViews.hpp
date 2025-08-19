#pragma once
#include <optional>
#include <string_view>
#include "core/Message.hpp"
#include "core/FieldTags.hpp"
#include "core/MessageType.hpp"

namespace fix::protocol::fixt11
{

  // Each view offers: static bool is(const Message&) and typed getters.

  struct LogonView
  {
    const fix::core::Message &m;
    static bool is(const fix::core::Message &m)
    {
      auto t = m.get_sv(fix::core::tags::MsgType);
      return t && *t == fix::core::msg::Logon;
    }
    std::optional<int> encrypt_method() const { return m.get_int(fix::core::tags::EncryptMethod); } // usually 0
    std::optional<int> heart_bt_int() const { return m.get_int(fix::core::tags::HeartBtInt); }
    std::optional<bool> reset_seqnum() const { return m.get_bool(fix::core::tags::ResetSeqNumFlag); }
    std::optional<std::string_view> password() const { return m.get_sv(fix::core::tags::Password); }
    std::optional<std::string_view> new_password() const { return m.get_sv(fix::core::tags::NewPassword); }
    std::optional<std::string_view> appl_ver_id() const { return m.get_sv(fix::core::tags::ApplVerID); }
    std::optional<std::string_view> default_appl_ver_id() const { return m.get_sv(fix::core::tags::DefaultApplVerID); }
  };

  struct LogoutView
  {
    const fix::core::Message &m;
    static bool is(const fix::core::Message &m)
    {
      auto t = m.get_sv(fix::core::tags::MsgType);
      return t && *t == fix::core::msg::Logout;
    }
    std::optional<int> session_status() const { return m.get_int(fix::core::tags::SessionStatus); }
    std::optional<std::string_view> text() const { return m.get_sv(fix::core::tags::Text); }
  };

  struct HeartbeatView
  {
    const fix::core::Message &m;
    static bool is(const fix::core::Message &m)
    {
      auto t = m.get_sv(fix::core::tags::MsgType);
      return t && *t == fix::core::msg::Heartbeat;
    }
    std::optional<std::string_view> test_req_id() const { return m.get_sv(fix::core::tags::TestReqID); }
  };

  struct TestRequestView
  {
    const fix::core::Message &m;
    static bool is(const fix::core::Message &m)
    {
      auto t = m.get_sv(fix::core::tags::MsgType);
      return t && *t == fix::core::msg::TestRequest;
    }
    std::optional<std::string_view> test_req_id() const { return m.get_sv(fix::core::tags::TestReqID); }
  };

  struct ResendRequestView
  {
    const fix::core::Message &m;
    static bool is(const fix::core::Message &m)
    {
      auto t = m.get_sv(fix::core::tags::MsgType);
      return t && *t == fix::core::msg::ResendRequest;
    }
    std::optional<int> begin_seq_no() const { return m.get_int(fix::core::tags::BeginSeqNo); }
    std::optional<int> end_seq_no() const { return m.get_int(fix::core::tags::EndSeqNo); }
  };

  struct RejectView
  {
    const fix::core::Message &m;
    static bool is(const fix::core::Message &m)
    {
      auto t = m.get_sv(fix::core::tags::MsgType);
      return t && *t == fix::core::msg::Reject;
    }
    std::optional<int> ref_seq_num() const { return m.get_int(fix::core::tags::RefSeqNum); }
    std::optional<std::string_view> ref_msg_type() const { return m.get_sv(fix::core::tags::RefMsgType); }
    std::optional<int> ref_tag_id() const { return m.get_int(fix::core::tags::RefTagID); }
    std::optional<int> reason() const { return m.get_int(fix::core::tags::SessionRejectReason); }
    std::optional<std::string_view> text() const { return m.get_sv(fix::core::tags::Text); }
  };

  struct SequenceResetView
  {
    const fix::core::Message &m;
    static bool is(const fix::core::Message &m)
    {
      auto t = m.get_sv(fix::core::tags::MsgType);
      return t && *t == fix::core::msg::SequenceReset;
    }
    std::optional<int> new_seq_no() const { return m.get_int(fix::core::tags::NewSeqNo); }
    std::optional<bool> gap_fill() const { return m.get_bool(fix::core::tags::GapFillFlag); }
  };

} // namespace fix::protocol::fixt11
