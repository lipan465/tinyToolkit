#ifndef __TINY_TOOLKIT__SYSTEM__SIGNAL__H__
#define __TINY_TOOLKIT__SYSTEM__SIGNAL__H__


/**
 *
 *  作者: hm
 *
 *  说明: 信号处理
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_UNIX || TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

	class TINY_TOOLKIT_API Signal
	{
	public:
		/**
		 *
		 * 向对应进程发送信号
		 *
		 * @param pid 进程id
		 * @param signalNo 待发送信号
		 *
		 * @return 发送结果
		 *
		 */
		static int32_t Kill(int32_t pid, int32_t signalNo)
		{
			return ::kill(pid, signalNo);
		}

		/**
		 *
		 * 向自身发送信号
		 *
		 * @param signalNo 待发送信号
		 *
		 * @return 发送结果
		 *
		 */
		static int32_t Raise(int32_t signalNo)
		{
			return std::raise(signalNo);
		}

		/**
		 *
		 * 注册忽略信号
		 *
		 */
		static void RegisterIgnore()
		{
			/**
			 *
			 * SIGPIPE 产生信号时就不会中止程序, 直接把这个信号忽略掉
			 *
			 * SIGCHLD 产生信号时就不会僵尸进程, 直接把这个信号忽略掉
			 *
			 */
			RegisterAction(SIGPIPE, SIG_IGN);
			RegisterAction(SIGCHLD, SIG_IGN);
		}

		/**
		 *
		 * 注册堆栈调试
		 *
		 */
		static void RegisterStackTrace()
		{
			RegisterFrame
			(
				[](int32_t signalNo)
				{
					void * array[TINY_TOOLKIT_KB];

					int32_t size = backtrace(array, TINY_TOOLKIT_KB);

					char ** stackString = backtrace_symbols(array, size);

					if (stackString)
					{
						std::cerr << "Signal " << signalNo << " StackTrace : " << std::endl;

						for (int32_t i = 0; i < size; ++i)
						{
							std::cerr << "    at " << stackString[i] << std::endl;
						}

						free(stackString);
					}

					Raise(SIGABRT);
				}
			);
		}

		/**
		 *
		 * 注册信号
		 *
		 * @param handler 信号触发时调用的函数
		 *
		 */
		static void RegisterRefresh(void(* handler)(int))
		{
			/**
			 *
			 * SIGHUP 终端关闭时.
			 * 		  系统对SIGHUP信号的默认处理是终止收到该信号的进程.
			 * 		  所以若程序中没有捕捉该信号, 当收到该信号时, 进程就会退出.
			 *
			 */
			RegisterAction(SIGHUP, handler);
		}

		/**
		 *
		 * 注册结束信号
		 *
		 * @param handler 信号触发时调用的函数
		 *
		 */
		static void RegisterTerminate(void(* handler)(int))
		{
			/**
			 *
			 * SIGINT   程序终止(interrupt)信号, 在用户键入INTR字符(通常是Ctrl-C)时发出, 用于通知前台进程组终止进程
			 *
			 * SIGABRT  调用abort函数生成的信号
			 *
			 * SIGQUIT  和SIGINT类似, 但由QUIT字符(通常是Ctrl-\)来控制.
			 * 			进程在因收到SIGQUIT退出时会产生core文件, 在这个意义上类似于一个程序错误信号
			 *
			 * SIGKILL  用来立即结束程序的运行.
			 * 			如果管理员发现某个进程终止不了, 可尝试发送这个信号
			 * 			本信号不能被阻塞、处理和忽略.
			 *
			 * SIGTERM  程序结束(terminate)信号, 与SIGKILL不同的是该信号可以被阻塞和处理.
			 * 			通常用来要求程序自己正常退出, shell命令kill缺省产生这个信号.
			 * 			如果进程终止不了, 我们才会尝试SIGKILL
			 *
			 * SIGSTOP  停止(stopped)进程的执行.
			 * 			注意它和terminate以及interrupt的区别:该进程还未结束, 只是暂停执行.
			 * 			本信号不能被阻塞, 处理或忽略.
			 *
			 * SIGTSTP  停止进程的运行, 但该信号可以被处理和忽略.
			 * 			用户键入SUSP字符时(通常是Ctrl-Z)发出这个信号
			 *
			 */
			RegisterAction(SIGINT,  handler);
			RegisterAction(SIGQUIT, handler);
			RegisterAction(SIGABRT, handler);
			RegisterAction(SIGKILL, handler);
			RegisterAction(SIGTERM, handler);
			RegisterAction(SIGSTOP, handler);
			RegisterAction(SIGTSTP, handler);
		}

		/**
		 *
		 * 注册帧信号
		 *
		 * @param handler 信号触发时调用的函数
		 *
		 */
		static void RegisterFrame(void(* handler)(int))
		{
			/**
			 *
			 * SIGSEGV 进程执行了一个无效的内存引用, 或发生段错误时发送给它的信号
			 *
			 */
			RegisterAction(SIGSEGV, handler);
		}

		/**
		 *
		 * 注册信号动作
		 *
		 * @param signalNo 待注册信号
		 * @param handler 信号触发时调用的函数
		 *
		 */
		static void RegisterAction(int signalNo, void(* handler)(int))
		{
			struct sigaction iSig = { };
			struct sigaction oSig = { };

			iSig.sa_flags = SA_SIGINFO;
			iSig.sa_handler = handler;

			sigemptyset(&iSig.sa_mask);  // 用来将参数信号集初始化并清空

#ifdef SA_RESTART

			iSig.sa_flags |= SA_RESTART;

#endif // SA_RESTART

			/**
			 *
			 * int sigaction(int signo, const struct sigaction * restrict, struct sigaction * restrict oact)
			 *
			 * signo 为信号编号
			 *
			 * restrict 为sigaction结构体
			 *
			 * oact 一般为NULL
			 *
			 */
			sigaction(signalNo, &iSig, &oSig);
		}
	};

#endif
}


#endif // __TINY_TOOLKIT__SYSTEM__SIGNAL__H__
