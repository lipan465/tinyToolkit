#ifndef __TINY_TOOLKIT__LOG__DAILY_FILE_SINK__H__
#define __TINY_TOOLKIT__LOG__DAILY_FILE_SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志时间文件节点
 *
 */


#include "file.h"
#include "sink.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API DailyFileLogSink : public ILogSink
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param name 节点名称
		 * @param path 日志路径
		 * @param hour 时
		 * @param minutes 分
		 * @param seconds 秒
		 *
		 */
		explicit DailyFileLogSink(std::string name, std::string path, int32_t hour = 0, int32_t minutes = 0, int32_t seconds = 0);

		/**
		 *
		 * 析构函数
		 *
		 */
		~DailyFileLogSink() override;

		/**
		 *
		 * 关闭日志
		 *
		 */
		void Close() override;

		/**
		 *
		 * 刷新日志
		 *
		 */
		void Flush() override;

		/**
		 *
		 * 重新打开日志
		 *
		 */
		void Reopen() override;

		/**
		 *
		 * 写入日志
		 *
		 * @param event 日志事件
		 *
		 */
		void Write(const LogEvent & event) override;

		/**
		 *
		 * 是否已经打开
		 *
		 * @return 是否已经打开
		 *
		 */
		bool IsOpen();

		/**
		 *
		 * 日志大小
		 *
		 * @return 日志大小
		 *
		 */
		std::size_t Size() const;

		/**
		 *
		 * 下次生成日志时间
		 *
		 * @return 下次生成日志时间
		 *
		 */
		std::time_t NextTime() const;

		/**
		 *
		 * 日志路径
		 *
		 * @return 日志路径
		 *
		 */
		const std::string & Path() const;

	protected:
		/**
		 *
		 * 计算下一次生成日志的时间
		 *
		 */
		void RotatingTime();

		/**
		 *
		 * 推导日志路径
		 *
		 * @param path 日志路径
		 *
		 * @return 日志路径
		 *
		 */
		std::string CalculatePath(const std::string & path);

	protected:
		LogFile _file{ };

		int32_t _hour{ 0 };
		int32_t _minutes{ 0 };
		int32_t _seconds{ 0 };

		std::time_t _time{ 0 };

		std::string _path{ };
	};
}


#endif // __TINY_TOOLKIT__LOG__DAILY_FILE_SINK__H__
