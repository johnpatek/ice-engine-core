#include <core/sdl.h>

void ice::core::init_sdl(const ice::core::subsystem_flag subsystems)
{
    SDL_Init(subsystems);
}
    
void ice::core::shutdown_sdl()
{
    SDL_Quit();
}
    

ice::core::window::window(
    const ice::core::string_type& title,
    const ice::core::size_type x, 
    const ice::core::size_type y,
    const ice::core::size_type w, 
    const ice::core::size_type h,
    const ice::core::window_flag flags)
{
    _window = SDL_CreateWindow(
        title.c_str(),
        x,
        y,
        w,
        h,
        flags);
}

ice::core::window::window(
    const ice::core::string_type& title,
    const ice::core::size_type w, 
    const ice::core::size_type h,
    const ice::core::window_flag flags)
{
    _window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        w,
        h,
        flags);
}

ice::core::window::~window()
{
    SDL_DestroyWindow(_window);
}

void ice::core::window::set_size(
    const ice::core::size_type w, 
    const ice::core::size_type h)
{
    SDL_SetWindowSize(_window,w,h);   
}