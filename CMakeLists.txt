cmake_minimum_required(VERSION 3.0)

project(ice-engine-core)

set(CXX_STANDARD 11)

set(3RDPARTY ../ice-engine-3rdparty/install)

set(3RDPARTY_INCLUDE ${3RDPARTY}/include)

set(3RDPARTY_LIB ${3RDPARTY}/lib)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)

add_subdirectory(src)