//
// created by kacchan on 2024/3/27
//

#include "image.h"
#include <iostream>
#include <fstream>

Image::Image(const Image& image) {
    data_ = image.data_;
    width_ = image.width_;
    height_ = image.height_;
}

Image::Image(const size_t width, const size_t height) {
    height_ = height;
    width_ = width;
}

Image::Image(const std::vector<std::vector<Pixel>>& data) {
    data_ = data;
    width_ = data.front().size();
    height_ = data.size();
}

size_t Image::GetWidth() const {
    return width_;
}

size_t Image::GetHeight() const {
    return height_;
}

const std::vector<std::vector<Pixel>>& Image::GetData() const {
    return data_;
}

void Image::SetData(std::vector<std::vector<Pixel>> new_data) {
    data_ = new_data;
    width_ = new_data.front().size();
    height_ = new_data.size();
}

Pixel& Image::GetPixel(size_t x, size_t y) {
    if (x < 0 || x >= height_ || y < 0 || y >= width_) {
        throw std::runtime_error("Pixel coordinates out of range");
    }
    return data_[x][y];
}

const Pixel& Image::GetPixel(size_t x, size_t y) const {
    if (x < 0 || x >= height_ || y < 0 || y >= width_) {
        throw std::runtime_error("Pixel coordinates out of range");
    }
    return data_[x][y];
}

void Image::SetPixel(size_t x, size_t y, const Pixel& pixel) {
    if (x < 0 || x >= height_ || y < 0 || y >= width_) {
        throw std::runtime_error("Pixel coordinates out of range");
    }
    data_[x][y] = pixel;
}