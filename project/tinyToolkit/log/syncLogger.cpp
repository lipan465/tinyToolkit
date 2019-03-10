/**
 *
 *  作者: hm
 *
 *  说明: 日志同步管理器
 *
 */


#include "syncLogger.h"

#include "../utilities/time.h"
#include "../utilities/singleton.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 */
	SyncLogger::SyncLogger() : ILogger()
	{

	}

	/**
	 *
	 * 构造函数
	 *
	 * @param name 日志名称
	 *
	 */
	SyncLogger::SyncLogger(std::string name) : ILogger(std::move(name))
	{

	}

	/**
	 *
	 * 获取单例
	 *
	 * @return 单例对象引用
	 *
	 */
	SyncLogger & SyncLogger::Instance()
	{
		return Singleton<SyncLogger>::Instance();
	}

	/**
	 *
	 * 等待日志写入
	 *
	 */
	void SyncLogger::Wait()
	{

	}

	/**
	 *
	 * 写入日志
	 *
	 * @param event 日志事件
	 *
	 */
	void SyncLogger::Write(LogEvent & event)
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
}
