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
	class TINY_TOOLKIT_API TestCompareHelper
	{
	public:
		/**
		 *
		 * 正确
		 *
		 * @param expression 表达式
		 * @param condition 条件
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult True(const char * expression, bool condition)
		{
			if (condition)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "Value of: " << expression << TINY_TOOLKIT_EOL
											<< "  Actual: " << "false" << TINY_TOOLKIT_EOL
											<< "Expected: " << "true" << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 错误
		 *
		 * @param expression 表达式
		 * @param condition 条件
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult False(const char * expression, bool condition)
		{
			if (!condition)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "Value of: " << expression << TINY_TOOLKIT_EOL
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
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		template <typename ExpectedValueTypeT, typename ActualValueTypeT>
		static TestResult EqualTo(const char * expectedExpr, const char * actualExpr, const ExpectedValueTypeT & expected, const ActualValueTypeT & actual)
		{
			if (expected == actual)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
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
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		template <typename ExpectedValueTypeT, typename ActualValueTypeT>
		static TestResult NotEqualTo(const char * expectedExpr, const char * actualExpr, const ExpectedValueTypeT & expected, const ActualValueTypeT & actual)
		{
			if (expected != actual)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
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
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		template <typename ExpectedValueTypeT, typename ActualValueTypeT>
		static TestResult LessThan(const char * expectedExpr, const char * actualExpr, const ExpectedValueTypeT & expected, const ActualValueTypeT & actual)
		{
			if (expected < actual)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
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
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		template <typename ExpectedValueTypeT, typename ActualValueTypeT>
		static TestResult GreaterThan(const char * expectedExpr, const char * actualExpr, const ExpectedValueTypeT & expected, const ActualValueTypeT & actual)
		{
			if (expected > actual)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
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
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		template <typename ExpectedValueTypeT, typename ActualValueTypeT>
		static TestResult LessThanOrEqualTo(const char * expectedExpr, const char * actualExpr, const ExpectedValueTypeT & expected, const ActualValueTypeT & actual)
		{
			if (expected <= actual)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
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
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		template <typename ExpectedValueTypeT, typename ActualValueTypeT>
		static TestResult GreaterThanOrEqualTo(const char * expectedExpr, const char * actualExpr, const ExpectedValueTypeT & expected, const ActualValueTypeT & actual)
		{
			if (expected >= actual)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " >= " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串等于
		 *
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrEqualTo(const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcmp(expected, actual) == 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " == " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串不等于
		 *
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrNotEqualTo(const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcmp(expected, actual) != 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " != " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串小于
		 *
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrLessThan(const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcmp(expected, actual) < 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " <  " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串大于
		 *
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrGreaterThan(const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcmp(expected, actual) > 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " >  " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串小于等于
		 *
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrLessThanOrEqualTo(const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcmp(expected, actual) <= 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " <= " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串大于等于
		 *
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrGreaterThanOrEqualTo(const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcmp(expected, actual) >= 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " >= " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串等于
		 *
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrCaseEqualTo(const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcasecmp(expected, actual) == 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " == " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串不等于
		 *
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrCaseNotEqualTo(const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcasecmp(expected, actual) != 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " != " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串小于
		 *
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrCaseLessThan(const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcasecmp(expected, actual) < 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " <  " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串大于
		 *
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrCaseGreaterThan(const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcasecmp(expected, actual) > 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " >  " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串小于等于
		 *
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrCaseLessThanOrEqualTo(const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcasecmp(expected, actual) <= 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " <= " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}

		/**
		 *
		 * 字符串大于等于
		 *
		 * @param expectedExpr 预期表达式
		 * @param actualExpr 现实表达式
		 * @param expected 预期数值
		 * @param actual 现实数值
		 *
		 * @return 测试结果
		 *
		 */
		static TestResult StrCaseGreaterThanOrEqualTo(const char * expectedExpr, const char * actualExpr, const char * expected, const char * actual)
		{
			if (strcasecmp(expected, actual) >= 0)
			{
				return TestSuccessResult();
			}
			else
			{
				return TestFailureResult()  << "  Actual: " << expected << " vs " << actual << TINY_TOOLKIT_EOL
											<< "Expected: " << expectedExpr << " >= " << actualExpr << TINY_TOOLKIT_EOL;
			}
		}
	};
}


#define TINY_TOOLKIT_EXPECT_TRUE(condition)						tinyToolkit::TestCompareHelper::True(#condition, condition)
#define TINY_TOOLKIT_EXPECT_FALSE(condition)					tinyToolkit::TestCompareHelper::False(#condition, condition)

#define TINY_TOOLKIT_EXPECT_EQ(expected, actual)				tinyToolkit::TestCompareHelper::EqualTo(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_NE(expected, actual)				tinyToolkit::TestCompareHelper::NotEqualTo(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_LT(expected, actual)				tinyToolkit::TestCompareHelper::LessThan(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_LE(expected, actual)				tinyToolkit::TestCompareHelper::LessThanOrEqualTo(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_GT(expected, actual)				tinyToolkit::TestCompareHelper::GreaterThan(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_GE(expected, actual)				tinyToolkit::TestCompareHelper::GreaterThanOrEqualTo(#expected, #actual, expected, actual)

#define TINY_TOOLKIT_EXPECT_STR_EQ(expected, actual)			tinyToolkit::TestCompareHelper::StrEqualTo(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_STR_NE(expected, actual)			tinyToolkit::TestCompareHelper::StrNotEqualTo(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_STR_LT(expected, actual)			tinyToolkit::TestCompareHelper::StrLessThan(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_STR_LE(expected, actual)			tinyToolkit::TestCompareHelper::StrLessThanOrEqualTo(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_STR_GT(expected, actual)			tinyToolkit::TestCompareHelper::StrGreaterThan(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_STR_GE(expected, actual)			tinyToolkit::TestCompareHelper::StrGreaterThanOrEqualTo(#expected, #actual, expected, actual)

#define TINY_TOOLKIT_EXPECT_STR_CASE_EQ(expected, actual)		tinyToolkit::TestCompareHelper::StrCaseEqualTo(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_STR_CASE_NE(expected, actual)		tinyToolkit::TestCompareHelper::StrCaseNotEqualTo(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_STR_CASE_LT(expected, actual)		tinyToolkit::TestCompareHelper::StrCaseLessThan(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_STR_CASE_LE(expected, actual)		tinyToolkit::TestCompareHelper::StrCaseLessThanOrEqualTo(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_STR_CASE_GT(expected, actual)		tinyToolkit::TestCompareHelper::StrCaseGreaterThan(#expected, #actual, expected, actual)
#define TINY_TOOLKIT_EXPECT_STR_CASE_GE(expected, actual)		tinyToolkit::TestCompareHelper::StrCaseGreaterThanOrEqualTo(#expected, #actual, expected, actual)


#endif // __TINY_TOOLKIT__TEST__COMPARE__H__
