#include "storage.hpp"

namespace storage {

void save(const std::string& filename, const json j)
{
    std::ofstream ofs(filename);
    if(!ofs.is_open())
        throw std::runtime_error("Could not open " + filename);

    ofs << j;
}

json load(const std::string& filename)
{
    std::ifstream ifs(filename);
    if(!ifs.is_open())
        throw std::runtime_error("Could not open " + filename);

    return json::parse(ifs);
}

} // namespace storage