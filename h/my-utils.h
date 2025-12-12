#ifndef I_MY_TEXT_PROCESSING_H
#define I_MY_TEXT_PROCESSING_H

#include "dr4/math/rect.hpp"
#include "dr4/texture.hpp"
#include "dr4/window.hpp"
#include "hui/widget.hpp"

#include "myvector.h"

std::string doubleToStr(double val);
long long getMillisecondsSinceEpoch();
char KeycodeToChar(dr4::KeyCode code, uint16_t mods);

void drawRoundedRect(dr4::Rect2f rect, dr4::Texture& texture, dr4::Window* window, dr4::Color color,
    bool draw_border = true, dr4::Color border_col = dr4::Color(127, 127, 127), float radius = 10);
void drawRectBorder(dr4::Rect2f rect, dr4::Texture& texture, dr4::Window* window, dr4::Color color);
// void drawBox(FixedVec box, dr4::Color col, dr4::Texture& texture, dr4::Window* window);
dr4::Vec2f getTextBounds(dr4::Window* window, std::string text);

#endif // I_MY_TEXT_PROCESSING_H
