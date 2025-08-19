#include <cstdlib>
#include <iostream>
#include <string>
#include "core/Parser.hpp"
#include "core/FieldTags.hpp"
#include "helpers/FrameBuilder.hpp"

int main()
{
  using namespace fix::core;
  using namespace fix::core::tags;

  // --- OK parse ---
  auto ok = fix::test::build_frame({{BeginString, "FIXT.1.1"},
                                    {MsgType, "A"},
                                    {SenderCompID, "C1"},
                                    {TargetCompID, "S1"},
                                    {MsgSeqNum, "1"},
                                    {SendingTime, "20250101-12:00:00.000"},
                                    {EncryptMethod, "0"},
                                    {HeartBtInt, "30"},
                                    {DefaultApplVerID, "9"}});

  Message m;
  auto res = Parser::parse(std::string_view(ok), m);
  if (res.status != Parser::Status::Ok)
  {
    std::cerr << "Expected Ok, got error: " << (res.error ? res.error : "") << " tag=" << res.error_tag << "\n";
    return EXIT_FAILURE;
  }
  if (m.get_sv(MsgType).value_or("") != "A")
    return EXIT_FAILURE;

  // --- NeedMore (truncate tail) ---
  std::string cut = ok.substr(0, ok.size() - 3);
  res = Parser::parse(std::string_view(cut), m);
  if (res.status != Parser::Status::NeedMore)
  {
    std::cerr << "Expected NeedMore\n";
    return EXIT_FAILURE;
  }

  // --- BadFrame (checksum) ---
  // flip one checksum digit
  auto bad = ok;
  auto pos10 = bad.find("10=");
  if (pos10 != std::string::npos && pos10 + 3 < bad.size())
  {
    // increase first checksum digit modulo 10
    if (bad[pos10 + 3] >= '0' && bad[pos10 + 3] <= '8')
      bad[pos10 + 3] += 1;
    else if (bad[pos10 + 3] == '9')
      bad[pos10 + 3] = '0';
  }
  res = Parser::parse(std::string_view(bad), m);
  if (res.status != Parser::Status::BadFrame)
  {
    std::cerr << "Expected BadFrame (checksum)\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
