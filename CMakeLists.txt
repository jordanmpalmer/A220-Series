cmake_minimum_required(VERSION 3.20)

project(A220SeriesPlugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

# ------------------------------------------------------
# SOURCES (grouped by architecture, not filesystem chaos)
# ------------------------------------------------------

set(SOURCES_CORE
    "src/Core/AircraftState.h"
)

set(SOURCES_PLUGIN
    "src/Plugin/main.cpp"
    "src/Plugin/FlightLoop.cpp"
    "src/Plugin/PluginLifecycle.cpp"
)

set(SOURCES_SIM
    "src/Sim/Simulation.cpp"
    "src/Sim/Scheduler.cpp"
    "src/Sim/Interface/SimInterface.cpp"
)

set(SOURCES_MODEL
    "src/Model/UpdateModel.cpp"
)

set(SOURCES_SYSTEMS
    "src/Systems/Systems.cpp"
)

set(SOURCES_DISPLAYS
	"src/Displays/Common/Draw.cpp"
	"src/Displays/PFD/AltitudeTape.cpp"
    "src/Displays/PFD/Device.cpp"
    "src/Displays/PFD/FMA.cpp"
	"src/Displays/PFD/Horizon.cpp"
    "src/Displays/PFD/Overlay.cpp"
	"src/Displays/PFD/SpeedTape.cpp"
    "src/Displays/PFD/VSI.cpp"
    "src/Displays/Common/Text.cpp" 
)

set(SOURCES_UI
    "src/UI/Debug.cpp"
    "src/UI/Menu.cpp"
)

set(SOURCES_DATAREFS
    "src/Sim/Interface/DataRefs/DataRefs.cpp"
    "src/Sim/Interface/DataRefs/EngineDataRefs.cpp"
    "src/Sim/Interface/DataRefs/FlightDataRefs.cpp"
    "src/Sim/Interface/DataRefs/LightingDataRefs.cpp"
)

# ------------------------------------------------------
# BUILD TARGET
# ------------------------------------------------------

add_library(A220SeriesPlugin MODULE
    ${SOURCES_CORE}
    ${SOURCES_PLUGIN}
    ${SOURCES_SIM}
    ${SOURCES_MODEL}
    ${SOURCES_SYSTEMS}
    ${SOURCES_DISPLAYS}
    ${SOURCES_UI}
    ${SOURCES_DATAREFS}
)

# ------------------------------------------------------
# PLATFORM DEFINITIONS
# ------------------------------------------------------

if(WIN32)
    target_compile_definitions(A220SeriesPlugin PRIVATE IBM=1 APL=0 LIN=0)
elseif(APPLE)
    target_compile_definitions(A220SeriesPlugin PRIVATE IBM=0 APL=1 LIN=0)
elseif(UNIX)
    target_compile_definitions(A220SeriesPlugin PRIVATE IBM=0 APL=0 LIN=1)
endif()

target_compile_definitions(A220SeriesPlugin PRIVATE
    XPLM200=1
    XPLM210=1
    XPLM300=1
    XPLM301=1
    XPLM400=1
    XPLM410=1
    XPLM420=1
    _CRT_SECURE_NO_WARNINGS
)

# ------------------------------------------------------
# SDK PATHS
# ------------------------------------------------------

set(XPLANE12_PATH $ENV{XPLANE12_PATH})
set(XPLANE_SDK_PATH $ENV{XPLANE_SDK})

if(NOT XPLANE12_PATH)
    message(FATAL_ERROR "XPLANE12_PATH not set")
endif()

if(NOT XPLANE_SDK_PATH)
    message(FATAL_ERROR "XPLANE_SDK environment variable is not set!")
endif()

# ------------------------------------------------------
# INCLUDE DIRECTORIES
# ------------------------------------------------------

target_include_directories(A220SeriesPlugin PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${XPLANE_SDK_PATH}/CHeaders/XPLM"
    "${XPLANE_SDK_PATH}/CHeaders/Widgets"
    "${XPLANE_SDK_PATH}/CHeaders/Wrappers"
)

# ------------------------------------------------------
# LINKING
# ------------------------------------------------------

if(WIN32)
    target_link_directories(A220SeriesPlugin PRIVATE "${XPLANE_SDK_PATH}/Libraries/Win")
    target_link_libraries(A220SeriesPlugin PRIVATE XPLM_64 opengl32)
elseif(APPLE)
    target_link_directories(A220SeriesPlugin PRIVATE "${XPLANE_SDK_PATH}/Libraries/Mac")
    target_link_libraries(A220SeriesPlugin PRIVATE XPLM "-framework OpenGL")
elseif(UNIX)
    # Linux doesn't link against the SDK libraries directly
    target_link_libraries(A220SeriesPlugin PRIVATE GL)
    target_link_options(A220SeriesPlugin PRIVATE
        -rdynamic -nodefaultlibs -fvisibility=hidden
    )
endif()


# ------------------------------------------------------
# OUTPUT SETTINGS
# ------------------------------------------------------

set_target_properties(A220SeriesPlugin PROPERTIES
    OUTPUT_NAME "A220-Series"
    PREFIX ""
    SUFFIX ".xpl"
)

if(APPLE)
    set_target_properties(A220SeriesPlugin PROPERTIES
        BUNDLE TRUE
        XCODE_ATTRIBUTE_WRAPPER_EXTENSION "xpl"
    )
elseif(UNIX)
    target_compile_options(A220SeriesPlugin PRIVATE -fvisibility=hidden)
endif()

# ------------------------------------------------------
# AUTO-DEPLOY TO X-PLANE AIRCRAFT FOLDER
# ------------------------------------------------------

set(XP_PLUGIN_PATH
    "${XPLANE12_PATH}/Aircraft/MyAircraft/A220-300/plugins/A220-Series/win_x64"
)

add_custom_command(TARGET A220SeriesPlugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${XP_PLUGIN_PATH}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:A220SeriesPlugin>
        ${XP_PLUGIN_PATH}
)
