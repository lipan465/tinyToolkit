#ifndef __TINY_TOOLKIT__DEBUG__EXCEPTION__H__
#define __TINY_TOOLKIT__DEBUG__EXCEPTION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 异常处理
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <string>
#  include <exception>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <string>
#  include <exception>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <string>
#  include <exception>
#
#endif


namespace tinyToolkit
{
	namespace debug
	{
		class TINY_TOOLKIT_API IException : public std::exception
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param file 文件
			 * @param line 行号
			 * @param func 函数
			 * @param info 信息
			 *
			 */
			IException(const char * file, int32_t line, const char * func, std::string info);

			/**
			 *
			 * 异常信息
			 *
			 * @return 异常信息
			 *
			 */
			const char * what() const noexcept override;

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
			const char * Func() const;

			/**
			 *
			 * 信息
			 *
			 * @return 信息
			 *
			 */
			const std::string & Info() const;

		private:
			int32_t _line{ 0 };

			std::string _what{ };
			std::string _info{ };

			const char * _file{ nullptr };
			const char * _func{ nullptr };
		};

		INHERITANCE_DECLARE(Exception, IException);
		INHERITANCE_DECLARE(IOException, IException);
		INHERITANCE_DECLARE(KeyException, IException);
		INHERITANCE_DECLARE(TimeException, IException);
		INHERITANCE_DECLARE(SizeException, IException);
		INHERITANCE_DECLARE(FileException, IException);
		INHERITANCE_DECLARE(TypeException, IException);
		INHERITANCE_DECLARE(ValueException, IException);
		INHERITANCE_DECLARE(IndexException, IException);
		INHERITANCE_DECLARE(AssertException, IException);
		INHERITANCE_DECLARE(MemoryException, IException);
		INHERITANCE_DECLARE(ThreadException, IException);
		INHERITANCE_DECLARE(ProcessException, IException);
		INHERITANCE_DECLARE(ParsingException, IException);
		INHERITANCE_DECLARE(NotFoundException, IException);
		INHERITANCE_DECLARE(NullPointException, IException);
		INHERITANCE_DECLARE(DirectoryException, IException);
		INHERITANCE_DECLARE(ArgumentsException, IException);
		INHERITANCE_DECLARE(SystemExitException, IException);
		INHERITANCE_DECLARE(PermissionException, IException);
	}
}


/**
 *
 * 抛出异常
 *
 */
#define TINY_TOOLKIT_THROW_EXCEPTION(type, ...) \
	throw type(TINY_TOOLKIT_FILE, TINY_TOOLKIT_LINE, TINY_TOOLKIT_FUNC, ##__VA_ARGS__)


#endif // __TINY_TOOLKIT__DEBUG__EXCEPTION__H__
