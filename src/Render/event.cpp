#include "render.hpp"

void Render::handleEvents() {
    SDL_Event event;
    static bool isDragging = false;
    static glm::vec2 lastMousePos = glm::vec2(0.0f);
    
    while (SDL_PollEvent(&event) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                quit = true;
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                isDragging = true;
                lastMousePos = glm::vec2(event.button.x, event.button.y);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                isDragging = false;
            }
            break;
        case SDL_MOUSEMOTION:
            if (isDragging) {
                glm::vec2 mouseDelta =
                    glm::vec2(event.motion.x, event.motion.y) - lastMousePos;
                lastMousePos = glm::vec2(event.motion.x, event.motion.y);

                float moveSpeed = 0.003f;
                glm::vec3 moveDirection(mouseDelta.x * moveSpeed,
                                        -mouseDelta.y * moveSpeed, 0.0f);

                if (objects) {
                    glm::vec3 currentPos = objects->getPosition();
                    objects->setPosition(currentPos + moveDirection);
                }
            }
            break;
        default:
            break;
        }
    }
}
