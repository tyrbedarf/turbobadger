project(TurboBadger)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(modules)

option(TB_BUILD_GFLW_DEMO "Build the Demo application. Depends on glfw." OFF)
option(TB_BUILD_SDL_DEMO "Build the Demo application. Depends on sdl." ON)
option(TB_DEBUG_POSTFIX "Add _d postfix for debug builds" ON)
option(TB_GENERATE_CONFIG "Generate the tb_config.h file from provided parameters in cmake. Default Off" OFF)
option(TB_BUILD_EDITOR "Build Editor Application. This means Editor Widgets will be part of the source as well. Default ON" ON)

# Configure file options
option(TB_FONT_RENDERER_TBBF "Enable to support TBBF fonts (Turbo Badger Bitmap Fonts)" ON)
option(TB_FONT_RENDERER_FREETYPE "Enable FreeType TB Font. Requires FreeType" OFF)
option(TB_IMAGE_LOADER_STB "Enable to support image loading using stb_image.c" OFF)
option(TB_RENDERER_BATCHER "Enable to get TBRendererBatcher" ON)
option(TB_RENDERER_GL "Enable renderer using OpenGL. This renderer depends on TB_RENDERER_BATCHER." ON)
option(TB_RENDERER_GLES_1 "Enable renderer using OpenGL ES. This renderer depends on TB_RENDERER_GL." OFF)
option(TB_IMAGE "Enable support for TBImage, TBImageManager, TBImageWidget." ON)

if(TB_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX _d)
endif()

if(TB_FONT_RENDERER_TBBF)
    set(TB_FONT_RENDERER_TBBF_CONFIG "#define TB_FONT_RENDERER_TBBF")
endif()
if(TB_FONT_RENDERER_FREETYPE)
    set(TB_FONT_RENDERER_FREETYPE_CONFIG "#define TB_FONT_RENDERER_FREETYPE")
endif()
if(TB_IMAGE_LOADER_STB)
    set(TB_IMAGE_LOADER_STB_CONFIG "#define TB_IMAGE_LOADER_STB")
endif()
if(TB_RENDERER_BATCHER)
    set(TB_RENDERER_BATCHER_CONFIG "#define TB_RENDERER_BATCHER")
endif()
if(TB_RENDERER_GL)
    set(TB_RENDERER_GL_CONFIG "#define TB_RENDERER_GL")
endif()
if(TB_RENDERER_GLES_1)
    set(TB_RENDERER_GLES_1_CONFIG "#define TB_RENDERER_GLES_1")
endif()
if(TB_IMAGE) 
    set(TB_IMAGE_CONFIG "#define TB_IMAGE")
endif()

if(TB_BUILD_EDITOR) 
    set(TB_BUILD_EDITOR_CONFIG "#define TB_BUILD_EDITOR")
endif()

configure_file(tb_config.h.in src/tb/config.h)
 
if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -fno-exceptions -fno-rtti -Wall -Wextra -Wno-unused-parameter")
 add_definitions(-DGCC)
endif()

if(APPLE)
 add_definitions(-DMACOSX)
 add_definitions(-Dnullptr=0)
endif(APPLE)

if(MSVC)
 add_definitions(-D_CRT_SECURE_NO_WARNINGS)
 add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
 # Remove exception handling
 STRING(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
 # Remove rtti
 STRING(REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
endif(MSVC)

# Add TurboBadgerLib project
include_directories("${PROJECT_SOURCE_DIR}/src/tb")
add_subdirectory(src/tb)

# Add Demo project
if(TB_BUILD_GFLW_DEMO)
 add_subdirectory(Demo/glfw)
endif(TB_BUILD_GFLW_DEMO)

if(TB_BUILD_SDL_DEMO)
 add_subdirectory(Demo/sdl)
endif(TB_BUILD_SDL_DEMO)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/tb
    DESTINATION include
    PATTERN "*.cpp" EXCLUDE
    PATTERN "*.mm" EXCLUDE
    PATTERN "*.txt" EXCLUDE
    PATTERN "test files" EXCLUDE
    PATTERN "*.h"
)
