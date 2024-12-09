#pragma once

#include "imgui.h"
#include "Common/util.hpp"

class ControlPanel {
  protected:
    const int width = 300;
    const int height = 345;

  private:
    SimulationParams params;
    ObjectType objectType;

    // GUI
    void initFont();
    void styleWidget();

  public:
    ControlPanel();

    ObjectType getObjectType();
    SimulationParams getSimulationParams();

    void setup();
    void run();
};
