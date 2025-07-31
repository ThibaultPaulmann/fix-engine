#pragma once
#include <string>
#include <optional>

namespace fix::core {

enum class MsgType {
    Logon,
    Logout,
    Heartbeat,
    TestRequest,
    NewOrderSingle,
    ExecutionReport,
    Unknown
};

std::optional<MsgType> parseMsgType(const std::string& value);
std::string toString(MsgType type);

}
