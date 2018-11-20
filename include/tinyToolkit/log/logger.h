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
	template <typename MutexTypeT>
	class TINY_TOOLKIT_API ILogger
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		ILogger() : _name(tinyToolkit::Application::Name())
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
		 * 关闭节点
		 *
		 */
		virtual void Close() = 0;

		/**
		 *
		 * 清空节点
		 *
		 */
		virtual void Clear() = 0;

		/**
		 *
		 * 刷新节点
		 *
		 */
		virtual void Flush() = 0;

		/**
		 *
		 * 重新打开节点
		 *
		 */
		virtual void Reopen() = 0;

		/**
		 *
		 * 写入节点
		 *
		 * @param message 日志信息
		 *
		 */
		void Debug(const std::string & message)
		{
			std::lock_guard<MutexTypeT> lock(_mutex);

			Write(LOG_PRIORITY_TYPE::DEBUG, message);
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
			std::lock_guard<MutexTypeT> lock(_mutex);

			Write(LOG_PRIORITY_TYPE::DEBUG, fmt, std::forward<Args>(args)...);
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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

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
			std::lock_guard<MutexTypeT> lock(_mutex);

			Write(LOG_PRIORITY_TYPE::EMERG, fmt, std::forward<Args>(args)...);
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
			std::lock_guard<MutexTypeT> lock(_mutex);

			_container.insert(std::make_pair(name, std::make_shared<SinkTypeT>(name, std::forward<Args>(args)...)));
		}

		/**
		 *
		 * 添加日志节点
		 *
		 * @tparam It [container iterator]
		 *
		 * @param begin 起始迭代器
		 * @param end 结尾迭代器
		 *
		 */
		template<class It>
		void AddSink(const It & begin, const It & end)
		{
			std::lock_guard<MutexTypeT> lock(_mutex);

			_container.insert(begin, end);
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
			std::lock_guard<MutexTypeT> lock(_mutex);

			_container.insert(std::make_pair(sink->Name(), sink));
		}

		/**
		 *
		 * 删除节点
		 *
		 * @param name 节点名称
		 *
		 */
		void EraseSink(const std::string & name)
		{
			std::lock_guard<MutexTypeT> lock(_mutex);

			_container.erase(name);
		}

		/**
		 *
		 * 设置日志过滤器
		 *
		 * @param filter 日志过滤器
		 *
		 */
		void SetFilter(const std::shared_ptr<ILogFilter> & filter)
		{
			std::lock_guard<MutexTypeT> lock(_mutex);

			_filter = filter;
		}

		/**
		 *
		 * 消息名称
		 *
		 * @return 消息名称
		 *
		 */
		const std::string & Name() const
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
		std::shared_ptr<ILogSink> & Sink(const std::string & name)
		{
			std::lock_guard<MutexTypeT> lock(_mutex);

			return _container[name];
		}

		/**
		 *
		 * 日志过滤器
		 *
		 * @return 日志过滤器
		 *
		 */
		std::shared_ptr<ILogFilter> & Filter()
		{
			std::lock_guard<MutexTypeT> lock(_mutex);

			return _filter;
		}

	protected:
		/**
		 *
		 * 写入节点
		 *
		 * @param event 日志事件
		 *
		 */
		virtual void Write(const LogEvent & event) = 0;

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

			tinyToolkit::Time::LocalTm(tinyToolkit::Time::Seconds(event.time), event.tm);

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
			LogEvent event(_name, tinyToolkit::String::Format(fmt, std::forward<Args>(args)...), priority);

			tinyToolkit::Time::LocalTm(tinyToolkit::Time::Seconds(event.time), event.tm);

			Write(event);
		}

	protected:
		MutexTypeT _mutex;

		std::string _name{ };

		std::shared_ptr<ILogFilter> _filter;

		std::unordered_map<std::string, std::shared_ptr<ILogSink>> _container{ };
	};
}


#endif // __TINY_TOOLKIT__LOG__LOGGER__H__
