#ifndef __TINY_TOOLKIT__LOG__LOGGER__H__
#define __TINY_TOOLKIT__LOG__LOGGER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志
 *
 */


#include "sink.h"

#include "../util/string.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <mutex>
#  include <unordered_map>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <unordered_map>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <mutex>
#  include <unordered_map>
#
#endif


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API ILogger
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param name 名称
			 *
			 */
			explicit ILogger(std::string name);

			/**
			 *
			 * 析构函数
			 *
			 */
			virtual ~ILogger() = default;

			/**
			 *
			 * 等待
			 *
			 */
			virtual void Wait() = 0;

			/**
			 *
			 * 关闭
			 *
			 */
			virtual void Close() = 0;

			/**
			 *
			 * 刷新
			 *
			 */
			virtual void Flush() = 0;

			/**
			 *
			 * 写入内容
			 *
			 * @param context 上下文
			 *
			 */
			virtual void Write(Context & context) = 0;

			/**
			 *
			 * 写入内容
			 *
			 * @param option 操作
			 *
			 */
			void Write(LOG_OPTION_TYPE option);

			/**
			 *
			 * 写入内容
			 *
			 * @param priority 优先级
			 * @param content 内容
			 *
			 */
			void Write(LOG_PRIORITY_TYPE priority, const std::string & content);

			/**
			 *
			 * 写入内容
			 *
			 * @param priority 优先级
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template<typename... Args>
			void Write(LOG_PRIORITY_TYPE priority, const char * fmt, Args &&... args)
			{
				Write(priority, util::String::Format(fmt, std::forward<Args>(args)...));
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param content 内容
			 *
			 */
			void Debug(const std::string & content);

			/**
			 *
			 * 写入内容
			 *
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template<typename... Args>
			void Debug(const char * fmt, Args &&... args)
			{
				Write(LOG_PRIORITY_TYPE::DEBUGS, fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param content 内容
			 *
			 */
			void Info(const std::string & content);

			/**
			 *
			 * 写入内容
			 *
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template<typename... Args>
			void Info(const char * fmt, Args &&... args)
			{
				Write(LOG_PRIORITY_TYPE::INFO, fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param content 内容
			 *
			 */
			void Notice(const std::string & content);

			/**
			 *
			 * 写入内容
			 *
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template<typename... Args>
			void Notice(const char * fmt, Args &&... args)
			{
				Write(LOG_PRIORITY_TYPE::NOTICE, fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param content 内容
			 *
			 */
			void Warning(const std::string & content);

			/**
			 *
			 * 写入内容
			 *
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template<typename... Args>
			void Warning(const char * fmt, Args &&... args)
			{
				Write(LOG_PRIORITY_TYPE::WARNING, fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param content 内容
			 *
			 */
			void Error(const std::string & content);

			/**
			 *
			 * 写入内容
			 *
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template<typename... Args>
			void Error(const char * fmt, Args &&... args)
			{
				Write(LOG_PRIORITY_TYPE::ERRORS, fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param content 内容
			 *
			 */
			void Critical(const std::string & content);

			/**
			 *
			 * 写入内容
			 *
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template<typename... Args>
			void Critical(const char * fmt, Args &&... args)
			{
				Write(LOG_PRIORITY_TYPE::CRITICAL, fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param content 内容
			 *
			 */
			void Alert(const std::string & content);

			/**
			 *
			 * 写入内容
			 *
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template<typename... Args>
			void Alert(const char * fmt, Args &&... args)
			{
				Write(LOG_PRIORITY_TYPE::ALERT, fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param content 内容
			 *
			 */
			void Fatal(const std::string & content);

			/**
			 *
			 * 写入内容
			 *
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template<typename... Args>
			void Fatal(const char * fmt, Args &&... args)
			{
				Write(LOG_PRIORITY_TYPE::FATAL, fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param content 内容
			 *
			 */
			void Emerg(const std::string & content);

			/**
			 *
			 * 写入内容
			 *
			 * @tparam Args [all built-in types]
			 *
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template<typename... Args>
			void Emerg(const char * fmt, Args &&... args)
			{
				Write(LOG_PRIORITY_TYPE::EMERG, fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 添加节点
			 *
			 * @param sink 节点
			 *
			 */
			void AddSink(const std::shared_ptr<ISink> & sink);

			/**
			 *
			 * 添加节点
			 *
			 * @param name 名称
			 * @param args 参数
			 *
			 */
			template<class SinkTypeT, typename... Args>
			void AddSink(const std::string & name, Args &&... args)
			{
				AddSink(std::make_shared<SinkTypeT>(name, std::forward<Args>(args)...));
			}

			/**
			 *
			 * 移除节点
			 *
			 * @param name 节点名称
			 *
			 */
			void RemoveSink(const std::string & name);

			/**
			 *
			 * 移除节点
			 *
			 * @param sink 节点对象
			 *
			 */
			void RemoveSink(const std::shared_ptr<ISink> & sink);

			/**
			 *
			 * 名称
			 *
			 * @return 名称
			 *
			 */
			const std::string & Name() const;

		protected:
			std::mutex _mutex{ };

			std::string _name{ };

			std::unordered_map<std::string, std::shared_ptr<ISink>> _sinks{ };
		};

		class TINY_TOOLKIT_API LoggerHelper
		{
		public:
			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Debug(ILogger & logger, const std::string & content)
			{
				logger.Debug(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Debug(ILogger & logger, const char * fmt, Args &&... args)
			{
				logger.Debug(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Debug(ILogger * logger, const std::string & content)
			{
				logger->Debug(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Debug(ILogger * logger, const char * fmt, Args &&... args)
			{
				logger->Debug(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入日志
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Debug(std::shared_ptr<ILogger> & logger, const std::string & content)
			{
				logger->Debug(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Debug(std::shared_ptr<ILogger> & logger, const char * fmt, Args &&... args)
			{
				logger->Debug(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Info(ILogger & logger, const std::string & content)
			{
				logger.Info(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Info(ILogger & logger, const char * fmt, Args &&... args)
			{
				logger.Info(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Info(ILogger * logger, const std::string & content)
			{
				logger->Info(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Info(ILogger * logger, const char * fmt, Args &&... args)
			{
				logger->Info(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Info(std::shared_ptr<ILogger> & logger, const std::string & content)
			{
				logger->Info(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Info(std::shared_ptr<ILogger> & logger, const char * fmt, Args &&... args)
			{
				logger->Info(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Notice(ILogger & logger, const std::string & content)
			{
				logger.Notice(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Notice(ILogger & logger, const char * fmt, Args &&... args)
			{
				logger.Notice(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Notice(ILogger * logger, const std::string & content)
			{
				logger->Notice(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Notice(ILogger * logger, const char * fmt, Args &&... args)
			{
				logger->Notice(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Notice(std::shared_ptr<ILogger> & logger, const std::string & content)
			{
				logger->Notice(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Notice(std::shared_ptr<ILogger> & logger, const char * fmt, Args &&... args)
			{
				logger->Notice(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Warning(ILogger & logger, const std::string & content)
			{
				logger.Warning(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Warning(ILogger & logger, const char * fmt, Args &&... args)
			{
				logger.Warning(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Warning(ILogger * logger, const std::string & content)
			{
				logger->Warning(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Warning(ILogger * logger, const char * fmt, Args &&... args)
			{
				logger->Warning(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Warning(std::shared_ptr<ILogger> & logger, const std::string & content)
			{
				logger->Warning(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Warning(std::shared_ptr<ILogger> & logger, const char * fmt, Args &&... args)
			{
				logger->Warning(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Error(ILogger & logger, const std::string & content)
			{
				logger.Error(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Error(ILogger & logger, const char * fmt, Args &&... args)
			{
				logger.Error(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Error(ILogger * logger, const std::string & content)
			{
				logger->Error(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Error(ILogger * logger, const char * fmt, Args &&... args)
			{
				logger->Error(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Error(std::shared_ptr<ILogger> & logger, const std::string & content)
			{
				logger->Error(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Error(std::shared_ptr<ILogger> & logger, const char * fmt, Args &&... args)
			{
				logger->Error(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Critical(ILogger & logger, const std::string & content)
			{
				logger.Critical(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Critical(ILogger & logger, const char * fmt, Args &&... args)
			{
				logger.Critical(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Critical(ILogger * logger, const std::string & content)
			{
				logger->Critical(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Critical(ILogger * logger, const char * fmt, Args &&... args)
			{
				logger->Critical(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Critical(std::shared_ptr<ILogger> & logger, const std::string & content)
			{
				logger->Critical(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Critical(std::shared_ptr<ILogger> & logger, const char * fmt, Args &&... args)
			{
				logger->Critical(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Alert(ILogger & logger, const std::string & content)
			{
				logger.Alert(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Alert(ILogger & logger, const char * fmt, Args &&... args)
			{
				logger.Alert(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Alert(ILogger * logger, const std::string & content)
			{
				logger->Alert(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Alert(ILogger * logger, const char * fmt, Args &&... args)
			{
				logger->Alert(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Alert(std::shared_ptr<ILogger> & logger, const std::string & content)
			{
				logger->Alert(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Alert(std::shared_ptr<ILogger> & logger, const char * fmt, Args &&... args)
			{
				logger->Alert(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Fatal(ILogger & logger, const std::string & content)
			{
				logger.Fatal(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Fatal(ILogger & logger, const char * fmt, Args &&... args)
			{
				logger.Fatal(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Fatal(ILogger * logger, const std::string & content)
			{
				logger->Fatal(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Fatal(ILogger * logger, const char * fmt, Args &&... args)
			{
				logger->Fatal(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Fatal(std::shared_ptr<ILogger> & logger, const std::string & content)
			{
				logger->Fatal(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Fatal(std::shared_ptr<ILogger> & logger, const char * fmt, Args &&... args)
			{
				logger->Fatal(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Emerg(ILogger & logger, const std::string & content)
			{
				logger.Emerg(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Emerg(ILogger & logger, const char * fmt, Args &&... args)
			{
				logger.Emerg(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Emerg(ILogger * logger, const std::string & content)
			{
				logger->Emerg(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Emerg(ILogger * logger, const char * fmt, Args &&... args)
			{
				logger->Emerg(fmt, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param content 内容
			 *
			 */
			static void Emerg(std::shared_ptr<ILogger> & logger, const std::string & content)
			{
				logger->Emerg(content);
			}

			/**
			 *
			 * 写入内容
			 *
			 * @param logger 对象
			 * @param fmt 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Emerg(std::shared_ptr<ILogger> & logger, const char * fmt, Args &&... args)
			{
				logger->Emerg(fmt, std::forward<Args>(args)...);
			}
		};
	}
}


#define TINY_TOOLKIT_LOG_DEBUG(logger, fmt, ...)		tinyToolkit::log::LoggerHelper::Debug(logger, fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_LOG_INFO(logger, fmt, ...)			tinyToolkit::log::LoggerHelper::Info(logger, fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_LOG_NOTICE(logger, fmt, ...)		tinyToolkit::log::LoggerHelper::Notice(logger, fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_LOG_WARNING(logger, fmt, ...)		tinyToolkit::log::LoggerHelper::Warning(logger, fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_LOG_ERROR(logger, fmt, ...)		tinyToolkit::log::LoggerHelper::Error(logger, fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_LOG_CRITICAL(logger, fmt, ...)		tinyToolkit::log::LoggerHelper::Critical(logger, fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_LOG_ALERT(logger, fmt, ...)		tinyToolkit::log::LoggerHelper::Alert(logger, fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_LOG_FATAL(logger, fmt, ...)		tinyToolkit::log::LoggerHelper::Fatal(logger, fmt, ##__VA_ARGS__)
#define TINY_TOOLKIT_LOG_EMERG(logger, fmt, ...)		tinyToolkit::log::LoggerHelper::Emerg(logger, fmt, ##__VA_ARGS__)


#endif // __TINY_TOOLKIT__LOG__LOGGER__H__
