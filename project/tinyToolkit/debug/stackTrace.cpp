/**
 *
 *  作者: hm
 *
 *  说明: 堆栈信息
 *
 */


#include "stackTrace.h"

#include "../system/os.h"
#include "../utilities/string.h"


namespace tinyToolkit
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
			return String::Format("{}:{}({}) [0x{:02x}]", line.FileName, line.LineNumber, symbol->Name, symbol->Address);
		}

		return { };
	}

#else

	/**
	 *
	 * 解析符号
	 *
	 * @param symbol 待解析符号
	 *
	 * @return 符号信息
	 *
	 */
	static std::string ParseSymbol(std::string symbol)
	{
		int32_t status = 0;

		std::size_t parse_head = 0;
		std::size_t parse_tail = 0;
		std::size_t search_pos = 0;

		std::string parseStr = { };

		while (search_pos < symbol.size())
		{
			parse_head = symbol.find ("_Z", search_pos);

			if (parse_head == std::string::npos)
			{
				break;
			}

			parse_tail = symbol.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_", parse_head);

			if (parse_tail == std::string::npos)
			{
				parse_tail = symbol.size();
			}

			parseStr = symbol.substr(parse_head, parse_tail - parse_head);

			std::unique_ptr<char, void(*)(void*)> parseSymbol
			{
				abi::__cxa_demangle(parseStr.c_str(), nullptr, nullptr, &status),
				std::free
			};

			if (status == 0)
			{
				symbol.erase(parse_head, parse_tail - parse_head);
				symbol.insert(parse_head, parseSymbol.get());

				search_pos = parse_head + strlen(parseSymbol.get());
			}
			else
			{
				search_pos = parse_head + 2;
			}
		}

		return symbol;
	}

#endif


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 打印堆栈信息
	 *
	 * @param signalNo 信号
	 *
	 */
	void StackTrace::Print(int32_t signalNo)
	{
		for (auto& iter : Content(signalNo))
		{
			std::cout << iter << std::endl;
		}
		
		if (signalNo > 0)
		{
			std::raise(SIGTERM);
		}
	}

	/**
	 *
	 * 堆栈信息
	 *
	 * @param signalNo 信号
	 *
	 * @return 堆栈信息
	 *
	 */
	std::vector<std::string> StackTrace::Content(int32_t signalNo)
	{
		std::vector<std::string> vec{ };

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		void * stack[TINY_TOOLKIT_KB]{ };

		HANDLE hProcess = ::GetCurrentProcess();

		SymInitialize(hProcess, NULL, TRUE);

		WORD size = CaptureStackBackTrace(0, TINY_TOOLKIT_KB, stack, NULL);

		if (size > 0)
		{
			vec.push_back(String::Format("Catch StackTrace [signal={}, pid={}] :", signalNo, OS::ProcessID()));
		}

		for (WORD i = 0; i < size; ++i)
		{
			auto str = ParseSymbol(hProcess, (DWORD64)(stack[i]));

			if (!str.empty())
			{
				vec.push_back(String::Format("    at {}", str));
			}
		}

#else

		void * stack[TINY_TOOLKIT_KB]{ };

		int32_t size = backtrace(stack, TINY_TOOLKIT_KB);

		std::unique_ptr<char *, void(*)(void *)> symbols
		{
			backtrace_symbols(stack, size),
			std::free
		};

		if (size > 0)
		{
			vec.push_back(String::Format("Catch StackTrace [signal={}, pid={}] :", signalNo, OS::ProcessID()));
		}

		for (int32_t i = 0; i < size; ++i)
		{
			auto str = ParseSymbol(symbols.get()[i]);

			if (!str.empty())
			{
				vec.push_back(String::Format("    at {}", str));
			}
		}

#endif

		return vec;
	}
}
