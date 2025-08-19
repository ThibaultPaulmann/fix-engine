#pragma once
#include <optional>
#include <string_view>

#include "FieldTags.hpp"
#include "Message.hpp"
#include "MessageType.hpp"

namespace fix::core
{

  struct HeaderView
  {
    const Message &msg;

    std::optional<std::string_view> begin_string() const
    {
      return msg.get_sv(tags::BeginString);
    }

    std::optional<int> body_length() const
    {
      return msg.get_int(tags::BodyLength);
    }

    std::optional<std::string_view> msg_type() const
    {
      return msg.get_sv(tags::MsgType);
    }

    std::optional<std::string_view> sender_comp_id() const
    {
      return msg.get_sv(tags::SenderCompID);
    }

    std::optional<std::string_view> target_comp_id() const
    {
      return msg.get_sv(tags::TargetCompID);
    }

    std::optional<int> msg_seq_num() const
    {
      return msg.get_int(tags::MsgSeqNum);
    }

    std::optional<std::string_view> sending_time() const
    {
      return msg.get_sv(tags::SendingTime);
    }
    std::optional<bool> poss_dup() const
    {
      return msg.get_bool(tags::PossDupFlag);
    }

    std::optional<bool> poss_resend() const
    {
      return msg.get_bool(tags::PossResend);
    }

    std::optional<std::string_view> appl_ver_id() const
    {
      return msg.get_sv(tags::ApplVerID);
    }

    std::optional<std::string_view> default_appl() const
    {
      return msg.get_sv(tags::DefaultApplVerID);
    }
  };

  struct TrailerView
  {
    const Message &msg;
    std::optional<int> checksum() const { return msg.get_int(tags::CheckSum); }
  };

  // Convenience helpers
  inline HeaderView header(const Message &m) { return HeaderView{m}; }
  inline TrailerView trailer(const Message &m) { return TrailerView{m}; }

} // namespace fix::core