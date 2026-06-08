#include "Core/Common.h"

#include "Sim/Interface/DataRefs/DataRefs.h"

#include "Sim/Interface/DataRefs/EngineDataRefs.h"
#include "Sim/Interface/DataRefs/FlightDataRefs.h"
#include "Sim/Interface/DataRefs/LightingDataRefs.h"

bool DataRefs::Initialize()
{
    bool ok = true;
    ok &= DataRefs::Engine::Initialize();
    ok &= DataRefs::Flight::Initialize();
    ok &= DataRefs::Lighting::Initialize();
    return ok;
}
