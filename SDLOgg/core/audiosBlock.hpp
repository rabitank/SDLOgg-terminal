#pragma once

#include "block.hpp"
#include "music.hpp"


void ChannelFinishedCallback(int in_channel);
namespace SO{

    using AudioList = std::vector<Audio>;
    using AudioPos = AudioList::iterator;


    /// @brief AudiosBlock 应该成为一个纯管理类. 用于拆分Sys中绘制AudioListBlock的责任
    class AudiosBlock:public Block
    {
    public:
        friend class AudioBase;
        friend void ::ChannelFinishedCallback(int in_channel); 
    public:
        AudiosBlock(const AudioList& in_list , AudioPos& in_curpos ,int& in_channel ):
        m_list(in_list),m_curPos(in_curpos),m_channel(in_channel)
        {}


        const ftxui::Component freshItemList();
        virtual ftxui::Component RenderComponent() override;

        /// @brief 构建出audio的item, 添加到块的组件列表中，不会修改。
        /// @param in_audio 
        /// @return 
        ftxui::Component AddAudioTItem( Audio in_audio);

    private:
        /// @brief temp. get renderer componet of an audio 
        /// @param in_audio 
        /// @return component.
        ftxui::Component AudioItemRender(Audio in_audio);
        static void audioitem_clickCallBack(const Audio& in_audio );

        const AudioList& m_list;
        AudioPos& m_curPos;
        int& m_channel;

        ftxui::Component m_itemList{ftxui::Container::Vertical({})};

        friend class Sys;

    };
}

