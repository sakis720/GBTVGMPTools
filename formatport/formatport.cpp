/*
                Format port made for:

    Ghostbusters The Video Multiplayer (PS3 & Xbox 360)

                        to

    Ghostbusters The Video Game Remastered

    by sakis720
*/
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

void modifyFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << filePath << std::endl;
        return;
    }

    // read the first byte
    char firstByte;
    file.read(&firstByte, 1);
    if (!file) {
        std::cerr << "Failed to read the first byte of the file." << std::endl;
        file.close();
        return;
    }
    file.close();

    std::string extension = std::filesystem::path(filePath).extension().string();
    bool modified = false;

    if (extension == ".tfb" && static_cast<unsigned char>(firstByte) == 0x08) {
        firstByte = 0x09;
        modified = true;
    } //else if (extension == ".bst" && static_cast<unsigned char>(firstByte) == 0x06) {
      //  firstByte = 0x08;
      //  modified = true;
    //}

    if (modified) {
        std::ofstream outFile(filePath, std::ios::binary | std::ios::in);
        if (!outFile.is_open()) {
            std::cerr << "Failed to open the file for writing: " << filePath << std::endl;
            return;
        }
        outFile.seekp(0);
        outFile.write(&firstByte, 1);
        if (!outFile) {
            std::cerr << "Failed to write to the file." << std::endl;
        } else {
            std::cout << "Modified the first byte of the file: " << filePath << std::endl;
        }
        outFile.close();
    } else {
        std::cout << "No modification needed for the file: " << filePath << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "formatport | drag and drop one of the supported formats" << std::endl;
        std::cerr << "GB TVG Multiplayer (PS3 & Xbox 360) To GB TVG Remastered" << std::endl;
        std::cerr << "Supported formats:" << std::endl;
        std::cerr << ".tfb" << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        modifyFile(argv[i]);
    }

    return 0;
}
