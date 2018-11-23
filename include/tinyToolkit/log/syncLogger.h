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
		 * 获取单例
		 *
		 * @return 单例对象引用
		 *
		 */
		static SyncLogger & Instance()
		{
			return Singleton<SyncLogger>::Instance();
		}

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
		void Write(LogEvent & event) override
		{
			std::lock_guard<std::mutex> lock(_mutex);

			event.time = Time::TimePoint();

			std::time_t second = Time::Seconds(event.time);

			if (second == _second)  /// 同一秒生成的日志
			{
				/// 不需要处理
			}
			else if (second / 60 == _minutes)  /// 同一分钟生成的日志
			{
				_second = second;

				_tm.tm_sec = static_cast<int32_t>(_second % 60);  /// 更新秒
			}
			else  /// 不同分钟生成的日志
			{
				_minutes = second / 60;

				Time::LocalTm(second, _tm);  /// 重新处理
			}

			event.tm = _tm;

			for (auto &iter : _container)
			{
				iter.second->Write(event);
			}
		}

	protected:
		std::tm _tm{ };
		std::time_t _second{ 0 };
		std::time_t _minutes{ 0 };
	};
}


#endif // __TINY_TOOLKIT__LOG__SYNC_LOGGER__H__
