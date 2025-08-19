#pragma once
#include <string_view>

#include "core/Message.hpp"

namespace fix::core
{

  class Parser
  {
  public:
    static constexpr char SOH = '\x01';

    enum class Status
    {
      NeedMore,
      Ok,
      BadFrame
    };

    struct Result
    {
      Status status{Status::NeedMore};
      const char *error{nullptr}; // set on BadFrame
      int error_tag{0};           // best-effort offending tag
      std::size_t frame_size{0};  // bytes from start of view that form the full frame
      std::string_view frame{};   // view of the full frame [0 .. frame_size)
    };

    // Parse a FIX frame that starts at buf.data().
    // Precondition: buf starts at "8=" (caller hands the message start).
    // Postcondition: on Ok, 'out' holds zero-copy field views into 'buf'; those views
    // must not be used after 'buf' is invalidated by the caller.
    static Result parse(std::string_view buf, Message &out);
  };

} // namespace fix::core
