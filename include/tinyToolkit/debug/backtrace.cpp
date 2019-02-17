/**
 *
 *  作者: hm
 *
 *  说明: 堆栈信息
 *
 */


#include "backtrace.h"


namespace tinyToolkit
{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

	/// todo

#else

	/**
	 *
	 * 解析符号
	 *
	 * @param symbol 待解析符号
	 *
	 */
	static void ParseSymbol(std::string & symbol)
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
	}

	/**
	 *
	 * 获取堆栈
	 *
	 * @return 堆栈
	 *
	 */
	static void ** __attribute__((regparm(1), noinline)) GetEBP()
	{
		void ** ebp = nullptr;

		__asm__ __volatile__
		(
		"mov %%rbp, %0;\n\t"
		:"=m"(ebp)					/// 输出
		:							/// 输入
		:"memory"					/// 不受影响的寄存器
		);

		return (void **)(*ebp);
	}

	/**
	 *
	 * 获取堆栈信息
	 *
	 * @param buffer 存储堆栈信息
	 * @param size 存储堆栈信息大小
	 *
	 * @return 获取堆栈信息个数
	 *
	 */
	static int32_t GetBacktrace(void ** buffer, int32_t size)
	{
		int32_t frame = 0;

		if (buffer && size > 0)
		{
			void ** ebp = GetEBP();
			void ** ret = nullptr;

			uint64_t func_frame_distance = reinterpret_cast<uint64_t>(*ebp) - reinterpret_cast<uint64_t>(ebp);

			/// assume function ebp more than 16M
			while (frame < size && func_frame_distance < (1ULL << 24) && func_frame_distance > 0)
			{
				ret = ebp + 1;

				buffer[frame++] = *ret;

				ebp = reinterpret_cast<void**>(*ebp);

				func_frame_distance = reinterpret_cast<uint64_t>(*ebp) - reinterpret_cast<uint64_t>(ebp);
			}
		}

		return frame;
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
	void Backtrace::Print(int32_t signalNo)
	{
	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		/// todo

	#else

		void * array[TINY_TOOLKIT_KB];

		int32_t size = GetBacktrace(array, TINY_TOOLKIT_KB);

		std::unique_ptr<char *, void(*)(void *)> symbols
		{
			backtrace_symbols(array, size),
			std::free
		};

		if (size > 0)
		{
			std::cerr << "Signal " << signalNo << " StackTrace : " << std::endl;
		}

		for (int32_t i = 0; i < size; ++i)
		{
			std::string symbolStr(symbols.get()[i]);

			ParseSymbol(symbolStr);

			std::cerr << "    at " << symbolStr << std::endl;
		}

		if (signalNo > 0)
		{
			std::raise(SIGQUIT);
		}

	#endif
	}
}
