#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#include <charconv>
#include <algorithm>

#include "engine/Session.hpp"
#include "engine/InMemoryStore.hpp"
#include "engine/ISessionHandler.hpp"
#include "core/FieldTags.hpp"

// mini frame builder (same as decode_demo)
static constexpr char SOH = '\x01';
static int checksum(std::string_view s){int sum=0;for(unsigned char c:s)sum=(sum+c)&0xFF;return sum;}
static std::string build_frame(const std::vector<std::pair<int,std::string>>& fields_in) {
  using namespace fix::core::tags;
  std::string out;
  auto it8 = std::find_if(fields_in.begin(), fields_in.end(), [](auto& p){return p.first==BeginString;});
  if (it8==fields_in.end()) throw std::runtime_error("8 missing");
  out += "8=" + it8->second + SOH;
  std::size_t pos9 = out.size(); out += "9=000"; out += SOH;
  for (auto& [tag,val] : fields_in) { if (tag==BeginString || tag==BodyLength) continue; out += std::to_string(tag); out+='='; out+=val; out+=SOH; }
  auto e9 = out.find(SOH,pos9);
  int body_len = static_cast<int>(out.size()-(e9+1));
  char buf[16]; auto [ptr,ec]=std::to_chars(buf,buf+sizeof(buf),body_len);
  std::string digits(buf,ptr);
  out.replace(pos9,5,"9="+digits+std::string(1,SOH));
  int cs = checksum(out); char csb[4]; auto [p2,ec2]=std::to_chars(csb,csb+sizeof(csb),cs);
  std::string cs3(csb,p2); if (cs3.size()<3) cs3.insert(cs3.begin(),3-cs3.size(),'0');
  out += "10="+cs3+std::string(1,SOH);
  return out;
}

// Simple handler printing events
struct DemoHandler : fix::engine::ISessionHandler {
  void on_bad_frame(const char* what, int tag) override {
    std::cout << "Bad frame: " << (what?what:"?") << " tag=" << tag << "\n";
  }
  void on_header_violation(int tag, const char* what) override {
    std::cout << "Header violation tag="<<tag<<" "<<what<<"\n";
  }
  void on_gap_detected(int exp, int got) override {
    std::cout << "Gap: expected "<<exp<<" got "<<got<<"\n";
  }
  void on_poss_dup(int seq) override { std::cout << "PossDup: seq="<<seq<<"\n"; }
  void on_seq_too_low(int exp, int got) override {
    std::cout << "Too low: expected "<<exp<<" got "<<got<<"\n";
  }
  void on_logon(const fix::protocol::fixt11::LogonView&) override { std::cout<<"Logon\n"; }
  void on_logout(const fix::protocol::fixt11::LogoutView&) override { std::cout<<"Logout\n"; }
  void on_heartbeat(const fix::protocol::fixt11::HeartbeatView&) override { std::cout<<"Heartbeat\n"; }
  void on_test_request(const fix::protocol::fixt11::TestRequestView&) override { std::cout<<"TestRequest\n"; }
  void on_resend_request(const fix::protocol::fixt11::ResendRequestView&) override { std::cout<<"ResendRequest\n"; }
  void on_reject(const fix::protocol::fixt11::RejectView&) override { std::cout<<"Reject\n"; }
  void on_sequence_reset(const fix::protocol::fixt11::SequenceResetView&) override { std::cout<<"SequenceReset\n"; }
  void on_application_message(std::string_view mt, const fix::core::Message&) override {
    std::cout<<"App message "<<mt<<"\n";
  }
};

int main() {
  using namespace fix::core::tags;
  fix::engine::SessionConfig cfg{.sender_comp_id="CLIENT1", .target_comp_id="FGW"};
  fix::engine::InMemoryStore store;
  DemoHandler h;
  fix::engine::Session session(cfg, store, h);

  // 1) Logon seq=1
  auto f1 = build_frame({
    {BeginString,"FIXT.1.1"},{MsgType,"A"},{SenderCompID,"CLIENT1"},{TargetCompID,"FGW"},
    {MsgSeqNum,"1"},{SendingTime,"20250101-12:00:00.000"},{EncryptMethod,"0"},{HeartBtInt,"30"},{DefaultApplVerID,"9"}
  });
  session.on_frame(f1);

  // 2) Heartbeat seq=2
  auto f2 = build_frame({
    {BeginString,"FIXT.1.1"},{MsgType,"0"},{SenderCompID,"CLIENT1"},{TargetCompID,"FGW"},
    {MsgSeqNum,"2"},{SendingTime,"20250101-12:00:00.500"}
  });
  session.on_frame(f2);
  return 0;
}
