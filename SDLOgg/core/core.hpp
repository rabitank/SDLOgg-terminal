#pragma once


#include "base.hpp"
#include <set>
#include "SDL.h"
namespace fs
{

    #define PI acos(-1)
    template<typename T,typename cp>
    using Ref_set = std::set<std::shared_ptr<T>,cp>;
}

#define WINDOW_MID_H 760
#define WINDOW_MID_W 1280


