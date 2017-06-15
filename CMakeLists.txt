cmake_minimum_required(3.6)

project(OpenGL_Playground)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libs)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#########################################################
# Output information
message("Root folder: ${PROJECT_SOURCE_DIR}")

#########################################################
# External dependency project
add_subdirectory(SOIL)

#########################################################
# Demo projects
add_subdirectory(1-Startup)
add_subdirectory(2-SphereTexture)
