#include "logBlock.hpp"
#include "sys.hpp"

const ftxui::Component SO::LogBlock::freshItemList()
{
    using namespace ftxui;

    m_itemList->DetachAllChildren();

    /* 这里之前由于auto没加&导致了 bad_alloc.
    呃呃. Renderer的Render lambda反复运行.局部变量的销毁造成了空悬的LogPara引用
    至于为什么导致了bad_alloc,呃呃,这个不知道 */
    for(auto& item : m_logvector)
    {
        m_itemList->Add(logItemRender(item));
    }


    return m_itemList;
}

ftxui::Component SO::LogBlock::RenderComponent()
{

    using namespace ftxui;    
    Component content = ftxui::Renderer(
            [this](){
                return vbox({
                text("SO Log")|bold|color(Color::BlueLight)|hcenter|xflex,
                separatorHeavy(),
                Sys::Get()->m_logBlock->m_itemList->Render()
                })|xflex;
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
    // ref . reference_wrapper.  view: 
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

    {
        m_logvector.emplace_back("null","empty");
    }

}

void SO::LogBlock::addEmpty()
{
    m_logvector.emplace_back("null","empty");
    m_itemList->Add(logItemRender(m_logvector.back()));
}

ftxui::Component SO::LogBlock::logItemRender(LogPara& in_log){
    using namespace ftxui;
    return Renderer([&](){
        return vbox({
            text(in_log.first)|bgcolor(Color::DarkCyan)|xflex,
            paragraph(in_log.second)                              
        });
    });
}