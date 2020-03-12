#ifndef __TINY_TOOLKIT__LOG__DAILY_FILE_SINK__H__
#define __TINY_TOOLKIT__LOG__DAILY_FILE_SINK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 时间文件节点
 *
 */


#include "file.h"
#include "sink.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <ctime>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#
#
#endif


namespace tinyToolkit
{
	namespace log
	{
		class TINY_TOOLKIT_API DailyFileSink : public ISink
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param name 名称
			 * @param path 路径
			 * @param hour 小时
			 * @param minutes 分钟
			 * @param seconds 秒数
			 *
			 */
			DailyFileSink(std::string name, std::string path, int32_t hour = 0, int32_t minutes = 0, int32_t seconds = 0);

			/**
			 *
			 * 析构函数
			 *
			 */
			~DailyFileSink() override;

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
			/**
			 *
			 * 刷新下次生成的时间
			 *
			 */
			void UpdateTime();

			/**
			 *
			 * 格式化路径
			 *
			 * @return 路径
			 *
			 */
			std::string FormatPath();

		private:
			File _file{ };

			int32_t _hour{ 0 };
			int32_t _minutes{ 0 };
			int32_t _seconds{ 0 };

			std::time_t _time{ 0 };

			std::string _path{ };
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__DAILY_FILE_SINK__H__
