#ifndef __TINY_TOOLKIT__TEST__CASE__H__
#define __TINY_TOOLKIT__TEST__CASE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 测试用例
 *
 */


#include "info.h"

#include "../debug/timeWatcher.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API TestCase
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param name 名称
		 *
		 */
		explicit TestCase(const char * name) : _name(name)
		{

		}

		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~TestCase() = default;

		/**
		 *
		 * 启动
		 *
		 */
		void Run()
		{
			_watcher.Start();

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
						Time::FormatMicrosecondElapsedString(static_cast<time_t>(iter->Watcher().All())),
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
						Time::FormatMicrosecondElapsedString(static_cast<time_t>(iter->Watcher().All())),
						TINY_TOOLKIT_EOL
					);

					_failedList.push_back(iter->Name());
				}
			}

			_watcher.Stop();
		}

		/**
		 *
		 * 设置环境
		 *
		 */
		virtual void SetUp()
		{

		}

		/**
		 *
		 * 销毁环境
		 *
		 */
		virtual void TearDown()
		{

		}

		/**
		 *
		 * 名称
		 *
		 * @return 名称
		 *
		 */
		const char * Name() const
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
		std::size_t Count()
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
		std::size_t PassedCount() const
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
		std::size_t FailedCount() const
		{
			return _failedList.size();
		}

		/**
		 *
		 * 观察用时
		 *
		 * @return 观察用时
		 *
		 */
		const TimeWatcher & Watcher() const
		{
			return _watcher;
		}

		/**
		 *
		 * 成功列表
		 *
		 * @return 成功列表
		 *
		 */
		const std::vector<std::string> & PassedList() const
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
		const std::vector<std::string> & FailedList() const
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
		const std::shared_ptr<TestInfo> & AddTestInfo(const std::shared_ptr<TestInfo> & testInfo)
		{
			_testInfoList.push_back(testInfo);

			return _testInfoList.back();
		}

	protected:
		const char * _name{ nullptr };

		TimeWatcher _watcher{ };

		std::vector<std::string> _passedList{ };
		std::vector<std::string> _failedList{ };

		std::vector<std::shared_ptr<TestInfo>> _testInfoList{ };
	};
}


#endif // __TINY_TOOLKIT__TEST__CASE__H__
