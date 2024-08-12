#pragma once

#include "../../external/imgui/imgui.h"
#include "../Common/util.hpp"

class ControlPanel {
  protected:
    const int width = 400;
    const int height = 200;

  private:
    SimulationParameters params;

    // GUI
    void initFont();
    void styleWidget();

  public:
    ControlPanel();

    SimulationParameters getSimulationParams();
    void setup();
    void run();
};
