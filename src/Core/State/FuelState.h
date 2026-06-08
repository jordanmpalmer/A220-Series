#pragma once

struct FuelState
{
    float leftTankKg;
    float rightTankKg;

    bool leftPumpOn;
    bool rightPumpOn;

    bool crossfeedOpen;
};
