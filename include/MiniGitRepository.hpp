#pragma once

#include "RepositoryPaths.hpp"

namespace minigit {

class MiniGitRepository {
public:
    explicit MiniGitRepository(RepositoryPaths paths);
    bool isInitialized() const;

private:
    RepositoryPaths paths_;
};

} // namespace minigit
