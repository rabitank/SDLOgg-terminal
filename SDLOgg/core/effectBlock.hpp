#pragma once
#include "block.hpp"

#include "SDL.h"


namespace SO
{
    class Audio;

    class  EffectBlock : public Block
    {
    
    friend class Sys;
    
    public:
        EffectBlock(Sint16& in_angle , Sint16& in_distance, const int in_dmx = 50, const int in_dmy = 50):
        m_angle(in_angle),m_distance(in_distance),m_dmx(in_dmx),m_dmy(in_dmy)
        {
            init();
        };


        virtual ftxui::Component RenderComponent() override;

    private:
        void init();
        void audioitem_clickCallBack( const Audio& in_audio  );
        void canvas_pressingCallBack(const ftxui::Mouse& in_mouse);
        void input_fresh();
        void setMixer();

        std::string m_angleinput;
        std::string m_distanceinput;

        Sint16 & m_angle;
        Sint16 & m_distance;

        int m_dmx{50}; 
        int m_dmy{50}; 
        int m_lmx{0} , m_lmy {0} ;
        int m_fullLen{21};
        int m_len{10};
    };
    
    
} // namespace 


