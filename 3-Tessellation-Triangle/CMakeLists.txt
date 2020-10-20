cmake_minimum_required(VERSION 3.6)

project(3-tessellation-triangle)

#########################################################
# Linker options
add_definitions(
	-D_CRT_SECURE_NO_WARNINGS
)

####################################
# source files
set(SOURCES 
    main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../common/shader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../common/shader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../common/glad.c)
	
set(RESOURCES
   	3-tessellation-triangle.frag 
	3-tessellation-triangle.vert
	3-tessellation-triangle.tcs
	3-tessellation-triangle.tes)
	
#####################################
# include external headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../external/include/GLFW
					${CMAKE_CURRENT_SOURCE_DIR}/../common)
					
# link externl libraries
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/lib
                 ${CMAKE_CURRENT_SOURCE_DIR}/../external/lib/glfw/lib-vc2015)
				 
# set variables
set(ALL_LIBS 
    glfw3.lib) 

find_package(OpenGL REQUIRED)

######################################
# Output
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS} ${OPENGL_LIBRARY})

######################################
# Custom command
if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

foreach(RESOURCE_ITEM ${RESOURCES})
add_custom_command(
   TARGET ${PROJECT_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_ITEM}" "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/" 
)
endforeach(RESOURCE_ITEM)

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode")