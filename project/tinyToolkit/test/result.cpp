/**
 *
 *  作者: hm
 *
 *  说明: 解析结果
 *
 */


#include "result.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param status 状态
	 *
	 */
	TestResult::TestResult(bool status) : _status(status)
	{

	}

	/**
	 *
	 * 构造函数
	 *
	 * @param rhs 实例化对象
	 *
	 */
	TestResult::TestResult(TestResult && rhs) noexcept : _status(rhs._status),
														 _message(std::move(rhs._message))
	{

	}

	/**
	 *
	 * 重载()操作
	 *
	 * @return 结果
	 *
	 */
	TestResult::operator bool() const
	{
		return _status;
	}

	/**
	 *
	 * 状态
	 *
	 * @return 状态
	 *
	 */
	bool TestResult::Status() const
	{
		return _status;
	}

	/**
	 *
	 * 信息
	 *
	 * @return 信息
	 *
	 */
	const std::string & TestResult::String()
	{
		return _message;
	}

	/**
	 *
	 * 信息
	 *
	 * @return 信息
	 *
	 */
	const std::string & TestResult::String() const
	{
		return _message;
	}

	/**
	 *
	 * 重载=操作
	 *
	 * @param rhs 实例化对象
	 *
	 */
	TestResult & TestResult::operator=(TestResult && rhs) noexcept
	{
		_status = rhs._status;
		_message = std::move(rhs._message);

		return *this;
	}

	/**
	 *
	 * 重载=操作
	 *
	 * @param lhs 实例化对象
	 *
	 */
	TestResult & TestResult::operator=(const TestResult & lhs)
	{
		if (this != &lhs)
		{
			_status = lhs._status;
			_message = lhs._message;

			if (!Status())
			{
				std::cout << _message << std::endl;
			}
		}

		return *this;
	}

	/**
	 *
	 * 重载<<操作
	 *
	 * @param os 输出流
	 * @param result 测试结果
	 *
	 * @return 输出流
	 *
	 */
	std::ostream & operator<<(std::ostream & os, const TestResult & result)
	{
		return os << result.String();
	}
}
