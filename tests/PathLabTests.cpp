
#include "PathLab.hpp"

#include <filesystem>
#include <iostream>

int main() {
    // Test the existing path smoke check.
    std::filesystem::path sample = "course/repo";

    if (sample.filename() != "repo") {
        std::cerr << "path smoke check failed\n";
        return 1;
    }

    // Test an ordinary repository path.
    std::filesystem::path ordinaryRoot = "project";

    std::filesystem::path ordinaryResult =
        minigit::metadataDirectoryPath(ordinaryRoot);

    if (ordinaryResult != ordinaryRoot / ".minigit") {
        std::cerr << "ordinary metadata path test failed\n";
        return 1;
    }

    // Test a nested repository path.
    std::filesystem::path nestedRoot = "course/projects/repo";

    std::filesystem::path nestedResult =
        minigit::metadataDirectoryPath(nestedRoot);

    if (nestedResult != nestedRoot / ".minigit") {
        std::cerr << "nested metadata path test failed\n";
        return 1;
    }

    std::cout << "All PathLab tests passed\n";
    return 0;
}
