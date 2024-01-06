#pragma once
#include <string>
#include <iostream>
#include "SDL.h"

// 区分此函数是在Windows环境调用还是Linux环境调用
#if defined (_WIN64) || defined (WIN32) || defined (_WIN32)

//printf("---Windows---\n");
#include <direct.h>

#else

//printf("---Linux---\n");
#include <unistd.h>

#endif



/******************************************************************************
 *
 * 功能：
 *		获得当前程序的工作路径(绝对路径)，即运行路径！
 *
 * 注意：
 *		头文件在unix下是unistd.h，VS下是direct.h，应该依编程者的环境而定.
 *		这里解释一下运行路径，即是程序开始运行的路径，例如：
 *			1.如果是在Windows环境的VS编译器中运行项目，则返回的是项目路径，
 *			  即代码文件路径(.h和.cpp路径)，因为是在编译器中运行的项目，所以
 *			  程序的运行路径也是才项目路径中开始运行的。
 *			2.如果是在Windows环境，运行已经编译好的.exe程序，则返回的是当前
 *			  .exe程序所在的路径，因为是在当前路径所运行的！
 *			3.在Linux环境，返回的都是可执行程序的路径！
 *
 * 参数：
 *		无.
 *
 * 返回值：
 *		成功返回程序的工作路径(绝对路径)；失败返回空串
 *
 ******************************************************************************/
std::string getOperationFilePath() {

	char *buffer = NULL;

	// 区分此函数是在Windows环境调用还是Linux环境调用
#if defined (_WIN64) || defined (WIN32) || defined (_WIN32)

	// 获取项目的工作路径
	buffer = _getcwd(NULL, 0);

#else

	// 获取项目的工作路径
	buffer = getcwd(NULL, 0);

#endif

	if (buffer) {
		std::string path = buffer;
		free(buffer);
		return path ;
	}

	return "";
}


inline void ShowCwdMessageBox(SDL_Window* in_window){
	std::cout<<getOperationFilePath()<<std::endl;
	SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_INFORMATION, "cwd",getOperationFilePath().c_str(),in_window);
} 
