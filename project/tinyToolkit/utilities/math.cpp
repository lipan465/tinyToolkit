/**
 *
 *  作者: hm
 *
 *  说明: 数学运算
 *
 */


#include "math.h"


namespace tinyToolkit
{
	/***
	 *
	 * 是否为ip
	 *
	 * @param value 待检测字符串
	 *
	 * @return 检测结果
	 *
	 */
	bool Math::IsIP(const char * value)
	{
		static std::regex rule("^(([01]?[0-9]{1,2}|2[0-4][0-9]|25[0-5])(\\.([01]?[0-9]{1,2}|2[0-4][0-9]|25[0-5])){3}|([0-9a-fA-F]{1,4}:)+:?([0-9a-fA-F]{1,4}:)*[0-9a-fA-F]{1,4})$");

		return std::regex_match(value, rule);
	}

	/***
	 *
	 * 是否为ip
	 *
	 * @param value 待检测字符串
	 *
	 * @return 检测结果
	 *
	 */
	bool Math::IsIP(const std::string & value)
	{
		static std::regex rule("^(([01]?[0-9]{1,2}|2[0-4][0-9]|25[0-5])(\\.([01]?[0-9]{1,2}|2[0-4][0-9]|25[0-5])){3}|([0-9a-fA-F]{1,4}:)+:?([0-9a-fA-F]{1,4}:)*[0-9a-fA-F]{1,4})$");

		return std::regex_match(value, rule);
	}
}
