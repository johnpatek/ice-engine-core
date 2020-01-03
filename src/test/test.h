#include <cassert>
#include <chrono>
#include <fstream>

#define ICE_TEST_CASE(test_case) static void test_case()
#define ICE_TEST_ASSERT(X)                                        \
{                                                                   \
  if( !( X ) )                                                      \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
    );                                                              \
  }                                                                 \
}

#define ICE_UNIT_TEST                                             \
static void unit_test();                                            \
int main(const int argc, const char ** argv)                        \
{                                                                   \
    int status(EXIT_SUCCESS);                                       \
    try                                                             \
    {                                                               \
        unit_test();                                                \
    }                                                               \
    catch(const std::exception& e)                                  \
    {                                                               \
        std::ofstream log(                                          \
            "/tmp/Test.log",                                        \
            std::ios::out | std::ios::app);                         \
        log << e.what() << std::endl;                               \
        status = EXIT_FAILURE;                                      \
    }                                                               \
    return status;                                                  \
}                                                                   \
static void unit_test()
