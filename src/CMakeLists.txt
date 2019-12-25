file(GLOB ENGINE_CORE_SOURCES
    "include/*.h"    
    "*.cpp"
)

add_library(ice-engine SHARED ${ENGINE_CORE_SOURCES})

target_include_directories(ice-engine PUBLIC include)
target_include_directories(ice-engine PUBLIC ${3RDPARTY_INCLUDE})
target_link_libraries(ice-engine json11 capnp uv uWS)

install(TARGETS ice-engine
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
        
install(DIRECTORY include/ 
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        PATTERN "include/*.h")