#include "pch.hpp"

#include "SDL.h"
#include "SDL_Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

#include "SDL_mixer.h"

#include "sys.hpp"


using namespace ftxui;


SO::Audio togg;



int main(int ,char**)
{
    using namespace SO;
    
    if(!Sys::Init()) 
    {
        SDL_Log("Sys backends init failed!");
    }

    auto togg = Sys::Load("resource/ogg_test1.ogg");
    Sys::Load("resource/ogg_test2.ogg");
    Sys::Load("resource/ogg_test3.ogg");

    Sys::Get()->MainLoop();

    Sys::Quit();
    return 0;
}

