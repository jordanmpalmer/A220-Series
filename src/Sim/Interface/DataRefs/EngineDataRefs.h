#pragma once

namespace DataRefs::Engine
{
    bool Initialize();

    float GetN1(int engine);
    float GetN2(int engine);
    float GetEGT(int engine);
    float GetFuelFlow(int engine);
    float GetOilPressure(int engine);
    float GetOilTemp(int engine);

    bool  GetRunning(int engine);
    bool  GetStarting(int engine);
    bool  GetFireHandle(int engine);
}
