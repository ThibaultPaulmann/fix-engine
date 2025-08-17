#pragma once
#include <string_view>
#include <vector>

#include "Field.hpp"

namespace fix::core {

class Message {
 public:
  using Fields = std::vector<Field>;
  using iterator = Fields::iterator;
  using const_iterator = Fields::const_iterator;

  Message() = default;
  Message(const Message&) = default;
  Message(Message&&) noexcept = default;
  Message& operator=(const Message&) = default;
  Message& operator=(Message&&) noexcept = default;

  void clear();
  void reserve(std::size_t n);

  // Append a field view (value must outlive this Message; typically points into
  // Parser buffer).
  void set_view(int tag, std::string_view value);

  // Accessors. If a tag repeats, getters return the first occurrence.
  [[nodiscard]] const Field* find(int tag) const noexcept;
  [[nodiscard]] std::optional<std::string_view> get_sv(int tag) const noexcept;
  [[nodiscard]] std::optional<int> get_int(int tag) const noexcept;
  [[nodiscard]] std::optional<long long> get_ll(int tag) const noexcept;
  [[nodiscard]] std::optional<bool> get_bool(int tag) const noexcept;

  // Iteration over all fields in wire order.
  iterator begin() noexcept;
  iterator end() noexcept;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;

  [[nodiscard]] std::size_t size() const noexcept { return fields_.size(); }
  [[nodiscard]] const Fields& fields() const noexcept { return fields_; }

 private:
  Fields fields_;
};
}  // namespace fix::core
