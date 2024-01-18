#include "music.hpp"
#include "utils/file.h"
#include "sys.hpp"

namespace SO
{
    
bool AudioBase::ChannelPlay(int in_channel)
{   
    if(m_state == notInChannel)
    {
        m_channel = in_channel; 
        Mix_PlayChannel(m_channel ,m_chunk,0);

    }
    if(m_state == playing)
    {
        return m_channel == in_channel ? true :false;
    }
    if(m_state == pause)
    {
        if(m_channel == in_channel) {
            Mix_Resume(m_channel) ;return true;
        }
        if(m_channel != in_channel) {
            m_channel = in_channel;
            Mix_PlayChannel(m_channel,m_chunk,0);
        } 
    }
    return false;
}




int AudioBase::Play()
{


    if(qresume())
        return m_channel;
    if(qchannelP())
    {
        Mix_RegisterEffect(SO::Sys::Get()->m_channel,SO::Sys::proccessInfo_effect,nullptr,nullptr); 
        return m_channel;
    }
    if(m_state == playing) return m_channel;

    SDL_Log("Warning!Sound at least no ready! -MusicBase::Play()");
    return -1;
}

int AudioBase::Stop()
{
    if(qpause()) return m_channel;
    if(m_state == pause) return m_channel;
    
    return -1;
}
std::string& AudioBase::Label()
{
    //@TODO: "ogg" -> m_name
    m_label = std::string("ogg")+" "+std::to_string(m_channel)+ " " +State2string(m_state);
    return m_label;
};

State  AudioBase::Toggle()
{
    ///temp
    SDL_Log(State2string(m_state).c_str());

    switch (m_state)
    {
    case pause:
    case notInChannel:
        Play();
        return m_state;
    case playing:
        Stop();
        return m_state;
    
    case noDefine:
    case notReady:
        SDL_Log("Warning! invalid state to toggle -MusicBase::Toggle");
        return m_state;
        break;
    };
    SDL_Log("Error: undefined state! - MusicBase::Toggle");
    return m_state;
}

void AudioBase::init()
{
    m_state = noDefine;

    if(IsFileExist(m_path)){
        m_state = notReady;   
        m_chunk  =  Mix_LoadWAV(m_path.c_str());
        if(m_chunk != NULL)
        {
            m_state = notInChannel;
            m_length = calculateLength(m_chunk);
        }
    }
    Label();
}

bool AudioBase::qresume()
{
    if(m_state==pause)
    {
        Mix_Resume(m_channel);
        m_state = playing;
        return true;
    }
    return false;
}

bool AudioBase::qpause()
{
    if(m_state==playing)
    {
        Mix_Pause(m_channel);
        m_state = pause;
        return true;
    }
    return false;
}

bool AudioBase::qexitChannel(){
    if(Mix_GetChunk(m_channel) == m_chunk)
    {
        Mix_Pause(m_channel);
        m_state = notInChannel;

        /* clear render state and proccess */
        m_pos = 0;
        m_proccess =0.f;
        
        return true;
    }
    return false;
}

float AudioBase::calculateLength(const Mix_Chunk* in_chunk, uint32_t in_frequence, uint8_t in_tracknum)
{
    if(in_chunk)
    {
        return ((float)in_chunk->alen/(int)in_frequence/(int)in_tracknum);
    }
    return 0.0f;
}

bool AudioBase::qchannelP()
{
    if(m_state == notInChannel)
    {
        if(m_channel ==-1)
        {
            
            m_channel = Mix_PlayChannel(m_channel,m_chunk,0);
            
            if(m_channel==-1){
                SDL_Log("Warning! No enough channel! - MusicBase::qchannelP()");
                return false;
            }
        }
        else 
        {
            Mix_PlayChannel(m_channel,m_chunk,0);
        }
        m_state = playing;
        return true;
    }
    
    return false;
}



#define TSTRING(x) case(x): \
                    return #x;
    

std::string State2string(State in_state)
{
    using namespace SO;
    switch (in_state)
    {
        TSTRING(noDefine)
        TSTRING(notReady)
        TSTRING(playing)
        TSTRING(pause)
        TSTRING(notInChannel)
    } 
    SDL_Log("Warning!Error state! - State2string");
    return std::string("error state");
}

    
} // namespace SO
