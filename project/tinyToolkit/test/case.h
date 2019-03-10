#ifndef __TINY_TOOLKIT_TEST__CASE__H__
#define __TINY_TOOLKIT_TEST__CASE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 测试用例
 *
 */


#include "info.h"


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
		explicit TestCase(const char * name);

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
		 * 名称
		 *
		 * @return 名称
		 *
		 */
		const char * Name() const;

		/**
		 *
		 * 任务个数
		 *
		 * @return 任务个数
		 *
		 */
		std::size_t Count() const;

		/**
		 *
		 * 通过个数
		 *
		 * @return 通过个数
		 *
		 */
		std::size_t PassedCount() const;

		/**
		 *
		 * 失败个数
		 *
		 * @return 失败个数
		 *
		 */
		std::size_t FailedCount() const;

		/**
		 *
		 * 观察间隔
		 *
		 * @return 观察间隔
		 *
		 */
		std::time_t Elapsed() const;

		/**
		 *
		 * 成功列表
		 *
		 * @return 成功列表
		 *
		 */
		const std::vector<std::string> & PassedList() const;

		/**
		 *
		 * 失败列表
		 *
		 * @return 失败列表
		 *
		 */
		const std::vector<std::string> & FailedList() const;

		/**
		 *
		 * 添加测试信息
		 *
		 * @param testInfo 测试信息
		 *
		 * @return 测试信息
		 *
		 */
		const std::shared_ptr<TestInfo> & AddTestInfo(const std::shared_ptr<TestInfo> & testInfo);

	protected:
		const char * _name{ nullptr };

		std::time_t _elapsed{ 0 };

		std::vector<std::string> _passedList{ };
		std::vector<std::string> _failedList{ };

		std::vector<std::shared_ptr<TestInfo>> _testInfoList{ };
	};
}


#endif // __TINY_TOOLKIT_TEST__CASE__H__
