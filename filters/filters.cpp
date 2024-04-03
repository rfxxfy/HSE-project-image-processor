//
// created by kacchan on 2024/3/27
//

#include "filters.h"
#include "../constants.h"

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <random>

template <typename T>
std::vector<T> Filter::MatrixApply(const std::vector<std::vector<T>>& matrix, const Image& image, const int x,
                                   const int y, const uint8_t radius) const {
    T red = 0;
    T green = 0;
    T blue = 0;
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix.front().size(); ++j) {
            red +=
                image
                    .GetPixel(std::clamp(static_cast<int>(i) + x - radius, 0, static_cast<int>(image.GetHeight() - 1)),
                              std::clamp(static_cast<int>(j) + y - radius, 0, static_cast<int>(image.GetWidth() - 1)))
                    .GetRed() *
                matrix[i][j];
            green +=
                image
                    .GetPixel(std::clamp(static_cast<int>(i) + x - radius, 0, static_cast<int>(image.GetHeight() - 1)),
                              std::clamp(static_cast<int>(j) + y - radius, 0, static_cast<int>(image.GetWidth() - 1)))
                    .GetGreen() *
                matrix[i][j];
            blue +=
                image
                    .GetPixel(std::clamp(static_cast<int>(i) + x - radius, 0, static_cast<int>(image.GetHeight() - 1)),
                              std::clamp(static_cast<int>(j) + y - radius, 0, static_cast<int>(image.GetWidth() - 1)))
                    .GetBlue() *
                matrix[i][j];
        }
    }
    return {red, green, blue};
}

uint8_t Clamp(int color) {
    if (color < 0) {
        return 0;
    } else if (color > MAX_RGB) {
        return MAX_RGB;
    }
    return static_cast<uint8_t>(color);
}

void Negative::Apply(Image& image) {
    size_t width = image.GetWidth();
    size_t height = image.GetHeight();
    for (size_t x = 0; x < height; x++) {
        for (size_t y = 0; y < width; y++) {
            Pixel pixel = image.GetPixel(x, y);
            uint8_t r = MAX_RGB - pixel.GetRed();
            uint8_t g = MAX_RGB - pixel.GetGreen();
            uint8_t b = MAX_RGB - pixel.GetBlue();
            image.SetPixel(x, y, Pixel{r, g, b});
        }
    }
}

void Grayscale::Apply(Image& image) {
    size_t width = image.GetWidth();
    size_t height = image.GetHeight();
    for (size_t x = 0; x < height; x++) {
        for (size_t y = 0; y < width; y++) {
            Pixel pixel = image.GetPixel(x, y);
            auto gray = static_cast<uint8_t>(GRAYSCALE_RED * static_cast<double>(pixel.GetRed()) +
                                             GRAYSCALE_GREEN * static_cast<double>(pixel.GetGreen()) +
                                             GRAYSCALE_BLUE * static_cast<double>(pixel.GetBlue()));
            image.SetPixel(x, y, Pixel{gray, gray, gray});
        }
    }
}

void Crop::Apply(Image& image) {
    size_t res_width = std::max(static_cast<size_t>(0), std::min(static_cast<size_t>(image.GetWidth()), crop_width_));
    size_t res_height =
        std::max(static_cast<size_t>(0), std::min(static_cast<size_t>(image.GetHeight()), crop_height_));
    std::vector<std::vector<Pixel>> crop_data;
    for (size_t x = image.GetHeight() - res_height; x < image.GetHeight(); ++x) {
        std::vector<Pixel> line;
        for (size_t y = 0; y < res_width; ++y) {
            line.push_back(image.GetPixel(x, y));
        }
        crop_data.push_back(line);
    }
    image.SetData(crop_data);
}

void Sharpening::Apply(Image& image) {
    std::vector<std::vector<Pixel>> sharp_data(image.GetHeight(), std::vector<Pixel>(image.GetWidth()));
    const std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    for (size_t x = 0; x < image.GetHeight(); ++x) {
        for (size_t y = 0; y < image.GetWidth(); ++y) {
            std::vector<int> colours = MatrixApply(matrix, image, static_cast<int>(x), static_cast<int>(y), 1);
            Pixel pixel;
            pixel.SetRed(Clamp(colours[0]));
            pixel.SetGreen(Clamp(colours[1]));
            pixel.SetBlue(Clamp(colours[2]));
            sharp_data[x][y] = pixel;
        }
    }
    image.SetData(sharp_data);
}

void EdgeDetection::Apply(Image& image) {
    Grayscale gs_filter;
    gs_filter.Apply(image);
    std::vector<std::vector<Pixel>> edge_data(image.GetHeight(), std::vector<Pixel>(image.GetWidth()));
    const std::vector<std::vector<int>> matrix = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    for (size_t x = 0; x < image.GetHeight(); ++x) {
        for (size_t y = 0; y < image.GetWidth(); ++y) {
            std::vector<int> colours = MatrixApply(matrix, image, static_cast<int>(x), static_cast<int>(y), 1);
            if (colours.front() > static_cast<uint8_t>(thresold_ * MAX_RGB)) {
                edge_data[x][y] = Pixel(MAX_RGB, MAX_RGB, MAX_RGB);
            } else {
                edge_data[x][y] = Pixel(MIN_RGB, MIN_RGB, MIN_RGB);
            }
        }
    }
    image.SetData(edge_data);
}

void Smoothing::Apply(Image& image) {
    std::vector<std::vector<Pixel>> mc_data(image.GetHeight(), std::vector<Pixel>(image.GetWidth()));

    for (size_t x = 0; x < image.GetHeight(); ++x) {
        for (size_t y = 0; y < image.GetWidth(); ++y) {

            int sum_red = 0;
            int sum_green = 0;
            int sum_blue = 0;
            int count = 0;

            for (int dx = -radius_; dx <= radius_; ++dx) {
                for (int dy = -radius_; dy <= radius_; ++dy) {
                    int nx = std::min(std::max(static_cast<int>(x) + dx, 0), static_cast<int>(image.GetWidth() - 1));
                    int ny = std::min(std::max(static_cast<int>(y) + dy, 0), static_cast<int>(image.GetHeight() - 1));
                    Pixel pixel = image.GetPixel(nx, ny);
                    sum_red += pixel.GetRed();
                    sum_green += pixel.GetGreen();
                    sum_blue += pixel.GetBlue();
                    count++;
                }
            }

            Pixel res_pixel;
            Pixel pixel = image.GetPixel(x, y);

            uint8_t avg_red = static_cast<uint8_t>(sum_red / count);
            uint8_t avg_green = static_cast<uint8_t>(sum_green / count);
            uint8_t avg_blue = static_cast<uint8_t>(sum_blue / count);

            res_pixel.SetRed((pixel.GetRed() + avg_red) / 2);
            res_pixel.SetGreen((pixel.GetGreen() + avg_green) / 2);
            res_pixel.SetBlue((pixel.GetBlue() + avg_blue) / 2);

            mc_data[x][y] = res_pixel;
        }
    }
    image.SetData(mc_data);
}

void GaussianBlur::Apply(Image& image) {
    std::vector<std::vector<Pixel>> blur_data(image.GetHeight(), std::vector<Pixel>(image.GetWidth()));
    for (size_t x = 0; x < image.GetHeight(); ++x) {
        for (size_t y = 0; y < image.GetWidth(); ++y) {
            double red = 0.0;
            double green = 0.0;
            double blue = 0.0;
            for (size_t x_1 = 0; x_1 < image.GetHeight(); ++x_1) {
                for (size_t y_1 = 0; y_1 < image.GetWidth(); ++y_1) {
                    Pixel pixel = image.GetPixel(x_1, y_1);
                    int distance = static_cast<int>((x_1 - x)) * static_cast<int>((x_1 - x)) +
                                   static_cast<int>((y_1 - y)) * static_cast<int>((y_1 - y));
                    double gauss_coeff = std::exp(-(distance) / (2 * sigma_ * sigma_)) / (2 * M_PI * sigma_ * sigma_);
                    red += pixel.GetRed() * gauss_coeff;
                    green += pixel.GetGreen() * gauss_coeff;
                    blue += pixel.GetBlue() * gauss_coeff;
                }
            }
            Pixel new_pixel;

            new_pixel.SetRed(Clamp(static_cast<uint8_t>(red)));
            new_pixel.SetGreen(Clamp(static_cast<uint8_t>(green)));
            new_pixel.SetBlue(Clamp(static_cast<uint8_t>(blue)));

            blur_data[x][x] = new_pixel;
        }
    }
    image.SetData(blur_data);
}