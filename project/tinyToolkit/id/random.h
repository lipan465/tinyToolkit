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
		template<typename TypeT>
		static TypeT FloatUniformDistribution(const TypeT min, const TypeT max)
		{
			static_assert(std::is_floating_point<TypeT>::value, "Template must be a float type");

			assert(min <= max);

			std::random_device rd;

			std::mt19937 mt(rd());

			std::uniform_real_distribution<TypeT> dis(min, max);

			return dis(mt);
		}

		/**
		 *
		 * 生成整数类型随机数
		 *
		 * @tparam TypeT [all integral types]
		 *
		 * @param min 最小值
		 * @param max 最大值
		 *
		 * @return 随机数
		 *
		 */
		template<typename TypeT>
		static TypeT IntUniformDistribution(const TypeT min, const TypeT max)
		{
			static_assert(std::is_integral<TypeT>::value, "Template must be an integral type");

			assert(min <= max);

			std::random_device rd;

			std::mt19937 mt(rd());

			std::uniform_int_distribution<TypeT> dis(min, max);

			return dis(mt);
		}
	};
}


#endif // __TINY_TOOLKIT__ID__RANDOM__H__
