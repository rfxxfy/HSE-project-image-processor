//
// created by kacchan on 2024/3/27
//

#pragma once

#include "../objects/Image/image.h"
#include <algorithm>
#include <cmath>
#include <memory>
#include <stdexcept>

class Filter {
public:
    Filter() = default;
    virtual ~Filter() = default;
    virtual void Apply(Image& image) = 0;

protected:
    template <typename T>
    std::vector<T> MatrixApply(const std::vector<std::vector<T>>& matrix, const Image& image, const int x, const int y,
                               const uint8_t radius) const;
};

class Negative : public Filter {
public:
    void Apply(Image& image) override;
};

class Grayscale : public Filter {
public:
    void Apply(Image& image) override;
};

class Crop : public Filter {
private:
    size_t crop_width_;
    size_t crop_height_;

public:
    explicit Crop(size_t width, size_t height) {
        crop_width_ = width;
        crop_height_ = height;
    }
    void Apply(Image& image) override;
};

class Sharpening : public Filter {
public:
    void Apply(Image& image) override;
};

class EdgeDetection : public Filter {
private:
    double thresold_;

public:
    explicit EdgeDetection(const double thresold) {
        thresold_ = thresold;
    }
    void Apply(Image& image) override;
};

class Smoothing : public Filter {
private:
    int radius_;

public:
    explicit Smoothing(const int radius) {
        radius_ = radius;
    }
    void Apply(Image& image) override;
};

class GaussianBlur : public Filter {
private:
    double sigma_;

public:
    explicit GaussianBlur(const double sigma) {
        sigma_ = sigma;
    }
    void Apply(Image& image) override;
};