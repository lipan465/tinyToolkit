/**
 *
 *  作者: hm
 *
 *  说明: 控制台节点
 *
 */


#include "consoleSink.h"


namespace tinyToolkit
{
	namespace log
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param name 名称
		 * @param file 句柄
		 *
		 */
		ConsoleSink::ConsoleSink(std::string name, FILE * file) : ISink(std::move(name)),
		                                                          _file(file)
		{

		}

		/**
		 *
		 * 析构函数
		 *
		 */
		ConsoleSink::~ConsoleSink()
		{
			if (_file)
			{
				fflush(_file);
			}
		}

		/**
		 *
		 * 关闭节点
		 *
		 */
		void ConsoleSink::Close()
		{
			Flush();
		}

		/**
		 *
		 * 刷新节点
		 *
		 */
		void ConsoleSink::Flush()
		{
			if (_file)
			{
				fflush(_file);
			}
		}

		/**
		 *
		 * 写入内容
		 *
		 * @param context 上下文
		 *
		 */
		void ConsoleSink::Write(const Context & context)
		{
			if (_file == nullptr)
			{
				return;
			}

			if (Filter() && Filter()->Decide(context))
			{
				return;
			}

			std::string value(Layout() ? Layout()->Format(context) : context.content);

			::fwrite(value.c_str(), sizeof(char), value.size(), _file);

			if (IsAutoFlush())
			{
				Flush();
			}
		}
	}
}
