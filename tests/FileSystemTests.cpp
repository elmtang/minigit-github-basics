#include "FileSystem.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main()
{
    const auto testRoot = std::filesystem::temp_directory_path() /
                          ("minigit-write-bytes-test-" +
                           std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    const auto testFile = testRoot / "binary.dat";
    const std::vector<char> expected = {'M', '\0', static_cast<char>(0xFF), '\n', '\r', 'G'};
    minigit::FileSystem fileSystem;

    try
    {
        std::filesystem::create_directories(testRoot);
        fileSystem.writeBytes(testFile, expected);

        std::vector<char> actual;
        bool readFailed = false;
        {
            std::ifstream input(testFile, std::ios::binary);
            const std::istreambuf_iterator<char> begin(input);
            const std::istreambuf_iterator<char> end;
            actual.assign(begin, end);
            readFailed = input.bad();
        }
        if (readFailed || actual != expected)
        {
            std::cerr << "writeBytes did not preserve the exact binary contents\n";
            std::filesystem::remove_all(testRoot);
            return 1;
        }

        const auto blocker = testRoot / "not-a-directory";
        {
            std::ofstream blockerFile(blocker, std::ios::binary);
            blockerFile << "file";
        }

        bool threw = false;
        try
        {
            fileSystem.writeBytes(blocker / "target.bin", expected);
        }
        catch (const std::ios_base::failure &)
        {
            threw = true;
        }
        std::filesystem::remove_all(testRoot);

        if (!threw)
        {
            std::cerr << "writeBytes should throw when the destination cannot be opened\n";
            return 1;
        }
    }
    catch (const std::exception &error)
    {
        std::filesystem::remove_all(testRoot);
        std::cerr << "writeBytes test failed: " << error.what() << '\n';
        return 1;
    }

    std::cout << "writeBytes binary and failure tests passed\n";
    return 0;
}
