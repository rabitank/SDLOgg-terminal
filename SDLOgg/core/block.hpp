#pragma once
#include "pch.hpp"

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component.hpp" 
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/dom/elements.hpp"

#include "defines.hpp"

namespace SO
{
    class Sys;
    
    /* 一些c++补漏 */
    // C++中把成员变量定义为引用有啥问题 view: https://zhuanlan.zhihu.com/p/262210907
    // 常识-呃呃 析构ref 不会删掉引用资源哈.
    class Block{
    
    friend class Sys;
    protected:
        Block(){};

    public:
        virtual ~Block(){};

        std::string name{"block"};
        virtual ftxui::Component RenderComponent() = 0;
    };

    class AudiosBlock;
    class LogBlock;
    class EffectBlock;



} // namespace SO

