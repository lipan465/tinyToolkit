/**
 *
 *  作者: hm
 *
 *  说明: 测试用例
 *
 */


#include "case.h"

#include "../utilities/time.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param name 名称
	 *
	 */
	TestCase::TestCase(const char * name) : _name(name)
	{

	}

	/**
	 *
	 * 启动
	 *
	 */
	void TestCase::Run()
	{
		auto last = Time::TimePoint();

		for (auto &iter : _testInfoList)
		{
			String::Print
			(
				"[ RUN      ] {}{}",
				iter->Name(),
				TINY_TOOLKIT_EOL
			);

			iter->SetUp();
			iter->Run();
			iter->TearDown();

			if (iter->Result())
			{
				String::Print
				(
					"[       OK ] {} ({}){}",
					iter->Name(),
					String::Format("{:%H:%M:%S}", std::chrono::microseconds(iter->Elapsed())),
					TINY_TOOLKIT_EOL
				);

				_passedList.push_back(iter->Name());
			}
			else
			{
				String::Print
				(
					"[  FAILED  ] {} ({}){}",
					iter->Name(),
					String::Format("{:%H:%M:%S}", std::chrono::microseconds(iter->Elapsed())),
					TINY_TOOLKIT_EOL
				);

				_failedList.push_back(iter->Name());
			}
		}

		_elapsed = Time::Microseconds(Time::TimePoint() - last);
	}

	/**
	 *
	 * 设置环境
	 *
	 */
	void TestCase::SetUp()
	{

	}

	/**
	 *
	 * 销毁环境
	 *
	 */
	void TestCase::TearDown()
	{

	}

	/**
	 *
	 * 名称
	 *
	 * @return 名称
	 *
	 */
	const char * TestCase::Name() const
	{
		return _name;
	}

	/**
	 *
	 * 任务个数
	 *
	 * @return 任务个数
	 *
	 */
	std::size_t TestCase::Count() const
	{
		return _testInfoList.size();
	}

	/**
	 *
	 * 通过个数
	 *
	 * @return 通过个数
	 *
	 */
	std::size_t TestCase::PassedCount() const
	{
		return _passedList.size();
	}

	/**
	 *
	 * 失败个数
	 *
	 * @return 失败个数
	 *
	 */
	std::size_t TestCase::FailedCount() const
	{
		return _failedList.size();
	}

	/**
	 *
	 * 观察间隔
	 *
	 * @return 观察间隔
	 *
	 */
	std::time_t TestCase::Elapsed() const
	{
		return _elapsed;
	}

	/**
	 *
	 * 成功列表
	 *
	 * @return 成功列表
	 *
	 */
	const std::vector<std::string> & TestCase::PassedList() const
	{
		return _passedList;
	}

	/**
	 *
	 * 失败列表
	 *
	 * @return 失败列表
	 *
	 */
	const std::vector<std::string> & TestCase::FailedList() const
	{
		return _failedList;
	}

	/**
	 *
	 * 添加测试信息
	 *
	 * @param testInfo 测试信息
	 *
	 * @return 测试信息
	 *
	 */
	const std::shared_ptr<TestInfo> & TestCase::AddTestInfo(const std::shared_ptr<TestInfo> & testInfo)
	{
		_testInfoList.push_back(testInfo);

		return _testInfoList.back();
	}
}
