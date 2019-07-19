#ifndef __TINY_TOOLKIT__ID__RANDOM__H__
#define __TINY_TOOLKIT__ID__RANDOM__H__


/**
 *
 *  作者: hm
 *
 *  说明: 随机数
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Random
	{
	public:
		/**
		 *
		 * 生成浮点型随机数
		 *
		 * @tparam TypeT [all float types]
		 *
		 * @param min 最小值
		 * @param max 最大值
		 *
		 * @return 浮点型随机数
		 *
		 */
		template<typename FLoatTypeT>
		static FLoatTypeT FloatUniformDistribution(const FLoatTypeT min, const FLoatTypeT max)
		{
			static_assert(std::is_floating_point<FLoatTypeT>::value, "Template must be a float type");

			assert(min <= max);

			std::random_device rd;

			std::mt19937 mt(rd());

			std::uniform_real_distribution<FLoatTypeT> dis(min, max);

			return dis(mt);
		}

		/**
		 *
		 * 生成整数类型随机数
		 *
		 * @tparam TypeT [all integer types]
		 *
		 * @param min 最小值
		 * @param max 最大值
		 *
		 * @return 随机数
		 *
		 */
		template<typename IntegerTypeT>
		static IntegerTypeT IntUniformDistribution(const IntegerTypeT min, const IntegerTypeT max)
		{
			static_assert(std::is_integral<IntegerTypeT>::value, "Template must be an integer type");

			assert(min <= max);

			std::random_device rd;

			std::mt19937 mt(rd());

			std::uniform_int_distribution<IntegerTypeT> dis(min, max);

			return dis(mt);
		}
	};
}


#endif // __TINY_TOOLKIT__ID__RANDOM__H__
