/**
 *
 *  作者: hm
 *
 *  说明: 堆栈信息
 *
 */


#include "coreDump.h"
#include "stackTrace.h"

#include "../system/signal.h"
#include "../system/application.h"

#include "../utilities/time.h"
#include "../utilities/string.h"
#include "../utilities/filesystem.h"


namespace tinyToolkit
{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

	static LPTOP_LEVEL_EXCEPTION_FILTER sExceptionFilter = nullptr;

	/**
	 *
	 * 是否是需要的数据
	 *
	 * @param moduleName 模块名称
	 *
	 * @return 是否是需要的数据
	 *
	 */
	BOOL IsDataSectionNeeded(const WCHAR * moduleName)
	{
		if (moduleName == nullptr)
		{
			return FALSE;
		}

		WCHAR fileName[TINY_TOOLKIT_NAME_MAX]{ };

		_wsplitpath_s(moduleName, nullptr, 0, nullptr, 0, fileName, TINY_TOOLKIT_NAME_MAX, nullptr, 0);

		return _wcsicmp(fileName, L"ntdll") == 0 ? TRUE : FALSE;
	}

	/**
	 *
	 * 转储文件回调函数
	 *
	 * @param param 参数
	 * @param input 输入
	 * @param output 输出
	 *
	 * @return 处理结果
	 *
	 */
	BOOL DumpCallback(PVOID param, const PMINIDUMP_CALLBACK_INPUT input, PMINIDUMP_CALLBACK_OUTPUT output)
	{
		if (input == nullptr || output == nullptr)
		{
			return FALSE;
		}

		switch (input->CallbackType)
		{
			case ModuleCallback:
			{
				if (output->ModuleWriteFlags & ModuleWriteDataSeg)
				{
					if (!IsDataSectionNeeded(input->Module.FullPath))
					{
						output->ModuleWriteFlags &= (~ModuleWriteDataSeg);
					}
				}
			}
			
			case ThreadCallback:
			case ThreadExCallback:
			case IncludeModuleCallback:
			case IncludeThreadCallback:
			{
				return TRUE;
			}

			default:
			{
				break;
			}
		}

		return FALSE;
	}

	/**
	 *
	 * 创建最小转储文件
	 *
	 * @param exception 异常
	 *
	 */
	void CreateMiniDump(PEXCEPTION_POINTERS exception)
	{
		std::string file = String::Format
		(
			"{}_{}.dmp",
			Application::Name(),
			Time::CurrentLocalTimeString("%4d_%02d_%02d_%02d_%02d_%02d")
		);

		HANDLE hFile = CreateFile(file.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile && (hFile != INVALID_HANDLE_VALUE))
		{
			MINIDUMP_EXCEPTION_INFORMATION exceptionInformation;

			exceptionInformation.ThreadId = GetCurrentThreadId();
			exceptionInformation.ExceptionPointers = exception;
			exceptionInformation.ClientPointers = FALSE;

			MINIDUMP_CALLBACK_INFORMATION callbackInformation;

			callbackInformation.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)(DumpCallback);
			callbackInformation.CallbackParam = 0;

			MINIDUMP_TYPE type = MiniDumpNormal;

			MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, type, &exceptionInformation, nullptr, &callbackInformation);

			CloseHandle(hFile);
		}
	}

	/**
	 *
	 * 崩溃处理
	 *
	 * @param exception 异常
	 *
	 * @return 崩溃处理结果
	 *
	 */
	LONG Crash(PEXCEPTION_POINTERS exception)
	{
		CreateMiniDump(exception);

		return EXCEPTION_EXECUTE_HANDLER;
	}

#else

	/**
	 *
	 * 崩溃处理
	 *
	 * @param signalNo 信号量
	 *
	 */
	static void Crash(int32_t signalNo)
	{
		std::string file = String::Format
		(
			"{}_{}.dmp",
			Application::Name(),
			Time::CurrentLocalTimeString("%4d_%02d_%02d_%02d_%02d_%02d")
		);

		std::string content = String::Format
		(
			"{0}{2}{1}{2}",
			String::Format("Catch File [{}]", Application::Path()),
			String::Format("Catch Time [{}]", Time::CurrentLocalTimeString()),
			TINY_TOOLKIT_EOL
		);

		for (auto &iter : StackTrace::Content(signalNo))
		{
			content += iter;
			content += TINY_TOOLKIT_EOL;
		}

		Filesystem::WriteFile(file, content);
	}

#endif


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 析构函数
	 *
	 */
	CoreDump::~CoreDump()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (sExceptionFilter)
		{
			SetUnhandledExceptionFilter(sExceptionFilter);

			sExceptionFilter = nullptr;
		}

#else

		(void)nullptr;

#endif
	}

	/**
	 *
	 * 注册
	 *
	 */
	void CoreDump::Register()
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		sExceptionFilter = ::SetUnhandledExceptionFilter(Crash);

#else

		Signal::RegisterCrash(Crash);

#endif
	}
}
