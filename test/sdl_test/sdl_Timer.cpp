#include"SDL.h"
#include "SDL_Image.h"
#include "SDL_ttf.h"
#include "string.h"

uint32_t turlydoneSomething(const char* password )
{
    SDL_Log("truly done! %s ",password);
    return 0;
}

uint32_t pushEventCall(uint32_t interval,void* params )
{
    SDL_Event custome_event;
    custome_event.type =SDL_USEREVENT;
    custome_event.user.data1 =(void*)turlydoneSomething;
    custome_event.user.data2 = params;
    SDL_PushEvent(&custome_event);
    return interval;
} 


int main(int,char** )
{
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0 ){
        SDL_Log("sdl init failed");
    } ;
    TTF_Init();

    SDL_Event event;
    bool quit =false;
    SDL_Window* window = SDL_CreateWindow("sdl",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1000,500,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,0);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);


    // SDL_timer: view https://github.com/libsdl-org/SDL/blob/SDL2/include/SDL_timer.h

    auto oldtick = SDL_GetTicks64();
    uint32_t deltams{0};
    void* paramsP = nullptr;

    // SDL_Addtimer: view https://wiki.libsdl.org/SDL2/SDL_AddTimer
    
    // pass an uint32 call(uint32 interval,void* params) function
    // lambda or function name
    // SDL 实际上调用了系统的计时器. 实现与系统有关. 许多系统通过多线程进行计时.
    SDL_TimerID timerid = SDL_AddTimer(1000,[](uint32_t interval,void* params)->uint32_t {
        // if return 0. 结束该计时器。
        // 必须要传递.作为下一次call的间隔。
        SDL_Log("time Arived at %d",SDL_GetTicks64());
        return interval; 
    },paramsP);


    // 可以利用event队列的单线程,通过event通信,让主线程去做,实现单线程.计时器只是定时pushevent.
    char* astring = (char*)malloc(256);
    strcpy(astring,"一个堆上字符串");
    SDL_TimerID eventCallTimerid = SDL_AddTimer(1000,pushEventCall,astring);
    while(!quit)
    {
        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit =true;
            }
            if(event.type == SDL_USEREVENT)
            {
                SDL_Log("recived event");
                ((decltype(&turlydoneSomething))event.user.data1)((char*)event.user.data2);
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);



        // SDL 帧率控制
        if(SDL_TICKS_PASSED(SDL_GetTicks64(),oldtick+1000/60))
        {
            SDL_Delay(1000/60 - deltams);
            deltams = 1000/60;
        };
        oldtick = SDL_GetTicks64();

    }
    SDL_RemoveTimer(timerid);

    SDL_DestroyRenderer(renderer);
    TTF_Quit();
    SDL_Quit();
    return 0;
}