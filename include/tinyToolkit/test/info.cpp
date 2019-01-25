/**
 *
 *  作者: hm
 *
 *  说明: 测试信息
 *
 */


#include "info.h"

#include "../utilities/time.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param prefix 前缀
	 * @param suffix 后缀
	 *
	 */
	TestInfo::TestInfo(const char * prefix, const char * suffix) : _prefix(prefix), _suffix(suffix)
	{
		_name += prefix;
		_name += ".";
		_name += suffix;
	}

	/**
	 *
	 * 启动
	 *
	 */
	void TestInfo::Run()
	{
		auto last = Time::TimePoint();

		TestLogic();

		_elapsed = Time::Microseconds(Time::TimePoint() - last);
	}

	/**
	 *
	 * 设置环境
	 *
	 */
	void TestInfo::SetUp()
	{

	}

	/**
	 *
	 * 销毁环境
	 *
	 */
	void TestInfo::TearDown()
	{

	}

	/**
	 *
	 * 测试逻辑
	 *
	 */
	void TestInfo::TestLogic()
	{

	}

	/**
	 *
	 * 前缀
	 *
	 * @return 测试信息前缀
	 *
	 */
	const char * TestInfo::Prefix() const
	{
		return _prefix;
	}

	/**
	 *
	 * 后缀
	 *
	 * @return 测试信息后缀
	 *
	 */
	const char * TestInfo::Suffix() const
	{
		return _suffix;
	}

	/**
	 *
	 * 名称
	 *
	 * @return 测试信息名称
	 *
	 */
	const std::string & TestInfo::Name() const
	{
		return _name;
	}

	/**
	 *
	 * 测试结果
	 *
	 * @return 测试结果
	 *
	 */
	TestResult & TestInfo::Result()
	{
		return _result;
	}

	/**
	 *
	 * 观察间隔
	 *
	 * @return 观察间隔
	 *
	 */
	std::time_t TestInfo::Elapsed() const
	{
		return _elapsed;
	}
}
