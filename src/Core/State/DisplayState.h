#pragma once

#include "Core/State/PFDState.h"
#include "Core/State/NDState.h"
#include "Core/State/ECAMState.h"

struct DisplayState
{
    PFDState  pfdPilot;
    PFDState  pfdCopilot;
    NDState   ndPilot;
    NDState   ndCopilot;
    ECAMState ecam;
};