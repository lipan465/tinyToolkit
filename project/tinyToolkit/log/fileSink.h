#ifndef __TINY_TOOLKIT__LOG__FILE_SINK__H__
#define __TINY_TOOLKIT__LOG__FILE_SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志文件节点
 *
 */


#include "sink.h"
#include "file.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API FileLogSink : public ILogSink
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param name 节点名称
		 * @param path 日志路径
		 * @param truncate 是否覆盖日志
		 *
		 */
		explicit FileLogSink(std::string name, std::string path, bool truncate = false);

		/**
		 *
		 * 析构函数
		 *
		 */
		~FileLogSink() override;

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
		LogFile _file{ };

		std::string _path{ };
	};
}


#endif // __TINY_TOOLKIT__LOG__FILE_SINK__H__
