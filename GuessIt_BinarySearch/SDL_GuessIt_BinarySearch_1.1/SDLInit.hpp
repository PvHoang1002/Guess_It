#pragma once

class sdl
{
public:
    sdl();

    sdl(sdl&& other) noexcept;


    ~sdl();

    auto operator = (sdl&& other) noexcept->sdl&;

    // Non-copyable
    sdl(sdl const&) = delete;

    auto operator = (sdl const&)->sdl& = delete;

    friend void _swap(sdl& a, sdl& b) noexcept;

private:
    bool _own = false;
};

class sdl_img
{
public:
    sdl_img();

    sdl_img(sdl_img&& other) noexcept;

    ~sdl_img();

    auto operator = (sdl_img&& other) noexcept->sdl_img&;

    // Non-copyable
    sdl_img(sdl_img const&) = delete;

    auto operator = (sdl_img const&)->sdl_img& = delete;

    friend void _swap(sdl_img& a, sdl_img& b) noexcept;

private:
    bool _own = false;
};

class sdl_ttf
{
public:
    sdl_ttf();

    sdl_ttf(sdl_ttf&& other) noexcept;

    ~sdl_ttf();

    auto operator=(sdl_ttf&& other) noexcept->sdl_ttf&;

    // Non-copyable
    sdl_ttf(sdl_ttf const&) = delete;

    auto operator=(sdl_ttf const&)->sdl_ttf& = delete;

    friend void _swap(sdl_ttf& a, sdl_ttf& b) noexcept;

private:
    bool _own = false;
};

