#pragma once

#include "../Fluid/fluid.hpp"
#include "../Objects/objects.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <memory>

class Render {
  protected:
    const int screenWidth = 1080;
    const int screenHeight = 720;
    const int fps = 60;
    const Uint32 frameDelay = 1000.0 / fps;
    const std::string title = "Fluid Simulation 3D";

    const glm::mat4 viewDefaultMat =
        glm::lookAt(glm::vec3(0.0f, 2.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));

  private:
    SDL_Window *window;
    SDL_GLContext context;
    std::shared_ptr<Fluid> fluid;
    std::shared_ptr<Objects> objects;

    bool quit;

  private:
    Uint32 frameStart;
    Uint32 frameTime;

    // Setup
    void clear();
    void initOpenGL();
    void setupWindow();
    void destroyWindow();

    // Event
    void handleEvents();

    void setup();
    void draw();

  public:
    Render();
    ~Render();

    void run();
};