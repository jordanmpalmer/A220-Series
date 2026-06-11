#pragma once

// One electrical power source (generator, APU gen, ext power, RAT, battery).
struct ElectricalSouce
{
	bool available = false;
	bool online	   = false;
	float voltage  = 0.0f;
};

// A battery is a source with charge state.
struct Battery
{
	ElectricalSource source;
	float charge = 0.0f;
};

// One electrical bus
struct ElectricalBus
{
	bool powered = false;
	float voltage = 0.0f;
};

// A transformer rectifier unit (TRU): AC in -> DC out.
struct TransformerRectifier
{
	bool  active = false;     // converting (fed by a powered AC bus)
	float dcOutput = 0.0f;     // VDC
};

enum class OffOnSwitch         { Off, On };
enum class OffAutoPosition     { Off, Auto };
enum class OffAutoOnPosition   { Off, Auto, On };
enum class OffRunStartPosition { Off, Run, Start };
enum class BusIsolPosition     { Main, Auto, Ess };

struct ElectricalState
{
	// --- AC sources (24-20) ---
	ElectricalSource genL;        // left VFG
	ElectricalSource genR;        // right VFG
	ElectricalSource apuGen;      // APU generator
	ElectricalSource extPower;    // external ground power
	ElectricalSource ratGen;      // ram air turbine generator (emergency)

	// --- DC generation (24-30) ---
	TransformerRectifier tru1;
	TransformerRectifier tru2;
	TransformerRectifier tru3;
	Battery battery1;
	Battery battery2;

	// --- AC buses (24-02) ---
	ElectricalBus acBus1;
	ElectricalBus acBus2;

	// --- DC buses (24-02) ---
	ElectricalBus dcBus1;
	ElectricalBus dcBus2;
	ElectricalBus dcEssBus;     // interrupt-free DC essential bus

	// --- Switch positions (ELECTRICAL panel) ---
	OffOnSwitch         genLSwitch      = OffOnSwitch::On;
	OffOnSwitch         genRSwitch      = OffOnSwitch::On;
	OffAutoPosition     apuGenSwitch    = OffAutoPosition::Off;
	OffAutoPosition     extPowerSwitch  = OffAutoPosition::Off;
	OffAutoPosition     battery1Switch  = OffAutoPosition::Auto;
	OffAutoPosition     battery2Switch  = OffAutoPosition::Auto;
	BusIsolPosition     busIsolSwitch   = BusIsolPosition::Auto;
	OffRunStartPosition apuMasterSwitch = OffRunStartPosition::Off;
	bool                cabinPwrSwitch  = false;
	bool                ratGenSwitch    = true;

	// Latched disconnect state (one-way action, not a toggle)
	bool genLDisconnected = false;
	bool genRDisconnected = false;

};