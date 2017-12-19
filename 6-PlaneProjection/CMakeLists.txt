cmake_minimum_required(VERSION 3.6)

project(6-plane-projection)

#########################################################
# Linker options
add_definitions(
    -DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

####################################
# source files
set(SOURCES 
    main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../common/shader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../common/shader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../common/models.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/../common/models.cpp)
	
set(RESOURCES
   	6-plane-projection.frag 
	6-plane-projection.vert
	input.jpg )
	
#####################################
# include external headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../external/include/GLFW
                    ${CMAKE_CURRENT_SOURCE_DIR}/../external/include/GLEW
					${CMAKE_CURRENT_SOURCE_DIR}/../external/include/SOIL					
					${CMAKE_CURRENT_SOURCE_DIR}/../common)
					
# link externl libraries
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../external/lib
                 ${CMAKE_CURRENT_SOURCE_DIR}/../external/lib/glfw/lib-vc2015)
				 
# set variables
set(ALL_LIBS 
    glfw3.lib 
	glew32s.lib 
	SOIL.lib) 

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