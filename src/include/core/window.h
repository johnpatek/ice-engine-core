#ifndef WINDOW_H
#define WINDOW_H
#include <core/types.h>
#include <core/memory.h>
#include <SDL2/SDL.h>

namespace ice
{
namespace core
{
    enum window_flag
    {
        NONE = 0,
        FULLSCREEN = SDL_WINDOW_FULLSCREEN,
        FULLSCREEN_DESKTOP = SDL_WINDOW_FULLSCREEN,
        VULKAN = SDL_WINDOW_VULKAN,
        OPENGL = SDL_WINDOW_OPENGL
    };

    class window
    {
    private:
        SDL_Window* _window;
    public:
        
        window(
            const string_type& title,
            const size_type x, 
            const size_type y,
            const size_type w, 
            const size_type h,
            const window_flag flags = window_flag::NONE);

        window(
            const string_type& title,
            const size_type w, 
            const size_type h,
            const window_flag flags = window_flag::NONE);
        
        ~window();


        void set_size(
            const size_type w, 
            const size_type h);
    };
}
}

#endif