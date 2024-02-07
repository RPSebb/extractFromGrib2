#include <string>
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[]) {

    std::string filename;

    if(argc > 1) { filename = argv[1]; }
    else { exit(1); }

    std::ifstream file(filename.c_str(), std::ios::binary);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
    }
}