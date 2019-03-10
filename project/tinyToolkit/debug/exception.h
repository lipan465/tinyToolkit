#ifndef __TINY_TOOLKIT__DEBUG__EXCEPTION__H__
#define __TINY_TOOLKIT__DEBUG__EXCEPTION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 异常处理
 *
 */


#include "../utilities/fileLine.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API IException : public std::exception
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param fileLine 文件信息
		 * @param message 信息
		 * @param option 操作
		 *
		 */
		explicit IException(const FileLine & fileLine, std::string message = "", std::string option = "");

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
		 * 操作
		 *
		 * @return 操作
		 *
		 */
		const std::string & Option() const;

		/**
		 *
		 * 信息
		 *
		 * @return 信息
		 *
		 */
		const std::string & Message() const;

		/**
		 *
		 * 打印堆栈信息
		 *
		 */
		void PrintStackTrace();

	protected:
		mutable std::string _what{ };
		mutable std::string _option{ };
		mutable std::string _message{ };
	};

	INHERITANCE_DECLARE(Exception, IException);
	INHERITANCE_DECLARE(IOException, IException);
	INHERITANCE_DECLARE(KeyException, IException);
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

	class TINY_TOOLKIT_API ExceptionHelper
	{
	public:
		/**
		 *
		 * 抛出异常
		 *
		 * @tparam ErrorTypeT [debug exception types]
		 * @tparam Args [all built-in types]
		 *
		 * @param fileLine 文件信息
		 * @param args 参数
		 *
		 */
		template <typename ErrorTypeT, typename... Args>
		static void Throw(const FileLine & fileLine, Args &&... args)
		{
			throw ErrorTypeT(fileLine, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 创建异常类型实例
		 *
		 * @tparam ErrorTypeT [debug exception types]
		 * @tparam Args [all built-in types]
		 *
		 * @param fileLine 文件信息
		 * @param args 参数
		 *
		 * @return 异常类型实例
		 *
		 */
		template <typename ErrorTypeT, typename... Args>
		static ErrorTypeT Create(const FileLine & fileLine, Args &&... args)
		{
			return ErrorTypeT(fileLine, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 创建异常类型实例智能指针
		 *
		 * @tparam ErrorTypeT [debug exception types]
		 * @tparam Args [all built-in types]
		 *
		 * @param fileLine 文件信息
		 * @param args 参数
		 *
		 * @return 异常类型实例智能指针
		 *
		 */
		template <typename ErrorTypeT, typename... Args>
		static std::exception_ptr CreatePoint(const FileLine & fileLine, Args &&... args)
		{
			return std::make_exception_ptr(ErrorTypeT(fileLine, std::forward<Args>(args)...));
		}
	};
}


#define TINY_TOOLKIT_EXCEPTION_THROW(type, ...)			tinyToolkit::ExceptionHelper::Throw<type>(TINY_TOOLKIT_FILE_LINE, ##__VA_ARGS__)
#define TINY_TOOLKIT_EXCEPTION_CREATE(type, ...)		tinyToolkit::ExceptionHelper::Create<type>(TINY_TOOLKIT_FILE_LINE, ##__VA_ARGS__)
#define TINY_TOOLKIT_EXCEPTION_CREATE_POINT(type, ...)	tinyToolkit::ExceptionHelper::CreatePoint<type>(TINY_TOOLKIT_FILE_LINE, ##__VA_ARGS__)


#endif // __TINY_TOOLKIT__DEBUG__EXCEPTION__H__
