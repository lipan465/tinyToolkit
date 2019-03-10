#ifndef __TINY_TOOLKIT__UTILITIES__FILE_LINE__H__
#define __TINY_TOOLKIT__UTILITIES__FILE_LINE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 文件信息
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API FileLine
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param file 文件
		 * @param line 行号
		 * @param func 函数
		 *
		 */
		FileLine(const char * file, int32_t line, const char * func);

		/**
		 *
		 * 行号
		 *
		 * @return 行号
		 *
		 */
		int32_t Line() const;

		/**
		 *
		 * 文件
		 *
		 * @return 文件
		 *
		 */
		const char * File() const;

		/**
		 *
		 * 函数
		 *
		 * @return 函数
		 *
		 */
		const char * Function() const;

		/**
		 *
		 * 信息
		 *
		 * @return 信息
		 *
		 */
		const std::string & Message() const;

	protected:
		int32_t _line{ 0 };

		const char * _file{ nullptr };
		const char * _func{ nullptr };

		std::string _message{ };
	};
}


#define TINY_TOOLKIT_FILE_LINE	tinyToolkit::FileLine(TINY_TOOLKIT_FILE, TINY_TOOLKIT_LINE, TINY_TOOLKIT_FUNC)


#endif // __TINY_TOOLKIT__UTILITIES__FILE_LINE__H__
