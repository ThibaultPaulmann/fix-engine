#pragma once
#include <string>
#include <chrono>

namespace fix::engine {

struct SessionConfig {
  std::string sender_comp_id;
  std::string target_comp_id;
  std::chrono::seconds heart_bt_int{30};

  bool require_appl_ver_id_from_server{false}; // LSEG nuance
  bool reset_on_logon{false};                  // expect 141=Y handshake
};

} // namespace fix::engine
