/**
 *
 *  作者: hm
 *
 *  说明: 文件信息
 *
 */


#include "fileLine.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param file 文件
	 * @param line 行号
	 * @param func 函数
	 *
	 */
	FileLine::FileLine(const char * file, int32_t line, const char * func) : _line(line), _file(file), _func(func)
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
	int32_t FileLine::Line() const
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
	const char * FileLine::File() const
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
	const char * FileLine::Function() const
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
	const std::string & FileLine::Message() const
	{
		return _message;
	}
}


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
