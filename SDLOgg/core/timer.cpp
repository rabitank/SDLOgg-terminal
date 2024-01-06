#include "core/timer.hpp"

uint32_t GetDeltaTick()
{
    static auto old_tick = SDL_GetTicks();
    const uint32_t delta_ms {SDL_GetTicks()- old_tick};
    old_tick = SDL_GetTicks();
    return delta_ms;
}