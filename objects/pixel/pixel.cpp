//
// created by kacchan on 2024/3/27
//

#include "pixel.h"

#include <iostream>

Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b) : red_(r), green_(g), blue_(b) {
}

uint8_t Pixel::GetRed() const {
    return red_;
}

uint8_t Pixel::GetGreen() const {
    return green_;
}

uint8_t Pixel::GetBlue() const {
    return blue_;
}

void Pixel::SetRed(uint8_t value) {
    red_ = value;
}

void Pixel::SetGreen(uint8_t value) {
    green_ = value;
}

void Pixel::SetBlue(uint8_t value) {
    blue_ = value;
}
