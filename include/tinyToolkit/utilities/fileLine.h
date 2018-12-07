#ifndef __TINY_TOOLKIT__UTILITIES__FILE_LINE__H__
#define __TINY_TOOLKIT__UTILITIES__FILE_LINE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 文件信息
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API FileLine
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param file 文件
		 * @param line 行号
		 * @param func 函数
		 *
		 */
		FileLine(const char * file, int32_t line, const char * func) : _line(line), _file(file), _func(func)
		{
			_message += _file;
			_message += ":";
			_message += std::to_string(_line);
			_message += " in ";
			_message += _func;
		}

		/**
		 *
		 * 行号
		 *
		 * @return 行号
		 *
		 */
		int32_t Line() const
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
		const std::string & File() const
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
		const std::string & Function() const
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
		const std::string & Message() const
		{
			return _message;
		}

	protected:
		int32_t _line;

		std::string _file;
		std::string _func;
		std::string _message{ };
	};

	/**
	 *
	 * 重载<<操作
	 *
	 * @param os 输出流
	 * @param info 信息
	 *
	 * @return 输出流
	 *
	 */
	std::ostream & operator << (std::ostream & os, tinyToolkit::FileLine const & info)
	{
		os << info.Message();

		return os;
	}
}


#define TINY_TOOLKIT_FILE_LINE	tinyToolkit::FileLine(TINY_TOOLKIT_FILE, TINY_TOOLKIT_LINE, TINY_TOOLKIT_FUNC)


#endif // __TINY_TOOLKIT__UTILITIES__FILE_LINE__H__
