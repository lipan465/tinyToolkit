/**
 *
 *  作者: hm
 *
 *  说明: 线程操作
 *
 */


#include "thread.h"


namespace tinyToolkit
{
	/**
	 *
	 * 析构函数
	 *
	 */
	ApplicationThread::~ApplicationThread()
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
	bool ApplicationThread::StartApplicationThread()
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
	void ApplicationThread::StopApplicationThread()
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
	void ApplicationThread::WaitApplicationThread() const
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
	bool ApplicationThread::ApplicationThreadStatus() const
	{
		return _status;
	}
}
