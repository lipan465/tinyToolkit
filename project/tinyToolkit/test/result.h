#ifndef __TINY_TOOLKIT_TEST__RESULT__H__
#define __TINY_TOOLKIT_TEST__RESULT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 解析结果
 *
 */


#include "../utilities/string.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API TestResult
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param status 状态
		 *
		 */
		explicit TestResult(bool status);

		/**
		 *
		 * 构造函数
		 *
		 * @param rhs 实例化对象
		 *
		 */
		TestResult(TestResult && rhs) noexcept;

		/**
		 *
		 * 构造函数
		 *
		 * @param lhs 实例化对象
		 *
		 */
		TestResult(const TestResult & lhs) = default;

		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~TestResult() = default;

		/**
		 *
		 * 重载()操作
		 *
		 * @return 结果
		 *
		 */
		explicit operator bool() const;

		/**
		 *
		 * 状态
		 *
		 * @return 状态
		 *
		 */
		bool Status() const;

		/**
		 *
		 * 信息
		 *
		 * @return 信息
		 *
		 */
		const std::string & String();

		/**
		 *
		 * 信息
		 *
		 * @return 信息
		 *
		 */
		const std::string & String() const;

		/**
		 *
		 * 重载<<操作
		 *
		 * @tparam TypeT [all built-in types]
		 *
		 * @param value 数据
		 *
		 * @return 实例化对象
		 *
		 */
		template <typename TypeT>
		TestResult & operator<<(TypeT && value)
		{
			_message += String::Join(std::forward<TypeT>(value));

			return *this;
		}

		/**
		 *
		 * 重载=操作
		 *
		 * @param rhs 实例化对象
		 *
		 */
		TestResult & operator=(TestResult && rhs) noexcept;

		/**
		 *
		 * 重载=操作
		 *
		 * @param lhs 实例化对象
		 *
		 */
		TestResult & operator=(const TestResult & lhs);

	protected:
		bool _status{ true };

		std::string _message{};
	};

	class TINY_TOOLKIT_API TestSuccessResult : public TestResult
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		TestSuccessResult() : TestResult(true)
		{

		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~TestSuccessResult() override = default;
	};

	class TINY_TOOLKIT_API TestFailureResult : public TestResult
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		TestFailureResult() : TestResult(false)
		{

		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~TestFailureResult() override = default;
	};

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
	std::ostream & operator<<(std::ostream & os, const TestResult & result);
}


#endif // __TINY_TOOLKIT_TEST__RESULT__H__
