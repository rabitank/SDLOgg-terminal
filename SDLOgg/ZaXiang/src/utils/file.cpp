#include "utils/file.hpp"
#include "core/application.hpp"

#include "Windows.h"
#include "commdlg.h" //Win32 API: for a bunch of spersific types about File? 通用对话框的头文件，包括文件对话框，颜色对话框，打印对话框。
#pragma comment(lib, "WS2_32")

#include "SDL.h"
#include "SDL_syswm.h"

namespace fs
{
    static HWND SDL_GetWindowHWND(SDL_Window* in_window){
    // view https://github.com/libsdl-org/SDL/blob/SDL2/include/SDL_syswm.h    
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(in_window, &wmInfo);
        return  wmInfo.info.win.window;
    }

    /// @brief OpenFile. 输入过滤格式
    /// @param filter 格式: "文本文件(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0\0"
    /// @return 返回选中项路径
    std::string  Utils::OpenFile(const char* filter) 
	{
		OPENFILENAMEA ofn; // 公共对话框结构。
		CHAR szFile[260] = { 0 }; // 用来保存获取文件名称的缓冲区。 

		ZeroMemory(&ofn, sizeof(OPENFILENAME)); // 初始化选择文件对话框
		ofn.lStructSize = sizeof(OPENFILENAME); //指定这个结构的大小，以字节为单位。
		ofn.hwndOwner = SDL_GetWindowHWND(Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile; //打开的文件的全路径
		ofn.nMaxFile = sizeof(szFile);//指定lpstrFile缓冲的大小，以TCHARs为单位
		ofn.lpstrFilter = filter; //打开文件类型过滤器
		ofn.nFilterIndex = 1; //指定在文件类型控件中当前选择的过滤器的索引

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; //位标记的设置，你可以使用来初始化对话框
		if(GetOpenFileNameA(&ofn) == true) // 显示"打开选择文件对话框"。  
		{
			return ofn.lpstrFile;

		}
		return std::string();

		//view https://blog.csdn.net/kevinfan2011/article/details/84982588

	}
	
    /// @brief SaveFile. 输入过滤格式
    /// @param filter 格式: "文本文件(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0\0"
    /// @return 返回存项路径
	std::string  Utils::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn; 
		CHAR szFile[260] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAME)); // 初始化选择文件对话框
		ofn.lStructSize = sizeof(OPENFILENAME); //指定这个结构的大小，以字节为单位。
		ofn.hwndOwner = SDL_GetWindowHWND(Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile; 
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter; 
		ofn.nFilterIndex = 1; 

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR; //: 路径必须存在,文件必须存在,不能操作文件
		if (GetSaveFileNameA(&ofn) == true) // 显示"保存文件对话框"。  
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}
    
} // namespace fs


