#ifndef __TINY_TOOLKIT__LOG__FILE_SINK__H__
#define __TINY_TOOLKIT__LOG__FILE_SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 文件节点
 *
 */


#include "sink.h"
#include "file.h"


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API FileSink : public ISink
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param name 名称
			 * @param path 路径
			 * @param truncate 是否截断
			 *
			 */
			FileSink(std::string name, std::string path, bool truncate = false);

			/**
			 *
			 * 析构函数
			 *
			 */
			~FileSink() override;

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
			File _file{ };

			std::string _path{ };
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__FILE_SINK__H__
