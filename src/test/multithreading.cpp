#include "test.h"
#include "core/multithreading.h"


ICE_TEST_CASE(threadpool_single_test)
{
    ice::core::thread_pool test_pool_1(1);
    ice::core::heap_buffer test_buffer_1;
    const int test_value_1 = 100;

    auto expected_result_1 = test_value_1;

    auto future_result = test_pool_1.enqueue(
        [](int x)
        {
            return x;
        },
        test_value_1);
    
    auto actual_result_1  = future_result.get();

    ICE_TEST_ASSERT(expected_result_1 == actual_result_1)
}

ICE_TEST_CASE(threadpool_multiple_test)
{
    const unsigned int tasks = 32;
    const unsigned int threads = std::thread::hardware_concurrency();
    const unsigned int delayms = 10;

    const unsigned int totalms = (tasks * delayms) / threads;
    
    ice::core::thread_pool test_pool_1(threads);
    
    std::vector<std::pair<int, std::future<int>>> test_results(tasks);
    
    const auto start = std::chrono::steady_clock::now();

    for (auto & p : test_results) {
        p.first = rand();
        p.second = test_pool_1.enqueue(
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