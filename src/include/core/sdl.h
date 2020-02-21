#ifndef WINDOW_H
#define WINDOW_H
#include <core/types.h>
#include <core/memory.h>

namespace ice
{
namespace core
{
    enum subsystem_flag
    {
        TIMER = SDL_INIT_TIMER,
        AUDIO = SDL_INIT_AUDIO,
        VIDEO = SDL_INIT_VIDEO,
        JOYSTICK = SDL_INIT_JOYSTICK,
        HAPTIC = SDL_INIT_HAPTIC,
        GAMMECONTROLLER = SDL_INIT_GAMECONTROLLER,
        EVENTS = SDL_INIT_EVENTS,
        EVERYTHING = SDL_INIT_EVERYTHING
    };

    static void init_sdl(const subsystem_flag subsystems);
    
    static void shutdown_sdl();
    
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
        window_type* _window;
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

    class surface
    {

    };
}
}

#endif