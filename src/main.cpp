#include "Render/render.hpp"
#include <memory>

int main(void) {
    try {
        auto render = std::make_shared<Render>();
        render->run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}