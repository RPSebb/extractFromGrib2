#include <utils.hpp>

void createDirectory(std::string path) {
    try {
        if (!std::filesystem::is_directory(path) || !std::filesystem::exists(path)) {
            std::filesystem::create_directories(path);
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

float toMultipleOf(float value, float multiple) {
    return round(value / multiple) * multiple;
}

void getFilesRecursive(std::string path, std::vector<std::string>& files) {

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_regular_file(entry)) {
            files.push_back(entry.path());
        } else if (std::filesystem::is_directory(entry)) {
            getFilesRecursive(entry.path(), files);
        }
    }

}