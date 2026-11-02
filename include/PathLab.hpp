
#pragma once

#include <filesystem>

namespace minigit {

// Returns the metadata directory path inside the repository.
std::filesystem::path metadataDirectoryPath(
    const std::filesystem::path& root
);

} // namespace minigit
