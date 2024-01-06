#pragma once
//some basic macro
#include<memory>

#ifdef FS_DEBUG
#if defined(FS_PLATFORM_WINDOWS)
#define FS_DEBUGBREAK() __debugbreak()
#elif defined(FS_PLATFORM_LINUX)
#include <signal.h>
#define FS_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define FS_ENABLE_ASSERTS
#else
#define FS_DEBUGBREAK()
#endif

#define  FS_EXPAND_MACRO(x) x
#define FS_STRINGIFY_MACRO(x) #x

#define BIT(x) (1<<x) //1位移x位

//不是很懂& 和 this 干嘛?: 函数引用?,使用this,是成员函数的隐藏参数吧
//记得 带上命名空间/类名
#define  BIND_EVENT_CALLFN(x) std::bind(&x,this,std::placeholders::_1) 


namespace fs
{
	template <typename T>
	using Scope = std::unique_ptr<T>;//指明作用域限定
	template<typename T ,typename... Args> //typename...  形参列表包 命名为 Args
	constexpr Scope<T> CreateScope(Args&& ... args)//Args引用折叠后 命名为 args(变为右值?)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);//转发形参args(...解包)
	}

	template <typename T>
	using Ref = std::shared_ptr<T>; //指明资产的ref性质,并对ref进行包装,方便之后的多线程通信,我们只需对自己的资产强调这一点
	template <typename T>
	using Uni = std::unique_ptr<T>;

	template <typename T,typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)//constexpr func() 常量表达式函数修饰....保证返回常量(只能有return和其他几种命令),会在编译时计算好
	{
		return std::make_shared<T>(std::forward<Args>(args)...); 
	}

}

#include "log.hpp"
#include "assert.hpp"
