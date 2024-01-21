#include "effectBlock.hpp"
#include "sys.hpp"

#include "stdlib.h"

#define _USE_MATH_DEFINES
#include <cmath>
namespace SO
{

    ftxui::Component EffectBlock::RenderComponent()
    {
        using namespace ftxui;



    /* canvas renderer */
        auto renderer =  Renderer([this]{

        {

            char logcontent[100]; 
            sprintf(logcontent,
    " dmx : %d   dmy : %d  lmx : %d  lmy : %d "
            " distance : %d , fullen : %d , angle: %d  " ,
             m_dmx,m_dmy,m_lmx,m_lmy,
             m_distance,m_fullLen,m_angle);

            INFO_LOG("effeckBlock pos",logcontent);
        }

            input_fresh();
            
            auto c = Canvas(m_dmx,m_dmy);

            c.DrawText(0, 0, "Positon Effect ");

            c.DrawBlock(0,2,true,Color::Red1);
            c.DrawBlock(m_dmx,m_dmy-2,true,Color::Red1);

            c.DrawPointCircle(m_dmx/2,m_dmy/2,m_len,Color::GrayDark);
            c.DrawPointCircle(m_dmx/2,m_dmy/2,m_fullLen,Color::GrayLight);

            c.DrawPointLine(m_dmx/2,m_dmy/2 , m_lmx,m_lmy);
            c.DrawPoint(m_lmx,m_lmy,true,Color::Cyan1);

            return canvas(std::move(c));// | focusPositionRelative(m_lmx, m_lmy);
        });

        renderer |=CatchEvent([this](ftxui::Event in_event){
            if( in_event.is_mouse()&&
                in_event.mouse().button == Mouse::Left )
            {

                auto& mous = in_event.mouse();
                canvas_pressingCallBack(mous);
            }

            return true;
        });

    /* inputs  */
        /* limited only int input to set angle and distance */

        auto intlimitedInput = [](std::string& in_stref, const char* in_placehold ,const char* in_name){
            
            auto comp = Input(&in_stref,in_placehold);
            return Renderer(comp,[=](){
                return hbox({
                    text(in_name),
                    comp->Render()
                });
            });
        };
        
        /* 窝不会嵌套捕获this 所以拆开了 */
        //TODO: 最好还是能响应式刷新. 所以怎么让input响应完后调用😢
        auto angleinput =  intlimitedInput(m_angleinput," 0 ~ 360 ","angle    :");
        angleinput |=CatchEvent([this](Event in_event){
                if(in_event.is_character())
                {
                    return !(std::isdigit(in_event.character()[0]) && m_angleinput.size()<3);
                }
                return false;
        });
        auto distanceinput = intlimitedInput(m_distanceinput," 0 ~ 255  ","distance :");
        distanceinput |=CatchEvent([this](Event in_event){
                if(in_event.is_character())
                {
                    return !(std::isdigit(in_event.character()[0]) && m_distanceinput.size()<3);
                }
                return false;
        });

            
    /* container  */
        auto renderer_eventcatch_warped_renderer = Container::Vertical({
            angleinput,
            distanceinput,
            renderer
        })|border;

        return renderer_eventcatch_warped_renderer;

    }

    void EffectBlock::init()
    {

        m_fullLen = std::min(m_dmx,m_dmy)/2 - 5;

        m_len = (float)m_distance / 255 * m_fullLen;

        /* why -90 . Mixer 0 度 默认后方开始逆时针旋转 (大概*/
        m_lmx = m_dmx/2 +(int) (std::cos((float)(m_angle-90)/180*M_PI )*m_len) ;
        m_lmy = m_dmy/2 -(int) (std::sin((float)(m_angle-90)/180*M_PI )*m_len) ; 

    }

    void EffectBlock::audioitem_clickCallBack(const Audio& in_audio )
    {
        setMixer();
    
    }
    void EffectBlock::canvas_pressingCallBack(const ftxui::Mouse &in_mouse)
    {
        // m_lmx = in_mouse.x ;
        // m_lmy = in_mouse.y ; 

        m_distance = std::sqrt( (float)( (m_lmx - m_dmx/2)^2 + (m_lmy - m_dmy/2)^2 )  );

        char logcontent[100]; 
        sprintf(logcontent,"x pos : %d ,y pos: %d " , in_mouse.x,in_mouse.y);
        
        INFO_LOG("canvas clicked mouse pos",logcontent);


    }
    void EffectBlock::input_fresh()
    {
        {
        std::istringstream ss(m_angleinput);
        ss>>m_angle;
        }
        {
        std::istringstream ss(m_distanceinput);
        ss>>m_distance;
        }



        m_len = (float)m_distance / 255 * m_fullLen;

        m_lmx = m_dmx/2 +(int) (std::cos((float)(m_angle-90)/180*M_PI )*m_len) ;
        m_lmy = m_dmy/2 -(int) (std::sin((float)(m_angle-90)/180*M_PI )*m_len) ; 
        

    }
    void EffectBlock::setMixer()
    {
        uint8_t distance;
        if(m_distance>255)
            distance = 255;
        ///@attention 在效果序列重置前(即chunk退出/channel完成) Mix_SetPosition无效。或者可以手动重新注册？
        if(  Mix_SetPosition(Sys::Get()->m_channel,m_angle,distance)) 
        {
            TEMP_LOG("postion effect set Position success!")
        }
        else
        {
            TEMP_LOG("postion effect set Position filed")
        }
    }
}