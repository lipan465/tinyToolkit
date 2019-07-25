/**
 *
 *  作者: hm
 *
 *  说明: 异常处理
 *
 */


#include "exception.h"
#include "stackTrace.h"

#include "../utilities/string.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param fileLine 文件信息
	 * @param message 信息
	 * @param option 操作
	 *
	 */
	IException::IException(const FileLine & fileLine, std::string message, std::string option) : _option(std::move(option)),
																								 _message(std::move(message))
	{
		if (_message.empty())
		{
			_what += fileLine.Message();
			_what += TINY_TOOLKIT_EOL;
		}
		else
		{
			_what += String::Format("[{}] {}", fileLine.Message(), _message);
			_what += TINY_TOOLKIT_EOL;
		}

		if (!_option.empty())
		{
			_what += TINY_TOOLKIT_EOL;
			_what += _option;
			_what += TINY_TOOLKIT_EOL;
		}
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
	 * 操作
	 *
	 * @return 操作
	 *
	 */
	const std::string & IException::Option() const
	{
		return _option;
	}

	/**
	 *
	 * 信息
	 *
	 * @return 信息
	 *
	 */
	const std::string & IException::Message() const
	{
		return _message;
	}
}
