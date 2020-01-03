add_executable(memory_test memory.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src/include)

target_include_directories(memory_test PUBLIC ${PROJECT_SOURCE_DIR}/src/include)

target_link_libraries(memory_test ice-engine)

add_test(NAME memory COMMAND memory_test)