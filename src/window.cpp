// #include "widget.hpp"

#include "sdl-adapter.h"
#include "mywindow.h"
// #include "optical-scene.h"
// #include "desktop.h"

#include <cassert>


// hui::State* hui_state = nullptr;





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

void dr4::MyWindow::SetSize(const dr4::Vec2f& size)
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
}

bool dr4::MyWindow::IsOpen() const { return is_open; }

void dr4::MyWindow::Close()
{
    //
}

void dr4::MyWindow::Clear(const Color &color)
{
    setColor(Vector(color.r, color.g, color.b));
    SDL_SetRenderTarget(getRenderer(), texture->t);
    SDL_RenderClear(getRenderer());
}

void dr4::MyWindow::Draw(const Texture &texture, Vec2f pos)
{
    const Texture* texture_ptr = &texture;
    const MyTexture* t = dynamic_cast<const MyTexture*>(texture_ptr);
    assert(t != nullptr);

    SDL_FRect dst_rect;
    dst_rect.x = pos.x;
    dst_rect.y = pos.y;
    dst_rect.w = texture.GetWidth();
    dst_rect.h = texture.GetHeight();

    SDL_SetRenderTarget(getRenderer(), NULL);
    SDL_RenderTexture(getRenderer(), t->t, NULL, &dst_rect);
}

void dr4::MyWindow::Display()
{
    SDL_SetRenderTarget(getRenderer(), NULL);
    SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 0);
    SDL_RenderClear(getRenderer());

    Draw(*texture, Vec2f(0, 0));

    SDL_RenderPresent(getRenderer());
}

dr4::Texture* dr4::MyWindow::CreateTexture()
{
    return texture;
}

dr4::Image* dr4::MyWindow::CreateImage()
{
    return nullptr;
}

std::optional<dr4::Event> dr4::MyWindow::PollEvent()
{
    return {};
}