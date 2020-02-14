file(GLOB TEST_SOURCES
    "*.h"    
    "*.cpp"
)

add_executable(unittest ${TEST_SOURCES})
target_include_directories(unittest PRIVATE ${PROJECT_SOURCE_DIR}/src/include ${3RDPARTY_INCLUDE})
target_link_libraries(unittest gtest gtest_main ice-engine-core cpprest ${BOOST_SYSTEM_LIBRARY})
