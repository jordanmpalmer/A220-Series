#pragma once

// Platform setup
#if IBM
	#include <windows.h>
#endif
#if LIN
	#include <GL/gl.h>
#elif __GNUC__
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

// SDK version guard — requires X-Plane 12.04 or newer
#ifndef XPLM420
	#error This plugin requires XPLM420 (X-Plane 12.3.0 or newer)
#endif
