#ifndef __TINY_TOOLKIT__TEST__POOL__H__
#define __TINY_TOOLKIT__TEST__POOL__H__


/**
 *
 *  作者: hm
 *
 *  说明: 测试池
 *
 */


#include "info.h"

#include "../debug/timeWatcher.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API TestPool
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param name 名称
		 *
		 */
		explicit TestPool(const char * name) : _name(name)
		{

		}

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
				iter->Run();
			}

			_watcher.Stop();
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
		 * 通过个税
		 *
		 * @return 通过个税
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

	protected:
		TimeWatcher _watcher{ };

		std::size_t _passedCount{ 0 };
		std::size_t _failedCount{ 0 };

		const char * _name{ nullptr };

		std::vector<std::shared_ptr<TestInfo>> _testInfoList{ };
	};
}


#endif // __TINY_TOOLKIT__TEST__POOL__H__
