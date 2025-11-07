#ifndef I_ABRACAT_WINDOW_H
#define I_ABRACAT_WINDOW_H


#include "dr4/window.hpp"

#include "texture.h"

namespace dr4
{

class MyWindow : public Window {
    public:
        dr4::MyTexture* texture;
    
        std::string title;
        int width, height;
        bool is_open;
    
        SDL_Window *window;
    
    public:
        MyWindow(Vec2f size, std::string title);
    
        virtual void SetTitle(const std::string &title) override;
        virtual const std::string &GetTitle() const override;

        virtual Vec2f GetSize() const override;
        virtual void SetSize(const ::dr4::Vec2f& size) override;

        virtual void Open() override;
        virtual bool IsOpen() const override;
        virtual void Close() override;

        virtual void Clear(const Color &color) override;
        virtual void Draw(const Texture &texture, Vec2f pos) override;
        virtual void Display() override;

        virtual dr4::Texture *CreateTexture() override;
        virtual dr4::Image *CreateImage() override;
        virtual dr4::Font *CreateFont() override;

        virtual std::optional<Event> PollEvent() override;
    };

}



#endif // I_ABRACAT_WINDOW_H