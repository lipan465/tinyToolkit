/**
 *
 *  作者: hm
 *
 *  说明: 异常处理
 *
 */


#include "exception.h"


namespace tinyToolkit
{
	namespace debug
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param file 文件
		 * @param line 行号
		 * @param func 函数
		 * @param info 信息
		 *
		 */
		IException::IException(const char * file, int32_t line, const char * func, std::string info) : _line(line),
		                                                                                               _info(std::move(info)),
		                                                                                               _file(file),
		                                                                                               _func(func)
		{
			_what += "[";
			_what += _file;
			_what += ":";
			_what += std::to_string(_line);
			_what += " in ";
			_what += _func;
			_what += "] ";
			_what += _info;
			_what += TINY_TOOLKIT_EOL;
		}

		/**
		 *
		 * 异常信息
		 *
		 * @return 异常信息
		 *
		 */
		const char * IException::what() const noexcept
		{
			return _what.c_str();
		}

		/**
		 *
		 * 行号
		 *
		 * @return 行号
		 *
		 */
		int32_t IException::Line() const
		{
			return _line;
		}

		/**
		 *
		 * 文件
		 *
		 * @return 文件
		 *
		 */
		const char * IException::File() const
		{
			return _file;
		}

		/**
		 *
		 * 函数
		 *
		 * @return 函数
		 *
		 */
		const char * IException::Func() const
		{
			return _func;
		}

		/**
		 *
		 * 信息
		 *
		 * @return 信息
		 *
		 */
		const std::string & IException::Info() const
		{
			return _info;
		}
	}
}
