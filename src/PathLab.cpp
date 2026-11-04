
#include "PathLab.hpp"
#include <fstream>
#include <system_error>
#include <iterator>

namespace minigit {

std::filesystem::path metadataDirectory(
    const std::filesystem::path& root) {
    return root / ".minigit";
}

std::filesystem::path headFilePath(
    const std::filesystem::path& root) {
    return metadataDirectory(root) / "HEAD";
}
//  This function is used to write a text file to the specified path. It creates any
// necessary parent directories if they exist. It returns true if the file was written successfully, 
// and false otherwise.
bool writeTextFile(const std::filesystem::path& filePath,
                   const std::string& text) {
    const auto parent = filePath.parent_path();
    if (!parent.empty()) {
        std::error_code error;
        std::filesystem::create_directories(parent, error);
        if (error) {
            return false;
        }
    }

    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        return false;
    }

    file << text;
    file.close();
    return !file.fail();
}

// Issue 4: Read the contents of a text file.
// Return false if the file is missing or cannot be read.
bool readTextFile(const std::filesystem::path& filePath,
                  std::string& text) {
    std::ifstream file(filePath, std::ios::binary);

    if (!file) {
        text.clear();
        return false;
    }

    std::string contents{
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    };

    if (file.bad()) {
        text.clear();
        return false;
    }

    text = contents;
    return true;
}

} // namespace minigit
