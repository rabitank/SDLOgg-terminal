#pragma once
#include "pch.hpp"

#include "SDL.h"
#include "SDL_mixer.h"

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component.hpp" 
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/dom/elements.hpp"

#include "music.hpp"


/// @brief reCall when: 1.当channel被要求播放新chunk时(此时仍持有旧chunk) 
//* 2. chunk正常完播时 
//* 3. haltchannel 终之channel时
/// @param in_channel 接受的channel
void ChannelFinishedCallback(int in_channel);

namespace SO{

class Sys{

public:
    using AudioList = std::vector<Audio>;
    using AudioPos = AudioList::iterator;

    struct DeviceConfig{
        
        /* 采样频率 */
        uint32_t frequency = MIX_DEFAULT_FREQUENCY;
        /* 这里指的是播放设备使用几个声道 */
        uint8_t  channel = MIX_DEFAULT_CHANNELS;
    }config;


public: 

    static Sys* Get(){return m_ins;};
    static void Quit();
    void MainLoop();

    static  Audio Load(const std::string &in_path);
    static  bool LoadDialog(const std::string &in_initPath);
    static  Audio LoadSingleDialog(const std::string &in_initPath);

    static Audio Cur(){return m_ins->m_curAudio;}
    static Audio FindBChunk(Mix_Chunk* in_chunk);

    /// @brief
    /// @return 0 : subsys init failed. 1 success, can continue call Get();
    static int Init();



public:
    
    ftxui::ScreenInteractive m_screen{ftxui::ScreenInteractive::Fullscreen()};
    ftxui::Component m_itemList{ftxui::Container::Vertical({})};
    //@TODO:decide
    ftxui::Component* m_listRendererHandle{nullptr};

    const ftxui::Component freshItemList();
    ftxui::Component ItemsRenderer();
    static ftxui::Component LogOutRender();
    /// @brief temp. get renderer componet of an audio 
    /// @param in_audio 
    /// @return component.
    static ftxui::Component AudioItemRender(Audio in_audio);
    
    /// @brief 获得一个组件指针,指针指向的组件由sys管理与刷新
    const ftxui::Component* ItemsRendererHandle(){ItemsRenderer();return m_listRendererHandle;};

//@temp: 暂时的public. 方便测试
public:
    using LogVector  = std::vector<std::pair<std::string,std::string>>;
    using LogPara = LogVector::value_type;
    LogVector logVector;

    AudioList m_list;
    AudioPos m_curPos;
    Audio m_curAudio{nullptr};
    int m_channel;

    /// @brief add audio to list and render:itemlist. call by Load
    /// @param in_audio 
    void addTList(const Audio& in_audio );

private:
    Sys(){
        m_channel = 1;    
        m_list.reserve(10);
        m_curPos = m_list.begin();
        logVector.reserve(8);
    }


    static bool subsys_init();
    void logInit();
    static void audioitem_clickCallBack(const Audio& in_audio );
    static void proccessInfo_effect(int chan, void *stream, int len, void *udata);

    friend class AudioBase;
    friend void ::ChannelFinishedCallback(int in_channel);
private:
    
    static Sys* m_ins ;

};




}
