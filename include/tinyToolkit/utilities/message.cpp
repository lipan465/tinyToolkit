/**
 *
 *  作者: hm
 *
 *  说明: 信息
 *
 */


#include "message.h"
#include "operator.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param rhs 实例化对象
	 *
	 */
	Message::Message(Message && rhs) noexcept
	{
		auto pos = rhs.String().size();

		_stream = std::move(rhs._stream);

		_stream.seekg(pos);
		_stream.seekp(pos);
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param lhs 实例化对象
	 *
	 */
	Message::Message(const Message & lhs)
	{
		_stream << lhs.String();
	}

	/**
	 *
	 * 清理
	 *
	 */
	void Message::Clear()
	{
		Operator::Clear(_stream);
	}

	/**
	 *
	 * 重载=操作
	 *
	 * @param rhs 实例化对象
	 *
	 */
	Message & Message::operator=(Message && rhs) noexcept
	{
		auto pos = rhs.String().size();

		_stream = std::move(rhs._stream);

		_stream.seekg(pos);
		_stream.seekp(pos);

		return *this;
	}

	/**
	 *
	 * 重载=操作
	 *
	 * @param lhs 实例化对象
	 *
	 */
	Message & Message::operator=(const Message & lhs)
	{
		if (this != &lhs)
		{
			Clear();

			_stream << lhs.String();
		}

		return *this;
	}

	/**
	 *
	 * 重载<<操作
	 *
	 * @param value 待写入数据
	 *
	 * @return 实例化对象
	 *
	 */
	Message & Message::operator <<(bool value)
	{
		return *this << (value ? "true" : "false");
	}

	/**
	 *
	 * 重载<<操作
	 *
	 * @param value 待写入数据
	 *
	 * @return 实例化对象
	 *
	 */
	Message & Message::operator <<(const Message & value)
	{
		return *this << value.String();
	}

	/**
	 *
	 * 重载<<操作
	 *
	 * @param value 待写入数据
	 *
	 * @return 实例化对象
	 *
	 */
	Message & Message::operator <<(const std::nullptr_t &)
	{
		return *this << "(null)";
	}

	/**
	 *
	 * 字符串
	 *
	 * @return 字符串
	 *
	 */
	std::string Message::String() const
	{
		return _stream.str();
	}

	/**
	 *
	 * 长度
	 *
	 * @return 长度
	 *
	 */
	std::size_t Message::Length() const
	{
		return String().length();
	}

	/**
	 *
	 * 重载<<操作
	 *
	 * @param os 输出流
	 * @param message 信息
	 *
	 * @return 输出流
	 *
	 */
	std::ostream & operator<<(std::ostream & os, Message const & message)
	{
		return os << message.String();
	}
}


