// #include "widget.hpp"

#include "sdl-adapter.h"
#include "mywindow.h"
// #include "optical-scene.h"
// #include "desktop.h"

#include <cassert>


// hui::State* hui_state = nullptr;



namespace dr4 {

dr4::MyWindow::MyWindow(dr4::Vec2f size, std::string title)
{
    width = size.x;
    height = size.y;
    this->title = title;
}

void dr4::MyWindow::SetTitle(const std::string &title)
{
    this->title = title;
}

const std::string& dr4::MyWindow::GetTitle() const
{
    return title;
}

dr4::Vec2f dr4::MyWindow::GetSize() const
{
    return Vec2f(width, height);
}

void dr4::MyWindow::SetSize(dr4::Vec2f size)
{
    width = size.x;
    height = size.y;
}

void dr4::MyWindow::Open()
{
    SDL_Renderer* rend;
    if (!SDL_CreateWindowAndRenderer(title.c_str(), width, height, SDL_WINDOW_RESIZABLE, &window, &rend)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return;
    }
    setRenderer(rend);

    this->texture = new MyTexture(Vec2f(width, height));
    is_open = true;
}

bool dr4::MyWindow::IsOpen() const { return is_open; }

void dr4::MyWindow::Close()
{
    is_open = false;
    delete texture;
}

void dr4::MyWindow::Clear(Color color)
{
    setColor(Vector(color.r, color.g, color.b));
    SDL_SetRenderTarget(getRenderer(), NULL);
    SDL_RenderClear(getRenderer());
}

void dr4::MyWindow::Draw(const Texture &texture)
{
    const Texture* texture_ptr = &texture;
    const MyTexture* t = dynamic_cast<const MyTexture*>(texture_ptr);
    assert(t != nullptr);

    SDL_FRect src_rect;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = texture.GetWidth();
    src_rect.h = texture.GetHeight();

    SDL_FRect dst_rect;
    dst_rect.x = texture.GetPos().x;
    dst_rect.y = texture.GetPos().y;
    dst_rect.w = texture.GetWidth();
    dst_rect.h = texture.GetHeight();

    SDL_SetRenderTarget(getRenderer(), NULL);
    SDL_RenderTexture(getRenderer(), t->t, &src_rect, &dst_rect);
}

void dr4::MyWindow::Display()
{
    SDL_RenderPresent(getRenderer());
}

void MyWindow::Sleep(double time) {} // TODO

double MyWindow::GetTime() { return 0; } // TODO

dr4::Texture* dr4::MyWindow::CreateTexture() { return new MyTexture(dr4::Vec2f(width, height)); }
dr4::Image* dr4::MyWindow::CreateImage() { return new MyImage(width, height); }

Font      *MyWindow::CreateFont()      { return new MyFont(); }
Line      *MyWindow::CreateLine()      { return new MyLine(); }
Circle    *MyWindow::CreateCircle()    { return new MyCircle(); }
Rectangle *MyWindow::CreateRectangle() { return new MyRectangle(); }
Text      *MyWindow::CreateText()      { return new MyText(); }

void MyWindow::StartTextInput() {}
void MyWindow::StopTextInput() {}


dr4::Event getMouseButtonEvent(SDL_Event event)
{
    dr4::Event evt;

    evt.mouseButton.pos.x = event.button.x;
    evt.mouseButton.pos.y = event.button.y;
    switch (event.button.type)
    {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            evt.type = dr4::Event::Type::MOUSE_DOWN;
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            evt.type = dr4::Event::Type::MOUSE_UP;
            break;
        default:
            evt.type = dr4::Event::Type::UNKNOWN;
            break;
    }
    switch (event.button.button)
    {
        case SDL_BUTTON_LEFT: 
            evt.mouseButton.button = dr4::MouseButtonType::LEFT;
            break;
        case SDL_BUTTON_RIGHT: 
            evt.mouseButton.button = dr4::MouseButtonType::RIGHT;
            break;
        case SDL_BUTTON_MIDDLE: 
            evt.mouseButton.button = dr4::MouseButtonType::MIDDLE;
            break;
        default: 
            evt.mouseButton.button = dr4::MouseButtonType::UNKNOWN;
            break;
    }

    return evt;
}

dr4::KeyCode getKeyCode(SDL_Keycode sdl_key)
{
    int dr4_key = dr4::KeyCode::KEYCODE_UNKNOWN;

    #define RANGE(lft, rgt, init)\
    if (sdl_key >= lft && sdl_key <= rgt)\
        dr4_key = init + sdl_key - lft;
        
    RANGE(SDLK_0, SDLK_9, dr4::KeyCode::KEYCODE_NUM0);
    RANGE(SDLK_A, SDLK_Z, dr4::KeyCode::KEYCODE_A);
    RANGE(SDLK_F1, SDLK_F12, dr4::KeyCode::KEYCODE_F1);
    #undef RANGE

    #define CASE_SHIFT(dr4key, sdlkey) case SDLK_ ## sdlkey: dr4_key = dr4::KeyCode::KEYCODE_ ## dr4key; break;
    switch (sdl_key) {
        CASE_SHIFT(BACKSPACE, BACKSPACE)
        CASE_SHIFT(ENTER, RETURN)
        CASE_SHIFT(ESCAPE, ESCAPE)
        CASE_SHIFT(SEMICOLON, SEMICOLON)
        CASE_SHIFT(COMMA, COMMA)
        CASE_SHIFT(PERIOD, PERIOD)
        CASE_SHIFT(LBRACKET, LEFTBRACKET)
        CASE_SHIFT(RBRACKET, RIGHTBRACKET)
        CASE_SHIFT(QUOTE, APOSTROPHE)
        CASE_SHIFT(SLASH, SLASH)
        CASE_SHIFT(BACKSLASH, BACKSLASH)
        CASE_SHIFT(TILDE, TILDE)
        CASE_SHIFT(EQUAL, EQUALS)
        CASE_SHIFT(HYPHEN, MINUS)
        CASE_SHIFT(SPACE, SPACE)
        CASE_SHIFT(TAB, TAB)
        CASE_SHIFT(LEFT, LEFT)
        CASE_SHIFT(RIGHT, RIGHT)
        CASE_SHIFT(UP, UP)
        CASE_SHIFT(DOWN, DOWN)
    }
    #undef CASE_SHIFT

    /*
    KEYCODE_LCONTROL,
    KEYCODE_LSHIFT,
    KEYCODE_LALT,
    KEYCODE_LSYSTEM,
    KEYCODE_RCONTROL,
    KEYCODE_RSHIFT,
    KEYCODE_RALT,
    KEYCODE_RSYSTEM,
    KEYCODE_MENU,

    KEYCODE_PAGEUP,
    KEYCODE_PAGEDOWN,
    KEYCODE_END,
    KEYCODE_HOME,
    KEYCODE_INSERT,
    KEYCODE_DELETE,
    KEYCODE_ADD,
    KEYCODE_SUBTRACT,
    KEYCODE_MULTIPLY,
    KEYCODE_DIVIDE,
    KEYCODE_NUMPAD0 - KEYCODE_NUMPAD9,
    KEYCODE_F13,
    KEYCODE_F14,
    KEYCODE_F15,
    KEYCODE_PAUSE,
    */

    return (dr4::KeyCode)dr4_key;
}

uint16_t getKeyMode(SDL_Keymod mod)
{
    uint16_t dr4_mod = 0;
    
    #define CASE_SHIFT(dr4mod, sdlmod) if (mod & SDL_KMOD_ ## sdlmod) dr4_mod |= dr4::KeyMode::KEYMOD_ ## dr4mod;
    CASE_SHIFT(LSHIFT, LSHIFT)
    CASE_SHIFT(RSHIFT, RSHIFT)
    CASE_SHIFT(LALT, LALT)
    CASE_SHIFT(RALT, RALT)
    CASE_SHIFT(LCTRL, LCTRL)
    CASE_SHIFT(RCTRL, RCTRL)
    CASE_SHIFT(CAPS, CAPS)
    #undef CASE_SHIFT

    return dr4_mod;
}

dr4::Event getKeyboardEvent(SDL_Event event)
{
    dr4::Event evt;

    switch (event.key.type)
    {
        case SDL_EVENT_KEY_DOWN:
            evt.type = dr4::Event::Type::KEY_DOWN;
            break;
        case SDL_EVENT_KEY_UP:
            evt.type = dr4::Event::Type::KEY_UP;
            break;
        default:
            evt.type = dr4::Event::Type::UNKNOWN;
            break;
    }
    evt.key.sym = getKeyCode(event.key.key);
    evt.key.mods = getKeyMode(event.key.mod);

    return evt;
}

std::optional<dr4::Event> dr4::MyWindow::PollEvent()
{
    SDL_Event event;
    if (!SDL_PollEvent(&event)) return {};
    
    dr4::Event evt;

    switch (event.type)
    {
        case SDL_EVENT_QUIT:
            evt.type = dr4::Event::Type::QUIT;
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN: case SDL_EVENT_MOUSE_BUTTON_UP:
            return getMouseButtonEvent(event);

        case SDL_EVENT_KEY_DOWN: case SDL_EVENT_KEY_UP:
            return getKeyboardEvent(event);

        case SDL_EVENT_MOUSE_MOTION:
            evt.type = dr4::Event::Type::MOUSE_MOVE;
            evt.mouseMove.pos.x = evt.mouseMove.rel.x = event.motion.x;
            evt.mouseMove.pos.y = evt.mouseMove.rel.y = event.motion.y;
            break;

        case SDL_EVENT_MOUSE_WHEEL:
            evt.type = dr4::Event::Type::MOUSE_WHEEL;
            evt.mouseWheel.pos.x = event.wheel.mouse_x;
            evt.mouseWheel.pos.y = event.wheel.mouse_y;
            evt.mouseWheel.delta.x = event.wheel.x;
            evt.mouseWheel.delta.y = event.wheel.y;
            break;

        default:
            evt.type = dr4::Event::Type::UNKNOWN;
            break;
    }

    return evt;
}

} // namespace dr4