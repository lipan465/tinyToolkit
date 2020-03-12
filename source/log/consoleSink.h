#ifndef __TINY_TOOLKIT__LOG__CONSOLE_SINK__H__
#define __TINY_TOOLKIT__LOG__CONSOLE_SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 控制台节点
 *
 */


#include "sink.h"


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API ConsoleSink : public ISink
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param name 名称
			 * @param file 句柄
			 *
			 */
			explicit ConsoleSink(std::string name, FILE * file = stdout);

			/**
			 *
			 * 析构函数
			 *
			 */
			~ConsoleSink() override;

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

		private:
			FILE * _file{ nullptr };
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__CONSOLE_SINK__H__
