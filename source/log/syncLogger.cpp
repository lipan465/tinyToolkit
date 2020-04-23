/**
 *
 *  作者: hm
 *
 *  说明: 同步日志
 *
 */


#include "syncLogger.h"

#include "../util/time.h"
#include "../util/singleton.h"
#include "../util/application.h"


namespace tinyToolkit
{
	namespace log
	{
		/**
		 *
		 * 构造函数
		 *
		 */
		SyncLogger::SyncLogger() : SyncLogger(util::Application::Name())
		{

		}

		/**
		 *
		 * 构造函数
		 *
		 * @param name 名称
		 *
		 */
		SyncLogger::SyncLogger(std::string name) : ILogger(std::move(name))
		{

		}

		/**
		 *
		 * 单例对象
		 *
		 * @return 单例对象
		 *
		 */
		SyncLogger & SyncLogger::Instance()
		{
			return util::Singleton<SyncLogger>::Instance();
		}

		/**
		 *
		 * 等待
		 *
		 */
		void SyncLogger::Wait()
		{

		}

		/**
		 *
		 * 关闭
		 *
		 */
		void SyncLogger::Close()
		{
			std::lock_guard<std::mutex> lock(_mutex);

			for (auto &iter : _sinks)
			{
				iter.second->Close();
			}
		}

		/**
		 *
		 * 刷新
		 *
		 */
		void SyncLogger::Flush()
		{
			std::lock_guard<std::mutex> lock(_mutex);

			for (auto &iter : _sinks)
			{
				iter.second->Flush();
			}
		}

		/**
		 *
		 * 写入内容
		 *
		 * @param context 上下文
		 *
		 */
		void SyncLogger::Write(Context & context)
		{
			std::lock_guard<std::mutex> lock(_mutex);

			context.time = util::Time::TimePoint();

			std::time_t second = util::Time::Seconds(context.time);

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

				util::Time::LocalTm(second, _tm);  /// 重新处理
			}

			context.tm = _tm;

			for (auto &iter : _sinks)
			{
				iter.second->Write(context);
			}
		}
	}
}
