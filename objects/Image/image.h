//
// created by kacchan on 2024/3/27
//

#pragma once

#include "../pixel/pixel.h"
#include <cstdint>
#include <iostream>
#include <vector>

class Image {
private:
    size_t width_ = 0;
    size_t height_ = 0;
    std::vector<std::vector<Pixel>> data_;

public:
    Image() = default;

    Image(const size_t width, const size_t height);

    ~Image() = default;

    explicit Image(const std::vector<std::vector<Pixel>>& data);

    Image(const Image& image);

    size_t GetWidth() const;

    size_t GetHeight() const;

    const std::vector<std::vector<Pixel>>& GetData() const;

    void SetData(std::vector<std::vector<Pixel>> new_data);

    Pixel& GetPixel(size_t x, size_t y);

    const Pixel& GetPixel(size_t x, size_t y) const;

    void SetPixel(size_t x, size_t y, const Pixel& pixel);
};