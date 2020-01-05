include_directories(${PROJECT_SOURCE_DIR}/src/include)

add_executable(memory_test memory.cpp)
add_executable(multithreading_test multithreading.cpp)

target_include_directories(memory_test PUBLIC ${PROJECT_SOURCE_DIR}/src/include)
target_include_directories(multithreading_test PUBLIC ${PROJECT_SOURCE_DIR}/src/include)

target_link_libraries(memory_test ice-engine)
target_link_libraries(multithreading_test ice-engine)

add_test(NAME memory COMMAND memory_test)
add_test(NAME multithreading COMMAND multithreading_test)