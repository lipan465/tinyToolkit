#ifndef __TINY_TOOLKIT__TEST__UNIT__H__
#define __TINY_TOOLKIT__TEST__UNIT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 单元测试
 *
 */


#include "case.h"
#include "environment.h"

#include "../debug/timeWatcher.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API UnitTest
	{
	public:
		/**
		 *
		 * 获取单例
		 *
		 * @return 单例对象引用
		 *
		 */
		static UnitTest & Instance()
		{
			return tinyToolkit::Singleton<UnitTest>::Instance();
		}

		/**
		 *
		 * 启动
		 *
		 */
		int32_t Run()
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

			_watcher.Start();

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
					Time::FormatMicrosecondElapsedString(static_cast<time_t>(iter->Watcher().All())),
					TINY_TOOLKIT_EOL,
					TINY_TOOLKIT_EOL
				);
			}

			_watcher.Stop();

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
				Time::FormatMicrosecondElapsedString(static_cast<time_t>(_watcher.All())),
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
		std::size_t Count() const
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
		std::size_t PassedCount() const
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
		std::size_t FailedCount() const
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
		const std::shared_ptr<TestInfo> & AddTestInfo(const std::shared_ptr<TestInfo> & testInfo)
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
		const std::shared_ptr<TestEnvironment> & AddEnvironment(const std::shared_ptr<TestEnvironment> & environment)
		{
			_testEnvironmentList.push_back(environment);

			return _testEnvironmentList.back();
		}

	protected:
		TimeWatcher _watcher{ };

		std::size_t _count{ 0 };
		std::size_t _passedCount{ 0 };
		std::size_t _failedCount{ 0 };

		std::map<std::string, std::size_t> _indexList{ };

		std::vector<std::shared_ptr<TestCase>> _testCaseList{ };
		std::vector<std::shared_ptr<TestEnvironment>> _testEnvironmentList{ };
	};
}


/**
 *
 * 生成并注册测试信息
 *
 */
#define TINY_TOOLKIT_TEST_REGISTER(prefix, suffix, parent)																						\
																																				\
class TINY_TOOLKIT_API TINY_TOOLKIT_TEST_CLASS_NAME(prefix, suffix) : public parent																				\
{																																				\
public:																																			\
	TINY_TOOLKIT_TEST_CLASS_NAME(prefix, suffix)() : parent(#prefix, #suffix)																	\
	{																																			\
																																				\
	}																																			\
																																				\
	void TestLogic() override;																													\
																																				\
protected:																																		\
	static std::shared_ptr<tinyToolkit::TestInfo> _instance;																					\
};																																				\
																																				\
std::shared_ptr<tinyToolkit::TestInfo> TINY_TOOLKIT_TEST_CLASS_NAME(prefix, suffix)::_instance = tinyToolkit::UnitTest::Instance().AddTestInfo	\
(																																				\
	std::make_shared<TINY_TOOLKIT_TEST_CLASS_NAME(prefix, suffix)>()																			\
);																																				\
																																				\
void TINY_TOOLKIT_TEST_CLASS_NAME(prefix, suffix)::TestLogic()																					\


/**
 *
 * 拼接注册类名称
 *
 */
#define TINY_TOOLKIT_TEST_CLASS_NAME(prefix, suffix) \
	prefix##_##suffix##_Test


/**
 *
 * 运行所有测试用例
 *
 */
#define TINY_TOOLKIT_RUN_ALL_TESTS() \
	(tinyToolkit::UnitTest::Instance().Run())

/**
 *
 * 生成测试用例
 *
 */
#define TINY_TOOLKIT_TEST(prefix, suffix) \
	TINY_TOOLKIT_TEST_REGISTER(prefix, suffix, tinyToolkit::TestInfo)

/**
 *
 * 生成测试用例
 *
 */
#define TINY_TOOLKIT_TEST_F(prefix, suffix) \
	TINY_TOOLKIT_TEST_REGISTER(prefix, suffix, prefix)

/**
 *
 * 添加测试环境
 *
 */
#define TINY_TOOLKIT_ADD_TEST_ENVIRONMENT(environment) \
	tinyToolkit::UnitTest::Instance().AddEnvironment(std::make_shared<environment>())


/**
 *
 * 重定义
 *
 */
#ifndef TINY_TOOLKIT_DONT_DEFINE_TEST
#
#  define RUN_ALL_TEST()						TINY_TOOLKIT_RUN_ALL_TESTS()
#
#  define TEST(prefix, suffix)					TINY_TOOLKIT_TEST(prefix, suffix)
#  define TEST_F(prefix, suffix)				TINY_TOOLKIT_TEST_F(prefix, suffix)
#
#  define ADD_TEST_ENVIRONMENT(environment)		TINY_TOOLKIT_ADD_TEST_ENVIRONMENT(environment)
#
#endif


#endif // __TINY_TOOLKIT__TEST__UNIT__H__
