#ifndef __TINY_TOOLKIT__TEST__COMPARE__H__
#define __TINY_TOOLKIT__TEST__COMPARE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 比较操作
 *
 */


#include "result.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API TestCompare
	{
	public:
		/**
		 *
		 * 正确
		 *
		 * @param fileLine 文件信息
		 * @param expression 表达式
		 * @param condition 条件
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult True(const FileLine & fileLine, const char * expression, bool condition)
		{
			if (condition)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "Value of: " << expression << TINY_TOOLKIT_EOL
											<< "  Actual: " << "false" << TINY_TOOLKIT_EOL
											<< "Expected: " << "true" << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 错误
		 *
		 * @param fileLine 文件信息
		 * @param expression 表达式
		 * @param condition 条件
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult False(const FileLine & fileLine, const char * expression, bool condition)
		{
			if (!condition)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "Value of: " << expression << TINY_TOOLKIT_EOL
											<< "  Actual: " << "true" << TINY_TOOLKIT_EOL
											<< "Expected: " << "false" << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 等于
		 *
		 * @tparam ExpectedValueTypeT [all built-in types]
		 * @tparam ActualValueTypeT [all built-in types]
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		template <typename ExpectedValueTypeT, typename ActualValueTypeT>
		static TestResult EqualTo(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const ExpectedValueTypeT & expected, const ActualValueTypeT & actual)
		{
			if (expected == actual)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " == " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 不等于
		 *
		 * @tparam ExpectedValueTypeT [all built-in types]
		 * @tparam ActualValueTypeT [all built-in types]
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		template <typename ExpectedValueTypeT, typename ActualValueTypeT>
		static TestResult NotEqualTo(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const ExpectedValueTypeT & expected, const ActualValueTypeT & actual)
		{
			if (expected != actual)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " != " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 小于
		 *
		 * @tparam ExpectedValueTypeT [all built-in types]
		 * @tparam ActualValueTypeT [all built-in types]
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		template <typename ExpectedValueTypeT, typename ActualValueTypeT>
		static TestResult LessThan(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const ExpectedValueTypeT & expected, const ActualValueTypeT & actual)
		{
			if (expected < actual)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " <  " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 大于
		 *
		 * @tparam ExpectedValueTypeT [all built-in types]
		 * @tparam ActualValueTypeT [all built-in types]
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		template <typename ExpectedValueTypeT, typename ActualValueTypeT>
		static TestResult GreaterThan(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const ExpectedValueTypeT & expected, const ActualValueTypeT & actual)
		{
			if (expected > actual)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " >  " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 小于等于
		 *
		 * @tparam ExpectedValueTypeT [all built-in types]
		 * @tparam ActualValueTypeT [all built-in types]
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		template <typename ExpectedValueTypeT, typename ActualValueTypeT>
		static TestResult LessThanOrEqualTo(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const ExpectedValueTypeT & expected, const ActualValueTypeT & actual)
		{
			if (expected <= actual)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " <= " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 大于等于
		 *
		 * @tparam ExpectedValueTypeT [all built-in types]
		 * @tparam ActualValueTypeT [all built-in types]
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		template <typename ExpectedValueTypeT, typename ActualValueTypeT>
		static TestResult GreaterThanOrEqualTo(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const ExpectedValueTypeT & expected, const ActualValueTypeT & actual)
		{
			if (expected >= actual)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " >= " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串等于
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrEqualTo(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcmp(expected, actual) == 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " == " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串不等于
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrNotEqualTo(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcmp(expected, actual) != 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " != " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串小于
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrLessThan(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcmp(expected, actual) < 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " <  " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串大于
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrGreaterThan(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcmp(expected, actual) > 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " >  " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串小于等于
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrLessThanOrEqualTo(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcmp(expected, actual) <= 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " <= " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串大于等于
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrGreaterThanOrEqualTo(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcmp(expected, actual) >= 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " >= " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串等于
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrCaseEqualTo(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcasecmp(expected, actual) == 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " == " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串不等于
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrCaseNotEqualTo(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcasecmp(expected, actual) != 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " != " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串小于
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrCaseLessThan(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcasecmp(expected, actual) < 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " <  " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串大于
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrCaseGreaterThan(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcasecmp(expected, actual) > 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " >  " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串小于等于
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrCaseLessThanOrEqualTo(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcasecmp(expected, actual) <= 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " <= " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串大于等于
		 *
		 * @param fileLine 文件信息
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrCaseGreaterThanOrEqualTo(const FileLine & fileLine, const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcasecmp(expected, actual) >= 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << fileLine.File() << ":" << fileLine.Line() << ": Failure" << TINY_TOOLKIT_EOL
											<< "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " >= " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}
	};
}


/**
 *
 * 测试期望逻辑
 *
 */
#define TINY_TOOLKIT_EXPECT_LOGIC(expression)			\
														\
if (tinyToolkit::TestResult __result__ = expression)	\
	;													\
else													\
	Result() = __result__								\

/**
 *
 * 布尔测试期望逻辑
 *
 */
#define TINY_TOOLKIT_EXPECT_BOOL_LOGIC(option, condition) \
	TINY_TOOLKIT_EXPECT_LOGIC(option(TINY_TOOLKIT_FILE_LINE, #condition, condition))

/**
 *
 * 比较测试期望逻辑
 *
 */
#define TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(option, expected, actual) \
	TINY_TOOLKIT_EXPECT_LOGIC(option(TINY_TOOLKIT_FILE_LINE, #expected, #actual, expected, actual))

/**
 *
 * 测试期望正确
 *
 */
#define TINY_TOOLKIT_EXPECT_TRUE(condition) \
	TINY_TOOLKIT_EXPECT_BOOL_LOGIC(tinyToolkit::TestCompare::True, condition)

/**
 *
 * 测试期望错误
 *
 */
#define TINY_TOOLKIT_EXPECT_FALSE(condition) \
	TINY_TOOLKIT_EXPECT_BOOL_LOGIC(tinyToolkit::TestCompare::False, condition)

/**
 *
 * 测试期望等于
 *
 */
#define TINY_TOOLKIT_EXPECT_EQ(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::EqualTo, expected, actual)

/**
 *
 * 测试期望不等于
 *
 */
#define TINY_TOOLKIT_EXPECT_NE(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::NotEqualTo, expected, actual)

/**
 *
 * 测试期望小于
 *
 */
#define TINY_TOOLKIT_EXPECT_LT(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::LessThan, expected, actual)

/**
 *
 * 测试期望小于且等于
 *
 */
#define TINY_TOOLKIT_EXPECT_LE(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::LessThanOrEqualTo, expected, actual)

/**
 *
 * 测试期望大于
 *
 */
#define TINY_TOOLKIT_EXPECT_GT(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::GreaterThan, expected, actual)

/**
 *
 * 测试期望大于且等于
 *
 */
#define TINY_TOOLKIT_EXPECT_GE(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::GreaterThanOrEqualTo, expected, actual)

/**
 *
 * 测试期望字符串等于
 *
 */
#define TINY_TOOLKIT_EXPECT_STR_EQ(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::StrEqualTo, expected, actual)

/**
 *
 * 测试期望字符串不等于
 *
 */
#define TINY_TOOLKIT_EXPECT_STR_NE(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::StrNotEqualTo, expected, actual)

/**
 *
 * 测试期望字符串小于
 *
 */
#define TINY_TOOLKIT_EXPECT_STR_LT(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::StrLessThan, expected, actual)

/**
 *
 * 测试期望字符串小于且等于
 *
 */
#define TINY_TOOLKIT_EXPECT_STR_LE(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::StrLessThanOrEqualTo, expected, actual)

/**
 *
 * 测试期望字符串大于
 *
 */
#define TINY_TOOLKIT_EXPECT_STR_GT(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::StrGreaterThan, expected, actual)

/**
 *
 * 测试期望字符串大于且等于
 *
 */
#define TINY_TOOLKIT_EXPECT_STR_GE(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::StrGreaterThanOrEqualTo, expected, actual)

/**
 *
 * 测试期望字符串等于(不区分大小写)
 *
 */
#define TINY_TOOLKIT_EXPECT_STR_CASE_EQ(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::StrCaseEqualTo, expected, actual)

/**
 *
 * 测试期望字符串不等于(不区分大小写)
 *
 */
#define TINY_TOOLKIT_EXPECT_STR_CASE_NE(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::StrCaseNotEqualTo, expected, actual)

/**
 *
 * 测试期望字符串小于(不区分大小写)
 *
 */
#define TINY_TOOLKIT_EXPECT_STR_CASE_LT(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::StrCaseLessThan, expected, actual)

/**
 *
 * 测试期望字符串小于且等于(不区分大小写)
 *
 */
#define TINY_TOOLKIT_EXPECT_STR_CASE_LE(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::StrCaseLessThanOrEqualTo, expected, actual)

/**
 *
 * 测试期望字符串大于(不区分大小写)
 *
 */
#define TINY_TOOLKIT_EXPECT_STR_CASE_GT(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::StrCaseGreaterThan, expected, actual)

/**
 *
 * 测试期望字符串大于且等于(不区分大小写)
 *
 */
#define TINY_TOOLKIT_EXPECT_STR_CASE_GE(expected, actual) \
	TINY_TOOLKIT_EXPECT_COMPARE_LOGIC(tinyToolkit::TestCompare::StrCaseGreaterThanOrEqualTo, expected, actual)


/**
*
* 重定义
*
*/
#ifndef TINY_TOOLKIT_DONT_DEFINE_TEST
#
#  define EXPECT_TRUE(condition) TINY_TOOLKIT_EXPECT_TRUE(condition)
#  define EXPECT_FALSE(condition) TINY_TOOLKIT_EXPECT_FALSE(condition)
#
#  define EXPECT_EQ(prefix, suffix) TINY_TOOLKIT_EXPECT_EQ(prefix, suffix)
#  define EXPECT_NE(prefix, suffix) TINY_TOOLKIT_EXPECT_NE(prefix, suffix)
#  define EXPECT_LT(prefix, suffix) TINY_TOOLKIT_EXPECT_LT(prefix, suffix)
#  define EXPECT_LE(prefix, suffix) TINY_TOOLKIT_EXPECT_LE(prefix, suffix)
#  define EXPECT_GT(prefix, suffix) TINY_TOOLKIT_EXPECT_GT(prefix, suffix)
#  define EXPECT_GE(prefix, suffix) TINY_TOOLKIT_EXPECT_GE(prefix, suffix)
#
#  define EXPECT_STR_EQ(prefix, suffix) TINY_TOOLKIT_EXPECT_STR_EQ(prefix, suffix)
#  define EXPECT_STR_NE(prefix, suffix) TINY_TOOLKIT_EXPECT_STR_NE(prefix, suffix)
#  define EXPECT_STR_LT(prefix, suffix) TINY_TOOLKIT_EXPECT_STR_LT(prefix, suffix)
#  define EXPECT_STR_LE(prefix, suffix) TINY_TOOLKIT_EXPECT_STR_LE(prefix, suffix)
#  define EXPECT_STR_GT(prefix, suffix) TINY_TOOLKIT_EXPECT_STR_GT(prefix, suffix)
#  define EXPECT_STR_GE(prefix, suffix) TINY_TOOLKIT_EXPECT_STR_GE(prefix, suffix)
#
#  define EXPECT_STR_CASE_EQ(prefix, suffix) TINY_TOOLKIT_EXPECT_STR_CASE_EQ(prefix, suffix)
#  define EXPECT_STR_CASE_NE(prefix, suffix) TINY_TOOLKIT_EXPECT_STR_CASE_NE(prefix, suffix)
#  define EXPECT_STR_CASE_LT(prefix, suffix) TINY_TOOLKIT_EXPECT_STR_CASE_LT(prefix, suffix)
#  define EXPECT_STR_CASE_LE(prefix, suffix) TINY_TOOLKIT_EXPECT_STR_CASE_LE(prefix, suffix)
#  define EXPECT_STR_CASE_GT(prefix, suffix) TINY_TOOLKIT_EXPECT_STR_CASE_GT(prefix, suffix)
#  define EXPECT_STR_CASE_GE(prefix, suffix) TINY_TOOLKIT_EXPECT_STR_CASE_GE(prefix, suffix)
#
#endif


#endif // __TINY_TOOLKIT__TEST__COMPARE__H__
