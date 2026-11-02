
#include "PathLab.hpp"

namespace minigit {

std::filesystem::path metadataDirectoryPath(
    const std::filesystem::path& root
) {
    return root / ".minigit";
}

} // namespace minigit
