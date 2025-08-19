#include "core/Message.hpp"

#include <algorithm>

namespace fix::core
{

  void Message::clear() { fields_.clear(); }

  void Message::reserve(std::size_t n) { fields_.reserve(n); }

  void Message::set_view(int tag, std::string_view value)
  {
    fields_.push_back(Field{tag, value});
  }

  const Field *Message::find(int tag) const noexcept
  {
    auto it = std::find_if(fields_.begin(), fields_.end(),
                           [tag](const Field &f)
                           { return f.tag == tag; });
    return it == fields_.end() ? nullptr : &*it;
  }

  std::optional<std::string_view> Message::get_sv(int tag) const noexcept
  {
    if (auto *f = find(tag))
      return f->value;
    return std::nullopt;
  }

  std::optional<int> Message::get_int(int tag) const noexcept
  {
    if (auto *f = find(tag))
      return sv_to_int(f->value);
    return std::nullopt;
  }

  std::optional<long long> Message::get_ll(int tag) const noexcept
  {
    if (auto *f = find(tag))
      return sv_to_ll(f->value);
    return std::nullopt;
  }

  std::optional<bool> Message::get_bool(int tag) const noexcept
  {
    if (auto *f = find(tag))
      return sv_to_bool(f->value);
    return std::nullopt;
  }

  Message::iterator Message::begin() noexcept { return fields_.begin(); }
  Message::iterator Message::end() noexcept { return fields_.end(); }
  Message::const_iterator Message::begin() const noexcept
  {
    return fields_.begin();
  }
  Message::const_iterator Message::end() const noexcept { return fields_.end(); }

} // namespace fix::core