#include "audiosBlock.hpp"
#include "music.hpp"
#include "sys.hpp"
#include "utils/fileTool.h"




namespace SO{

ftxui::Component AudiosBlock::AudioItemRender(Audio in_audio)
{
    using namespace ftxui;
        
    /* 播放键*/
    ///@attention in_audio 是临时变量. 捕获ref无用 , 需要 = .
    auto playButton = Button( &(in_audio->Label()) , [=](){
        AudiosBlock::audioitem_clickCallBack(in_audio);
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
                if(Mix_GetChunk(m_channel) == in_audio->GetChunk())
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

const ftxui::Component AudiosBlock::freshItemList()
{
    m_itemList->DetachAllChildren();
    for(auto audio:m_list)
    {
        m_itemList->Add(AudioItemRender(audio));
    }
    return m_itemList;

}


ftxui::Component AudiosBlock::RenderComponent()
{
    using namespace ftxui;

    freshItemList();
    
    auto fileLoad = Button("FileLoad",[](){
        auto success =  Sys::LoadDialog("F:\\");
        #ifdef PLOG
            if(!success)
                TEMP_LOG("load not success");
        #endif
    }, ButtonOption::Ascii()) |color(Color::LightCyan3) ;

    auto head =  ftxui::Renderer( 
            fileLoad,
            [=](){
            return vbox({ 
                hbox({
                    fileLoad->Render(),
                    text("Label")|center|bold|color(Color::CyanLight)|xflex
                }),
                separatorHeavy()
            });
            }
    );
    
    auto vlist = Renderer(Container::Vertical({head,m_itemList}) , [=]{
        return vbox({
            head->Render(),
            separatorHeavy(),
            m_itemList->Render()| vscroll_indicator | frame | yflex,
        });
    }); 
    return vlist;
}

ftxui::Component AudiosBlock::AddAudioTItem(Audio in_audio)
{
    auto comp = AudioItemRender(in_audio);
    m_itemList->Add(comp);
    return comp;
}

void AudiosBlock::audioitem_clickCallBack(const Audio &in_audio)
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

        Sys::audioitem_clickCallBack(in_audio);

    };

}


}

