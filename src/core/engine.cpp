#include <core/engine.h>

ice::core::engine::engine(const ice::core::size_type threads)
{
    _thread_pool = std::make_shared<ice::core::thread_pool>(threads);
}

