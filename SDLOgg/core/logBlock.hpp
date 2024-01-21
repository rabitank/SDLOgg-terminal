#pragma once

#include "block.hpp"
#include "defines.hpp"




namespace SO{
    
    using LogVector  = std::vector<std::pair<std::string,std::string>>;
    using LogPara = LogVector::value_type;  

/// @brief LogBlock 应该成为一个管理类. 用于拆分Sys中绘制Log的责任
class LogBlock:public Block
{
public:
    LogBlock(LogVector& in_vectorref):
    m_logvector(in_vectorref){init(); };

public:

    virtual ftxui::Component RenderComponent() override;

    /// @brief 获取一个空位然后设置log信息 ,没空位就加。vector不能再加再加就realloc时就返回空.
    /// @return 
    std::optional<std::reference_wrapper<SO::LogPara>> SetALog(const char* in_tile,const char* in_content);


    /// @brief 设置log信息,通过下表确定位置
    /// @param pos 
    /// @param in_content 
    /// @param in_title 
    /// @return 没找到时返nullopt
    std::optional<std::reference_wrapper<SO::LogPara>>  SetALogBPos(int pos , const char* in_content,const char* in_title = nullptr);
    
    /// @brief 设置log信息,通过标题找到位置
    /// @param in_title 
    /// @param in_content 
    /// @return 没找到时返nullopt
    std::optional<std::reference_wrapper<SO::LogPara>> SetALogBTitle(const char* in_title , const char* in_content);

    /* may need adapt */
    std::optional<std::reference_wrapper<SO::LogPara>> SetALogMayMatch(const char* in_mayMatchtile,const char* in_content);
    
    std::optional<std::reference_wrapper<SO::LogPara>> FindBTitle(const char* in_title);


public:
    /// @brief logs list. ver item has an tile an paragrah
    ftxui::Component m_itemList{ftxui::Container::Vertical({})};
private:
    const static int MAX_LOG_NUM = 16;

    void init();
    void addEmpty();

    SO::LogVector& m_logvector;

};


}


#ifndef TEMP_LOG

#ifdef PLOG

#define TEMP_LOG(x) SO::Sys::Get()->logVector[0].second = x;
#define WAR_LOG(x)  SO::Sys::Get()->logVector[1].second = x;
#define INFO_LOG(x,y) \
        do{\
            static auto logpos = [](){\
            return SO::Sys::Get()->m_logBlock->SetALog(x,"empty");\
            }(); \
            if(logpos) logpos.value().get().second.assign(y);\
        } while(0)  

#else
#define TEMP_LOG
#define WAR_LOG
#define INFO_LOG
#endif

#endif