#pragma once
#include <string>
#include <vector>
#include <utility>

namespace fix::test {

// Build a valid FIX frame string from (tag,value) pairs.
// Computes BodyLength(9) and CheckSum(10). Assumes BeginString(8) present.
std::string build_frame(const std::vector<std::pair<int, std::string>>& fields);

} // namespace fix::test
