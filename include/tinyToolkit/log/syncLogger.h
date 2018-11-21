#ifndef __TINY_TOOLKIT__LOG__SYNC_LOGGER__H__
#define __TINY_TOOLKIT__LOG__SYNC_LOGGER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志同步管理器
 *
 */


#include "logger.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API SyncLogger : public ILogger
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		SyncLogger() : ILogger()
		{

		}

		/**
		 *
		 * 构造函数
		 *
		 * @param name 日志名称
		 *
		 */
		explicit SyncLogger(std::string name) : ILogger(std::move(name))
		{

		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~SyncLogger() override = default;

		/**
		 *
		 * 等待日志写入
		 *
		 */
		void Wait() override
		{

		}

	protected:
		/**
		 *
		 * 写入日志
		 *
		 * @param event 日志事件
		 *
		 */
		void Write(const LogEvent & event) override
		{
			std::lock_guard<std::mutex> lock(_mutex);

			for (auto &iter : _container)
			{
				iter.second->Write(event);
			}
		}
	};
}


#endif // __TINY_TOOLKIT__LOG__SYNC_LOGGER__H__
