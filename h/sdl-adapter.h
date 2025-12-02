#ifndef MY_SDL_ADAPTER_H
#define MY_SDL_ADAPTER_H

#include "dr4/math/color.hpp"

#include "myvector.h"
#include "colors.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>

extern const int lft_text_pad;

void setRenderer(SDL_Renderer* renderer);
SDL_Renderer* getRenderer();
void setFont(TTF_Font* font);
TTF_Font* getFont();
void setColor(Vector color);

void drawPoint(Vector p);
void drawLine(Vector p1, Vector p2);
void drawRect(Vector tl, Vector br, bool fill);

void drawCircle(Vector centre, int r, bool fill);
void fillConvexPolygon(std::vector<Vector> points);

void putText(std::string text, Vector tl, Vector br, dr4::Color color = white_color);

#endif // MY_SDL_ADAPTER_H