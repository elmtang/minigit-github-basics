#pragma once

#include <filesystem>
#include <vector>

namespace minigit {

class FileSystem {
public:
    void writeBytes(const std::filesystem::path& path, const std::vector<char>& data) const;
};

} // namespace minigit
