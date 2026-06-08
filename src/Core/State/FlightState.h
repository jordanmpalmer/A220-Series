#pragma once

struct FlightState
{
    float pitchDeg;
    float rollDeg;
    float yawDeg;
    float headingDeg;

    float altitudeFt;

    float indicatedAirspeedKt;
    float trueAirspeedKt;

    float mach;

    float alphaDeg;
    float betaDeg;

    float verticalSpeedFpm;
};
