//
// created by kacchan on 2024/3/27
// 

#pragma once
#include <string>
#include "../objects/Image/image.h"
#include "../constants.h"
#include "../reader/reader.h"
#include <fstream>

class Writer {
public:
    static void WriteFile(const std::string& file_name, const Image& image);
    static void WriteByte(std::ostream& out_file, uint8_t byte_value);
    static void WriteValue(std::ostream& out_file, uint32_t value);
    static void WriteZero(std::ostream& out_file, size_t size);
};
