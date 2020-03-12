#ifndef __TINY_TOOLKIT__LOG__SYSLOG_SINK__H__
#define __TINY_TOOLKIT__LOG__SYSLOG_SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 系统节点
 *
 */


#include "sink.h"


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API SyslogSink : public ISink
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param name 名称
			 *
			 */
			explicit SyslogSink(std::string name);

			/**
			 *
			 * 析构函数
			 *
			 */
			~SyslogSink() override;

			/**
			 *
			 * 关闭节点
			 *
			 */
			void Close() override;

			/**
			 *
			 * 刷新节点
			 *
			 */
			void Flush() override;

			/**
			 *
			 * 写入内容
			 *
			 * @param context 上下文
			 *
			 */
			void Write(const Context & context) override;
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__SYSLOG_SINK__H__
