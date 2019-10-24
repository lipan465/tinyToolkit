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
		static UnitTest & Instance();

		/**
		 *
		 * 启动
		 *
		 */
		int32_t Run();

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
		 * 添加测试信息
		 *
		 * @param testInfo 测试信息
		 *
		 * @return 测试信息
		 *
		 */
		const std::shared_ptr<TestInfo> & AddTestInfo(const std::shared_ptr<TestInfo> & testInfo);

		/**
		 *
		 * 添加测试环境
		 *
		 * @param environment 测试环境
		 *
		 * @return 测试环境
		 *
		 */
		const std::shared_ptr<TestEnvironment> & AddEnvironment(const std::shared_ptr<TestEnvironment> & environment);

	protected:
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
class TINY_TOOLKIT_API TINY_TOOLKIT_TEST_CLASS_NAME(prefix, suffix) : public parent																\
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
