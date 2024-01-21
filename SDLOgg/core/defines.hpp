#pragma once
#include "pch.hpp"

#include "SDL.h"

#define PLOG


#ifdef PLOG
#define SO_LOG(format , ...) \
    SDL_Log(format, ##__VA_ARGS__ );
#else
#define SO_LOG(format , ...)
#endif


namespace SO
{  
    template<typename T>
    using Ref = std::shared_ptr<T> ;
}