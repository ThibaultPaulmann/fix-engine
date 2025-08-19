#include <cstdlib>
#include <string>
#include "core/Parser.hpp"
#include "core/FieldTags.hpp"
#include "core/MessageType.hpp"
#include "protocol/fix50/ApplicationViews.hpp"
#include "helpers/FrameBuilder.hpp"

int main()
{
  using namespace fix::core;
  using namespace fix::core::tags;

  auto frame = fix::test::build_frame({{BeginString, "FIXT.1.1"},
                                       {MsgType, "D"},
                                       {SenderCompID, "C1"},
                                       {TargetCompID, "S1"},
                                       {MsgSeqNum, "1"},
                                       {SendingTime, "20250101-12:00:00.000"},
                                       {ClOrdID, "ABC"},
                                       {Symbol, "IBM"},
                                       {Side, "1"},
                                       {TransactTime, "20250101-12:00:01.000"},
                                       {OrdType, "2"},
                                       {OrderQty, "100"},
                                       {Price, "10.5"}});

  Message m;
  auto res = Parser::parse(std::string_view(frame), m);
  if (res.status != Parser::Status::Ok)
    return EXIT_FAILURE;

  auto t = m.get_sv(MsgType).value_or("");
  if (fix::core::msg::is_admin(t))
    return EXIT_FAILURE;
  if (!fix::core::msg::is_application(t))
    return EXIT_FAILURE;
  if (!fix::protocol::fix50::NewOrderSingleView::is(m))
    return EXIT_FAILURE;

  fix::protocol::fix50::NewOrderSingleView v{m};
  if (v.order_qty().value_or("") != "100")
    return EXIT_FAILURE;
  if (v.price().value_or("") != "10.5")
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

