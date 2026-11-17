#include "FileSystem.hpp"

#include <algorithm>
#include <fstream>
#include <ios>
#include <limits>

namespace minigit {

void FileSystem::writeBytes(const std::filesystem::path& path, const std::vector<char>& data) const {
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        throw std::ios_base::failure("Unable to open file for binary writing");
    }

    const auto maxChunk = static_cast<std::size_t>(std::numeric_limits<std::streamsize>::max());
    std::size_t offset = 0;
    while (offset < data.size()) {
        const auto chunk = std::min(maxChunk, data.size() - offset);
        file.write(data.data() + offset, static_cast<std::streamsize>(chunk));
        if (!file) {
            throw std::ios_base::failure("Unable to write all binary data");
        }
        offset += chunk;
    }

    file.close();
    if (!file) {
        throw std::ios_base::failure("Unable to close file after binary writing");
    }
}

} // namespace minigit
