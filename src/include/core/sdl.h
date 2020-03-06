#ifndef WINDOW_H
#define WINDOW_H
#include <core/types.h>
#include <core/engine.h>
#include <core/memory.h>
#define SUBSYSTEM_FLAG(FLAG) FLAG = SDL_INIT_ ## FLAG
#define WINDOW_FLAG(FLAG) FLAG = SDL_WINDOW_ ## FLAG
#define EVENT_TYPE(TYPE) TYPE = SDL_ ## TYPE
#define KEY_CODE(CODE) CODE = SDLK_ ## CODE
#define KEY_STATE(STATE) STATE = SDL_ ## STATE


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

    enum event_types
    {
        EVENT_TYPE(QUIT),
        EVENT_TYPE(KEYDOWN),
        EVENT_TYPE(KEYUP)
    };

    enum key_codes
    {
        KEY_CODE(UP),
        KEY_CODE(DOWN),
        KEY_CODE(LEFT),
        KEY_CODE(RIGHT)
    };

    enum key_states
    {
        KEY_STATE(PRESSED),
        KEY_STATE(RELEASED)
    };

    struct key_event
    {
        int type;
        int state;
        int window_id;
        int code;
        int time;
    };

    typedef std::function<void(const key_event&)> keyboard_callback_t;
    
    typedef std::function<void()> quit_callback_t;

    static std::atomic<bool> _sdl_init(false);

    static void init_sdl(const subsystem_flag subsystems);
    
    static void quit_sdl();

    class sdl_window
    {
    private:
        window_type* _window;
    public:
        sdl_window();

        sdl_window(
            const string_type & title,
            const size_type x,
            const size_type y,
            const size_type width,
            const size_type height);
        
        sdl_window(
            const string_type & title,
            const size_type width,
            const size_type height);

        sdl_window(const sdl_window& window) = delete;
        
        sdl_window(sdl_window&& window);

        ~sdl_window();
    };

    class sdl_app
    {
    protected:
        std::shared_ptr<engine> _engine;

    public:
        sdl_app(const std::shared_ptr<engine>& engine);
        
        sdl_app(const sdl_app& app) = default;

        sdl_app(sdl_app&& app) = default;

        ~sdl_app();

        void sdl_main(
            keyboard_callback_t & on_key,
            quit_callback_t & on_quit,
            std::ostream & log);

        std::shared_ptr<sdl_window> create_window(
            const string_type & title,
            const size_type width,
            const size_type height)
        {
            return std::make_shared<sdl_window>(
                title,
                width,
                height);
        }
    };

    class opengl_app
    {

    };

    class vulkan_app
    {

    };
    
}
}

#endif