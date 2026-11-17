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

        // FS-4: readBytes
    const auto readRoot = std::filesystem::temp_directory_path() /
        ("minigit-read-bytes-test-" +
         std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    try {
        std::filesystem::create_directories(readRoot);

        // Read a normal text file
        const auto textFile = readRoot / "text.txt";
        const std::string text = "hello\nworld";
        {
            std::ofstream out(textFile, std::ios::binary);
            out << text;
        }
        const std::vector<char> expectedText(text.begin(), text.end());
        if (fileSystem.readBytes(textFile) != expectedText) {
            std::cerr << "readBytes did not return the text file contents\n";
            std::filesystem::remove_all(readRoot);
            return 1;
        }

        // Read an empty file
        const auto emptyFile = readRoot / "empty.txt";
        {
            std::ofstream out(emptyFile, std::ios::binary);
        }
        if (!fileSystem.readBytes(emptyFile).empty()) {
            std::cerr << "readBytes should return an empty vector for an empty file\n";
            std::filesystem::remove_all(readRoot);
            return 1;
        }

        // A missing file should throw
        bool threw = false;
        try {
            fileSystem.readBytes(readRoot / "missing.txt");
        } catch (const std::ios_base::failure&) {
            threw = true;
        }
        if (!threw) {
            std::cerr << "readBytes should throw for a missing file\n";
            std::filesystem::remove_all(readRoot);
            return 1;
        }
    } catch (const std::exception& error) {
        std::filesystem::remove_all(readRoot);
        std::cerr << "readBytes test failed: " << error.what() << '\n';
        return 1;
    }
    std::filesystem::remove_all(readRoot);
    std::cout << "readBytes tests passed\n";


    return 0;
}
