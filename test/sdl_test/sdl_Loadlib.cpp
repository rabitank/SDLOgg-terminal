#include"SDL.h"
#include "SDL_Image.h"
#include "SDL_ttf.h"


typedef  int(*FuncType)(int,int);


int main(int,char** )
{
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0 ){
        SDL_Log("sdl init failed");
    } ;
    SDL_Event event;
    bool quit =false;
    SDL_Window* window = SDL_CreateWindow("sdl",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1000,500,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,0);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);


    // SDL LoadDll -> 热更新操作: 1. 运行时代码链接动态库.

    void* lib = SDL_LoadObject("libvmath.dll");
    if(!lib)
    {
        SDL_Log("%s",SDL_GetError());
    }
    FuncType func = (FuncType)SDL_LoadFunction(lib,"mysum");
    SDL_Log("my sum 1+1 = %d",func(1,1));

    // SDL -> 热更新操作: 2. 链接嵌入式代码,不需要编译 比如lua

    SDL_UnloadObject(lib);


    while(!quit)
    {
        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit =true;
            }

        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);


    }


    SDL_DestroyRenderer(renderer);

    SDL_Quit();
    return 0;
}