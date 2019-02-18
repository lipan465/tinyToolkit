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

#include "../utilities/string.h"


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
		 * @param fileLine 文件信息
		 * @param format 格式化
		 * @param args 参数
		 *
		 */
		template <typename... Args>
		static void Debug(const FileLine & fileLine, const char * format, Args &&... args)
		{
			std::cout << std::endl << String::Format("[{}] {}", fileLine.Message(), String::Format(format, std::forward<Args>(args)...)) << std::endl;
		}

		/**
		 *
		 * 调试信息
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param fileLine 文件信息
		 * @param format 格式化
		 * @param args 参数
		 *
		 */
		template <typename... Args>
		static void Debug(const FileLine & fileLine, const std::string & format, Args &&... args)
		{
			std::cout << std::endl << String::Format("[{}] {}", fileLine.Message(), String::Format(format, std::forward<Args>(args)...)) << std::endl;
		}

		/**
		 *
		 * 致命异常
		 *
		 * @param fileLine 文件信息
		 *
		 */
		static void Fatal(const FileLine & fileLine)
		{
			ExceptionHelper::Throw<SystemExitException>(fileLine);
		}

		/**
		 *
		 * 致命异常
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param fileLine 文件信息
		 * @param format 格式化
		 * @param args 参数
		 *
		 */
		template <typename... Args>
		static void Fatal(const FileLine & fileLine, const char * format, Args &&... args)
		{
			ExceptionHelper::Throw<SystemExitException>(fileLine, String::Format(format, std::forward<Args>(args)...));
		}

		/**
		 *
		 * 致命异常
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param fileLine 文件信息
		 * @param format 格式化
		 * @param args 参数
		 *
		 */
		template <typename... Args>
		static void Fatal(const FileLine & fileLine, const std::string & format, Args &&... args)
		{
			ExceptionHelper::Throw<SystemExitException>(fileLine, String::Format(format, std::forward<Args>(args)...));
		}

		/**
		 *
		 * 断言异常
		 *
		 * @param fileLine 文件信息
		 * @param cond 判断表达式
		 *
		 */
		static void Assert(const FileLine & fileLine, const char * cond)
		{
			ExceptionHelper::Throw<AssertException>(fileLine, "", cond);
		}

		/**
		 *
		 * 断言异常
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param fileLine 文件信息
		 * @param cond 判断表达式
		 * @param format 格式化
		 * @param args 参数
		 *
		 */
		template <typename... Args>
		static void Assert(const FileLine & fileLine, const char * cond, const char * format, Args &&... args)
		{
			ExceptionHelper::Throw<AssertException>(fileLine, String::Format(format, std::forward<Args>(args)...), cond);
		}

		/**
		 *
		 * 断言异常
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param fileLine 文件信息
		 * @param cond 判断表达式
		 * @param format 格式化
		 * @param args 参数
		 *
		 */
		template <typename... Args>
		static void Assert(const FileLine & fileLine, const char * cond, const std::string & format, Args &&... args)
		{
			ExceptionHelper::Throw<AssertException>(fileLine, String::Format(format, std::forward<Args>(args)...), cond);
		}
	};
}


#define TINY_TOOLKIT_DEBUG(...)			tinyToolkit::TraceHelper::Debug(TINY_TOOLKIT_FILE_LINE, ##__VA_ARGS__);
#define TINY_TOOLKIT_FATAL(...)			tinyToolkit::TraceHelper::Fatal(TINY_TOOLKIT_FILE_LINE, ##__VA_ARGS__);


#ifdef DEBUG

#define TINY_TOOLKIT_ASSERT(cond, ...)	if (!(cond)) tinyToolkit::TraceHelper::Assert(TINY_TOOLKIT_FILE_LINE, #cond, ##__VA_ARGS__);

#else

#define TINY_TOOLKIT_ASSERT(cond, ...)	{ }

#endif


#endif // __TINY_TOOLKIT__DEBUG__TRACE__H__
