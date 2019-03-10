#ifndef __TINY_TOOLKIT_TEST__INFO__H__
#define __TINY_TOOLKIT_TEST__INFO__H__


/**
 *
 *  作者: hm
 *
 *  说明: 测试信息
 *
 */


#include "result.h"


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
		TestInfo(const char * prefix, const char * suffix);

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
		void Run();

		/**
		 *
		 * 设置环境
		 *
		 */
		virtual void SetUp();

		/**
		 *
		 * 销毁环境
		 *
		 */
		virtual void TearDown();

		/**
		 *
		 * 测试逻辑
		 *
		 */
		virtual void TestLogic();

		/**
		 *
		 * 前缀
		 *
		 * @return 测试信息前缀
		 *
		 */
		const char * Prefix() const;

		/**
		 *
		 * 后缀
		 *
		 * @return 测试信息后缀
		 *
		 */
		const char * Suffix() const;

		/**
		 *
		 * 名称
		 *
		 * @return 测试信息名称
		 *
		 */
		const std::string & Name() const;

		/**
		 *
		 * 测试结果
		 *
		 * @return 测试结果
		 *
		 */
		TestResult & Result();

		/**
		 *
		 * 观察间隔
		 *
		 * @return 观察间隔
		 *
		 */
		std::time_t Elapsed() const;

	private:
		TestResult _result{ true };

		std::string _name{ };

		std::time_t _elapsed{ 0 };

		const char * _prefix{ nullptr };
		const char * _suffix{ nullptr };
	};
}


#endif // __TINY_TOOLKIT_TEST__INFO__H__
