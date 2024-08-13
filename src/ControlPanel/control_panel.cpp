#include "control_panel.hpp"
#include <vector>

ControlPanel::ControlPanel()
    : params({16, 1.0e-5f, 1.0e-6f, 0.1f}), objectType(ObjectType::None) {}

ObjectType ControlPanel::getObjectType() { return objectType; }

SimulationParams ControlPanel::getSimulationParams() { return params; }

void ControlPanel::setup() {
    styleWidget();
    initFont();
}

void ControlPanel::run() {
    ImGui::Begin("Control Panel");
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::SetWindowSize(ImVec2(width, height));

    if (ImGui::CollapsingHeader("Simulation Parameters",
                                ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::PushItemWidth(200);
        ImGui::InputInt("Iterations", &params.iter, 1, 30);
        ImGui::InputFloat("Diffusion", &params.diffusion, 0.0f, 0.0f, "%.1e");
        ImGui::InputFloat("Viscosity", &params.viscosity, 0.0f, 0.0f, "%.1e");
        ImGui::InputFloat("dt", &params.dt, 0.0f, 0.0f, "%.1e");
        ImGui::PopItemWidth();
        if (ImGui::Button("Reset")) {
            params = {16, 1.0e-5f, 1.0e-6f, 0.1f};
        }
    }

    if (ImGui::CollapsingHeader("Object Selection")) {
        ImGui::PushItemWidth(200);
        std::vector<const char *> objectTypeNames = {
            "None", "Sphere", "Cylinder", "Plane", "Cube"};
        int currentType = static_cast<int>(objectType);
        if (ImGui::Combo(" ", &currentType, objectTypeNames.data(),
                         objectTypeNames.size())) {
            objectType = static_cast<ObjectType>(currentType);
        }
        ImGui::PopItemWidth();
        ImGui::SameLine();
        if (ImGui::Button("Default")) {
            objectType = ObjectType::None;
        }
    }

    ImGui::End();
}