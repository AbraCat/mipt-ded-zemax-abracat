#ifndef I_DR4_MATH_COLOR
#define I_DR4_MATH_COLOR

#include <cstdint>

namespace dr4 {

struct Color {

    uint8_t r, g, b, a;

    inline Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_)
        :r(r_), g(g_), b(b_), a(a_) {}

    bool operator==(const Color& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    bool operator!=(const Color& other) const {
        return !(*this == other);
    }
};

}; // namespace dr4

#endif // I_DR4_COLOR
