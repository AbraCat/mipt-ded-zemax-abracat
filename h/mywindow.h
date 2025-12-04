#ifndef I_ABRACAT_WINDOW_H
#define I_ABRACAT_WINDOW_H

#include "dr4/window.hpp"

#include "texture.h"

namespace dr4
{

class MyWindow : public Window {
public:
    dr4::MyTexture* texture;
    const dr4::Font* default_font;
    std::string clipboard;
    long long time_opened; // milliseconds

    std::string title;
    int width, height;
    bool is_open;

    SDL_Window *window;

public:
    MyWindow(Vec2f size, std::string title);

    virtual void SetTitle(const std::string &title) override;
    virtual const std::string &GetTitle() const override;
    virtual Vec2f GetSize() const override;
    virtual void SetSize(Vec2f size) override;

    virtual void Open() override;
    virtual bool IsOpen() const override;
    virtual void Close() override;

    virtual void Clear(Color color) override;
    virtual void Draw(const Texture &texture) override;
    virtual void Display() override;

    virtual void Sleep(double time) override;
    virtual double GetTime() override;
    virtual std::optional<Event> PollEvent() override;

    virtual Texture   *CreateTexture()   override;
    virtual Image     *CreateImage()     override;
    virtual Font      *CreateFont()      override;
    virtual Line      *CreateLine()      override;
    virtual Circle    *CreateCircle()    override;
    virtual Rectangle *CreateRectangle() override;
    virtual Text      *CreateText()      override;

    virtual void StartTextInput() override;
    virtual void StopTextInput() override;

    virtual void SetDefaultFont( const dr4::Font* font ) override;
    virtual const dr4::Font* GetDefaultFont() override;
    virtual void SetClipboard( const std::string& string ) override;
    virtual std::string GetClipboard() override;
    };

}



#endif // I_ABRACAT_WINDOW_H