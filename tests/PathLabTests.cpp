#include "PathLab.hpp"
#include <iostream>
int main()
{
    // Baseline smoke check: the project and CTest wiring work before students
    // begin the path Issues. Add focused assertions with each Issue.
    std::filesystem::path sample = "course/repo";
    if (sample.filename() != "repo")
    {
        std::cerr << "path smoke check failed\n";
        return 1;
    }
    std::cout << "PathLab smoke check passed\n";

    // Issue 1: ordinary root
    std::filesystem::path result = minigit::metadataDirectory("project");
    if (result != std::filesystem::path("project") / ".minigit")
    {
        std::cerr << "ordinary root test failed\n";
        return 1;
    }
    std::cout << "ordinary root test passed\n";

    // Issue 1: nested root
    std::filesystem::path nested = minigit::metadataDirectory("course/team/project");
    if (nested != std::filesystem::path("course") / "team" / "project" / ".minigit")
    {
        std::cerr << "nested root test failed\n";
        return 1;
    }
    std::cout << "nested root test passed\n";

    // Issue 2: ordinary root
    std::filesystem::path head = minigit::headFilePath("project");
    if (head != std::filesystem::path("project") / ".minigit" / "HEAD")
    {
        std::cerr << "ordinary HEAD path test failed\n";
        return 1;
    }
    std::cout << "ordinary HEAD path test passed\n";

    // Issue 2: nested root
    std::filesystem::path nestedHead = minigit::headFilePath("course/team/project");
    if (nestedHead != std::filesystem::path("course") / "team" / "project" / ".minigit" / "HEAD")
    {
        std::cerr << "nested HEAD path test failed\n";
        return 1;
    }
    std::cout << "nested HEAD path test passed\n";

    return 0;
}