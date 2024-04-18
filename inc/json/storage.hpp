#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

namespace storage {

using json = nlohmann::json;

void save(const std::string& filename, const json j);

json load(const std::string& filename);

} // namespace storage