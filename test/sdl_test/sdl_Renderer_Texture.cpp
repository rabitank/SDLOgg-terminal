#include <iostream>
#include "debug.hpp"
#include "SDL.h"



int main(int , char**){
    if( SDL_Init(SDL_INIT_EVERYTHING)!=0) // 与sdl_quit 对应
    {
        SDL_Log("sdl init failed");
        return 1;
    };
    const int window_width = 1280;
    const int window_heigth = 760;
    SDL_Window* window = SDL_CreateWindow("renderer!",
                            SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                            window_width,window_heigth,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE);

    SDL_Renderer* renderer = SDL_CreateRenderer(window , -1,SDL_RENDERER_SOFTWARE); 
    // SDL 开启融混
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);// blend: a: (1-a)src+a*new;

    bool quit = false;
    SDL_Event event;

    SDL_Rect rect{0,0,100,100};


    // texture create: way 1: CreateTextureFromSurface
    SDL_Surface* surface = SDL_LoadBMP("resources/quancifang.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    int h = surface->h,
        w = surface->w;
    SDL_FreeSurface(surface);
    
    
    // texture create: way 2:
    // texture format: infoList&infoBit
    // texture access: 访问描述:
    //      STATIC: '承诺'基本不会改变纹理内容 (way 1 默认访问性)
    //      STREAMING: 流 (用后即抛,不会保存 -> 摄像头/直播)
    //      TARGET: 作为renderTarget (可被绘制)

    int h_2 = 200,
        w_2 = 300;
    SDL_Texture* texture_2 = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_TARGET|SDL_TEXTUREACCESS_STATIC,w_2,h_2);
    SDL_SetRenderTarget(renderer,texture_2);
    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,6*17,12*17,15*17,255);
    SDL_RenderDrawLine(renderer,0,0,window_width,window_heigth);

    // texture 透明度
    // 开启texture 融混
    SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
    // 设置alpha
    SDL_SetTextureAlphaMod(texture,100);
    // 设置叠加色
    SDL_SetTextureColorMod(texture,255,22,2);

    // SDL 实现拉伸
    // 利用 纹理复制时的拉伸效果实现整个窗口内容的适应.
    SDL_Texture* texture_window = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_TARGET,window_width,window_heigth);
    {
        // SDL 纹理查询
        uint32_t format{0};
        int access{0};
        int q_w{0};
        int q_h{0};
        SDL_QueryTexture(texture_2,&format,&access,&q_w,&q_h);
        // 不想查的就NULL
    }

    while (!quit)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit=true;
            }
        }

        SDL_SetRenderTarget(renderer,texture_window);

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer); // renderer颜色清屏 (full recover renderer's buffer)

        SDL_Rect rect{0,0,w_2,h_2};
        SDL_RenderCopy(renderer,texture_2,NULL,&rect);

        // RenderCopy
        // src rect ->坐标原点为图片左上角, 用于指定图片上的部分
        // dis rect ->坐标原点为窗口左上角, 用于指定绘制到窗口上的哪部分(铺展: 拉伸)
        const SDL_Rect srcrect{0,0,200,200}; 
        const SDL_Rect disrect{0,100,200,200}; 
        // src/dis rect = NULL 时意为全部绘制
        SDL_RenderCopy(renderer,texture,NULL,&disrect);

        // RenderCopyEx
        // arc: rotate ; center:rotate center ; flip: 翻转;
        // c++17 不允许 enum隐式转换为int,从而不能直接使用按位运算
        // static double arc=0;
        // arc+=0.01;
        // SDL_Point center{0,0}; // 旋转中心
        // SDL_RenderCopyEx(renderer,texture,NULL,&disrect,arc,&center,(SDL_RendererFlip)( (int)SDL_FLIP_HORIZONTAL | (int)SDL_FLIP_VERTICAL) );

        // renderer 的目标切换回 缓冲区.
        SDL_SetRenderTarget(renderer,nullptr);
        SDL_RenderCopy(renderer,texture_window,NULL,NULL);

        SDL_RenderPresent(renderer); // present renderer's buffer.



    }

    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture_2);
    SDL_DestroyTexture(texture_window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    

    SDL_Quit();
    return 0;
}
