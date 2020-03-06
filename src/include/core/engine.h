#ifndef ENGINE_H
#define ENGINE_H

#include <core/multithreading.h>
#include <core/memory.h>

namespace ice
{
namespace core
{
    class engine
    {
    private:
        std::shared_ptr<ice::core::thread_pool> _thread_pool;
    public:
        engine(const ice::core::size_type threads = std::thread::hardware_concurrency());
        
        engine(const engine& other) = default;
        
        engine(engine&& other) = default;

        ~engine() = default;

        template<class F, class... Args>
        auto execute_async(F&& f, Args&&... args) 
            -> std::future<typename std::result_of<F(Args...)>::type>
        {
            return _thread_pool->enqueue(f,args...);
        }

        template<class F, class... Args>
        auto execute(F&& f, Args&&... args) 
            -> typename std::result_of<F(Args...)>::type
        {
            auto future = _thread_pool->enqueue(f,args...);
            future.wait();
            return future.get();
        }

    };
}
}

#endif