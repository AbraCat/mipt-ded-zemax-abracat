#include "my-utils.h"

#include <chrono>
#include <sstream>
#include <vector>

const int angle_radius = 10;

std::string doubleToStr(double val)
{
    std::ostringstream out;
    out.precision(2);
    out << std::fixed << val;
    return std::move(out).str();
}

long long getMillisecondsSinceEpoch() {
    auto now_chrono = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now_chrono);
    auto epoch = now_ms.time_since_epoch();
    long long milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();

    return milliseconds_since_epoch;
}

char KeycodeToChar(dr4::KeyCode code, uint16_t mods) {
    #define RANGE_CAPITAL(code_lft, code_rgt, chr_lft, chr_lft_capital)\
        if (code >= dr4::KeyCode::KEYCODE_ ## code_lft && code <= dr4::KeyCode::KEYCODE_ ## code_rgt) {\
            chr = (capital ? chr_lft_capital : chr_lft) + code - dr4::KeyCode::KEYCODE_ ## code_lft;\
        }
    #define CASE_SHIFT(dr4_code, character, char_shift)\
        case dr4::KeyCode::KEYCODE_ ## dr4_code: chr = (shift ? char_shift : character); break;


    char chr = '\0';
    bool shift = mods & dr4::KeyMode::KEYMOD_SHIFT, caps = mods & dr4::KeyMode::KEYMOD_CAPS;
    bool capital = shift && !caps || !shift && caps;

    RANGE_CAPITAL(A, Z, 'a', 'A')

    switch (code) {
        CASE_SHIFT(NUM0, '0', ')')
        CASE_SHIFT(NUM1, '1', '!')
        CASE_SHIFT(NUM2, '2', '@')
        CASE_SHIFT(NUM3, '3', '#')
        CASE_SHIFT(NUM4, '4', '$')
        CASE_SHIFT(NUM5, '5', '%')
        CASE_SHIFT(NUM6, '6', '^')
        CASE_SHIFT(NUM7, '7', '&')
        CASE_SHIFT(NUM8, '8', '*')
        CASE_SHIFT(NUM9, '9', '(')

        CASE_SHIFT(SEMICOLON, ';', ':')
        CASE_SHIFT(COMMA, ',', '<')
        CASE_SHIFT(PERIOD, '.', '>')
        CASE_SHIFT(LBRACKET, '[', '{')
        CASE_SHIFT(RBRACKET, ']', '}')
        CASE_SHIFT(QUOTE, '\'', '"')
        CASE_SHIFT(SLASH, '/', '?')
        CASE_SHIFT(BACKSLASH, '\\', '|')
        CASE_SHIFT(TILDE, '`', '~')
        CASE_SHIFT(EQUAL, '=', '+')
        CASE_SHIFT(HYPHEN, '-', '_')
        CASE_SHIFT(SPACE, ' ', ' ')
    }

    // if (code == dr4::KeyCode::KEYCODE_ENTER && (mods & dr4::KeyMode::KEYMOD_SHIFT)) chr = '\n';

    return chr;
    #undef RANGE_CAPITAL
    #undef CASE_SHIFT
}

void drawRoundedRect(dr4::Rect2f rect, dr4::Texture& texture, dr4::Window* window, dr4::Color color,
    bool draw_border, dr4::Color border_col) {
    std::vector<dr4::Vec2f> circle_pos { {angle_radius, angle_radius}, {rect.size.x - angle_radius, angle_radius},
        {angle_radius, rect.size.y - angle_radius}, {rect.size.x - angle_radius, rect.size.y - angle_radius}};
    for (dr4::Vec2f pos: circle_pos) {
        dr4::Circle* circle = window->CreateCircle();
        circle->SetCenter(pos);
        circle->SetRadius(angle_radius);
        circle->SetFillColor(color);
        if (draw_border) circle->SetBorderColor(border_col);
        else circle->SetBorderColor(color);
        circle->SetBorderThickness(3);
        texture.Draw(*circle);
        delete circle;
    }

    dr4::Rectangle* vertical_rect = window->CreateRectangle();
    vertical_rect->SetFillColor(color);
    vertical_rect->SetBorderColor(color);
    vertical_rect->SetPos(rect.pos + dr4::Vec2f(angle_radius, 0));
    vertical_rect->SetSize(rect.size - dr4::Vec2f(angle_radius * 2, 0));
    texture.Draw(*vertical_rect);
    delete vertical_rect;

    dr4::Rectangle* horiz_rect = window->CreateRectangle();
    horiz_rect->SetFillColor(color);
    horiz_rect->SetBorderColor(color);
    horiz_rect->SetPos(rect.pos + dr4::Vec2f(0, angle_radius));
    horiz_rect->SetSize(rect.size - dr4::Vec2f(0, angle_radius * 2));
    texture.Draw(*horiz_rect);
    delete horiz_rect;

    if (!draw_border) return;

    std::vector<dr4::Rect2f> line_pos {{{angle_radius, 0}, {rect.size.x - angle_radius, 0}},
                                       {{angle_radius, rect.size.y - 1}, {rect.size.x - angle_radius, rect.size.y - 1}},
                                       {{0, angle_radius}, {0, rect.size.y - angle_radius}},
                                       {{rect.size.x - 1, angle_radius}, {rect.size.x - 1, rect.size.y - angle_radius}}};
    for (dr4::Rect2f rect: line_pos) {
        dr4::Line* line = window->CreateLine();
        line->SetStart(rect.pos);
        line->SetEnd(rect.size);
        line->SetThickness(3);
        line->SetColor(border_col);
        texture.Draw(*line);
        delete line;
    }
}

void drawRectBorder(dr4::Rect2f rect, dr4::Texture& texture, dr4::Window* window, dr4::Color color) {
    dr4::Line *top_line = window->CreateLine(), *bottom_line = window->CreateLine(),
        *left_line = window->CreateLine(), *right_line = window->CreateLine();
    dr4::Vec2f p1 = rect.pos, p2 = rect.pos + rect.size;

    top_line->SetStart(dr4::Vec2f(p1.x, p1.y));
    top_line->SetEnd(dr4::Vec2f(p2.x, p1.y));
    bottom_line->SetStart(dr4::Vec2f(p1.x, p2.y - 1));
    bottom_line->SetEnd(dr4::Vec2f(p2.x, p2.y - 1));
    left_line->SetStart(dr4::Vec2f(p1.x, p1.y));
    left_line->SetEnd(dr4::Vec2f(p1.x, p2.y));
    right_line->SetStart(dr4::Vec2f(p2.x, p1.y));
    right_line->SetEnd(dr4::Vec2f(p2.x, p2.y));


    top_line->SetColor(color);
    bottom_line->SetColor(color);
    left_line->SetColor(color);
    right_line->SetColor(color);
    texture.Draw(*top_line);
    texture.Draw(*bottom_line);
    texture.Draw(*left_line);
    texture.Draw(*right_line);
}

dr4::Vec2f getTextBounds(dr4::Window* window, std::string text) {
    dr4::Text* text_drawable = window->CreateText();
    text_drawable->SetText(text);
    dr4::Vec2f bounds = text_drawable->GetBounds();

    delete text_drawable;
    return bounds;
}
