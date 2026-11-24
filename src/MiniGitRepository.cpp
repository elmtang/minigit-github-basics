#include "MiniGitRepository.hpp"

#include <system_error>
#include <utility>

namespace minigit {

MiniGitRepository::MiniGitRepository(RepositoryPaths paths)
    : paths_(std::move(paths)) {
}

bool MiniGitRepository::isInitialized() const {
    std::error_code error;
    if (!std::filesystem::exists(paths_.metadataRoot(), error) || error) {
        return false;
    }

    error.clear();
    if (!std::filesystem::exists(paths_.headFile(), error) || error) {
        return false;
    }

    error.clear();
    return std::filesystem::exists(paths_.indexFile(), error) && !error;
}

} // namespace minigit
