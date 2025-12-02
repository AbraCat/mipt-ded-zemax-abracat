#ifndef I_MY_TEXT_PROCESSING_H
#define I_MY_TEXT_PROCESSING_H

#include "dr4/math/rect.hpp"
#include "dr4/texture.hpp"
#include "dr4/window.hpp"

char KeycodeToChar(dr4::KeyCode code, uint16_t mods);
void drawRectBorder(dr4::Rect2f rect, dr4::Texture& texture, dr4::Window* window, dr4::Color color);

#endif // I_MY_TEXT_PROCESSING_H