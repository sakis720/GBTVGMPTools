/*
        PKG Extractor made for:

    Ghostbusters The Video (PS3 & Xbox 360)
    Kinect Star Wars

    This project was made possible with "kinect_star_wars.bms"

    by sakis720
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstring>

namespace fs = std::filesystem;

void extract_pkg(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return;
    }

    fs::path input_path(filepath);
    std::string folder_name = input_path.stem().string();
    fs::path output_dir = input_path.parent_path() / folder_name;
    fs::create_directories(output_dir);

    // Get file size
    file.seekg(0, std::ios::end);
    size_t pkg_size = file.tellg();
    file.seekg(0, std::ios::beg);

    //std::cout << "File size: " << pkg_size << " bytes\n";

    size_t offset = 0;
    int file_count = 0; // to count extracted files

    while (offset < pkg_size) {
        //std::cout << "Processing offset: " << offset << "\n";

        char id[4];
        file.read(id, 4);
        if (file.gcount() < 4 || std::strncmp(id, "adoY", 4) != 0) {
            std::cerr << "Error: Invalid ID string at offset " << offset << std::endl;
            break;
        }

        char type[5] = { 0 };
        file.read(type, 4);

        uint32_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));

        char name[0x54 + 1] = { 0 };
        file.read(name, 0x54);

        // save the current offset for extraction
        size_t data_offset = file.tellg();

        //std::cout << "Found file: " << name << ", size: " << size << " bytes, type: " << type << "\n";

        // if size is not zero, extract the data
        if (size > 0) {
            fs::path output_path = output_dir / name;

            fs::create_directories(output_path.parent_path());

            // create and write to the output file
            std::ofstream output_file(output_path.string(), std::ios::binary);
            if (!output_file.is_open()) {
                std::cerr << "Error: Could not create output file " << output_path << std::endl;
                break;
            }

            std::vector<char> buffer(size);
            file.read(buffer.data(), size);
            output_file.write(buffer.data(), size);
            output_file.close();

            std::cout << "Extracted: " << name << std::endl;
            file_count++;
        }

        // move to the next entry
        offset = data_offset + size;
        offset = ((offset + 0x1F) / 0x20) * 0x20;  // align to the next 0x20 boundary
        file.seekg(offset, std::ios::beg);

        if (offset >= pkg_size) {
            std::cout << "Reached end of file.\n";
            break;
        }
    }

    std::cout << "Extraction complete. Total files extracted: " << file_count << "\n";
    file.close();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: drag and drop a .pkg file onto this executable." << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        std::string filepath = argv[i];
        std::cout << "Processing file: " << filepath << std::endl;
        extract_pkg(filepath);
    }

    return 0;
}
