#include "test.h"
#include "core/multithreading.h"


ICE_TEST_CASE(threadpool_single_test)
{
    ice::core::thread_pool test_pool(1);
    ice::core::heap_buffer test_buffer;
    const int test_value = 100;

    auto expected_result = test_value;

    auto future_result = test_pool.enqueue(
        [](int x)
        {
            return x;
        },
        test_value);
    
    auto actual_result  = future_result.get();

    ICE_TEST_ASSERT(expected_result == actual_result)
}

ICE_TEST_CASE(threadpool_multiple_test)
{
    const unsigned int tasks = 32;
    const unsigned int threads = std::thread::hardware_concurrency();
    const unsigned int delayms = 10;

    const unsigned int totalms = (tasks * delayms) / threads;
    
    ice::core::thread_pool test_pool(threads);
    
    std::vector<std::pair<int, std::future<int>>> test_results(tasks);
    
    const auto start = std::chrono::steady_clock::now();

    for (auto & p : test_results) {
        p.first = rand();
        p.second = test_pool.enqueue(
            [delayms](int x)
            {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(delayms));
                return x;
            },
            p.first);
    }

    for (auto & p : test_results) {
        ICE_TEST_ASSERT(p.first == p.second.get());
    }

    const auto end = std::chrono::steady_clock::now();

    ICE_TEST_ASSERT(
        (std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start).count()) <
        (2 * ((tasks * delayms) / threads)));
}

ICE_UNIT_TEST
{
    threadpool_single_test();
    threadpool_multiple_test();
}