#pragma once

#include "../../external/imgui/imgui.h"
#include "../Common/util.hpp"

class ControlPanel {
  protected:
    const int width = 400;
    const int height = 200;

  private:
    SimulationParams params;

    // GUI
    void initFont();
    void styleWidget();

  public:
    ControlPanel();

    SimulationParams getSimulationParams();
    void setup();
    void run();
};
