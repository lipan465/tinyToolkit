#ifndef __TINY_TOOLKIT__CONTAINER__MESSAGE__H__
#define __TINY_TOOLKIT__CONTAINER__MESSAGE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 信息处理
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Message
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		Message()
		{
			_stream.precision(std::numeric_limits<double>::digits10 + 2);
		}

		/**
		 *
		 * 构造函数
		 *
		 * @param rhs 实例化对象
		 *
		 */
		Message(Message && rhs) noexcept : _stream(std::move(rhs._stream))
		{

		}

		/**
		 *
		 * 构造函数
		 *
		 * @param lhs 实例化对象
		 *
		 */
		Message(const Message & lhs) : _stream(lhs.String())
		{

		}

		/**
		 *
		 * 清理
		 *
		 */
		void Clear()
		{
			ContainerOperator::Clear(_stream);
		}

		/**
		 *
		 * 重载=操作
		 *
		 * @param rhs 实例化对象
		 *
		 */
		Message & operator=(Message && rhs) noexcept
		{
			_stream = std::move(rhs._stream);

			return *this;
		}

		/**
		 *
		 * 重载=操作
		 *
		 * @param lhs 实例化对象
		 *
		 */
		Message & operator=(const Message & lhs)
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
		Message & operator <<(bool value)
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
		Message & operator <<(const Message & value)
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
		Message & operator <<(const std::nullptr_t & value)
		{
			return *this << "(null)";
		}

		/**
		 *
		 * 重载<<操作
		 *
		 * @tparam ValueTypeT [all built-in types]
		 *
		 * @param value 待写入数据
		 *
		 * @return 实例化对象
		 *
		 */
		template <typename ValueTypeT>
		Message & operator <<(const ValueTypeT & value)
		{
			_stream << value;

			return *this;
		}

		/**
		 *
		 * 重载<<操作
		 *
		 * @tparam ValueTypeT [all built-in types]
		 *
		 * @param value 待写入数据
		 *
		 * @return 实例化对象
		 *
		 */
		template <typename ValueTypeT>
		Message & operator <<(const ValueTypeT * value)
		{
			if (value == nullptr)
			{
				_stream << "(null)";
			}
			else
			{
				_stream << value;
			}

			return *this;
		}

		/**
		 *
		 * 字符串
		 *
		 * @return 字符串
		 *
		 */
		std::string String() const
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
		std::size_t Length() const
		{
			return String().length();
		}

	protected:
		std::stringstream _stream{ };
	};

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
	std::ostream & operator<<(std::ostream & os, const Message & message)
	{
		return os << message.String();
	}
}


#endif // __TINY_TOOLKIT__CONTAINER__MESSAGE__H__
