#ifndef __TINY_TOOLKIT__DEBUG__TRACE__H__
#define __TINY_TOOLKIT__DEBUG__TRACE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 调试跟踪
 *
 */


#include "exception.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API TraceHelper
	{
	public:
		/**
		 *
		 * 调试信息
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param file 文件
		 * @param line 行号
		 * @param func 函数
		 * @param format 格式化
		 * @param args 参数
		 *
		 */
		template <typename... Args>
		static void Debug(const char * file, const int32_t line, const char * func, const char * format, Args &&... args)
		{
			std::cout << std::endl << tinyToolkit::String::Format("[{}:{} {}] {}",
																  file,
																  line,
																  func,
																  tinyToolkit::String::Format(format, std::forward<Args>(args)...))
					  << std::endl;
		}

		/**
		 *
		 * 调试信息
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param file 文件
		 * @param line 行号
		 * @param func 函数
		 * @param format 格式化
		 * @param args 参数
		 *
		 */
		template <typename... Args>
		static void Debug(const char * file, const int32_t line, const char * func, const std::string & format, Args &&... args)
		{
			std::cout << std::endl << tinyToolkit::String::Format("{[}:{} {}] {}",
																  file,
																  line,
																  func,
																  tinyToolkit::String::Format(format, std::forward<Args>(args)...))
					  << std::endl;
		}

		/**
		 *
		 * 致命异常
		 *
		 * @param file 文件
		 * @param line 行号
		 * @param func 函数
		 *
		 */
		static void Fatal(const char * file, int32_t line, const char * func)
		{
			ExceptionHelper::Throw<SystemExitException>(file, line, func);
		}

		/**
		 *
		 * 致命异常
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param file 文件
		 * @param line 行号
		 * @param func 函数
		 * @param format 格式化
		 * @param args 参数
		 *
		 */
		template <typename... Args>
		static void Fatal(const char * file, int32_t line, const char * func, const char * format, Args &&... args)
		{
			ExceptionHelper::Throw<SystemExitException>(file, line, func, tinyToolkit::String::Format(format, std::forward<Args>(args)...));
		}

		/**
		 *
		 * 致命异常
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param file 文件名
		 * @param line 行号
		 * @param func 函数名
		 * @param format 格式化
		 * @param args 参数
		 *
		 */
		template <typename... Args>
		static void Fatal(const char * file, int32_t line, const char * func, const std::string & format, Args &&... args)
		{
			ExceptionHelper::Throw<SystemExitException>(file, line, func, tinyToolkit::String::Format(format, std::forward<Args>(args)...));
		}

		/**
		 *
		 * 断言异常
		 *
		 * @param file 文件
		 * @param line 行号
		 * @param func 函数
		 * @param cond 判断表达式
		 *
		 */
		static void Assert(const char * file, int32_t line, const char * func, const char * cond)
		{
			ExceptionHelper::Throw<AssertException>(file, line, func, "", cond);
		}

		/**
		 *
		 * 断言异常
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param file 文件
		 * @param line 行号
		 * @param func 函数
		 * @param cond 判断表达式
		 * @param format 格式化
		 * @param args 参数
		 *
		 */
		template <typename... Args>
		static void Assert(const char * file, const int32_t line, const char * func, const char * cond, const char * format, Args &&... args)
		{
			ExceptionHelper::Throw<AssertException>(file, line, func, tinyToolkit::String::Format(format, std::forward<Args>(args)...), cond);
		}

		/**
		 *
		 * 断言异常
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param file 文件
		 * @param line 行号
		 * @param func 函数
		 * @param cond 判断表达式
		 * @param format 格式化
		 * @param args 参数
		 *
		 */
		template <typename... Args>
		static void Assert(const char * file, const int32_t line, const char * func, const char * cond, const std::string & format, Args &&... args)
		{
			ExceptionHelper::Throw<AssertException>(file, line, func, tinyToolkit::String::Format(format, std::forward<Args>(args)...), cond);
		}
	};
}


#define TINY_TOOLKIT_DEBUG(...)			tinyToolkit::TraceHelper::Debug(TINY_TOOLKIT_FILE, TINY_TOOLKIT_LINE, TINY_TOOLKIT_FUNC, ##__VA_ARGS__);
#define TINY_TOOLKIT_FATAL(...)			tinyToolkit::TraceHelper::Fatal(TINY_TOOLKIT_FILE, TINY_TOOLKIT_LINE, TINY_TOOLKIT_FUNC, ##__VA_ARGS__);


#ifdef DEBUG

#define TINY_TOOLKIT_ASSERT(cond, ...)	if (!(cond)) { tinyToolkit::TraceHelper::Assert(TINY_TOOLKIT_FILE, TINY_TOOLKIT_LINE, TINY_TOOLKIT_FUNC, #cond, ##__VA_ARGS__); }

#else

#define TINY_TOOLKIT_ASSERT(cond, ...)	{ }

#endif


#endif // __TINY_TOOLKIT__DEBUG__TRACE__H__
