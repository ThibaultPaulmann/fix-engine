#pragma once

namespace fix::core::tags {

inline constexpr int BeginString = 8; // Always first field in a message
inline constexpr int BodyLength = 9; // Always second field in a message
inline constexpr int MsgType = 35; //
inline constexpr int CheckSum = 10; // Always last field in a message

}
