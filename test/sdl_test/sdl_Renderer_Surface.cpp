#include <iostream>
#include "debug.hpp"
#include "SDL.h"
#include "SDL_image.h"


int main(int, char**){

    /* init */
    {
        
        if( SDL_Init(SDL_INIT_EVERYTHING)!=0) // 与sdl_quit 对应
        {
            SDL_Log("sdl init failed");
            return 1;
        };
        if(  IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP)==0 )
        {
            SDL_Log("IMG init faild!");
            return 1;
        };
    }

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

    // SDL Surface
    // Surface : CPU 数据缓存区
    // Texture : GPU 数据缓冲区


    SDL_Surface* surface =nullptr;
    // surface 由于为CPU 缓存,可以直接操纵内存数据
    const int in_depth{32};
    surface =  SDL_CreateRGBSurfaceWithFormat(0,300,200,in_depth,SDL_PIXELFORMAT_RGBA8888); // flag 遗留
    // 清屏: MapColor() 分量到像素格式。
    SDL_FillRect(surface,NULL,SDL_MapRGBA(surface->format,255,255,255,255));
    uint32_t* pptr = (uint32_t*)surface->pixels;

    // SDL 转换surface 格式 (safe)
    auto newSurface = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_ABGR8888,0);

    // surface pitch 反应一行像素占用字节数
    //      pitch 不完全等于 in_depth*surface->w。 
    //      对于depth , SDL 会优化 in_depth 到最接近的 2的幂次方
    //      pitch = trueDepth*w  
    //      因此直接按in_depth 赋值pixel 有可能爆炸
    //      请使用 SDL_Map[format](); 
    SDL_Log(std::to_string(surface->pitch).c_str());
    
    // surface 按行存储 逐像素绘制
    // 对于像素点操作,CPU surface 是比renderDrawPoint快很多的
    for(int i = 0; i<surface->w*surface->h/2;i++)
    {
        //*pptr = 0x66ccffff;
        *pptr = 0x66ccffff;
        ++pptr;
    }


    
    // Surface -> texture
    // SDL_Texture* texture_fromSurface = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_Texture* texture_fromSurface = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,300,200);
    {
        const SDL_Rect rect{0,0,300,200};
        SDL_UpdateTexture(texture_fromSurface,&rect,surface->pixels,surface->pitch);
    }

    // SDL loadBMP 设置透明色
    SDL_FreeSurface(surface);
    //surface = SDL_LoadBMP("resources/noy.bmp");
    // 使用超级方便的 IMG load!!
    surface  = IMG_Load("resources/noy.bmp");
    
    SDL_SetColorKey(surface,SDL_TRUE,SDL_MapRGB(surface->format,255,255,255));
    SDL_Texture* texture_img = SDL_CreateTextureFromSurface(renderer,surface);


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

        const SDL_Rect sur_disrect{100,100,300,200}; 
        // renderer 的目标切换回 缓冲区.
        SDL_SetRenderTarget(renderer,nullptr);
        //SDL_RenderCopy(renderer,texture_fromSurface,NULL,&sur_disrect);
        SDL_RenderCopy(renderer,texture_img,NULL,&sur_disrect);
        SDL_RenderPresent(renderer); // present renderer's buffer.



    }

    SDL_DestroyTexture(texture_fromSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    

    IMG_Quit();
    SDL_Quit();
    return 0;
}

// SDL_Imgae 方便的直接loadtexture,loadImage, ImageInit(),Img_quit() etc. 支持各种格式
// SDL_image 还支持loadimage