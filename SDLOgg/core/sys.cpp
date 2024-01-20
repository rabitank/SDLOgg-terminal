#include "sys.hpp"
#include "defines.hpp"
#include "utils/fileTool.h"

#ifdef PLOG
#define TEMP_LOG(x) SO::Sys::Get()->logVector.back().second = x;
#else
#define TEMP_LOG()
#endif
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
    

    /* temp logPos , always last*/
    {
        Sys::Get()->logVector.emplace_back(  std::string("temp info :"), std::string("empty") 
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

void Sys::MainLoop()
{
    using namespace ftxui;

    auto Sysrenderer = ItemsRenderer();
    auto Logrenderer = LogOutRender();
    auto renderer = Renderer(Sysrenderer,[&](){
        char cont[64];
        sprintf(cont, "channel 1 current chunk: %d",(void*)Mix_GetChunk(1));
        
        return hbox({
            (Sysrenderer)->Render(),
            separator(),
            Logrenderer->Render()
            })|border;
    });

    auto screen = ScreenInteractive::Fullscreen(); 
    screen.Loop(renderer);
    
}

ftxui::Component Sys::AudioItemRender(Audio in_audio)
{
    using namespace ftxui;
        
    /* 播放键*/
    ///@attention in_audio 是临时变量. 捕获ref无用 , 需要 = .
    auto playButton = Button( &(in_audio->Label()) , [=](){
        Sys::audioitem_clickCallBack(in_audio);
    } , ButtonOption::Animated(Color::Default,Color::DarkCyan,
                                        Color::GrayDark,Color::LightCyan3))
                                    | size(HEIGHT,EQUAL,3);
    /* 似乎 对于 animated button 的warp renderer 不起作用啊哈哈*/

    /* 进度条 */
    SliderOption<float> slideroption_f;
    slideroption_f.min = 0.f;
    slideroption_f.max = 100.f;
    slideroption_f.increment = 0.01f;
    slideroption_f.value = &(in_audio->m_proccess);
    slideroption_f.color_inactive = Color::GrayDark;
    slideroption_f.color_active = Color::GrayLight;
    auto jindutiao = Slider(slideroption_f) | size(WIDTH,GREATER_THAN,40)|
                                        CatchEvent([](Event in_event){
                                        return false;
                                    });

    auto component = ftxui::Renderer(
                playButton,
            [=](){
                if(Mix_GetChunk(Get()->m_channel) == in_audio->GetChunk())
                    in_audio->m_proccess ;
                    
                Element buttonrender =  playButton->Render(); 
                return vbox({
                    buttonrender,
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

const ftxui::Component Sys::freshItemList()
{
    m_itemList->DetachAllChildren();
    for(auto audio:m_list)
    {
        m_itemList->Add(AudioItemRender(audio));
    }
    return m_itemList;

}

void Sys::addTList(const Audio &in_audio)
{
    m_list.push_back(in_audio);
    m_itemList->Add(AudioItemRender(in_audio));
    m_curAudio = in_audio;
    m_curPos = m_list.end()-1; 
}


ftxui::Component Sys::ItemsRenderer()
{
    using namespace ftxui;

    freshItemList();
    
    auto fileLoad = Button("FileLoad",[](){
        auto success =  LoadDialog("F:\\");
        #ifdef PLOG
            if(!success)
                TEMP_LOG("load not success");
        #endif

    })|size(HEIGHT,Constraint::EQUAL,3);
    auto head =  ftxui::Renderer( 
            fileLoad,
            [=](){
            return vbox({ 
                hbox({
                    fileLoad->Render(),
                    text("Label")|center|bold|color(Color::Orange3)|xflex
                }),
                separatorHeavy()
            });
            }
    );
    
    auto vlist = Renderer(Container::Vertical({head,m_itemList}) , [=]{
        return vbox({
            head->Render(),
            separatorHeavy(),
            m_itemList->Render()| vscroll_indicator | frame |
                size(HEIGHT, LESS_THAN, 20),
        });
    }); 
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
        sprintf(log,"cur channel %d ,cur audio: %s , ptr:%d",(*pos)->m_channel,(*pos)->name.c_str(),(*pos)->m_chunk);
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
