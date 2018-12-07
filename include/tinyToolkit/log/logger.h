#ifndef __TINY_TOOLKIT__LOG__LOGGER__H__
#define __TINY_TOOLKIT__LOG__LOGGER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志管理器
 *
 */


#include "sink.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API ILogger
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		ILogger() : _name(Application::Name())
		{

		}

		/**
		 *
		 * 构造函数
		 *
		 * @param name 日志名称
		 *
		 */
		explicit ILogger(std::string name) : _name(std::move(name))
		{

		}

		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~ILogger() = default;

		/**
		 *
		 * 等待日志写入
		 *
		 */
		virtual void Wait() = 0;

		/**
		 *
		 * 写入节点
		 *
		 * @param message 日志信息
		 *
		 */
		void Debug(const std::string & message)
		{
			Write(LOG_PRIORITY_TYPE::DEBUGS, message);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template<typename... Args>
		void Debug(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::DEBUGS, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @param message 日志信息
		 *
		 */
		void Info(const std::string & message)
		{
			Write(LOG_PRIORITY_TYPE::INFO, message);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template<typename... Args>
		void Info(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::INFO, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @param message 日志信息
		 *
		 */
		void Notice(const std::string & message)
		{
			Write(LOG_PRIORITY_TYPE::NOTICE, message);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template<typename... Args>
		void Notice(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::NOTICE, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @param message 日志信息
		 *
		 */
		void Warning(const std::string & message)
		{
			Write(LOG_PRIORITY_TYPE::WARNING, message);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template<typename... Args>
		void Warning(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::WARNING, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @param message 日志信息
		 *
		 */
		void Error(const std::string & message)
		{
			Write(LOG_PRIORITY_TYPE::ERROR, message);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template<typename... Args>
		void Error(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::ERROR, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @param message 日志信息
		 *
		 */
		void Critical(const std::string & message)
		{
			Write(LOG_PRIORITY_TYPE::CRITICAL, message);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template<typename... Args>
		void Critical(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::CRITICAL, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @param message 日志信息
		 *
		 */
		void Alert(const std::string & message)
		{
			Write(LOG_PRIORITY_TYPE::ALERT, message);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template<typename... Args>
		void Alert(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::ALERT, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @param message 日志信息
		 *
		 */
		void Fatal(const std::string & message)
		{
			Write(LOG_PRIORITY_TYPE::FATAL, message);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template<typename... Args>
		void Fatal(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::FATAL, fmt, std::forward<Args>(args)...);
		}
		/**
		 *
		 * 写入节点
		 *
		 * @param message 日志信息
		 *
		 */
		void Emerg(const std::string & message)
		{
			Write(LOG_PRIORITY_TYPE::EMERG, message);
		}

		/**
		 *
		 * 写入节点
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template<typename... Args>
		void Emerg(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::EMERG, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 删除节点
		 *
		 * @param name 节点名称
		 *
		 */
		void DelSink(const std::string & name)
		{
			std::lock_guard<std::mutex> lock(_mutex);

			_container.erase(name);
		}

		/**
		 *
		 * 删除节点
		 *
		 * @param sink 节点对象
		 *
		 */
		void DelSink(const std::shared_ptr<ILogSink> & sink)
		{
			std::lock_guard<std::mutex> lock(_mutex);

			_container.erase(sink->Name());
		}

		/**
		 *
		 * 添加日志节点
		 *
		 * @param sink 日志节点
		 *
		 */
		void AddSink(const std::shared_ptr<ILogSink> & sink)
		{
			std::lock_guard<std::mutex> lock(_mutex);

			_container.insert(std::make_pair(sink->Name(), sink));
		}

		/**
		 *
		 * 添加日志节点
		 *
		 * @tparam SinkTypeT [sink types]
		 * @tparam Args [all types]
		 *
		 * @param name 节点名称
		 * @param args 节点参数
		 *
		 */
		template<class SinkTypeT, typename... Args>
		void AddSink(const std::string & name, Args &&... args)
		{
			std::lock_guard<std::mutex> lock(_mutex);

			_container.insert(std::make_pair(name, std::make_shared<SinkTypeT>(name, std::forward<Args>(args)...)));
		}

		/**
		 *
		 * 关闭节点
		 *
		 */
		void CloseSink()
		{
			std::lock_guard<std::mutex> lock(_mutex);

			for (auto &iter : _container)
			{
				iter.second->Close();
			}
		}

		/**
		 *
		 * 刷新节点
		 *
		 */
		void FlushSink()
		{
			std::lock_guard<std::mutex> lock(_mutex);

			for (auto &iter : _container)
			{
				iter.second->Flush();
			}
		}

		/**
		 *
		 * 重新打开节点
		 *
		 */
		void ReopenSink()
		{
			std::lock_guard<std::mutex> lock(_mutex);

			for (auto &iter : _container)
			{
				iter.second->Reopen();
			}
		}

		/**
		 *
		 * 设置日志节点过滤器
		 *
		 * @param filter 日志节点过滤器
		 *
		 */
		void SetSinkFilter(const std::shared_ptr<ILogFilter> & filter)
		{
			std::lock_guard<std::mutex> lock(_mutex);

			for (auto &iter : _container)
			{
				iter.second->SetFilter(filter);
			}
		}

		/**
		 *
		 * 设置日志节点布局
		 *
		 * @param layout 日志节点布局
		 *
		 */
		void SetSinkLayout(const std::shared_ptr<ILogLayout> & layout)
		{
			std::lock_guard<std::mutex> lock(_mutex);

			for (auto &iter : _container)
			{
				iter.second->SetLayout(layout);
			}
		}

		/**
		 *
		 * 消息名称
		 *
		 * @return 消息名称
		 *
		 */
		const std::string & Name()
		{
			return _name;
		}

		/**
		 *
		 * 日志节点
		 *
		 * @param name 节点名称
		 *
		 * @return 日志节点
		 *
		 */
		std::shared_ptr<ILogSink> & FindSink(const std::string & name)
		{
			std::lock_guard<std::mutex> lock(_mutex);

			static std::shared_ptr<ILogSink> nullSink = nullptr;

			auto iter = _container.find(name);

			return iter == _container.end() ? nullSink : _container[name];
		}

	protected:
		/**
		 *
		 * 写入节点
		 *
		 * @param event 日志事件
		 *
		 */
		virtual void Write(LogEvent & event) = 0;

		/**
		 *
		 * 写入节点
		 *
		 * @param priority 日志优先级
		 * @param message 日志信息
		 *
		 */
		void Write(LOG_PRIORITY_TYPE priority, const std::string & message)
		{
			LogEvent event(_name, message, priority);

			Write(event);
		}

		/**
		  *
		  * 写入节点
		  *
		  * @tparam Args [all built-in types]
		  *
		  * @param priority 日志优先级
		  * @param fmt 日志信息格式
		  * @param args 日志信息参数
		  *
		  */
		template<typename... Args>
		void Write(LOG_PRIORITY_TYPE priority, const char * fmt, Args &&... args)
		{
			LogEvent event(_name, String::Format(fmt, std::forward<Args>(args)...), priority);

			Write(event);
		}

	protected:
		std::mutex _mutex{ };

		std::string _name{ };

		std::unordered_map<std::string, std::shared_ptr<ILogSink>> _container{ };
	};

	class TINY_TOOLKIT_API LoggerHelper
	{
	public:
		/**
		 *
		 * 写入日志
		 *
		 * @param logger 日志对象
		 * @param message 日志信息
		 *
		 */
		static void Debug(ILogger & logger, const std::string & message)
		{
			logger.Debug(message);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param logger 日志对象
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template <typename... Args>
		static void Debug(ILogger & logger, const char * fmt, Args &&... args)
		{
			logger.Debug(fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @param logger 日志对象
		 * @param message 日志信息
		 *
		 */
		static void Info(ILogger & logger, const std::string & message)
		{
			logger.Info(message);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param logger 日志对象
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template <typename... Args>
		static void Info(ILogger & logger, const char * fmt, Args &&... args)
		{
			logger.Info(fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @param logger 日志对象
		 * @param message 日志信息
		 *
		 */
		static void Notice(ILogger & logger, const std::string & message)
		{
			logger.Notice(message);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param logger 日志对象
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template <typename... Args>
		static void Notice(ILogger & logger, const char * fmt, Args &&... args)
		{
			logger.Notice(fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @param logger 日志对象
		 * @param message 日志信息
		 *
		 */
		static void Warning(ILogger & logger, const std::string & message)
		{
			logger.Warning(message);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param logger 日志对象
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template <typename... Args>
		static void Warning(ILogger & logger, const char * fmt, Args &&... args)
		{
			logger.Warning(fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @param logger 日志对象
		 * @param message 日志信息
		 *
		 */
		static void Error(ILogger & logger, const std::string & message)
		{
			logger.Error(message);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param logger 日志对象
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template <typename... Args>
		static void Error(ILogger & logger, const char * fmt, Args &&... args)
		{
			logger.Error(fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @param logger 日志对象
		 * @param message 日志信息
		 *
		 */
		static void Critical(ILogger & logger, const std::string & message)
		{
			logger.Critical(message);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param logger 日志对象
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template <typename... Args>
		static void Critical(ILogger & logger, const char * fmt, Args &&... args)
		{
			logger.Critical(fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @param logger 日志对象
		 * @param message 日志信息
		 *
		 */
		static void Alert(ILogger & logger, const std::string & message)
		{
			logger.Alert(message);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param logger 日志对象
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template <typename... Args>
		static void Alert(ILogger & logger, const char * fmt, Args &&... args)
		{
			logger.Alert(fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @param logger 日志对象
		 * @param message 日志信息
		 *
		 */
		static void Fatal(ILogger & logger, const std::string & message)
		{
			logger.Fatal(message);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param logger 日志对象
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template <typename... Args>
		static void Fatal(ILogger & logger, const char * fmt, Args &&... args)
		{
			logger.Fatal(fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @param logger 日志对象
		 * @param message 日志信息
		 *
		 */
		static void Emerg(ILogger & logger, const std::string & message)
		{
			logger.Emerg(message);
		}

		/**
		 *
		 * 写入日志
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param logger 日志对象
		 * @param fmt 日志信息格式
		 * @param args 日志信息参数
		 *
		 */
		template <typename... Args>
		static void Emerg(ILogger & logger, const char * fmt, Args &&... args)
		{
			logger.Emerg(fmt, std::forward<Args>(args)...);
		}
	};
};


#define TINY_TOOLKIT_LOG_DEBUG(logger, fmt, ...)		tinyToolkit::LoggerHelper::Debug(logger, fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_LOG_INFO(logger, fmt, ...)			tinyToolkit::LoggerHelper::Info(logger, fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_LOG_NOTICE(logger, fmt, ...)		tinyToolkit::LoggerHelper::Notice(logger, fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_LOG_WARNING(logger, fmt, ...)		tinyToolkit::LoggerHelper::Warning(logger, fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_LOG_ERROR(logger, fmt, ...)		tinyToolkit::LoggerHelper::Error(logger, fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_LOG_CRITICAL(logger, fmt, ...)		tinyToolkit::LoggerHelper::Critical(logger, fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_LOG_ALERT(logger, fmt, ...)		tinyToolkit::LoggerHelper::Alert(logger, fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_LOG_FATAL(logger, fmt, ...)		tinyToolkit::LoggerHelper::Fatal(logger, fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_LOG_EMERG(logger, fmt, ...)		tinyToolkit::LoggerHelper::Emerg(logger, fmt, ##__VA_ARGS__);


#define TINY_TOOLKIT_SYNC_LOG_DEBUG(fmt, ...)			tinyToolkit::LoggerHelper::Debug(tinyToolkit::SyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_SYNC_LOG_INFO(fmt, ...)			tinyToolkit::LoggerHelper::Info(tinyToolkit::SyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_SYNC_LOG_NOTICE(fmt, ...)			tinyToolkit::LoggerHelper::Notice(tinyToolkit::SyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_SYNC_LOG_WARNING(fmt, ...)			tinyToolkit::LoggerHelper::Warning(tinyToolkit::SyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_SYNC_LOG_ERROR(fmt, ...)			tinyToolkit::LoggerHelper::Error(tinyToolkit::SyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_SYNC_LOG_CRITICAL(fmt, ...)		tinyToolkit::LoggerHelper::Critical(tinyToolkit::SyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_SYNC_LOG_ALERT(fmt, ...)			tinyToolkit::LoggerHelper::Alert(tinyToolkit::SyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_SYNC_LOG_FATAL(fmt, ...)			tinyToolkit::LoggerHelper::Fatal(tinyToolkit::SyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_SYNC_LOG_EMERG(fmt, ...)			tinyToolkit::LoggerHelper::Emerg(tinyToolkit::SyncLogger::Instance(), fmt, ##__VA_ARGS__);


#define TINY_TOOLKIT_ASYNC_LOG_DEBUG(fmt, ...)			tinyToolkit::LoggerHelper::Debug(tinyToolkit::AsyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_ASYNC_LOG_INFO(fmt, ...)			tinyToolkit::LoggerHelper::Info(tinyToolkit::AsyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_ASYNC_LOG_NOTICE(fmt, ...)			tinyToolkit::LoggerHelper::Notice(tinyToolkit::AsyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_ASYNC_LOG_WARNING(fmt, ...)		tinyToolkit::LoggerHelper::Warning(tinyToolkit::AsyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_ASYNC_LOG_ERROR(fmt, ...)			tinyToolkit::LoggerHelper::Error(tinyToolkit::AsyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_ASYNC_LOG_CRITICAL(fmt, ...)		tinyToolkit::LoggerHelper::Critical(tinyToolkit::AsyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_ASYNC_LOG_ALERT(fmt, ...)			tinyToolkit::LoggerHelper::Alert(tinyToolkit::AsyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_ASYNC_LOG_FATAL(fmt, ...)			tinyToolkit::LoggerHelper::Fatal(tinyToolkit::AsyncLogger::Instance(), fmt, ##__VA_ARGS__);
#define TINY_TOOLKIT_ASYNC_LOG_EMERG(fmt, ...)			tinyToolkit::LoggerHelper::Emerg(tinyToolkit::AsyncLogger::Instance(), fmt, ##__VA_ARGS__);


#endif // __TINY_TOOLKIT__LOG__LOGGER__H__
