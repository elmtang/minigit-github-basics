#include "RepositoryPaths.hpp"
#include <stdexcept>
#include <utility>

namespace minigit {

    RepositoryPaths::RepositoryPaths(std::filesystem::path projectRoot)
        : projectRoot_(std::move(projectRoot)),
          metadataRoot_(projectRoot_ / ".minigit") {}

    const std::filesystem::path& RepositoryPaths::projectRoot() const {
        return projectRoot_;
    }

    const std::filesystem::path& RepositoryPaths::metadataRoot() const {
        return metadataRoot_;
    }
    // Returns the path to a file inside the staging area of the repository.
    std::filesystem::path RepositoryPaths::stageFile(
         const std::string& path) const {
            return metadataRoot_/"stage" / path;
         }
   //Issue 13: Returns the path to a checkpoint directory inside 
   //the metadata directory of the repository.
    std::filesystem::path RepositoryPaths::checkpointDirectory(const std::string& id) const {
       if (id.empty())
       throw std::invalid_argument("checkpointDirectory ID must not be empty");
       return metadataRoot() / "commits" / id; 
    }

} // namespace minigit