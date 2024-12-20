#include "render.hpp"

Render::Render()
    : window(nullptr), context(nullptr),
      fluid(std::make_shared<Fluid>(projDefaultMat)),
      objects(std::make_shared<Objects>(projDefaultMat)),
      controlPanel(std::make_shared<ControlPanel>()), quit(false) {}

Render::~Render() { destroyWindow(); }

void Render::setup() {
    setupWindow();
    initOpenGL();
    setupImGui();

    controlPanel->setup();
    objects->setup();
    fluid->setup();
}

void Render::draw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    controlPanel->run();

    fluid->setSimulationParams(controlPanel->getSimulationParams());
    objects->setObjectType(controlPanel->getObjectType());

    fluid->detectCollisions(objects);

    objects->run();
    fluid->run();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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