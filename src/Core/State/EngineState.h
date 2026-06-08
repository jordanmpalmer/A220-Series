#pragma once

struct EngineState
{
	// Core speeds
	float n1 = 0.0f;   // fan speed (%)
	float n2 = 0.0f;   // core speed (%)

	// Temperatures
	float egt = 0.0f;  // exhaust gas temperatures

	// Fuel
	float fuelFlow = 0.0f; // kg/h

	// Stats
	float oilPressure;
	float oilTemp;

	// Status
	bool running    = false;
	bool starting   = false;
	bool fireHandle = false; // fire handle pulled
};
