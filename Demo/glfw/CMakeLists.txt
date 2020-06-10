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

# Configure glfw
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)

# Add sources
set(LOCAL_SRCS "")
file(GLOB LOCAL_SRCS 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_SOURCE_DIR}/Demo/*.cpp 
    ${CMAKE_SOURCE_DIR}/Demo/*.h
	${CMAKE_SOURCE_DIR}/Demo/Demo_01/*.cpp 
    ${CMAKE_SOURCE_DIR}/Demo/Demo_01/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

assign_source_group(${LOCAL_SRCS})
include_directories(".")

# Add glfw project
target_include_directories(TurboBadgerLib PUBLIC "thirdparty/glfw/include")
include_directories("thirdparty/glfw/include")
add_subdirectory(thirdparty/glfw)

# Platform specific
if(CMAKE_COMPILER_IS_MINGW)
 # Avoid dll dependencies by linking statically.
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
endif(CMAKE_COMPILER_IS_MINGW)

# Demo 01
set(LOCAL_SRCS_01 "")	
file(GLOB LOCAL_SRCS_01 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_SOURCE_DIR}/Demo/*.cpp 
    ${CMAKE_SOURCE_DIR}/Demo/*.h
	${CMAKE_SOURCE_DIR}/Demo/Demo_01/*.cpp 
    ${CMAKE_SOURCE_DIR}/Demo/Demo_01/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
	
add_executable(TurboBadgerGLFWDemo_01 WIN32 ${LOCAL_SRCS_01})
target_link_libraries(TurboBadgerGLFWDemo_01 TurboBadgerLib ${EXTRA_LIBS} glfw ${GLFW_LIBRARIES})
install(TARGETS TurboBadgerGLFWDemo_01
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
	
set_target_properties(TurboBadgerGLFWDemo_01 PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)

# Demo 02
set(LOCAL_SRCS_02 "")	
file(GLOB LOCAL_SRCS_02 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_SOURCE_DIR}/Demo/*.cpp 
    ${CMAKE_SOURCE_DIR}/Demo/*.h
	${CMAKE_SOURCE_DIR}/Demo/Demo_02/*.cpp 
    ${CMAKE_SOURCE_DIR}/Demo/Demo_02/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
	
add_executable(TurboBadgerGLFWDemo_02 WIN32 ${LOCAL_SRCS_02})
target_link_libraries(TurboBadgerGLFWDemo_02 TurboBadgerLib ${EXTRA_LIBS} glfw ${GLFW_LIBRARIES})
install(TARGETS TurboBadgerGLFWDemo_02
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
	
set_target_properties(TurboBadgerGLFWDemo_02 PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)

# Editor
set(EDITOR_SRCS "")	
file(GLOB EDITOR_SRCS 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_SOURCE_DIR}/Demo/*.cpp 
    ${CMAKE_SOURCE_DIR}/Demo/*.h
	${CMAKE_SOURCE_DIR}/Demo/Demo_02/*.cpp 
    ${CMAKE_SOURCE_DIR}/Demo/Demo_02/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
	
add_executable(EditorGLFW WIN32 ${EDITOR_SRCS})
target_link_libraries(EditorGLFW TurboBadgerLib ${EXTRA_LIBS} glfw ${GLFW_LIBRARIES})
install(TARGETS EditorGLFW
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
	
set_target_properties(EditorGLFW PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)
