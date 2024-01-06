#include"SDL.h"
#include "SDL_Image.h"
#include "SDL_mixer.h"
#include "debug.hpp"
// SDL audio
// sdl原生api 非常底层,支持录音.适合造轮子满足特殊需求
// sdl_mixer 可直接用

int main(int,char** )
{
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0 ){
        SDL_Log("sdl init failed");
    } ;
    Mix_Init(MIX_INIT_OGG |MIX_INIT_MP3 | MIX_INIT_MOD );

    SDL_Log("%s",getOperationFilePath().c_str());

    // load 并 播放前需要打开声音设备
    // chuck size 建议为 2048 / 4096 等符合音频帧整数倍的尺寸
    // CHANNELS 并不是声道,而是音轨. 音轨可接受一个缓存,最后混合输出.
    // CHANNEL 总数与硬件相关.是有限的
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,4096);

    // 这里的chunk类似缓存?
    // LoadWav: 一次性?解码为Wav导入
    // Wav为原始采样数据.
    // Chunk + 声道播放适合短音频
    Mix_Chunk* sound1 = Mix_LoadWAV("resources/mantianxing.mp3");
    Mix_Chunk* sound2 = Mix_LoadWAV("resources/Avid.mp3");
    
    // Mix Playmusic: 适合播放长音频.有专用音轨
    // LoadMUS(music) 导入长音频,将会边播放音轨边解码音频. 也许每次解码上面设置的 4096 长的音频?
    Mix_Music* music = Mix_LoadMUS("resources/doublechannel.mp3");
    if(!music){SDL_Log("music load faild !");}
    // 渐入(音乐)
    // Mix_FadeInMusic(music,0,10000);


    // Playmusic 不能同时播放,只能播放最后一次playmusic 指定的music
    // Mix_PlayMusic(music,0);
    // SDL_Delay(2000);
    // Mix_PauseMusic();
    // SDL_Delay(1000);
    // Mix_ResumeMusic();
    // SDL_Delay(1000);
    // Mix_RewindMusic(); // 回到起点
    // Mix_VolumeMusic(100); // 音量参数 [0 ~ 255] 

    // Mix_Pause(1); 暂停一个channel
    // Mix_Paused(1); 查询这个channel是不是暂停状态

    

    SDL_Event event;
    bool quit =false;
    SDL_Window* window = SDL_CreateWindow("sdl",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1000,500,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,0);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

    Sint16 angle{0}; // 逆时针从脑后开始
    int channel{0};
    uint8_t distance{0};

    while(!quit)
    {
        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit =true;
            }
            if(event.type== SDL_KEYDOWN)
            {
                if(event.key.keysym.scancode == SDL_SCANCODE_1)
                {
                    // -1 channel: auto find a relax channel
                    // return channel: the channel it get. -1 mean not find usable channel
                    channel = Mix_PlayChannel(-1,sound1,0);
                };
                if(event.key.keysym.scancode == SDL_SCANCODE_2)
                {
                    Mix_PlayChannel(-1,sound2,0);
                };
                if(event.key.keysym.scancode == SDL_SCANCODE_D)
                {
                    // Mix SetPosition 立体环绕声
                    // 极坐标
                    angle-=1;
                };
                if(event.key.keysym.scancode == SDL_SCANCODE_A)
                {
                    angle+=1;
                };
                if(event.key.keysym.scancode == SDL_SCANCODE_W)
                {
                    distance+=1;
                };
                if(event.key.keysym.scancode == SDL_SCANCODE_S)
                {
                    distance-=1;
                };

            }
        }

        SDL_Log("angle %dd , distance %d",angle,distance);

        Mix_SetPosition(channel,angle,distance);

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);


    }
    Mix_FreeChunk(sound1);
    Mix_FreeChunk(sound2);
    Mix_FreeMusic(music);
    Mix_Quit();
    SDL_DestroyRenderer(renderer);

    SDL_Quit();
    return 0;
}