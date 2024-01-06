#include"SDL.h"
#include "SDL_Image.h"
#include "SDL_ttf.h"


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

    TTF_Font* font = TTF_OpenFont("resources/DENG.TTF",20);
    // Render text Solid: 最低质量渲染
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font,"solid 你好 nihao 123",{0,0,0,255}); // 选用何种编码 需要与你的源文件编码格式匹配
    SDL_Texture* solid_nihoa = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_Rect solid_rect = {0,0,surface->w,surface->h};
    SDL_FreeSurface(surface);

    // Render text Shaded: 中等质量渲染
    // frontColor:前景色,文字颜色. backgroundColor: 文字背景色
    surface = TTF_RenderUTF8_Shaded(font,"shaded 你好 nihao 123",{0,0,0,255},{255,255,255,255}); 
    SDL_Texture* shaded_nihoa = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_Rect shaded_rect = {0,0,surface->w,surface->h};
    SDL_FreeSurface(surface);
    
    // Render text LCD
    surface = TTF_RenderUTF8_LCD(font,"lcd 你好 nihao 123",{0,0,0,255},{255,255,255,255}); 
    SDL_Texture* lcd_nihoa = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_Rect lcd_rect = {0,0,surface->w,surface->h};
    SDL_FreeSurface(surface);
    
    // Render text Blended: 最高质量
    surface = TTF_RenderUTF8_Blended(font,"blended 你好 nihao 123",{0,0,0,255}); 
    SDL_Texture* blended_nihoa = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_Rect blended_rect = {0,0,surface->w,surface->h};
    SDL_FreeSurface(surface);

    // Wrap 版本. 超过长度自动换行
    surface = TTF_RenderUTF8_Blended_Wrapped(font,"blended 这是一句很长很长很长很长很长很长很长很长很长很长很长很长很长很长的中文",{0,0,0,255},100); 
    SDL_Texture* blended_long = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_Rect long_rect = {0,0,surface->w,surface->h};
    SDL_FreeSurface(surface);

    // 请求知晓文本长度
    {
        int x=0,y=x;
        TTF_SizeText(font, "asdasdsd",&x,&y);
    }
    

    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

    // SDL_ttf . lib  添加文字支持

    while(!quit)
    {
        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit =true;
            }
        }
        SDL_SetRenderDrawColor(renderer,0,255,0,255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer,solid_nihoa,nullptr,&solid_rect);

        lcd_rect.y = solid_rect.h;
        SDL_RenderCopy(renderer,lcd_nihoa,nullptr,&lcd_rect);

        shaded_rect.y = lcd_rect.h+lcd_rect.y;
        SDL_RenderCopy(renderer,shaded_nihoa,nullptr,&shaded_rect);

        blended_rect.y = shaded_rect.h+shaded_rect.y;
        SDL_RenderCopy(renderer,blended_nihoa,nullptr,&blended_rect);

        long_rect.y = blended_rect.y+blended_rect.h;
        SDL_RenderCopy(renderer,blended_long,nullptr,&long_rect);

        SDL_RenderPresent(renderer);


    }

    SDL_DestroyRenderer(renderer);
    TTF_Quit();
    SDL_Quit();
    return 0;
}