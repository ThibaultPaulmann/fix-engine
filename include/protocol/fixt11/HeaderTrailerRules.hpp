#pragma once
#include <vector>
#include "core/Message.hpp"
#include "core/FieldTags.hpp"

namespace fix::protocol::fixt11
{

  struct HeaderTrailerRules
  {
    struct Violation
    {
      int tag;
      const char *what;
    };

    // Minimal session-layer header/trailer checks for decoded messages.
    static std::vector<Violation> validate(const fix::core::Message &m,
                                           bool outbound_from_server = false);
  };

} // namespace fix::protocol::fixt11
