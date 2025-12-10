#ifndef I_MY_COLORS_H
#define I_MY_COLORS_H

#include "dr4/math/color.hpp"

enum ColComponent { COL_R, COL_G, COL_B };

static const dr4::Color white_color(255, 255, 255), black_color(0, 0, 0), gray_color(127, 127, 127), light_gray_color(160, 160, 160),
    red_color(255, 0, 0), green_color(0, 255, 0), blue_color(0, 0, 255), dark_gray_color(63, 63, 63),
    light_green_color(0, 255, 127), yellow_color(255, 255, 0), purple_color(127, 0, 127), orange_color(255, 127, 0);

#endif // I_MY_COLORS_H