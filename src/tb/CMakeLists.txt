cmake_minimum_required(VERSION 2.8)

include(modules)

set(LOCAL_SRCS "")

file(GLOB LOCAL_SRCS 
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/image/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/image/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/animation/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/animation/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/parser/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/renderers/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/renderers/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/utf8/*.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/utf8/*.h)
	
file(GLOB HEADER 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/animation/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/renderers/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utf8/*.h)

if(TB_BUILD_EDITOR) 
    file(GLOB_RECURSE EDITOR_SRCS
		${CMAKE_CURRENT_SOURCE_DIR}/editor/*.cpp 
		${CMAKE_CURRENT_SOURCE_DIR}/editor/*.h)
		
	file(GLOB_RECURSE EDITOR_HEADER 
		${CMAKE_CURRENT_SOURCE_DIR}/editor/*.h)
		
	list(APPEND LOCAL_SRCS ${EDITOR_SRCS})
	list(APPEND HEADER ${EDITOR_HEADER})
endif()

assign_source_group(${LOCAL_SRCS})
add_library(TurboBadgerLib ${LOCAL_SRCS})

if(TB_FONT_RENDERER_FREETYPE)
	add_subdirectory("thirdparty/freetype2")
	target_link_libraries (TurboBadgerLib freetype)
endif()

install(TARGETS TurboBadgerLib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
	
install(FILES ${HEADER} DESTINATION include)