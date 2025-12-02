#include "my-utils.h"

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

    if (code == dr4::KeyCode::KEYCODE_ENTER && (mods & dr4::KeyMode::KEYMOD_SHIFT)) chr = '\n';

    return chr;
    #undef RANGE_CAPITAL
    #undef CASE_SHIFT
}

void drawRectBorder(dr4::Rect2f rect, dr4::Texture& texture, dr4::Window* window, dr4::Color color) {
    dr4::Line *top_line = window->CreateLine(), *bottom_line = window->CreateLine(),
        *left_line = window->CreateLine(), *right_line = window->CreateLine();
    dr4::Vec2f p1 = rect.pos, p2 = rect.pos + rect.size;

    top_line->SetStart(dr4::Vec2f(p1.x, p1.y));
    top_line->SetEnd(dr4::Vec2f(p2.x, p1.y));
    bottom_line->SetStart(dr4::Vec2f(p1.x, p2.y));
    bottom_line->SetEnd(dr4::Vec2f(p2.x, p2.y));
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
