# A220-Series

Airbus A220 for X-Plane 12, a solo project aiming at high level
systems depth and avionics fidelity. Aircraft systems are modelled from a single
authoritative state, and the displays are drawn as custom avionics devices rather
than relying on the default instruments.

## Status

Active development. The current focus is the pilot-side primary flight display
(PFD), rendered as a custom avionics device in immediate-mode OpenGL. Working: the
artificial horizon (with pitch ladder, roll, and rounded scissor-clipped caps), the
aircraft reference symbol, and the translucent speed/altitude tape backings. In
progress: the FMA, the vertical speed indicator, and the full tape internals (ticks,
numerals, scrolling, V-speeds, selected-value bugs). The first complete vertical
systems slice, the electrical system with its ECAM synoptic page, is the next
milestone after the PFD.

## Requirements

- X-Plane 12, building against the XPLM SDK (`XPLM420`)
- A C++17 toolchain (MSVC on Windows)
- CMake 3.x
- X-Plane SDK headers (XPLM, Widgets, Wrappers)

## Building

The plugin builds as an X-Plane module:

- CMake target type is `MODULE`, not `SHARED`.
- `OUTPUT_NAME` is `A220-Series`, matching the aircraft folder.
- The build output lives in the aircraft's own `plugins/` directory (an
  aircraft-specific plugin, not a global one).

```
cmake -S . -B build
cmake --build build --config Release
```

### Iteration workflow

X-Plane keeps the loaded plugin DLL locked while running, and `XPLMReloadPlugins`
has proven unstable in practice. The reliable loop is:

> **close X-Plane → build → reopen X-Plane**

## Architecture

State is the spine of the project. `AircraftState` is the single source of truth.
Everything either writes into it (the sim interface, the systems) or reads from it
(the displays). Dependencies flow in one direction only:

```
Utils  ←  Core  ←  Sim  ←  Model  ←  Systems  ←  Displays / UI
```

Read right-to-left as "depends on": each layer may use the layers to its left and
never the layers to its right. This keeps the graph acyclic and makes the boundaries
enforceable.

- **Utils**: leaf helpers, depend on nothing internal.
- **Core**: `AircraftState` (the single source of truth, composed of domain
  sub-structs), `PluginState` (owns all plugin-lifetime handles), and `Common.h`.
- **Sim**: the X-Plane boundary: DataRef access and the flight loop.
- **Model**: intermediate/derived state computed from raw sim inputs.
- **Systems**: aircraft system logic (electrical, fuel, …) operating on state.
- **Displays / UI**: the PFD and other rendering, plus debug windows. Read-only
  consumers of state.

Each loop runs a fixed pipeline:

```
Read  →  Model  →  Systems  →  Write  →  Debug
(sim → state)                 (state → sim)
```

DataRefs follow a find-once / read-many pattern: handles are resolved once at
registration and cached as file-scope `static` variables (deliberately not `extern`,
so the boundary is enforced by the compiler), organised into domain namespaces
(`DataRefs::Flight`, `DataRefs::Engine`, `DataRefs::Lighting`).

## Directory layout

```
A220-Series/
├── CMakeLists.txt
└── src/
    ├── Core/
    │   ├── Common.h            # included first in every .cpp, never in a header
    │   ├── AircraftState.h     # single source of truth (domain sub-structs)
    │   └── PluginState.*       # owns plugin-lifetime handles
    ├── Plugin/                 # plugin entry point; thin (~100-line) wiring layer
    ├── Sim/
    │   ├── Interface/          # DataRef access, domain-split namespaces
    │   └── Simulation.cpp      # flight loop, Read→…→Write pipeline
    ├── Model/                  # derived state from raw sim inputs
    ├── Systems/                # electrical, fuel, … (operate on AircraftState)
    ├── Displays/
    │   ├── Common/             # display-agnostic drawing
    │   │   ├── Geometry.h      # primitives: Rect, Point, Inflate, Offset, MirrorX
    │   │   ├── Draw.h/.cpp     # FillRect, FillRectColor, FillPoly, StrokePoly, DrawLine
    │   │   ├── Text.h/.cpp     # DrawText (XPLMDrawString wrapper)
    │   │   └── Colors.h        # shared EFIS palette (RGB)
    │   └── PFD/
    │       ├── Layout.h        # PFD spatial arrangement (screen size, regions, splits)
    │       ├── Theme.h         # PFD appearance scalars (overlay alpha, …)
    │       ├── Device.h/.cpp   # avionics device Register / Unregister
    │       ├── Overlay.h/.cpp  # orchestrator: component frames + shared GL state
    │       ├── Horizon.h/.cpp
    │       ├── SpeedTape.h/.cpp
    │       ├── AltitudeTape.h/.cpp
    │       ├── FMA.h/.cpp
    │       └── VSI.h/.cpp
    ├── UI/                     # debug windows (tabbed: Flight, Engines, Electrical, Fuel)
    └── Utils/                  # leaf helpers
```

## Avionics rendering

The PFD is a custom avionics device (`XPLMCreateAvionicsEx`) drawn in immediate-mode
OpenGL. The rendering code is split by reuse:

- **`Displays/Common/`** holds everything display-agnostic — geometric primitives,
  the draw and text helpers, and the shared EFIS colour palette.
- **`Displays/PFD/`** holds the PFD-specific pieces.

Three header roles keep the PFD's spatial and visual constants honest:

- **Geometry**: the shapes and operations (general, in Common).
- **Layout**: where things go on *this* screen (regions, splits, fractions).
- **Theme**: how things look on *this* screen (overlay translucency, etc.).

Layout follows a strict orchestrator pattern. `Overlay` computes every component's
frame and sets the shared GL state (blend mode and colour mask) once around the whole
overlay pass. Each component — speed tape, altitude tape, FMA, VSI — receives its
frame as a parameter and is responsible only for its own internal sub-layout. Data
flows *down* through parameters; no component reaches back up into the orchestrator,
which is what keeps the include graph and the data flow acyclic. Any geometry shared
across components (a common edge, an aligned row) is decided in the orchestrator, not
negotiated between siblings.

All raw OpenGL is confined to the thin `Draw`/`Text` helpers. That isolation is
deliberate: it is the seam for an eventual migration to Laminar's Panel Graphics API,
which should touch only those helpers rather than every component.

## Conventions

- `Common.h` is included first in every `.cpp` and **never** in a header (keeps
  `windows.h` out of the headers).
- `static_cast` everywhere, except genuine C-API `void*` / `char*` interop.
- File-scope `static` variables are prefixed `s`.
- Header constants are `inline constexpr`, headers are self-contained and include
  what they use.
- `main.cpp` stays a thin wiring layer (~100 lines).

## Roadmap

1. **Finish the PFD** — FMA, VSI, full tape internals (ticks, numerals, scrolling,
   V-speeds, selected bugs), lateral/vertical deviation scales, radio altitude box,
   HSI/mini-nav, and data blocks.
2. **Electrical system** — the first full vertical slice: state structs, update
   logic, an ECAM synoptic page, and a debug tab with input-injection toggles.
3. **Further systems** — fuel and beyond, each as its own vertical slice.
4. **Panel Graphics migration** — move the display draw calls onto Laminar's Panel
   Graphics API when available, replacing `XPLMDrawString` with crisp, scalable text.

---

*Not affiliated with Airbus or Laminar Research.*