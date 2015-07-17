cmake_minimum_required(VERSION 2.8)

project(7-Gears)

option(UseValgrind "UseValgrind" OFF)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/cmake")
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/3rdParty/solar-cmake")

include(Solar)

# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH "")

target_architecture(arch)
message(STATUS "arch is ${arch}")

# i386 for 32bit and x86_64 for 64bit
if ("${arch}" STREQUAL "i386")
    set (arch "x86")
elseif("${arch}" STREQUAL "x86_64")
    set (arch "x64")
endif()

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

enable_testing()

# The version number.
set(7GEARS_VERSION_MAJOR 0)
set(7GEARS_VERSION_MINOR 0)
set(7GEARS_VERSION_PATCH 0)

# Enable Visual Studio solution "folders"
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON) 

# On Linux gtest needs pthreads
if (UNIX)
    find_package (Threads)
endif()

if ( APPLE )
# Required to prevent runtime crash on 64bit OSX - however this breaks 32bit compile
  set ( CMAKE_EXE_LINKER_FLAGS "-pagezero_size 10000 -image_base 100000000 -image_base 7fff04c4a000" )
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "release" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
  #message("RELEASE set")
endif ()

add_definitions(-DGLEW_STATIC)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/nanovg/src
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glew-1.11.0/include
    ${SDL2_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty
)

SET(nanovg_src 
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/nanovg/src/fontstash.h
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/nanovg/src/nanovg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/nanovg/src/nanovg.h
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/nanovg/src/nanovg_gl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/nanovg/src/nanovg_gl_utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/nanovg/src/stb_image.h
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/nanovg/src/stb_truetype.h
)
add_library(NanoVg STATIC ${nanovg_src})
SET_PROPERTY(TARGET NanoVg PROPERTY FOLDER "3rdparty")

SET(glew_src 
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glew-1.11.0/src/glew.c
)

add_library(glew STATIC ${glew_src})
SET_PROPERTY(TARGET glew PROPERTY FOLDER "3rdparty")


add_executable(7-Gears MACOSX_BUNDLE
    inc/kernel/texfile.hpp
    src/kernel/texfile.cpp
    src/kernel/lgp.cpp
    inc/kernel/lgp.hpp
    src/kernel/filesystem.cpp
    inc/kernel/filesystem.hpp
    src/kernel/stream.cpp
    inc/kernel/stream.hpp
    src/kernel/kernel.cpp
    inc/kernel/kernel.hpp
    src/menu/menu.cpp
    inc/menu/menu.hpp
    inc/exceptions.hpp
    inc/logger.hpp
    inc/engine.hpp
    src/engine.cpp
    src/main.cpp
)

set(CPACK_PACKAGE_EXECUTABLES 7-Gears "7-Gears")
set(CPACK_WIX_PROGRAM_MENU_FOLDER "7-Gears")
set(CPACK_PACKAGE_VENDOR "7-Gears team")


TARGET_LINK_LIBRARIES(7-Gears glew NanoVg ${OPENGL_LIBRARIES} ${SDL2_LIBRARY} )
install(
    TARGETS 7-Gears 
    BUNDLE DESTINATION .
    RUNTIME DESTINATION .)



if (WIN32 AND MSVC)
   add_custom_command(TARGET 7-Gears POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy "${SDL2_INCLUDE_DIR}/../lib/${arch}/SDL2.dll" "$(OutDir)SDL2.dll")
   INSTALL(FILES "${SDL2_INCLUDE_DIR}/../lib/${arch}/SDL2.dll" DESTINATION ".")
endif()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

INCLUDE(CPack)
