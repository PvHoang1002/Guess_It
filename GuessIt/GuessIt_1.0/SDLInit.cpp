#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "SDLInit.hpp"

//Initialize SDL
sdl::sdl()
{
    auto const result = SDL_Init(SDL_INIT_VIDEO);
    if (result != 0)
        std::cout << "SDL could not initialize";
}

sdl::sdl(sdl&& other) noexcept
{
    _swap(*this, other);
}

sdl::~sdl()
{
    if (_own)
        SDL_Quit();
}

auto sdl::operator=(sdl&& other) noexcept -> sdl&
{
    _swap(*this, other);
    return *this;
}

void _swap(sdl& a, sdl& b) noexcept
{
    using std::swap;
    swap(a._own, b._own);
}

//Initialize SDL Image
sdl_img::sdl_img()
{
    auto const result = IMG_Init(IMG_INIT_PNG);
    if (result == 0)
        std::cout << "SDL Image could not initialize";
}

sdl_img::sdl_img(sdl_img&& other) noexcept
{
    _swap(*this, other);
}

sdl_img::~sdl_img()
{
    if (_own)
        IMG_Quit();
}

auto sdl_img::operator=(sdl_img&& other) noexcept -> sdl_img&
{
    _swap(*this, other);
    return *this;
}

void _swap(sdl_img& a, sdl_img& b) noexcept
{
    using std::swap;
    swap(a._own, b._own);
}

//Initialize SDL True Type Fonts
sdl_ttf::sdl_ttf()
{
    auto const result = TTF_Init();
    if (result != 0)
        std::cout << "SDL TTF could not initialize";
}

sdl_ttf::sdl_ttf(sdl_ttf&& other) noexcept
{
    _swap(*this, other);
}

sdl_ttf::~sdl_ttf()
{
    if (_own)
        TTF_Quit();
}

auto sdl_ttf::operator=(sdl_ttf&& other) noexcept -> sdl_ttf&
{
    _swap(*this, other);
    return *this;
}

void _swap(sdl_ttf& a, sdl_ttf& b) noexcept
{
    using std::swap;
    swap(a._own, b._own);
}
