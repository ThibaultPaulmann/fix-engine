#pragma once

namespace fix::engine {

// Minimal interface for sequence tracking. Swap out for a file-backed impl later.
class SessionStore {
public:
  virtual ~SessionStore() = default;

  virtual int next_incoming()  const = 0;  // expected inbound SeqNum
  virtual int next_outgoing()  const = 0;  // next SeqNum to assign outbound

  virtual void set_next_incoming(int v) = 0;
  virtual void set_next_outgoing(int v) = 0;

  // optional: cache raw outbound frames for resend – omitted for decode-only phase
};

} // namespace fix::engine
