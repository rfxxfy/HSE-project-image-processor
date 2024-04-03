//
// created by kacchan on 2024/3/27
//

#pragma once
#include <string>
#include <stdexcept>
#include "../objects/Image/image.h"

class Reader {
private:
    std::string path_;
    size_t BytesToRead(unsigned char* bytes);

public:
    explicit Reader(const std::string& path);
    Image Read();
};

size_t GetPaddingSize(size_t width);