#pragma once

#include "SDL.h"
#define DY(x,s) if(!x) SDL_Log(s)
#define WDY(x,s) if(!x) SDL_Log((std::string("Waraing!")+s).c_str())
