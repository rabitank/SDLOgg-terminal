#include "logBlock.hpp"
#include "sys.hpp"



ftxui::Component SO::LogBlock::RenderComponent()
{

    using namespace ftxui;    
    Component content = ftxui::Renderer(
            [this](){
                Elements alllog;
                for(const auto& item:m_logvector)
                {
                    alllog.push_back(text(item.first)|color(Color::Black)|bold |bgcolor(Color::DarkCyan)|xflex);
                    alllog.push_back(hflow(paragraph(item.second)));
                }

                return vbox({
                text("SO Log")|bold|color(Color::CyanLight)|hcenter|xflex,
                separatorHeavy(),
                
                vbox(alllog) 
                //Sys::Get()->m_logBlock->m_itemList->Render()|vscroll_indicator | yframe|flex
                    
                })|flex;
            }
        );
    return content;
}

std::optional<std::reference_wrapper<SO::LogPara>>  SO::LogBlock::SetALog(const char *in_tile, const char *in_content)
{
    auto pos =  std::find_if(m_logvector.begin(),m_logvector.end(),[](const LogPara& in_log){
        return in_log.first == "null";
    });
    if(pos == m_logvector.end())
    {
        /* list没下标,我们使用的是 vector ,所以不得不限定一下maxlognum啊... */
        //TODO: ! 但是, 只要有了传说中的! vector<shared_ptr<>>(allocator{make_shared}) 的话!就一定....
        if(m_logvector.size() >= MAX_LOG_NUM  )
        {
            return std::nullopt;
        }
        else{
            addEmpty();
            pos = m_logvector.end()-1;
        }
    }

    pos->first = in_tile;
    pos->second = in_content;
    int len = pos-m_logvector.begin();


    return std::ref(*pos) ;
}

std::optional<std::reference_wrapper<SO::LogPara>> SO::LogBlock::SetALogBPos(int pos, const char *in_content, const char *in_title)
{
    // optional view: https://zhuanlan.zhihu.com/p/64985296
    // ref . reference_wrapper. view: https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper
    // 如需显式返回. 请使用make_optional<>: in_place API

    if(pos>=m_logvector.size())
    {
        return std::nullopt;
    }
    LogPara& log = m_logvector[pos];
    if(in_title) log.first = in_title;
    log.second = in_content;

    

    return std::ref(log);
}

std::optional<std::reference_wrapper<SO::LogPara>> SO::LogBlock::SetALogBTitle(const char *in_title, const char *in_content)
{
    auto oLog = FindBTitle(in_title);
    if(oLog.has_value())
    {
        auto log  = oLog.value();
        log.get().second = in_content;
    }

    return oLog;
}
#include <list>

std::optional<std::reference_wrapper<SO::LogPara>> SO::LogBlock::FindBTitle(const char *in_title)
{

    auto pos =  std::find_if(m_logvector.begin(),m_logvector.end(),[=](const LogPara& in_log){
        return in_log.first == in_title;
    });

    std::list<std::string> slist;
    
    if(pos == m_logvector.end()) return std::nullopt;
    auto& log = *pos;

    return {log};
}

void SO::LogBlock::init()
{
    m_logvector.reserve(MAX_LOG_NUM + 2 );
    
    /* temp logPos */
    {
        Sys::Get()->logVector.emplace_back(  std::string("temp info :"), std::string("empty") 
                                        );
    }

    /* waring logpos*/
    {
        Sys::Get()->logVector.emplace_back(  std::string("warning info :"), std::string("empty") 
                                        );
    }
    
    /* 一个标准空log位置 */
    {
        Sys::Get()->logVector.emplace_back(  std::string("null"), std::string("empty") );
    }

    // {
    //     m_logvector.emplace_back("null","empty");
    // }

}

void SO::LogBlock::addEmpty()
{
    m_logvector.emplace_back("null","empty");
}