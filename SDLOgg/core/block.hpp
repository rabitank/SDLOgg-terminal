#pragma once
#include "pch.hpp"

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component.hpp" 
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/dom/elements.hpp"

namespace SO
{
    class Block{
    public:
        Block(){};
        virtual ~Block(){};

        std::string name{"block"};
        virtual ftxui::Component RenderComponent() = 0;
    };

} // namespace SO

