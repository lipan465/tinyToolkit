#ifndef __TINY_TOOLKIT__UTILITIES__THREAD__H__
#define __TINY_TOOLKIT__UTILITIES__THREAD__H__


/**
 *
 *  作者: hm
 *
 *  说明: 线程操作
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API ApplicationThread
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~ApplicationThread()
		{
			StopApplicationThread();
		}

		/**
		 *
		 * 启动线程
		 *
		 * @return 是否启动成功
		 *
		 */
		bool StartApplicationThread()
		{
			if (ApplicationThreadStatus())
			{
				return false;
			}

			_applicationThreadStatus = true;

			_applicationThread = std::thread(&ApplicationThread::ThreadProcess, this);

			return true;
		}

		/**
		 *
		 * 停止线程
		 *
		 */
		void StopApplicationThread()
		{
			if (ApplicationThreadStatus())
			{
				if (_applicationThread.joinable())
				{
					_applicationThreadStatus = false;

					_applicationThread.join();
				}
			}
		}

		/**
		 *
		 * 等待线程执行结束
		 *
		 */
		void WaitApplicationThread() const
		{
			while (ApplicationThreadStatus())
			{
				TINY_TOOLKIT_SLEEP_MS(10);
			}
		}

		/**
		 *
		 * app线程状态
		 *
		 * @return app线程状态
		 *
		 */
		bool ApplicationThreadStatus() const
		{
			return _applicationThreadStatus;
		}

	protected:
		/**
		 *
		 * app线程逻辑函数
		 *
		 */
		virtual void ThreadProcess()
		{
			while (ApplicationThreadStatus())
			{
				TINY_TOOLKIT_SLEEP_S(1)

				break;
			}
		}

	protected:
		bool _applicationThreadStatus{ false };

		std::thread _applicationThread;
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__THREAD__H__
