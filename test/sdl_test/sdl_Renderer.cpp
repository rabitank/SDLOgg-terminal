#include <iostream>
#include "SDL.h"



int main(int argc, char *argv[]){
    if( SDL_Init(SDL_INIT_EVERYTHING)!=0) // 与sdl_quit 对应
    {
        SDL_Log("sdl init failed");
        return 1;
    };
    const int window_width = 150;
    const int window_heigth = 300;
    SDL_Window* window = SDL_CreateWindow("renderer!",
                            SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                            window_width,window_heigth,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP);


    // SDL 窗口坐标系为刨去了标题栏的左上.
    SDL_Renderer* renderer = SDL_CreateRenderer(window , -1,SDL_RENDERER_SOFTWARE); 
    // index: -1 ; flag:0 -> sdl auto set renderer(always best choose);
    SDL_SetRenderDrawColor(renderer , 255,100,0,255); // rgba
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);// blend: a: (1-a)src+a*new;
    // RGBA: red green blue alpha
    // CMYK: red yellow blue
    // HSV: Hue s value (max value with full S -> oringal hue color)
    // HSL: Hue s light (max light with everyS -> white)

    bool quit = false;
    SDL_Event event;

    SDL_Rect rect{0,0,100,100};

    while (!quit)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit=true;
            }
        }
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer); // renderer颜色清屏 (full recover renderer's buffer)


        SDL_SetRenderDrawColor(renderer , 255,255,255,255);
        SDL_RenderDrawRect(renderer,&rect);
        SDL_SetRenderDrawColor(renderer,255,122,122,200);
        SDL_RenderFillRect(renderer,&rect);
        SDL_RenderDrawLine(renderer,0,0,window_width,window_heigth);

        // 对于float等浮点格式. SDL的结构体和绘制函数也做了特化的类型

        const SDL_FRect frect{10.f,50.0f,100.f,200.f};
        
        SDL_RenderDrawRectF(renderer,&frect);
        // SDL_FPoint fpoint = SDL_FPoint();

        // RenderPresent '只会'将显示缓冲帧输出到显示器(双帧交换)
        SDL_RenderPresent(renderer); // present renderer's buffer.


        // auto surface = SDL_GetWindowSurface(window);
        // surface(in CPU) 可以直接绘图. 
        // render 没有renderTarget 时.
        // render使用GPU工作时 CPU -data-> GPU -image present->CPU displayBuffer -image present-> displayer显示器
        // render使用CPU工作时 CPU -data-draw-> CPU displayBuffer -image present-> displayer显示器
        // image present -> SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    

    SDL_Quit();
    return 0;
}
