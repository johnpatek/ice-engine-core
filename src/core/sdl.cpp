#include <core/sdl.h>

ice::core::sdl_app::sdl_app(const std::shared_ptr<ice::core::engine> & engine)
{
    if(!_sdl_init.load())
    {
        ice::core::init_sdl(ice::core::subsystem_flag::EVERYTHING);
    }
    _engine = engine;
}


ice::core::sdl_app::~sdl_app()
{
    if(_sdl_init.load())
    {
        ice::core::quit_sdl();
    }
}

void ice::core::init_sdl(const ice::core::subsystem_flag subsystems)
{
    int result;
    result = SDL_Init(subsystems);
    if(result < 0)
    {
        throw std::runtime_error(
            "Error(" 
            + std::to_string(-result) 
            + "):" 
            + SDL_GetError());
        _sdl_init.store(true);
    }
}
    
void ice::core::quit_sdl()
{
    SDL_Quit();
}

void ice::core::sdl_app::sdl_main(
    ice::core::keyboard_callback_t & on_key,
    ice::core::quit_callback_t & on_quit,
    std::ostream & log)
{
    log << "main start" << std::endl;
    SDL_Event event;
    int event_type = SDL_PollEvent(&event);
    if(event_type != 0)
    {
        if(event_type == SDL_QUIT)
        {
            log << "quit event" << std::endl;
            _engine->execute(on_quit);
        }
        else if(
            event_type == ice::core::event_types::KEYDOWN 
            || event_type == ice::core::event_types::KEYUP)
        {
            log << "key event" << std::endl;
            ice::core::key_event key_event;
            key_event.type = event_type;
            _engine->execute_async(
                on_key,
                key_event);
        }
        else
        {
            std::cerr << event_type << std::endl;
        }
    }
    log << "main end" << std::endl;
}

ice::core::sdl_window::sdl_window()
{
    _window = NULL;
}

ice::core::sdl_window::~sdl_window()
{
    if(_window != NULL)
    {
        SDL_DestroyWindow(_window);
        _window = NULL;
    }
}

ice::core::sdl_window::sdl_window(
    const ice::core::string_type & title,
    const ice::core::size_type x,
    const ice::core::size_type y,
    const ice::core::size_type width,
    const ice::core::size_type height)
{
    _window = SDL_CreateWindow(
        title.c_str(),
        x,
        y,
        width,
        height,
        0);
}

ice::core::sdl_window::sdl_window(
    const ice::core::string_type & title,
    const ice::core::size_type width,
    const ice::core::size_type height)
{
    _window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        0);
}

ice::core::sdl_window::sdl_window(
    sdl_window&& window)
{
    _window = window._window;
    window._window = NULL;
}