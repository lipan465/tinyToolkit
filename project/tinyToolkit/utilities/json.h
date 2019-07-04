#ifndef __TINY_TOOLKIT__UTILITIES__JSON__H__
#define __TINY_TOOLKIT__UTILITIES__JSON__H__


/**
 *
 *  作者: hm
 *
 *  说明: json处理
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Json
	{
	public:
		/**
		 *
		 * 收缩
		 *
		 * @param value 待格收缩数据
		 *
		 * @return 收缩后数据
		 *
		 */
		static std::string Reduce(const std::string & value);

		/**
		 *
		 * 格式化
		 *
		 * @param value 待格式化数据
		 *
		 * @return 格式化后数据
		 *
		 */
		static std::string Format(const std::string & value);
	};
}




#endif // __TINY_TOOLKIT__UTILITIES__JSON__H__
