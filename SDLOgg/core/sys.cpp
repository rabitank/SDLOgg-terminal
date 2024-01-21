#include "sys.hpp"
#include "defines.hpp"
#include "utils/fileTool.h"



namespace SO
{
   
Sys* Sys::m_ins = nullptr;

void Sys::Quit()
{
    SDL_Quit();
    Mix_Quit();
    IMG_Quit();
    delete m_ins;
    m_ins = nullptr;
}

Audio Sys::Load(const std::string& in_path)
{

    auto audio = Audio::Create(in_path,m_ins->m_channel);

    if(audio->m_state != noDefine)
    {
        audio->m_length = AudioBase::calculateLength(audio->m_chunk,
                                m_ins->config.frequency,m_ins->config.channel);
    }
    m_ins->addTList(audio);
    return audio;
}

bool Sys::LoadDialog(const std::string &in_initPath)
{
    const char* pattern[1] ={"*.ogg"};
    const char* repath = OpenFileDialog("(￣﹃￣)",in_initPath.c_str(),1,pattern,nullptr,1);
    TEMP_LOG(repath);
    
    if(!repath)
    {
        SDL_Log("open file failed! or you close it?");
        return false;
    }


    std::string pathes =repath;
    std::stringstream ss(pathes);
    std::string pathbuf;

    while( std::getline(ss,pathbuf,'|'))
    {
        Load(pathbuf);
    };
    return true;

}

Audio Sys::LoadSingleDialog(const std::string &in_initPath)
{
    const char* pattern[1] ={"*.ogg"};
    const char* repath = OpenFileDialog("(￣﹃￣)",in_initPath.c_str(),1,pattern,nullptr,0);
    
    if(!repath)
    {
        SDL_Log("open file failed! or you close it?");
    }
    std::string path =repath;
    return Load(path);
}


int Sys::Init()
{
    if(!m_ins)
    {
        if(subsys_init())
        {
            m_ins = new Sys();
            Mix_OpenAudio(m_ins->config.frequency,MIX_DEFAULT_FORMAT,m_ins->config.channel,4096);

#ifdef PLOG
            m_ins->m_logBlock.reset(new LogBlock(m_ins->logVector));
#endif
            m_ins->m_audiosBlock.reset(new AudiosBlock(m_ins->m_list,m_ins->m_curPos,m_ins->m_channel));
            m_ins->m_effectBlock.reset(new EffectBlock(m_ins->m_audioAngle,m_ins->m_audioDistance ,m_ins-> m_effectcanvaWidth ,m_ins->m_effectcanvaHeight));

            Mix_ChannelFinished(ChannelFinishedCallback);
            
            Mix_RegisterEffect(m_ins->m_channel,Sys::proccessInfo_effect,nullptr,nullptr);

            
        }
        else return 0;
    }
    return 1;
}

void Sys::MainLoop()
{
    using namespace ftxui;

    auto AudiosBlock = m_audiosBlock->RenderComponent();
#ifdef PLOG
    auto Logrenderer = m_logBlock->RenderComponent();
#endif
    auto Effectrenderer = m_effectBlock->RenderComponent();

    auto SysRenderer = Container::Horizontal(
        {
            AudiosBlock,
            Container::Vertical(
                {
                    #ifdef PLOG
                    Logrenderer,
                    #endif
                    Effectrenderer,
                }
            )

        }
    );


    auto renderer = Renderer(SysRenderer,[&](){
        char cont[64];
        sprintf(cont, "channel 1 current chunk: %d",(void*)Mix_GetChunk(1));
        
        return window(text("这是SDLogg-------------------想歪的自重")|bold|color(Color::CyanLight)|center , 
        hbox({
            (AudiosBlock)->Render(),
            separatorHeavy(),
            vbox({
#ifdef PLOG
            Logrenderer->Render()|xflex|vscroll_indicator,
            separatorHeavy(),
#endif
            Effectrenderer->Render()|flex
            })|xflex
        })
        );
    });

    auto screen = ScreenInteractive::Fullscreen(); 
    screen.Loop(renderer);
    
}



Audio Sys::FindBChunk(Mix_Chunk *in_chunk)
{
    AudioPos pos = std::find_if(m_ins->m_list.begin(),m_ins->m_list.end(),[&](const Audio& audio){
        return  audio->GetChunk()==in_chunk;
    });
    if(pos == m_ins->m_list.end()) return Audio(nullptr);
    return *pos;
}


void Sys::addTList(const Audio &in_audio)
{
    m_list.push_back(in_audio);
    if(m_audiosBlock) m_audiosBlock->AddAudioTItem(in_audio);
    m_curAudio = in_audio;
    m_curPos = m_list.end()-1; 
}



bool Sys::subsys_init()
{
    const bool SDL_init_success = SDL_Init( SDL_INIT_AUDIO|SDL_INIT_TIMER) == 0;
    const bool Mixer_init_success = Mix_Init( MIX_INIT_OGG |MIX_INIT_MP3 | MIX_INIT_MOD)!=0 ;
    const bool IMG_init_success = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP) !=0;

    return SDL_init_success &Mixer_init_success&  IMG_init_success ;
}


/// @brief  被调用于 AudioBlock::audioitem_clickCallBack(),AudioBlock 已经确认了in_audio为list中的audio.
///         Sys在此函数中处理audio.并传递给其他static callback.
/// @param in_audio 
void Sys::audioitem_clickCallBack(const Audio &in_audio)
{

#ifdef PLOG
        char log[100];
        sprintf(log,"cur channel %d ,cur audio: %s , ptr:%d",in_audio->m_channel,in_audio->name.c_str(),in_audio->m_chunk);
        

    /* lambda 定义式子加() mean: 立即执行*/
    static auto logpos = [](){
        return Sys::Get()->m_logBlock->SetALog("curAudio info","empty");
        }(); 

         if(logpos) logpos.value().get().second.assign(log);
#endif
    Sys::Get()->m_effectBlock->audioitem_clickCallBack(in_audio);
}

void Sys::proccessInfo_effect(int chan, void *stream, int len, void *udata)
{
    //const Audio* audioref = (Audio*)udata;
    if(chan!= Sys::Get()->m_channel) return;


    if((Sys::Get()->m_curAudio).Get() != nullptr)
    {
        const Audio& audio = Sys::Get()->m_curAudio;
        audio->m_pos+= len;
        audio->m_proccess =(float) audio->m_pos / audio->GetChunk()->alen * 100.f;

        
        
#ifdef PLOG
        char logcontent[100] ;
        sprintf(logcontent,"proccessinfo curAudio's pos: %d , streamLen: %d , chucklen: %d ",
            audio->m_pos,
            len,
            audio->GetChunk()->alen
        );

        static auto logpos = [](){
        return Sys::Get()->m_logBlock->SetALog("procces info","empty");
        }(); 

        if(logpos) logpos.value().get().second.assign(logcontent);

#endif
    
    }

}

} // namespace SO


void ChannelFinishedCallback(int in_channel)
{
    if(in_channel==SO::Sys::Get()->m_channel)
    {
        auto audio = SO::Sys::FindBChunk(Mix_GetChunk(SO::Sys::Get()->m_channel));
        if(audio.Get() != nullptr)
        {
            audio->qexitChannel();
            audio->Label();
        }
        ///@attention 在播放完成后依然保留有chunk.
        ///if(Mix_GetChunk(1) == togg->GetChunk()) SDL_Log("still have chunk");
        SDL_Log("channel finshed call");
    }
}
