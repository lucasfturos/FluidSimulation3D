#include "control_panel.hpp"

ControlPanel::ControlPanel() {}

void ControlPanel::setup() {
    styleWidget();
    initFont();
}

void ControlPanel::run() {
    ImGui::Begin("Control Panel");
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::SetWindowSize(ImVec2(width, height));
    ImGui::Text("Hello, World!");
    ImGui::End();
}
