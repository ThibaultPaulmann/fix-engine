#pragma once
#include "Field.hpp"
#include <map>
#include <string>
#include <vector>
#include <optional>

namespace fix::core {

class Message {
public:
    void addField(int tag, const std::string& value);
    [[nodiscard]] std::optional<std::string> getField(int tag) const;
    [[nodiscard]] std::vector<Field> getFieldsInOrder() const;

private:
    std::map<int, std::string> fields_;
};

}
