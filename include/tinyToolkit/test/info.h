#ifndef __TINY_TOOLKIT__TEST__INFO__H__
#define __TINY_TOOLKIT__TEST__INFO__H__


/**
 *
 *  作者: hm
 *
 *  说明: 测试信息
 *
 */


#include "result.h"

#include "../debug/timeWatcher.h"
#include "../utilities/fileLine.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API TestInfo
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param prefix 前缀
		 * @param suffix 后缀
		 *
		 */
		TestInfo(const char * prefix, const char * suffix) : _prefix(prefix), _suffix(suffix)
		{
			_name += prefix;
			_name += ".";
			_name += suffix;
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~TestInfo() = default;

		/**
		 *
		 * 启动
		 *
		 */
		void Run()
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
		 * 前缀
		 *
		 * @return 测试信息前缀
		 *
		 */
		const char * Prefix() const
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
		const char * Suffix() const
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
		const std::string & Name() const
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
		TestResult & Result()
		{
			return _result;
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

	private:
		TestResult _result{ true };

		TimeWatcher _watcher{ };

		std::string _name{ };

		const char * _prefix{ nullptr };
		const char * _suffix{ nullptr };
	};
}


#endif // __TINY_TOOLKIT__TEST__INFO__H__
