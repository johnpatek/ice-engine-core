file(GLOB ENGINE_CORE_SOURCES
    "include/core/*.h"    
    "core/*.cpp"
)

include_directories(include)
add_library(ice-engine-core SHARED ${ENGINE_CORE_SOURCES})

target_include_directories(ice-engine-core PUBLIC include)
target_include_directories(ice-engine-core PUBLIC ${3RDPARTY_INCLUDE})
target_link_libraries(ice-engine-core PUBLIC pthread ssl uWS crypto SDL2 curlpp curl)

set_target_properties(ice-engine-core PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)

install(TARGETS ice-engine-core
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
        
install(DIRECTORY include/ 
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        PATTERN "include/*.h")