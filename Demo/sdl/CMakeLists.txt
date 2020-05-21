# Use project source dir for demo output. We have to set
# the release and debug specific defines too, for MSVC.
include(modules)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR})

if(CMAKE_SYSTEM MATCHES "Windows")
 add_definitions(-DGLFW_EXPOSE_NATIVE_WIN32)
 add_definitions(-DGLFW_EXPOSE_NATIVE_WGL)
 set(EXTRA_LIBS ${EXTRA_LIBS} winmm)
endif(CMAKE_SYSTEM MATCHES "Windows")

# Add sources
set(LOCAL_SRCS "")
file(GLOB_RECURSE LOCAL_SRCS 
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/source/events/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/events/*.h)

assign_source_group(${LOCAL_SRCS})
include_directories(".")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

add_executable(TurboBadgerSDLDemo WIN32 ${LOCAL_SRCS})
target_link_libraries(TurboBadgerSDLDemo TurboBadgerLib ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${EXTRA_LIBS})
install(TARGETS TurboBadgerSDLDemo
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
	
set_target_properties(TurboBadgerSDLDemo PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)
