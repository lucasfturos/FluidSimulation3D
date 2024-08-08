# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.25 FATAL_ERROR)

# -------------------------------------------------------------------

set(SRC_HEADER
    # Common
    Common/color.hpp
    Common/perlin.hpp
    # Fluid
    Fluid/fluid.hpp
    # Objects
    Objects/objects.hpp
    # Render
    Render/render.hpp
)

set(SRC_SOURCE
    # Fluid
    Fluid/fluid.cpp
    Fluid/logic.cpp
    Fluid/util.cpp
    # Objects
    Objects/objects.cpp
    # Render
    Render/event.cpp
    Render/render.cpp
    Render/setup.cpp
    # Main
    main.cpp
)

add_subdirectory(GLObjects)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} 
    ${SRC_HEADER} 
    ${SRC_SOURCE}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${JSONCPP_INCLUDE_DIRS}
    PRIVATE ${SDL2_INCLUDE_DIRS}
    PRIVATE ${OPENGL_INCLUDE_DIRS}
        PRIVATE Common
    PRIVATE Objects
    PRIVATE GLObjects
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE External_lib
    PRIVATE GLObjects_lib
)

target_link_libraries(${PROJECT_NAME}
    # Jsoncpp
    PRIVATE jsoncpp
    # SDL2
    PRIVATE ${SDL2_LIBRARIES}
    # OpenGL
    PRIVATE glm::glm
    PRIVATE GLEW::GLEW
    PRIVATE ${CMAKE_DL_LIBS}
    PRIVATE ${OPENGL_LIBRARIES}
)

# -------------------------------------------------------------------