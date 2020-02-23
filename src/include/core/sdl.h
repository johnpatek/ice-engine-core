#ifndef WINDOW_H
#define WINDOW_H
#include <core/types.h>
#include <core/engine.h>
#include <core/memory.h>

#define SUBSYSTEM_FLAG(FLAG) FLAG = SDL_INIT_ ## FLAG
#define WINDOW_FLAG(FLAG) FLAG = SDL_WINDOW_ ## FLAG
#define KEY_CODE(CODE) CODE = SDLK_ ## FLAG


namespace ice
{
namespace core
{
    enum subsystem_flag
    {
        SUBSYSTEM_FLAG(TIMER),
        SUBSYSTEM_FLAG(AUDIO),
        SUBSYSTEM_FLAG(VIDEO),
        SUBSYSTEM_FLAG(JOYSTICK),
        SUBSYSTEM_FLAG(HAPTIC),
        SUBSYSTEM_FLAG(GAMECONTROLLER),
        SUBSYSTEM_FLAG(EVENTS),
        SUBSYSTEM_FLAG(EVERYTHING)
    };

    enum window_flag
    {
        NONE = 0,
        WINDOW_FLAG(FULLSCREEN),
        WINDOW_FLAG(FULLSCREEN_DESKTOP),
        WINDOW_FLAG(VULKAN),
        WINDOW_FLAG(OPENGL)
    };

    static void init_sdl(const subsystem_flag subsystems);
    
    static void shutdown_sdl();

    class ui_application
    {

    };

    
}
}

#endif