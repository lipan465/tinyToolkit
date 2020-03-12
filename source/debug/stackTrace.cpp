/**
 *
 *  作者: hm
 *
 *  说明: 堆栈信息
 *
 */


#include "stackTrace.h"

#include "../util/string.h"
#include "../util/application.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <DbgHelp.h>
#
#  pragma comment( lib, "dbghelp.lib")
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <cxxabi.h>
#  include <execinfo.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <cxxabi.h>
#  include <execinfo.h>
#
#endif


namespace tinyToolkit
{
	namespace debug
	{
	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/**
		 *
		 * 解析符号
		 *
		 * @param hProcess 句柄
		 * @param address 地址
		 *
		 * @return 符号信息
		 *
		 */
		static std::string ParseSymbol(HANDLE hProcess, DWORD64 address)
		{
			DWORD displacementLine = 0;
			DWORD64 displacementSym = 0;

			char buffer[sizeof(IMAGEHLP_SYMBOL) + MAX_SYM_NAME]{ 0 };

			IMAGEHLP_LINE line{ };
			PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)buffer;

			line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

			symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
			symbol->MaxNameLength = MAX_SYM_NAME;

			if (SymGetSymFromAddr64(hProcess, address, &displacementSym, symbol) &&
				SymGetLineFromAddr64(hProcess, address, &displacementLine, &line))
			{
				return util::String::Format
				(
					"{}:{}({}) [0x{:02x}]",
					line.FileName,
					line.LineNumber,
					symbol->Name,
					symbol->Address
				);
			}

			return { };
		}

	#else

		/**
		 *
		 * 解析符号
		 *
		 * @param symbol 符号
		 *
		 * @return 符号信息
		 *
		 */
		static std::string ParseSymbol(const std::string & symbol)
		{
			std::size_t pos = 0;
			std::size_t head;
			std::size_t tail;

			std::string content{ };

			while (pos < symbol.size())
			{
				head = symbol.find("_Z", pos);

				if (head == std::string::npos)
				{
					content += symbol.substr(pos);

					break;
				}

				tail = symbol.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_", head);

				if (tail == std::string::npos)
				{
					tail = symbol.size();
				}

				int32_t status = 0;

				std::unique_ptr<char, void(*)(void *)> temp
				{
					abi::__cxa_demangle
					(
						symbol.substr(head, tail - head).c_str(),
						nullptr,
						nullptr,
						&status
					),
					std::free
				};

				if (status == 0)
				{
					content += symbol.substr(pos, head - pos);
					content += temp.get();

					pos = tail;
				}
				else
				{
					pos = head + 2;
				}
			}

			return content;
		}

	#endif


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 内容
		 *
		 * @return 内容
		 *
		 */
		std::string StackTrace::Content()
		{
			std::string temp{ };
			std::string content{ };

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			void * stack[1024]{ };

			HANDLE hProcess = ::GetCurrentProcess();

			::SymInitialize(hProcess, NULL, TRUE);

			WORD size = ::CaptureStackBackTrace(0, 1024, stack, NULL);

			if (size <= 0)
			{
				return { };
			}

			content += util::String::Format
			(
				"StackTrace [pid={}] : {}",
				util::Application::ProcessID(),
				TINY_TOOLKIT_EOL
			);

			for (WORD i = 0; i < size; ++i)
			{
				temp = ParseSymbol(hProcess, (DWORD64)(stack[i]));

				if (temp.empty())
				{
					continue;
				}

				content += util::String::Format
				(
					"    at {}{}",
					temp,
					TINY_TOOLKIT_EOL
				);
			}

		#else

			void * stack[1024]{ };

			auto size = ::backtrace(stack, 1024);

			if (size <= 0)
			{
				return { };
			}

			content += util::String::Format
			(
				"StackTrace [pid={}] : {}",
				util::Application::ProcessID(),
				TINY_TOOLKIT_EOL
			);

			std::unique_ptr<char *, void(*)(void *)> symbols
			{
				backtrace_symbols(stack, size),
				std::free
			};

			for (int32_t i = 0; i < size; ++i)
			{
				temp = ParseSymbol(symbols.get()[i]);

				if (temp.empty())
				{
					continue;
				}

				content += util::String::Format
				(
					"    at {}{}",
					temp,
					TINY_TOOLKIT_EOL
				);
			}

		#endif

			return content;
		}
	}
}
