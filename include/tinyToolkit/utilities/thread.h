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
		virtual ~ApplicationThread();

		/**
		 *
		 * 启动线程
		 *
		 * @return 是否启动成功
		 *
		 */
		bool StartApplicationThread();

		/**
		 *
		 * 停止线程
		 *
		 */
		void StopApplicationThread();

		/**
		 *
		 * 等待线程执行结束
		 *
		 */
		void WaitApplicationThread() const;

		/**
		 *
		 * app线程状态
		 *
		 * @return app线程状态
		 *
		 */
		bool ApplicationThreadStatus() const;

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
