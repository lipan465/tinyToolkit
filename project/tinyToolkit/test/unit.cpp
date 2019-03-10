/**
 *
 *  作者: hm
 *
 *  说明: 单元测试
 *
 */


#include "unit.h"

#include "../utilities/time.h"
#include "../utilities/singleton.h"


namespace tinyToolkit
{
	/**
	 *
	 * 获取单例
	 *
	 * @return 单例对象引用
	 *
	 */
	UnitTest & UnitTest::Instance()
	{
		return Singleton<UnitTest>::Instance();
	}

	/**
	 *
	 * 启动
	 *
	 */
	int32_t UnitTest::Run()
	{
		String::Print
		(
			"[==========] Running {} test{} from {} test case{}.{}",
			_count,
			_count == 1 ? "" : "s",
			_testCaseList.size(),
			_testCaseList.size() == 1 ? "" : "s",
			TINY_TOOLKIT_EOL
		);

		String::Print
		(
			"[----------] Global test environment set-up.{}",
			TINY_TOOLKIT_EOL
		);

		for (auto &iter : _testEnvironmentList)
		{
			iter->SetUp();
		}

		auto last = Time::TimePoint();

		for (auto &iter : _testCaseList)
		{
			String::Print
			(
				"[----------] {} test{} from {}{}",
				iter->Count(),
				iter->Count() == 1 ? "" : "s",
				iter->Name(),
				TINY_TOOLKIT_EOL
			);

			iter->SetUp();
			iter->Run();
			iter->TearDown();

			_passedCount += iter->PassedCount();
			_failedCount += iter->FailedCount();

			String::Print
			(
				"[----------] {} test{} from {} ({} total){}{}",
				iter->Count(),
				iter->Count() == 1 ? "" : "s",
				iter->Name(),
				String::Format("{:%H:%M:%S}", std::chrono::microseconds(iter->Elapsed())),
				TINY_TOOLKIT_EOL,
				TINY_TOOLKIT_EOL
			);
		}

		auto elapsed = Time::Microseconds(Time::TimePoint() - last);

		for (auto &iter : _testEnvironmentList)
		{
			iter->TearDown();
		}

		String::Print
		(
			"[----------] Global test environment tear-down{}",
			TINY_TOOLKIT_EOL
		);

		String::Print
		(
			"[==========] {} test{} from {} test case{} ran. ({} total){}",
			_count,
			_count == 1 ? "" : "s",
			_testCaseList.size(),
			_testCaseList.size() == 1 ? "" : "s",
			String::Format("{:%H:%M:%S}", std::chrono::microseconds(elapsed)),
			TINY_TOOLKIT_EOL
		);

		if (_passedCount > 0)
		{
			String::Print
			(
				"[  PASSED  ] {} test{}.{}",
				_passedCount,
				_passedCount == 1 ? "" : "s",
				TINY_TOOLKIT_EOL
			);
		}

		if (_failedCount > 0)
		{
			String::Print
			(
				"[  FAILED  ] {} test{}, listed below:{}",
				_failedCount,
				_failedCount == 1 ? "" : "s",
				TINY_TOOLKIT_EOL
			);

			for (auto &casePtr : _testCaseList)
			{
				for (auto &iter : casePtr->FailedList())
				{
					String::Print
					(
						"[  FAILED  ] {}{}",
						iter,
						TINY_TOOLKIT_EOL
					);
				}
			}

			String::Print
			(
				"{} {} FAILED TEST{}{}",
				TINY_TOOLKIT_EOL,
				_failedCount,
				_failedCount == 1 ? "" : "S",
				TINY_TOOLKIT_EOL
			);
		}

		return _failedCount == 0 ? 0 : 1;
	}

	/**
	 *
	 * 任务个数
	 *
	 * @return 任务个数
	 *
	 */
	std::size_t UnitTest::Count() const
	{
		return _count;
	}

	/**
	 *
	 * 通过个数
	 *
	 * @return 通过个数
	 *
	 */
	std::size_t UnitTest::PassedCount() const
	{
		return _passedCount;
	}

	/**
	 *
	 * 失败个数
	 *
	 * @return 失败个数
	 *
	 */
	std::size_t UnitTest::FailedCount() const
	{
		return _failedCount;
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
	const std::shared_ptr<TestInfo> & UnitTest::AddTestInfo(const std::shared_ptr<TestInfo> & testInfo)
	{
		++_count;

		auto find = _indexList.find(testInfo->Prefix());

		if (find == _indexList.end())
		{
			_indexList.insert(std::make_pair(testInfo->Prefix(), _indexList.size()));

			_testCaseList.push_back(std::make_shared<TestCase>(testInfo->Prefix()));

			return _testCaseList.back()->AddTestInfo(testInfo);
		}
		else
		{
			return _testCaseList[find->second]->AddTestInfo(testInfo);
		}
	}

	/**
	 *
	 * 田间测试环境
	 *
	 * @param environment 测试环境
	 *
	 * @return 测试环境
	 *
	 */
	const std::shared_ptr<TestEnvironment> & UnitTest::AddEnvironment(const std::shared_ptr<TestEnvironment> & environment)
	{
		_testEnvironmentList.push_back(environment);

		return _testEnvironmentList.back();
	}
}
