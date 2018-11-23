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

			_status = true;
			_thread = std::thread(&ApplicationThread::ThreadProcess, this);

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
				if (_thread.joinable())
				{
					_status = false;

					_thread.join();
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
				TINY_TOOLKIT_YIELD()
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
			return _status;
		}

	protected:
		/**
		 *
		 * app线程逻辑函数
		 *
		 */
		virtual void ThreadProcess() = 0;

	private:
		bool _status{ false };

		std::thread _thread;
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__THREAD__H__
