cmake_minimum_required(3.6)

project(1-Startup)

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
	../common/shader.hpp
	../common/shader.cpp
	simple.frag
	simple.geom
	simple.vert)
	
#####################################
# include external headers
include_directories(../external/include)
include_directories(../external/include/GLFW)
include_directories(../external/include/GLEW)
# link externl libraries
link_directories(../external/lib)
link_directories(../external/lib/glfw/lib-vc2015)
# set variables
set(GLFW_LIB glfw3.lib)
set(GLEW_LIB glew32.lib) 

find_package(OpenGL REQUIRED)

######################################
# Output
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${GLFW} ${GLEW} ${OPENGL_LIBRARY});

######################################
# Custom command
if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET ${PROJECT_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/"
)
elseif (${CMAKE_GENERATOR} MATCHES "Xcode")
# Do nothing
endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode")