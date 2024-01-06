#include <iostream>
#include "SDL.h"

// main 被 SDL_main 替换. 因此用户的main不会被编译器优化,需要自行严格符合SDL_main的签名.
// SDL_main(int , char**) 
// 对main 使用 f12吧!

// 对于exe设置,SDL_main预设了控制台应用的输入,因此对于cmake ,cmake tools , vscode 中的控制台设置选项需要小心.



int main(int , char **){
    std::cout << "Hello, from vcsdl!\n";
    if( SDL_Init(SDL_INIT_EVERYTHING)!=0) // 与sdl_quit 对应
    {
        SDL_Log("sdl init failed");
        return 1;
    };
    // view:https://wiki.libsdl.org/SDL2/CategoryInit
    // SDL_VideoInit("");// 初始化模块 
    // if( SDL_WasInit(SDL_INIT_AUDIO | SDL_INIT_EVENTS) & SDL_INIT_AUDIO) {} //check audio if init;
    // Uint32 是sdl为c写的,不要用

    int x = 0;
    const int window_width = 150;
    const int window_heigth = 300;

    // SDL 屏幕坐标系左上
    // window flag view: https://wiki.libsdl.org/SDL2/SDL_WindowFlags
    // windowPos view: https://wiki.libsdl.org/SDL2/SDL_CreateWindow
    SDL_Window* window = SDL_CreateWindow("hellow sdl!",x,SDL_WINDOWPOS_CENTERED,window_width,window_heigth,SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);
    
    // 不稳定
    // SDL_CreateShapedWindow("自定义形状的窗口",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,100,100,SDL_WINDOW_SHOWN);

    // SDL_SetWindowSize(window ,1,1);
    // SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN_DESKTOP);


    SDL_Event event;
    bool quit = false;



    // SDL bool 请使用SDL_TRUE/FALSE;
    SDL_SetWindowBordered(window,SDL_TRUE);
    // 焦点于该窗口时:
    // SDL 限制鼠标在窗口内. -> 关闭窗口快捷键 ; Alt+f4
    SDL_SetWindowMouseGrab(window,SDL_TRUE);
    SDL_Rect mouse_rect{0,0,10,10};
    // SDL 限制鼠标在窗口的一个矩形内
    SDL_SetWindowMouseRect(window,&mouse_rect);
    // SDL 设置window最小化选项,窗口最小的限制.
    SDL_SetWindowMinimumSize(window,20,20);

    SDL_Rect display_rect;
    SDL_GetDisplayBounds(0,&display_rect); // 获取显示器的框 0-> 默认显示器

    // SDL 窗口闪烁
    SDL_FlashWindow(window,SDL_FLASH_BRIEFLY);

    int x_direction = 1;
    while (!quit)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit=true;
            }
        }

        
        x+=x_direction*10;
        SDL_Delay(10);
        SDL_SetWindowPosition(window,x,SDL_WINDOWPOS_CENTERED);

        if(x+window_width>=display_rect.w){
            x_direction=-1;
        }
        else if(x<0)
        {
            x_direction = 1;
        }

    }

    SDL_DestroyWindow(window);
    

    SDL_Quit();
    return 0;
}
