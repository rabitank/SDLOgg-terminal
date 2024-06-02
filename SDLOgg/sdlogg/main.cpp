#define RUNMODE

#ifdef RUNMODE

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

    // auto togg = Sys::Load("resource/ogg_test1.ogg");
    // Sys::Load("resource/ogg_test2.ogg");
    // Sys::Load("resource/ogg_test3.ogg");

    Sys::Get()->MainLoop();

    Sys::Quit();
    return 0;



}


#else

// Copyright 2023 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace ftxui;

Component DummyWindowContent() {
  class Impl : public ComponentBase {
   private:
    bool checked[3] = {false, false, false};
    float slider = 50;

   public:
    Impl() {
      Add(Container::Vertical({
          Checkbox("Check me", &checked[0]),
          Checkbox("Check me", &checked[1]),
          Checkbox("Check me", &checked[2]),
          Slider("Slider", &slider, 0.f, 100.f),
      }));
    }
  };
  return Make<Impl>();
}

int main() {
  int window_1_left = 0;
  int window_1_top = 0;
  int window_1_width = 40;
  int window_1_height = 20;

  auto window_1 = Window({
      .inner = DummyWindowContent(),
      .title = "First window",
      .left = &window_1_left,
      .top = &window_1_top,
      .width = &window_1_width,
      .height = &window_1_height,
  });

  auto window_2 = Window({
      .inner = DummyWindowContent(),
      .title = "My window",
      .left = 40,
      .top = 20,
  });

  auto window_3 = Window({
      .inner = DummyWindowContent(),
      .title = "My window",
      .left = 60,
      .top = 30,
  });

  auto window_4 = Window({
      .inner = DummyWindowContent(),
  });

  auto window_5 = Window({});

  auto window_container = Container::Stacked({
      window_1,
      window_2,
      window_3,
      window_4,
      window_5,
  });

  auto display_win_1 = Renderer([&] {
    return vbox({

    text("window_1: " +  //
                std::to_string(window_1_width) + "x" +
                std::to_string(window_1_height) + " + " +
                std::to_string(window_1_left) + "," +
                std::to_string(window_1_top)),
    text("s"),
    text("s"),
    text("s"),
    text("s"),
    }) ;
    
  });

  auto layout = Container::Vertical({
      display_win_1,
      window_container,
  });

  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(layout);

  return EXIT_SUCCESS;
}




#endif