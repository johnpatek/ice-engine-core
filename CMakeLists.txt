cmake_minimum_required(VERSION 3.0)

project(ice-engine-core)

SET(CMAKE_CXX_FLAGS  "-std=c++14")

set(3RDPARTY ../ice-engine-3rdparty/build/install)

set(3RDPARTY_INCLUDE ${3RDPARTY}/include)

set(3RDPARTY_LIB ${3RDPARTY}/lib)

set(CURL_INCLUDE_DIR ${3RDPARTY_INCLUDE})

set(ZLIB_LIBRARY libz.a)
set(ZLIB_INCLUDE ${3RDPARTY}/include)

set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(include ${3RDPARTY_INCLUDE})
link_directories(${3RDPARTY_LIB})

add_subdirectory(src)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

if (WIN32)
    set(PLATFORM "WINDOWS")
    set(PACKAGE "MSI")
elseif (APPLE)
    set(PACKAGE "APPLE")
    set(PLATFORM "APPLE")
    set(GENERATOR "APPLE")
else()
    set(PACKAGE "DEB")
    set(PLATFORM "UBUNTU")
    set(GENERATOR "DEBIAN")
endif()

set(CPACK_PACKAGE_NAME "ice-engine-core")

install(
  DIRECTORY ${CMAKE_INSTALL_PREFIX}/
  DESTINATION .
  COMPONENT core
  USE_SOURCE_PERMISSIONS
  FILES_MATCHING
  PATTERN *.h
  PATTERN *.hpp
  PATTERN *.a
  PATTERN *.so*
  PATTERN share EXCLUDE
  PATTERN bin EXCLUDE
  PATTERN pkgconfig EXCLUDE
  PATTERN cmake EXCLUDE)

set(CPACK_${PACKAGE}_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_ALL 3rdparty)


set(CPACK_GENERATOR ${PACKAGE})

if(WIN32)
    set(CPACK_PACKAGING_INSTALL_PREFIX "C:/Program Files/ice-core")
else()
    set(CPACK_PACKAGING_INSTALL_PREFIX /usr/share/ice-core)
endif()

set(CPACK_PACKAGE_CONTACT "John R. Patek <johnpatek2@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION "ICEngine Core")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_VENDOR "John Patek")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
include (CPack)