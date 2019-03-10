#ifndef __TINY_TOOLKIT__LOG__ROTATING_FILE_SINK__H__
#define __TINY_TOOLKIT__LOG__ROTATING_FILE_SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志回卷文件节点
 *
 */


#include "sink.h"
#include "file.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API RotatingFileLogSink : public ILogSink
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param name 节点名称
		 * @param path 日志路径
		 * @param limit 日志大小限制
		 * @param count 日志个数限制
		 *
		 */
		explicit RotatingFileLogSink(std::string name, std::string path, std::size_t limit = TINY_TOOLKIT_GB, std::size_t count = 10);

		/**
		 *
		 * 析构函数
		 *
		 */
		~RotatingFileLogSink() override;

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
		 * 日志路径
		 *
		 * @return 日志路径
		 *
		 */
		const std::string & Path() const;

	protected:
		/**
		 *
		 * 轮转日志
		 *
		 */
		void Rotating();

		/**
		 *
		 * 推导日志路径
		 *
		 * @param path 日志路径
		 * @param index 索引
		 *
		 * @return 日志路径
		 *
		 */
		std::string CalculatePath(const std::string & path, std::size_t index = 0);

	protected:
		LogFile _file{ };

		std::size_t _count{ 10 };
		std::size_t _limit{ TINY_TOOLKIT_GB };

		std::string _path{ };
	};
}


#endif // __TINY_TOOLKIT__LOG__ROTATING_FILE_SINK__H__
