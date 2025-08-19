#pragma once

namespace fix::core::tags
{
  inline constexpr int BeginSeqNo = 7;
  inline constexpr int BeginString = 8;
  inline constexpr int BodyLength = 9;
  inline constexpr int CheckSum = 10;
  inline constexpr int EndSeqNo = 16;
  inline constexpr int MsgSeqNum = 34;
  inline constexpr int MsgType = 35;
  inline constexpr int NewSeqNo = 36;
  inline constexpr int PossDupFlag = 43;
  inline constexpr int RefSeqNum = 45;
  inline constexpr int SenderCompID = 49;
  inline constexpr int SendingTime = 52;
  inline constexpr int TargetCompID = 56;
  inline constexpr int Text = 58;
  inline constexpr int PossResend = 97;
  inline constexpr int EncryptMethod = 98;
  inline constexpr int HeartBtInt = 108;
  inline constexpr int TestReqID = 112;
  inline constexpr int OrigSendingTime = 122;
  inline constexpr int GapFillFlag = 123;
  inline constexpr int ResetSeqNumFlag = 141;
  inline constexpr int RefTagID = 371;
  inline constexpr int RefMsgType = 372;
  inline constexpr int SessionRejectReason = 373;
  inline constexpr int Password = 554;
  inline constexpr int NewPassword = 925;
  inline constexpr int ApplVerID = 1128;
  inline constexpr int DefaultApplVerID = 1137;
  inline constexpr int SessionStatus = 1409;
} // namespace fix::core::tags
