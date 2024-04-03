//
// created by kacchan on 2024/3/27
//

#include <stdint.h>

class Pixel {
private:
    uint8_t red_;
    uint8_t green_;
    uint8_t blue_;

public:
    Pixel() = default;

    ~Pixel() = default;

    Pixel(uint8_t r, uint8_t g, uint8_t b);

    uint8_t GetRed() const;

    uint8_t GetGreen() const;

    uint8_t GetBlue() const;

    void SetRed(uint8_t value);

    void SetGreen(uint8_t value);

    void SetBlue(uint8_t value);
};
