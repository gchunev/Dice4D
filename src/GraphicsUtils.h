#ifndef GRAPHICS_UTILS_H
#define GRAPHICS_UTILS_H

#include <stdlib.h>

#include "SDL_opengles2.h"

typedef struct GLES2_Context
{
#define SDL_PROC(ret,func,params) ret (APIENTRY *func) params;
#include "../src/render/opengles2/SDL_gles2funcs.h"
#undef SDL_PROC
} GLES2_Context;

extern GLES2_Context ctx;

#endif
