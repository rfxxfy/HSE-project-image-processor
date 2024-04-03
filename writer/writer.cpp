//
// created by kacchan on 2024/3/27
// 

#include "writer.h"
#include <utility>
#include "../constants.h"

void Writer::WriteByte(std::ostream& out_file, uint8_t byte_value) {
    out_file.write(reinterpret_cast<char*>(&byte_value), 1);
}

void Writer::WriteValue(std::ostream& out_file, uint32_t value) {
    for (size_t i = 0; i < BYTES_PER_INT; ++i) {
        WriteByte(out_file, (value >> i * BITS_PER_BYTE) & BYTE_MASK);
    }
}

void Writer::WriteZero(std::ostream& out_file, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        WriteByte(out_file, 0);
    }
}

void Writer::WriteFile(const std::string& file_name, const Image& img) {
    std::ofstream out_file(file_name, std::ios::binary);
    WriteByte(out_file, HEADER_SIGNATURE[0]);
    WriteByte(out_file, HEADER_SIGNATURE[1]);
    auto header_size = BMP_HEADER_SIZE + DIB_HEADER_SIZE;
    auto file_size = BMP_HEADER_SIZE + DIB_HEADER_SIZE + img.GetHeight() * img.GetWidth() * BYTES_PER_PIXEL +
                     GetPaddingSize(img.GetWidth()) * img.GetHeight();
    WriteValue(out_file, file_size);
    WriteZero(out_file, 4);
    WriteValue(out_file, header_size);
    WriteValue(out_file, DIB_HEADER_SIZE);
    WriteValue(out_file, img.GetWidth());
    WriteValue(out_file, img.GetHeight());
    WriteByte(out_file, COLOR_PLANES);
    WriteZero(out_file, COLOR_PLANES);
    WriteByte(out_file, BITS_PER_PIXEL);
    WriteZero(out_file, COLOR_PLANES);
    WriteZero(out_file, BITS_PER_PIXEL);
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            WriteByte(out_file, img.GetPixel(i, j).GetBlue());
            WriteByte(out_file, img.GetPixel(i, j).GetGreen());
            WriteByte(out_file, img.GetPixel(i, j).GetRed());
        }
        WriteZero(out_file, GetPaddingSize(img.GetWidth()));
    }
    out_file.close();
}