#ifndef __TINY_TOOLKIT__LOG__SYNC_LOGGER__H__
#define __TINY_TOOLKIT__LOG__SYNC_LOGGER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 同步日志管理器
 *
 */


#include "logger.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API SyncLogger : public ILogger<tinyToolkit::FakeMutex>
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		SyncLogger() = default;

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
		 * 关闭日志
		 *
		 */
		void Close() override
		{
			for (auto &iter : _container)
			{
				iter.second->Close();
			}
		}

		/**
		 *
		 * 清空节点
		 *
		 */
		void Clear() override
		{
			Close();

			tinyToolkit::Container::Clear(_container);
		}

		/**
		 *
		 * 刷新日志
		 *
		 */
		void Flush() override
		{
			for (auto &iter : _container)
			{
				iter.second->Flush();
			}
		}

		/**
		 *
		 * 重新打开日志
		 *
		 */
		void Reopen() override
		{
			for (auto &iter : _container)
			{
				iter.second->Reopen();
			}
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
			for (auto &iter : _container)
			{
				iter.second->Write(event);
			}
		}
	};
}


#endif // __TINY_TOOLKIT__LOG__SYNC_LOGGER__H__
