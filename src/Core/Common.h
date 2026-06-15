#pragma once

// Platform setup
#if IBM
	#include <windows.h>
    #include <GL/gl.h>
#elif APL
    #include <OpenGL/gl.h>
//    #include <OpenGL/gl3.h>
#elif LIN
	#include <GL/gl.h>
#endif

// SDK version guard — requires X-Plane 12.04 or newer
#ifndef XPLM420
	#error This plugin requires XPLM420 (X-Plane 12.3.0 or newer)
#endif
