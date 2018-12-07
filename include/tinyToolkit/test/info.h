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
		 * @param fileLine 文件信息
		 *
		 */
		TestInfo(const char * prefix, const char * suffix, FileLine fileLine) : _fileLine(std::move(fileLine)),
																				_prefix(prefix),
																				_suffix(suffix)
		{

		}

		/**
		 *
		 * 启动
		 *
		 */
		void Run()
		{

		}

		/**
		 *
		 * 刷新
		 *
		 * @param result 结果
		 * @param message 信息
		 *
		 */
		void Flush(bool result, const Message & message)
		{
			if (result)
			{
				_result = TestSuccessResult();
			}
			else
			{
				_result = TestFailureResult() << message;
			}
		}

		/**
		 *
		 * 前缀
		 *
		 * @return 测试用例前缀
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
		 * @return 测试用例后缀
		 *
		 */
		const char * Suffix() const
		{
			return _suffix;
		}

		/**
		 *
		 * 文件信息
		 *
		 * @return 文件信息
		 *
		 */
		const FileLine & FileInfo() const
		{
			return _fileLine;
		}

		/**
		 *
		 * 测试结果
		 *
		 * @return 测试结果
		 *
		 */
		const TestResult & Result() const
		{
			return _result;
		}

	protected:
		FileLine _fileLine;

		TestResult _result{ true };

		const char * _prefix{ nullptr };
		const char * _suffix{ nullptr };
	};
}


#define TINY_TOOLKIT_TEST_INFO(prefix, suffix)	tinyToolkit::TestInfo(prefix, suffix, TINY_TOOLKIT_FILE_LINE)


#endif // __TINY_TOOLKIT__TEST__INFO__H__
