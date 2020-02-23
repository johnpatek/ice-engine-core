#include <core/sdl.h>

void ice::core::init_sdl(const ice::core::subsystem_flag subsystems)
{
    SDL_Init(subsystems);
}
    
void ice::core::shutdown_sdl()
{
    SDL_Quit();
}