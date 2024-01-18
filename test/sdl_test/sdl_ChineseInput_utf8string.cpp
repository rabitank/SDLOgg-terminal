#include"SDL.h"
#include"SDL_ttf.h"
#include<string>
#include"utf8string.h"


SDL_Surface* surface = nullptr;
// SDL 中文输入问题.
// 推荐utf8统一格式. event.edit.text 就是utf8
// 创建utf-8字符.

inline SDL_Point RenderFont(SDL_Renderer* renderer,const std::string& text,TTF_Font* font,const int x,const int y,const SDL_Color& fc)
{
    if(text.empty()) return {-1,-1};
    surface = TTF_RenderUTF8_Shaded(font,text.c_str(),fc,{255,255,255,255});
    if(!surface) 
    {
        SDL_Log(TTF_GetError());
        return {-1,-1};
    }
    const SDL_Rect rect{x,y,surface->w,surface->h};
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer,texture,nullptr,&rect);
    SDL_DestroyTexture(texture);

    return {rect.w+rect.x,rect.y+rect.h};
}
SDL_Point RenderFontEditing(SDL_Renderer* renderer,const std::string& text,TTF_Font* font,const int x,const int y)
{
    const auto p  = RenderFont(renderer,text,font,x,y,{255,0,0,255});
    return p;
}
SDL_Point RenderFontContent(SDL_Renderer* renderer,const std::string& text,TTF_Font* font,const int x,const int y)
{
    const auto p  = RenderFont(renderer,text,font,x,y,{0,0,0,255});
    return p;
}

int main(int,char** )
{
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0 ){
        SDL_Log("sdl init failed");
    } ;
    TTF_Init();

    auto font = TTF_OpenFont("resources/DENG.TTF",20);

    SDL_Event event;
    bool quit =false;

    // 开启原生IME组件显示. 要在窗口前设置.
    SDL_SetHint(SDL_HINT_IME_SHOW_UI,"1");

    SDL_Window* window = SDL_CreateWindow("sdl",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1000,500,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,0);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

    std::string edits ; 
    utf8string content ; 
    utf8string selection;

    const SDL_Rect rect{0,0,100,50};


    // 开启输入. 这一版sdl 默认开启
    SDL_StartTextInput(); 
    // 设置输入法条位置 : view: https://wiki.libsdl.org/SDL2/SDL_SetTextInputRect
    // 默认为右下角
    SDL_SetTextInputRect(&rect);
    
    while(!quit)
    {
        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit =true;
            }
            if(event.type == SDL_TEXTINPUT)
            {
                content += event.edit.text;
            }
            if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
            {
                content.pop_back();
            }
            if(event.type ==SDL_TEXTEDITING) // 功能键输入不会被触发
            {
                edits = event.edit.text;
                SDL_Log("editing: editstar(光标位): %d , editLen: %d , edittext: %s",event.edit.start,event.edit.length,event.edit.text);
                selection = SDL_GetPrimarySelectionText();
                if(selection.empty())
                { SDL_Log(SDL_GetError());}
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        const SDL_Point pos = RenderFontContent(renderer,content.to_string(),font,10,10);
        RenderFontEditing(renderer,edits,font ,pos.x+10,10);
        RenderFontEditing(renderer,selection.to_string(),font ,pos.x+10,pos.y+10);
        
        SDL_RenderPresent(renderer);


    }
    SDL_StopTextInput();
    SDL_DestroyRenderer(renderer);

    TTF_Quit();
    SDL_Quit();
    return 0;
}