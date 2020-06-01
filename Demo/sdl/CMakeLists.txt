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

assign_source_group(${LOCAL_SRCS})
include_directories(".")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# Demo 01
set(LOCAL_SRCS_01 "")
file(GLOB LOCAL_SRCS_01 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_SOURCE_DIR}/Demo/*.cpp 
    ${CMAKE_SOURCE_DIR}/Demo/*.h
	${CMAKE_SOURCE_DIR}/Demo/Demo_01/*.cpp 
    ${CMAKE_SOURCE_DIR}/Demo/Demo_01/*.h)
	
add_executable(TurboBadgerSDLDemo_01 WIN32 ${LOCAL_SRCS_01})
target_link_libraries(TurboBadgerSDLDemo_01 TurboBadgerLib ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${EXTRA_LIBS})
install(TARGETS TurboBadgerSDLDemo_01
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
	
set_target_properties(TurboBadgerSDLDemo_01 PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)

# Demo 02
set(LOCAL_SRCS_02 "")
file(GLOB LOCAL_SRCS_02 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_SOURCE_DIR}/Demo/*.cpp 
    ${CMAKE_SOURCE_DIR}/Demo/*.h
	${CMAKE_SOURCE_DIR}/Demo/Demo_02/*.cpp 
    ${CMAKE_SOURCE_DIR}/Demo/Demo_02/*.h)
	
add_executable(TurboBadgerSDLDemo_02 WIN32 ${LOCAL_SRCS_02})
target_link_libraries(TurboBadgerSDLDemo_02 TurboBadgerLib ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${EXTRA_LIBS})
install(TARGETS TurboBadgerSDLDemo_02
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

set_target_properties(TurboBadgerSDLDemo_02 PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)