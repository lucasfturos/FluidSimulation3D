# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.25 FATAL_ERROR)

# -------------------------------------------------------------------

set(PROJECT_NAME
    Fluid3D
    LANGUAGES CXX
    VERSION 1.0.0
)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

set(RESOURCES_DIRECTORY 
  "${CMAKE_SOURCE_DIR}/assets"
)

file(COPY ${RESOURCES_DIRECTORY} 
  DESTINATION ${CMAKE_BINARY_DIR}
)

add_compile_options(
    -Wall -Wextra -Wpedantic -Werror -pedantic -g -Ofast 
)

add_subdirectory(src)

# -------------------------------------------------------------------