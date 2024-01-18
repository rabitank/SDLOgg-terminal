#include "sys.hpp"
#include "defines.hpp"

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

void Sys::logInit()
{
    {
        char logcontent[100] ;
        sprintf(logcontent,"proccessinfo curAudio's pos: %s , streamLen: %s , chucklen: %s ",
            "null",
            "null",
            "null"
        );
        Sys::Get()->logVector.emplace_back(std::string("procces info"),std::string(logcontent));
    }

    {
        char log[100];
        sprintf(log,"cur channel %d ,cur audio: %s , ptr:%d",Sys::Get()->m_channel,"null","null");
        Sys::Get()->logVector.emplace_back(  std::string("last chunk ptr:"), std::string(log) 
                                        );
    }
        
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
            m_ins->logInit();
#endif

            Mix_ChannelFinished(ChannelFinishedCallback);
            Mix_RegisterEffect(m_ins->m_channel,Sys::proccessInfo_effect,nullptr,nullptr);
            
        }
        else return 0;
    }
    return 1;
}


ftxui::Component Sys::AudioItemRender(Audio in_audio)
{
    using namespace ftxui;
        
    /* 播放键*/
    ///@attention in_audio 是临时变量. 捕获ref无用 , 需要 = .
    auto playButton = Button( &(in_audio->Label()) , [=](){
        Sys::audioitem_clickCallBack(in_audio);
    });


    /* 进度条 */
    SliderOption<float> slideroption_f;
    slideroption_f.min = 0.f;
    slideroption_f.max = 100.f;
    slideroption_f.increment = 0.01f;
    slideroption_f.value = &(in_audio->m_proccess);
    auto jindutiao = Slider(slideroption_f) | size(WIDTH,GREATER_THAN,40)|
                                    CatchEvent([](Event in_event){
                                        return false;
                                    });
        
    auto component = ftxui::Renderer(
            Container::Vertical({
                playButton,
                jindutiao
            }),
            [=](){
                if(Mix_GetChunk(Get()->m_channel) == in_audio->GetChunk())
                    in_audio->m_proccess ;

                return vbox({
                    playButton->Render(),
                    jindutiao->Render()
                });
            }
    );

    return component;

}

ftxui::Component Sys::LogOutRender()
{
    using namespace ftxui;
    Component content = Container::Vertical({
        ftxui::Renderer(
            [](){
                return vbox({
                text("SO Log")|bold|color(Color::BlueLight)|hcenter|xflex,
                separatorHeavy()
                })|xflex;
            }
        )
    });


    content->Add(Renderer([](){
        Elements logs;
        for(auto item : Sys::Get()->logVector)
        {
            logs.push_back(text(item.first)|bgcolor(Color::DarkBlue)|xflex);
            logs.push_back(paragraph(item.second));
            logs.push_back(separator());
        }
        return vbox(logs);
        
    }));

    return content;
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
    m_curAudio = in_audio;
    m_curPos = m_list.end()-1; 
}

ftxui::Component Sys::ItemsRenderer()
{
    using namespace ftxui;
    
    auto vlist = Container::Vertical({
        ftxui::Renderer(
            [](){
            return vbox( {
            text("Label -oggpath -channel -state "),
            separatorHeavy()
            });
            }
        )
    });


    for(auto audio:  Sys::Get()->m_list )
    {
        vlist->Add(Sys::AudioItemRender(audio));
    }
    return vlist;
}


bool Sys::subsys_init()
{
    const bool SDL_init_success = SDL_Init( SDL_INIT_AUDIO|SDL_INIT_TIMER) == 0;
    const bool Mixer_init_success = Mix_Init( MIX_INIT_OGG |MIX_INIT_MP3 | MIX_INIT_MOD)!=0 ;
    const bool IMG_init_success = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP) !=0;

    return SDL_init_success &Mixer_init_success&  IMG_init_success ;
}



void Sys::audioitem_clickCallBack(const Audio &in_audio)
{
    in_audio->Toggle();
    in_audio->Label();

    if(in_audio == Sys::Get()->m_curAudio) return;

    AudioList& list = Sys::Get()->m_list;
    const AudioPos pos = std::find(list.begin(),list.end(), in_audio);
    if(pos!= list.end())
    {
        Sys::Get()->m_curPos = pos;
        Sys::Get()->m_curAudio = *pos;

#ifdef PLOG
        char log[100];
        sprintf(log,"cur channel %d ,cur audio: %s , ptr:%d",(*pos)->m_channel,(*pos)->name,(*pos)->m_chunk);
        Sys::Get()->logVector.at(1).second = log;
#endif

    };


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

        if(&SO::Sys::Get()->logVector[0]!=nullptr)
        {
            LogPara* const  logpos = &SO::Sys::Get()->logVector[0];
            logpos->second.assign(logcontent);
        }
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
