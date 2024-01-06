 #define SDL_MAIN_HANDLED
 
#include <iostream>
#include "SDL.h"

  
bool quit = false;
uint32_t delta_ms{0};
SDL_Rect player_rect;

#define WINDOW_WIDTH  640
#define  WINDOW_HEIGHT  360
int main(int, char **);
#define TRIANGLE_WIDTH  20
#define  TRIANGLE_HEIGHT  15
#define INTERVAL 1000/30

int  window_width    = WINDOW_WIDTH;
int  window_heigth  =  WINDOW_HEIGHT;

bool is_Gameover = false;



void EventHandle(SDL_Event&);
void ClearRenderer(SDL_Renderer*);
void Present(SDL_Renderer*);

void LogicUpdate(SDL_Window* in_window, int delta_ms);
void Render(SDL_Renderer*);
void GetDeltaTick(uint32_t&in_delta);

int speed = 1;


int main(int argc, char *argv[]){
    if( SDL_Init(SDL_INIT_EVERYTHING)!=0) // 与sdl_quit 对应
    {
        SDL_Log("sdl init failed");
        return 1;
    };

    // 需要设置窗口resizeable
    SDL_Window* window = SDL_CreateWindow("renderer!",
                            SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                            window_width,window_heigth,
                            SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    // 设置最大化大小限制,防止最大化后无法改变窗口尺寸
    SDL_SetWindowMaximumSize(window,1000,1000); 
    // 可以直接ban掉最大化
    

    player_rect.w = 40;
    player_rect.h = 60;
    player_rect.x = (window_width-player_rect.w)/2;
    player_rect.y = (window_heigth-player_rect.h)/2;

    SDL_SetWindowMinimumSize(window,player_rect.w+TRIANGLE_WIDTH*2,player_rect.h+TRIANGLE_HEIGHT*2);


    SDL_Renderer* renderer = SDL_CreateRenderer(window , -1,SDL_RENDERER_SOFTWARE); 
 
    SDL_Event event;


    while (!quit)
    {

        EventHandle(event);
        LogicUpdate(window,delta_ms);
        Render(renderer);

        GetDeltaTick(delta_ms);
        if(delta_ms<INTERVAL){
            SDL_Delay(INTERVAL-delta_ms);
            delta_ms = INTERVAL;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

void ClearRenderer(SDL_Renderer* in_renderer)
{
    SDL_SetRenderDrawColor(in_renderer , 0,0,0,255); 

    // custom draw


    SDL_RenderClear(in_renderer);

}
void Present(SDL_Renderer* in_renderer)
{
    SDL_RenderPresent(in_renderer);

}

void LogicUpdate(SDL_Window* in_window, int delta_ms)
{
    //your logic

    if(is_Gameover){
        return;
    }
    static uint32_t timer{0};
    if(++timer>=600)
        speed=2;
        else if(timer>=2000)
            speed = 3;


    SDL_GetWindowSize(in_window,&window_width,&window_heigth);

    window_width -= speed;
    window_heigth -= speed;

    SDL_SetWindowSize(in_window,window_width,window_heigth);
    if(player_rect.x+player_rect.w>=window_width-TRIANGLE_WIDTH ||
    player_rect.y+player_rect.h>=window_heigth-TRIANGLE_WIDTH )
    {
        is_Gameover = true;
        SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION,"!","Q",in_window);
        SDL_SetWindowResizable(in_window, SDL_FALSE);
    }

}
void GetDeltaTick(uint32_t & in_delta)
{
    static uint32_t oldtick = SDL_GetTicks();
    const uint32_t curtick = SDL_GetTicks();
    in_delta = curtick -oldtick;
    oldtick = curtick;
}

void Render(SDL_Renderer * in_renderer)
{
    ClearRenderer(in_renderer);
    if(!is_Gameover)
    {
        SDL_SetRenderDrawColor(in_renderer,6*17,12*17,15*17,255);
        SDL_RenderDrawRect(in_renderer,&player_rect);
    }

    const int triangle_num = (int)(window_width/(float)TRIANGLE_WIDTH+0.5);
    SDL_SetRenderDrawColor(in_renderer,255,2*17,1*17,255);
    for(int i=0;i<triangle_num;i++)
    {
        SDL_RenderDrawLine(in_renderer,i*TRIANGLE_WIDTH,0,i*TRIANGLE_WIDTH+TRIANGLE_WIDTH/2,TRIANGLE_HEIGHT);
        SDL_RenderDrawLine(in_renderer,i*TRIANGLE_WIDTH+TRIANGLE_WIDTH/2,TRIANGLE_HEIGHT,i*TRIANGLE_WIDTH+TRIANGLE_WIDTH,0);
        
        SDL_RenderDrawLine(in_renderer,i*TRIANGLE_WIDTH,window_heigth,i*TRIANGLE_WIDTH+TRIANGLE_WIDTH/2,window_heigth-TRIANGLE_HEIGHT);
        SDL_RenderDrawLine(in_renderer,i*TRIANGLE_WIDTH+TRIANGLE_WIDTH/2,window_heigth-TRIANGLE_HEIGHT,i*TRIANGLE_WIDTH+TRIANGLE_WIDTH,window_heigth);
    }
    
    const int triangle_num_v = (int)(window_heigth/(float)TRIANGLE_HEIGHT+0.5);
    for(int i=0;i<triangle_num_v;i++)
    {
        SDL_RenderDrawLine(in_renderer,0,i*TRIANGLE_HEIGHT,TRIANGLE_WIDTH,i*TRIANGLE_HEIGHT+TRIANGLE_HEIGHT/2);
        SDL_RenderDrawLine(in_renderer, TRIANGLE_WIDTH, i * TRIANGLE_HEIGHT + TRIANGLE_HEIGHT / 2,  0 ,i * TRIANGLE_HEIGHT + TRIANGLE_HEIGHT);
        SDL_RenderDrawLine(in_renderer,window_width,i*TRIANGLE_HEIGHT,window_width-TRIANGLE_WIDTH,i*TRIANGLE_HEIGHT+TRIANGLE_HEIGHT/2);
        SDL_RenderDrawLine(in_renderer, window_width-TRIANGLE_WIDTH, i * TRIANGLE_HEIGHT + TRIANGLE_HEIGHT / 2, window_width ,i * TRIANGLE_HEIGHT + TRIANGLE_HEIGHT);
    }



    Present(in_renderer);
        
}

void EventHandle(SDL_Event& in_event)
{
    while(SDL_PollEvent(&in_event))
    {
        if(in_event.type == SDL_QUIT)
        {
            quit=true;
        }
    }

}

