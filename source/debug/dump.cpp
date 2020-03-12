/**
 *
 *  作者: hm
 *
 *  说明: 转储
 *
 */


#include "dump.h"

#include "../util/time.h"
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
#  include "stackTrace.h"
#
#  include "../util/signal.h"
#  include "../util/filesystem.h"
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include "stackTrace.h"
#
#  include "../util/signal.h"
#  include "../util/filesystem.h"
#
#endif


namespace tinyToolkit
{
	namespace debug
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
			std::string file = util::String::Format
			(
				"{}_{}.dmp",
				util::Application::Name(),
				util::Time::LocalTimeString("%4d_%02d_%02d_%02d_%02d_%02d")
			);

			HANDLE hFile = CreateFile(file.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

			if (hFile && (hFile != INVALID_HANDLE_VALUE))
			{
				MINIDUMP_EXCEPTION_INFORMATION exceptionInformation{ };

				exceptionInformation.ThreadId = GetCurrentThreadId();
				exceptionInformation.ExceptionPointers = exception;
				exceptionInformation.ClientPointers = FALSE;

				MINIDUMP_CALLBACK_INFORMATION callbackInformation{ };

				callbackInformation.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)(DumpCallback);
				callbackInformation.CallbackParam = 0;

				MINIDUMP_TYPE type = MiniDumpNormal;

				MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, type, &exceptionInformation, nullptr, &callbackInformation);

				CloseHandle(hFile);
			}
		}

		/**
		 *
		 * 异常处理
		 *
		 * @param exception 异常
		 *
		 * @return 异常处理结果
		 *
		 */
		LONG ExceptionHandler(PEXCEPTION_POINTERS exception)
		{
			CreateMiniDump(exception);

			return EXCEPTION_EXECUTE_HANDLER;
		}

	#else

		/**
		 *
		 * 异常处理
		 *
		 * @param signalNo 信号量
		 *
		 */
		static void ExceptionHandler(int32_t signalNo)
		{
			std::string file = util::String::Format
			(
				"{}_{}.dmp",
				util::Application::Name(),
				util::Time::LocalTimeString("%4d_%02d_%02d_%02d_%02d_%02d")
			);

			std::string content = util::String::Format
			(
				"{0}{3}{1}{3}{2}{3}{3}{4}",
				util::String::Format("Catch File [{}]", util::Application::Path()),
				util::String::Format("Catch Time [{}]", util::Time::LocalTimeString()),
				util::String::Format("Catch Signal [{}]", signalNo),
				TINY_TOOLKIT_EOL,
				StackTrace::Content()
			);

			util::Filesystem::WriteFile(file, content);
		}

	#endif


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 注册
		 *
		 */
		void Dump::Register()
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			sExceptionFilter = ::SetUnhandledExceptionFilter(ExceptionHandler);

		#else

			util::Signal::RegisterException(ExceptionHandler);

		#endif
		}

		/**
		 *
		 * 注销
		 *
		 */
		void Dump::UnRegister()
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (sExceptionFilter)
			{
				SetUnhandledExceptionFilter(sExceptionFilter);

				sExceptionFilter = nullptr;
			}

		#endif
		}
	}
}
