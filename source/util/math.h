#ifndef __TINY_TOOLKIT__UTIL__MATH__H__
#define __TINY_TOOLKIT__UTIL__MATH__H__


/**
 *
 *  作者: hm
 *
 *  说明: 数学处理
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <random>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <random>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <random>
#
#endif


namespace tinyToolkit
{
	namespace util
	{
		class TINY_TOOLKIT_API Math
		{
		public:
			/***
			 *
			 * 是否为奇数
			 *
			 * @param value 数字
			 *
			 * @return 是否为奇数
			 *
			 */
			template <typename TypeT>
			static bool IsOdd(TypeT value)
			{
				static_assert(std::is_integral<TypeT>::value, "Template must be an integral type");

				return (value & 1) != 0;
			}

			/**
			 *
			 * 是否为偶数
			 *
			 * @param value 数字
			 *
			 * @return 是否为偶数
			 *
			 */
			template <typename TypeT>
			static bool IsEven(TypeT value)
			{
				static_assert(std::is_integral<TypeT>::value, "Template must be an integral type");

				return (value & 1) == 0;
			}

			/**
			 *
			 * 浮点型均匀分布随机数
			 *
			 * @param min 最小值
			 * @param max 最大值
			 *
			 * @return 浮点型随机数
			 *
			 */
			template<typename FloatTypeT>
			static FloatTypeT FloatUniformDistributionRandom(const FloatTypeT min, const FloatTypeT max)
			{
				static_assert(std::is_floating_point<FloatTypeT>::value, "Template must be a float type");

				std::random_device rd{ };

				std::mt19937 mt(rd());

				std::uniform_real_distribution<FloatTypeT> dis(min, max);

				return dis(mt);
			}

			/**
			 *
			 * 整数型均匀分布随机数
			 *
			 * @param min 最小值
			 * @param max 最大值
			 *
			 * @return 整数型随机数
			 *
			 */
			template<typename IntegerTypeT>
			static IntegerTypeT IntegerUniformDistributionRandom(const IntegerTypeT min, const IntegerTypeT max)
			{
				static_assert(std::is_integral<IntegerTypeT>::value, "Template must be an integer type");

				std::random_device rd{ };

				std::mt19937 mt(rd());

				std::uniform_int_distribution<IntegerTypeT> dis(min, max);

				return dis(mt);
			}
		};
	}
}


#endif // __TINY_TOOLKIT__UTIL__MATH__H__
