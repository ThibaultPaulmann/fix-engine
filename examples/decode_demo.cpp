#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#include <charconv>
#include <algorithm>

#include "core/Parser.hpp"
#include "core/Header.hpp"
#include "core/FieldTags.hpp"
#include "core/MessageType.hpp"
#include "protocol/fixt11/AdminViews.hpp"

static constexpr char SOH = '\x01';

// --- tiny, demo-only frame builder (computes 9 and 10) ---
static int checksum(std::string_view s) {
    int sum = 0; for (unsigned char c : s) sum = (sum + c) & 0xFF; return sum;
}
static std::string build_frame(std::vector<std::pair<int, std::string>> fields) {
    using namespace fix::core::tags;
    std::string out;

    auto it8 = std::find_if(fields.begin(), fields.end(), [](auto& p){ return p.first==BeginString; });
    if (it8 == fields.end()) throw std::runtime_error("BeginString(8) missing");
    out += "8=" + it8->second + SOH;

    std::size_t pos9 = out.size();
    out += "9=000"; out += SOH;

    for (auto& [tag, val] : fields) {
        if (tag==BeginString || tag==BodyLength) continue;
        out += std::to_string(tag); out += '='; out += val; out += SOH;
    }

    auto pos9soh = out.find(SOH, pos9);
    int body_len = static_cast<int>(out.size() - (pos9soh + 1));
    {
        char buf[16]; auto [ptr, ec] = std::to_chars(buf, buf+sizeof(buf), body_len);
        if (ec != std::errc{}) throw std::runtime_error("to_chars body_len failed");
        std::string digits(buf, ptr);
        std::string rep = "9=" + digits + std::string(1, SOH);
        out.replace(pos9, 5, rep);
    }

    int cs = checksum(out);
    char csbuf[4]; auto [ptr, ec] = std::to_chars(csbuf, csbuf+sizeof(csbuf), cs);
    if (ec != std::errc{}) throw std::runtime_error("to_chars checksum failed");
    std::string cs3(csbuf, ptr);
    if (cs3.size() < 3) cs3.insert(cs3.begin(), 3 - cs3.size(), '0');
    out += "10="; out += cs3; out += SOH;

    return out;
}

int main() {
    using namespace fix::core;
    using namespace fix::core::tags;

    // Build a sample Logon frame
    std::vector<std::pair<int, std::string>> fields = {
        {BeginString, "FIXT.1.1"},
        {MsgType,     std::string{fix::core::msg::Logon}},
        {SenderCompID, "CLIENT1"},
        {TargetCompID, "FGW"},
        {MsgSeqNum,    "1"},
        {SendingTime,  "20250101-12:00:00.000"},
        {EncryptMethod,"0"},
        {HeartBtInt,   "30"},
        {DefaultApplVerID, "9"}
    };
    std::string wire = build_frame(std::move(fields));

    // Decode
    Message m;
    auto res = Parser::parse(std::string_view{wire}, m);
    if (res.status != Parser::Status::Ok) {
        std::cerr << "Parse error: " << (res.error?res.error:"?") << " tag=" << res.error_tag << "\n";
        return 1;
    }

    auto hv = header(m);
    std::cout << "BeginString: " << hv.begin_string().value_or("?") << "\n";
    std::cout << "MsgType:     " << hv.msg_type().value_or("?") << "\n";
    std::cout << "SenderCompID:" << hv.sender_comp_id().value_or("?") << "\n";
    std::cout << "TargetCompID:" << hv.target_comp_id().value_or("?") << "\n";
    std::cout << "MsgSeqNum:   " << hv.msg_seq_num().value_or(-1) << "\n";

    if (fix::protocol::fixt11::LogonView::is(m)) {
        fix::protocol::fixt11::LogonView v{m};
        std::cout << "HeartBtInt:  " << v.heart_bt_int().value_or(-1) << "\n";
        std::cout << "DefaultAppl: " << v.default_appl_ver_id().value_or("") << "\n";
    }
    return 0;
}
