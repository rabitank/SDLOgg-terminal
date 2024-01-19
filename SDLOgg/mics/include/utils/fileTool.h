#pragma once
#include "pch.hpp"
//// about file tools.
#include <string>

#include "tinyfiledialogs.h" //for 跨平台的文件选择框


bool IsFileExist(const std::string &in_path);

///@attention 获取不带路径的完整文件名
std::string GetFilename(const std::string &in_path , char in_separator = '/' );

///@attention 获取不带路径的文件名和后缀
void GetFileNameASuffix( const std::string &in_path , std::string* g_name , std::string* g_suffix,char in_separator = '/');


/// @brief in case of multiple files, the separator is | . 
/// @param in_Title NULL or ""
/// @param in_DefaultPathAndOrFile  NULL or "" 
/// @param in_NumOfFilterPatterns num
/// @param in_FilterPatterns  NULL or {"*.jpg","*.png"} 
/// @param in_SingleFilterDescription  NULL or "image files" 
/// @param in_AllowMultipleSelects 0 or 1. 0 means:forbid multiple selections
/// @return the prt is Null or a static char[]. don't need to free.
char const * OpenFileDialog( 	
        char const * in_Title , 
		char const * in_DefaultPathAndOrFile, 
	    int in_NumOfFilterPatterns ,
		char const * const * in_FilterPatterns, 
		char const * in_SingleFilterDescription,
	    int in_AllowMultipleSelects = 1
);

