#ifndef __TINY_TOOLKIT__UTILITIES__MATH__H__
#define __TINY_TOOLKIT__UTILITIES__MATH__H__


/**
 *
 *  作者: hm
 *
 *  说明: 数学运算
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Math
	{
	public:
		/***
		 *
		 * 是否为ip
		 *
		 * @param value 待检测字符串
		 *
		 * @return 检测结果
		 *
		 */
		static bool IsIP(const char * value);

		/***
		 *
		 * 是否为ip
		 *
		 * @param value 待检测字符串
		 *
		 * @return 检测结果
		 *
		 */
		static bool IsIP(const std::string & value);

		/***
		 *
		 * 是否为奇数
		 *
		 * @tparam TypeT [all integral types]
		 *
		 * @param value 待判断的数字
		 *
		 * @return 判断结果
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
		 * @tparam TypeT [all integral types]
		 *
		 * @param value 待判断的数字
		 *
		 * @return 判断结果
		 *
		 */
		template <typename TypeT>
		static bool IsEven(TypeT value)
		{
			static_assert(std::is_integral<TypeT>::value, "Template must be an integral type");

			return (value & 1) == 0;
		}
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__MATH__H__
