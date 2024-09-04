# -------------------------------------------------------------------

cmake_minimum_required(VERSION 3.4...3.25 FATAL_ERROR)

# -------------------------------------------------------------------

set(SRC_HEADER
    # ControlPanel
    ControlPanel/control_panel.hpp
    # Fluid
    Fluid/fluid.hpp
    # GLObjects
    GLObjects/vertex_buffer_layout.hpp
    # Objects
    Objects/objects.hpp
    # Render
    Render/render.hpp
)

set(SRC_SOURCE
    # ControlPanel
    ControlPanel/control_panel.cpp
    ControlPanel/gui.cpp
    # Fluid
    Fluid/fluid.cpp
    Fluid/logic.cpp
    Fluid/util.cpp
    # GLObjects
    GLObjects/vertex_buffer_layout.cpp
    # Objects
    Objects/objects.cpp
    # Render
    Render/event.cpp
    Render/render.cpp
    Render/setup.cpp
    # Main
    main.cpp
)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} 
    ${SRC_HEADER} 
    ${SRC_SOURCE}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    External_lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    # SDL2
    ${SDL2_LIBRARIES}
    # OpenGL
    glm::glm
    GLEW::GLEW
    ${CMAKE_DL_LIBS}
    ${OPENGL_LIBRARIES}
)

# -------------------------------------------------------------------