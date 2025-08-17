// engine/InMemoryStore.hpp
#pragma once
#include "engine/SessionStore.hpp"

namespace fix::engine {

class InMemoryStore final : public SessionStore {
public:
  explicit InMemoryStore(int in_start=1, int out_start=1)
    : in_(in_start), out_(out_start) {}

  int  next_incoming() const override { return in_; }
  int  next_outgoing() const override { return out_; }
  void set_next_incoming(int v) override { in_ = v; }
  void set_next_outgoing(int v) override { out_ = v; }

private:
  int in_{1};
  int out_{1};
};

} // namespace fix::engine
