#pragma once

#include "SDL.h"
#include "SDL_Image.h"
#include "SDL_mixer.h"
#include <string>
#include <iostream>

#include "defines.hpp"


void ChannelFinishedCallback(int in_channel);

namespace SO{

    class AudioBase;
    //view: https://blog.csdn.net/10km/article/details/105094115. enum declration
    
    enum State: uint8_t ;
    
    std::string State2string( State in_state);

    enum State: uint8_t {
        /* no exists file*/
        noDefine = 0,
        /* is not ready to enter channel. (not yet load to chuck - useless*/
        notReady = 1,
        /* is playing */
        playing=2,
        /* is paused */
        pause = 3,
        /* is not on channel */
        notInChannel = 4,

        /* not init */
        notInit = 255
    };

    class AudioBase{

    public:
        /// @brief init a Music. will check in_file is exist;
        /// @param in_file 
        AudioBase(const std::string& in_file,int in_channel = -1):
        m_path(in_file),m_channel(in_channel){ 
            init();
        };

    public:

        ~AudioBase(){
            if(m_state>notReady && m_chunk!= nullptr ){Mix_FreeChunk(m_chunk);}; 
        }
        const std::string&  GetPath() const { return m_path;}

        /// @brief return audio s length as seconds.
        /// @return 
        float       GetLength()     const{return m_length;}
        int         GetChannel()    const{return m_channel;}
        State       GetState()      const{return m_state;}
        Mix_Chunk*  GetChunk()      const{return  m_chunk;};
        

        /// @brief also will check.
        /// @param in_file 
        /// @return  old path
        std::string ReSetPath(const std::string& in_file)  { const std::string old_p = m_path; m_path = in_file; init(); return old_p; };
    
    public:

        /// @brief decide which channel in device ,and play it.
        /// @param in_channel 
        /// @return 
        bool ChannelPlay(int in_channel);

        /// @brief it will take place the channel's original sound
        /// @return its channel
        int Play();
        int Stop();

        State Toggle();
        
        /// @brief fresh label and return ref of label.
        /// @return m_label 's ref
        std::string& Label();
        

    private:
        void init();
        
        bool qresume();
        bool qpause();
        bool qchannelP();
        bool qexitChannel();

        static float calculateLength(const Mix_Chunk* in_chunk,uint32_t in_frequence = MIX_DEFAULT_FREQUENCY, uint8_t in_tracknum = MIX_DEFAULT_CHANNELS);


    public:
        std::string name{"notExits"};
        
        friend void ::ChannelFinishedCallback(int in_channel);    
        friend class Sys;
    private:
        /// @brief m_channel -1 means not load to channle to play.
        int m_channel{-1};
        
        Mix_Chunk* m_chunk{nullptr};
        std::string m_path;

        State m_state{notInit};

        /* for item render*/
        std::string m_label;
        float m_proccess{0.f};  // 百分比
        uint32_t m_pos{0};           // 采样点位置
        float m_length{-1.f};   // 时长

    };

    class Audio{
        public:
            Audio():
            m_audioBase(nullptr){};

            Audio(Ref<AudioBase> audio):m_audioBase(audio){};
            const AudioBase* Get() const {return m_audioBase.get();};

            static Audio Create(const std::string& in_file,int in_channel = -1){
                return std::make_shared<AudioBase>(in_file,in_channel);
            }

            const Ref<AudioBase> operator->()const {
                if(m_audioBase == nullptr) throw("Error!:nullptr 未初始化的Audio - Audio::operator->");                
                return m_audioBase;
            }
            Ref<AudioBase> operator->() {
                return m_audioBase;
            }

            bool operator==(const Audio& in_audio) const{
                return m_audioBase == in_audio.m_audioBase;
            }

        private:

            Ref<AudioBase> m_audioBase;
        private:
            
    };

}