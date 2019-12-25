file(GLOB ENGINE_CORE_SOURCES
    "include/*.h"    
    "*.cpp"
)

add_library(ice-engine ${CORE_SOURCES})

target_include_directories(ice-engine include)
target_include_directories(ice-engine ${3RDPARTY_INCLUDE})
target_link_libraries(ice-engine ${3RDPARTY_LIB})