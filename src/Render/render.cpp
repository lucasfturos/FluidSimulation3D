#include "render.hpp"

Render::Render()
    : window(nullptr), context(nullptr),
      fluid(std::make_shared<Fluid>(viewDefaultMat, projDefaultMat)),
      objects(std::make_shared<Objects>(viewDefaultMat, projDefaultMat)),
      quit(false) {
    setupWindow();
    initOpenGL();

    fluid->setFilename("./assets/data/simulation_params.json");
}

Render::~Render() { destroyWindow(); }

void Render::setup() {
    // objects->setup();
    fluid->setup();
}

void Render::draw() {
    // objects->run();
    fluid->run();
}

void Render::run() {
    static float t = 0;

    setup();
    Uint32 lastTime = SDL_GetTicks();

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        t += deltaTime;
        objects->setTime(t);
        fluid->setTime(t);

        frameStart = SDL_GetTicks();
        handleEvents();

        clear();
        draw();

        SDL_GL_SwapWindow(window);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}