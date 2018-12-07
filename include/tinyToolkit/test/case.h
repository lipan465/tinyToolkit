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
		 * 启动
		 *
		 */
		virtual void Run()
		{
			_watcher.Start();

			TestLogic();

			_watcher.Stop();
		}

		/**
		 *
		 * 测试逻辑
		 *
		 */
		virtual void TestLogic()
		{

		}

		/**
		 *
		 * 添加测试信息
		 *
		 * @param testInfo 测试信息
		 *
		 */
		void AddTestInfo(const std::shared_ptr<TestInfo> & testInfo)
		{
			_testInfoList.push_back(testInfo);
		}

		/**
		 *
		 * 观察结果
		 *
		 * @return 观察结果
		 *
		 */
		const TimeWatcher & Watcher() const
		{
			return _watcher;
		}

	protected:
		const char * _name{ nullptr };

		TimeWatcher _watcher{ };

		std::vector<std::shared_ptr<TestInfo>> _testInfoList{ };
	};
}


#endif // __TINY_TOOLKIT__TEST__CASE__H__
