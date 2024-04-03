//
// created by kacchan on 2024/3/27
//

#include <cerrno>
#include <fstream>
#include "reader.h"
#include "../constants.h"

Reader::Reader(const std::string& filename) {
    path_ = filename;
}

size_t Reader::BytesToRead(unsigned char* bytes) {
    size_t number = *bytes;
    for (size_t i = 0; i < SHIFT_BITS.size(); ++i) {
        number += *(bytes + i + 1) << SHIFT_BITS[i];
    }
    return number;
}

size_t GetPaddingSize(size_t width) {
    return (PADDING_BYTES - (width * BYTES_PER_PIXEL) % PADDING_BYTES) % PADDING_BYTES;
}

Image Reader::Read() {
    unsigned char bmp_header[BMP_HEADER_SIZE];
    unsigned char dib_header[DIB_HEADER_SIZE];

    std::ifstream img;
    img.open(path_, std::ios::in | std::ios::binary);
    if (!img.is_open()) {
        throw std::invalid_argument(std::string("File ") + path_ + std::string(" not found"));
    }
    if (errno == EACCES) {
        throw std::invalid_argument(std::string("NO permission to read file ") + path_);
    }
    img.read(reinterpret_cast<char*>(bmp_header), BMP_HEADER_SIZE);
    if (bmp_header[0] != HEADER_SIGNATURE[0] || bmp_header[1] != HEADER_SIGNATURE[1]) {
        throw std::invalid_argument(std::string("File ") + path_ + std::string(" is not a BMP file"));
    }
    try {
        img.read(reinterpret_cast<char*>(dib_header), DIB_HEADER_SIZE);
        size_t width = BytesToRead(dib_header + HEADER_WIDTH_OFFSET);
        size_t height = BytesToRead(dib_header + HEADER_HEIGHT_OFFSET);
        Image work_image(width, height);

        std::vector<std::vector<Pixel>> data(height);
        std::vector<Pixel> row(width);

        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                unsigned char pix[BYTES_PER_PIXEL];
                img.read(reinterpret_cast<char*>(pix), BYTES_PER_PIXEL);
                row[j].SetRed(static_cast<uint8_t>(pix[2]));
                row[j].SetGreen(static_cast<uint8_t>(pix[1]));
                row[j].SetBlue(static_cast<uint8_t>(pix[0]));
            }
            data[i] = row;
            img.ignore(static_cast<std::streamsize>(GetPaddingSize(width)));
        }
        work_image = Image(data);
        img.close();
        return work_image;
    } catch (const std::exception& e) {
        throw std::string("Error while reading file") + static_cast<std::string>(e.what());
    }
}