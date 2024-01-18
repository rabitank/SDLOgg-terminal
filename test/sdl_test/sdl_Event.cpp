#include"SDL.h"
#include "SDL_Image.h"

int main(int,char** )
{
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0 ){
        SDL_Log("sdl init failed");
    } ;

    SDL_Event event;
    bool quit =false;
    SDL_Window* window = SDL_CreateWindow("asd",100,100,100,100,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    // 向消息队列添加消息
    enum{ MY_EVENT_1 = 1<<5 };
    SDL_Event newevent;
    newevent.type = MY_EVENT_1;
    const char* eventname = "custom event";
    newevent.user.data1 = (void*)eventname; 
    newevent.user.data2 = (void*)1; 
    SDL_PushEvent(&newevent);

    
    while(!quit)
    {
        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit =true;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                event.button.button == SDL_BUTTON_LEFT;
            }
            if(event.type == SDL_KEYDOWN)
            {
                if((event.key.keysym.scancode == SDL_SCANCODE_S) &&
                    (event.key.keysym.mod & KMOD_CTRL)) // mod is by bit
                    {
                        SDL_Log("ctrl + s save!! " );
                    };
            }
            if(event.type == MY_EVENT_1)
            {
                SDL_Log("custom event 1 name: %s" , event.user.data1);
            }
            if(event.type == SDL_WINDOWEVENT)
            {
                if( event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    // data1,data2 contain's info is related with window event type;
                    SDL_Log("window;s resized! new size: %d , %d ", event.window.data1 , event.window.data2); // width , height

                }                    

            }


        }

        // Query ; 比较慢缓慢, 并且有可能错过帧间事件

        int mx = 0,
            my =0;
        auto button =  SDL_GetMouseState(&mx,&my);
        if((button & SDL_BUTTON_LMASK) && (button & SDL_BUTTON_RMASK))
        {
            SDL_Log("mouse left and right button pressed!");
        }

        // 硬件 scancode 查询
        // view: https://wiki.libsdl.org/SDL2/SDL_Keycode 
        int keynum{0};
        auto keyborad = SDL_GetKeyboardState(&keynum); // 硬件 只有硬件码部分会被map state
        if(keyborad[SDL_SCANCODE_R])  // 硬件键码 Scancode
        {
            SDL_Log("r key pressed!" );
            SDL_Log("all key nums %d", keynum);
        }
        // failed!
        // 虚拟键码(KeyCode) : "<" SDLK_LESS 没有对应的scancode
        // 请使用event
        if(keyborad[ SDLK_LESS]   ) 
        {
            SDL_Log("< key(virtual) pressed!" );
        }

        if(keyborad[ SDLK_COMMA] ) // ',' SDLK 有对应 scancode 
        {
            SDL_Log(", key(virtual) pressed!" );
        }

        

    }

    SDL_Quit();
    return 0;
}