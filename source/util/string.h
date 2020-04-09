#ifndef __TINY_TOOLKIT__UTIL__STRING__H__
#define __TINY_TOOLKIT__UTIL__STRING__H__


/**
 *
 *  作者: hm
 *
 *  说明: 字符串处理
 *
 */


#include <fmt/printf.h>
#include <fmt/format.h>

#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <vector>
#  include <sstream>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <sstream>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <vector>
#  include <sstream>
#
#endif


namespace tinyToolkit
{
	namespace util
	{
		class TINY_TOOLKIT_API String
		{
		public:
			/**
			 *
			 * 格式化输出
			 *
			 * @param format 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Print(const char * format, Args &&... args)
			{
				Print(stdout, format, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 格式化输出
			 *
			 * @param file 文件
			 * @param format 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Print(FILE * file, const char * format, Args &&... args)
			{
				fmt::print(file, format, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 格式化输出
			 *
			 * @param stream 输出流
			 * @param format 格式化
			 * @param args 参数
			 *
			 */
			template <typename... Args>
			static void Print(std::ostream & stream, const char * format, Args &&... args)
			{
				fmt::print(stream, format, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 流对象填充内容
			 *
			 * @param stream 流对象
			 * @param content 内容
			 *
			 */
			template <typename ContentT>
			static void JoinToStream(std::stringstream & stream, ContentT && content)
			{
				stream << std::forward<ContentT>(content);
			}

			/**
			 *
			 * 流对象填充内容
			 *
			 * @param stream 流对象
			 * @param content 内容
			 * @param args 内容
			 *
			 */
			template <typename ContentT, typename... Args>
			static void JoinToStream(std::stringstream & stream, ContentT && content, Args && ... args)
			{
				JoinToStream(stream, std::forward<ContentT>(content));
				JoinToStream(stream, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 转换小写
			 *
			 * @param content 处理内容
			 * @param offset 偏移位置
			 *
			 */
			static void Lower(std::string & content, std::size_t offset = 0);

			/**
			 *
			 * 转换大写
			 *
			 * @param content 处理内容
			 * @param offset 偏移位置
			 *
			 */
			static void Upper(std::string & content, std::size_t offset = 0);

			/**
			 *
			 * 删除字符
			 *
			 * @param content 处理内容
			 * @param key 匹配字符
			 * @param offset 偏移位置
			 *
			 */
			static void Erase(std::string & content, char key, std::size_t offset = 0);

			/**
			 *
			 * 删除内容
			 *
			 * @param content 处理内容
			 * @param key 匹配内容
			 * @param offset 偏移位置
			 *
			 */
			static void Erase(std::string & content, const std::string & key, std::size_t offset = 0);

			/**
			 *
			 * 匹配拆分内容
			 *
			 * @param content 处理内容
			 * @param key 匹配内容
			 * @param container 结果容器
			 * @param keepEmpty 是否保留空串
			 *
			 */
			static void Split(const std::string & content, const std::string & key, std::vector<std::string> & container, bool keepEmpty = false);

			/**
			 *
			 * 按行拆分内容
			 *
			 * @param content 处理内容
			 * @param container 结果容器
			 * @param keepEnter 是否保留换行符
			 *
			 */
			static void SplitLines(const std::string & content, std::vector<std::string> & container, bool keepEnter = false);

			/**
			 *
			 * 替换内容
			 *
			 * @param content 处理内容
			 * @param src 被替换内容
			 * @param dst 待替换内容
			 * @param offset 偏移位置
			 *
			 */
			static void Replace(std::string & content, const std::string & src, const std::string & dst, std::size_t offset = 0);

			/**
			 *
			 * 替换内容
			 *
			 * @param content 处理内容
			 * @param src 被替换内容
			 * @param dst 待替换内容
			 * @param offset 偏移位置
			 *
			 */
			static void ReplaceLoop(std::string & content, const std::string & src, const std::string & dst, std::size_t offset = 0);

			/**
			 *
			 * 是否为ip
			 *
			 * @param content 处理内容
			 *
			 * @return 是否为ip
			 *
			 */
			static bool IsIP(const char * content);

			/**
			 *
			 * 是否为ip
			 *
			 * @param content 处理内容
			 *
			 * @return 是否为ip
			 *
			 */
			static bool IsIP(const std::string & content);

			/**
			 *
			 * 是否包含字符
			 *
			 * @param content 处理内容
			 * @param key 匹配字符
			 * @param offset 偏移位置
			 *
			 * @return 是否包含字符
			 *
			 */
			static bool With(const char * content, char key, std::size_t offset = 0);

			/**
			 *
			 * 是否包含内容
			 *
			 * @param content 处理内容
			 * @param key 匹配内容
			 * @param offset 偏移位置
			 *
			 * @return 是否包含内容
			 *
			 */
			static bool With(const char * content, const char * key, std::size_t offset = 0);

			/**
			 *
			 * 是否包含内容
			 *
			 * @param content 处理内容
			 * @param key 匹配内容
			 * @param offset 偏移位置
			 *
			 * @return 是否包含内容
			 *
			 */
			static bool With(const char * content, const std::string & key, std::size_t offset = 0);

			/**
			 *
			 * 是否包含字符
			 *
			 * @param content 处理内容
			 * @param key 匹配字符
			 * @param offset 偏移位置
			 *
			 * @return 是否包含字符
			 *
			 */
			static bool With(const std::string & content, char key, std::size_t offset = 0);

			/**
			 *
			 * 是否包含内容
			 *
			 * @param content 处理内容
			 * @param key 匹配内容
			 * @param offset 偏移位置
			 *
			 * @return 是否包含内容
			 *
			 */
			static bool With(const std::string & content, const char * key, std::size_t offset = 0);

			/**
			 *
			 * 是否包含内容
			 *
			 * @param content 处理内容
			 * @param key 匹配内容
			 * @param offset 偏移位置
			 *
			 * @return 是否包含内容
			 *
			 */
			static bool With(const std::string & content, const std::string & key, std::size_t offset = 0);

			/**
			 *
			 * 结尾是否符合内容
			 *
			 * @param content 处理内容
			 * @param suffix 匹配内容
			 *
			 * @return 结尾是否符合内容
			 *
			 */
			static bool EndWith(const std::string & content, const std::string & suffix);

			/**
			 *
			 * 开头是否符合内容
			 *
			 * @param content 处理内容
			 * @param prefix 匹配内容
			 *
			 * @return 开头是否符合内容
			 *
			 */
			static bool StartWith(const std::string & content, const std::string & prefix);

			/**
			 *
			 * 填充内容
			 *
			 * @param content 内容
			 * @param args 内容
			 *
			 * @return 填充后内容
			 *
			 */
			template <typename ContentT, typename... Args>
			static std::string Join(ContentT && content, Args && ... args)
			{
				std::stringstream stream{ };

				JoinToStream(stream, std::forward<ContentT>(content), std::forward<Args>(args)...);

				return stream.str();
			}

			/**
			 *
			 * 格式化字符串
			 *
			 * @param format 格式化
			 * @param args 参数
			 *
			 * @return 格式化后字符串
			 *
			 */
			template <typename... Args>
			static std::string Format(const char * format, Args &&... args)
			{
				return fmt::format(format, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 格式化字符串
			 *
			 * @param format 格式化
			 * @param args 参数
			 *
			 * @return 格式化后字符串
			 *
			 */
			template <typename... Args>
			static std::string Format(const std::string & format, Args &&... args)
			{
				return fmt::format(format, std::forward<Args>(args)...);
			}

			/**
			 *
			 * 去除首尾内容
			 *
			 * @param content 处理内容
			 * @param pattern 匹配规则
			 *
			 * @return 去除后内容
			 *
			 */
			static std::string Trim(const std::string & content, const char * pattern = "\r\n\t ");

			/**
			 *
			 * 过滤注释
			 *
			 * @param content 处理内容
			 *
			 * @return 过滤后内容
			 *
			 */
			static std::string FilterNote(const std::string & content);

			/**
			 *
			 * 收缩json
			 *
			 * @param content 处理内容
			 *
			 * @return 收缩后数据
			 *
			 */
			static std::string ReduceJson(const std::string & content);

			/**
			 *
			 * 美化json
			 *
			 * @param content 处理内容
			 *
			 * @return 格式化后数据
			 *
			 */
			static std::string BeautifyJson(const std::string & content);

			/**
			 *
			 * 转换为16进制内容
			 *
			 * @param content 处理内容
			 * @param reverse 是否反向转换
			 *
			 * @return 转换后内容
			 *
			 */
			static std::string AsHexString(const char * content, bool reverse = false);

			/**
			 *
			 * 转换为16进制内容
			 *
			 * @param content 处理内容
			 * @param reverse 是否反向转换
			 *
			 * @return 转换后内容
			 *
			 */
			static std::string AsHexString(const uint8_t * content, bool reverse = false);

			/**
			 *
			 * 转换为16进制内容
			 *
			 * @param content 处理内容
			 * @param reverse 是否反向转换
			 *
			 * @return 转换后内容
			 *
			 */
			static std::string AsHexString(const std::string & content, bool reverse = false);

			/**
			 *
			 * 转换为16进制内容
			 *
			 * @param content 处理内容
			 * @param length 处理长度
			 * @param reverse 是否反向转换
			 *
			 * @return 转换后内容
			 *
			 */
			static std::string AsHexString(const char * content, std::size_t length, bool reverse = false);

			/**
			 *
			 * 转换为16进制内容
			 *
			 * @param content 处理内容
			 * @param length 处理长度
			 * @param reverse 是否反向转换
			 *
			 * @return 转换后内容
			 *
			 */
			static std::string AsHexString(const uint8_t * content, std::size_t length, bool reverse = false);

			/**
			 *
			 * 转换为16进制内容
			 *
			 * @param content 处理内容
			 * @param length 处理长度
			 * @param reverse 是否反向转换
			 *
			 * @return 转换后内容
			 *
			 */
			static std::string AsHexString(const std::string & content, std::size_t length, bool reverse = false);

			/**
			 *
			 * 转换为10进制内容
			 *
			 * @param content 处理内容
			 * @param reverse 是否反向转换
			 *
			 * @return 转换后内容
			 *
			 */
			static std::string FromHexString(const char * content, bool reverse = false);

			/**
			 *
			 * 转换为10进制内容
			 *
			 * @param content 处理内容
			 * @param reverse 是否反向转换
			 *
			 * @return 转换后内容
			 *
			 */
			static std::string FromHexString(const uint8_t * content, bool reverse = false);

			/**
			 *
			 * 转换为10进制内容
			 *
			 * @param content 处理内容
			 * @param reverse 是否反向转换
			 *
			 * @return 转换后内容
			 *
			 */
			static std::string FromHexString(const std::string & content, bool reverse = false);

			/**
			 *
			 * 转换为10进制内容
			 *
			 * @param content 处理内容
			 * @param length 处理长度
			 * @param reverse 是否反向转换
			 *
			 * @return 转换后内容
			 *
			 */
			static std::string FromHexString(const char * content, std::size_t length, bool reverse = false);

			/**
			 *
			 * 转换为10进制内容
			 *
			 * @param content 处理内容
			 * @param length 处理长度
			 * @param reverse 是否反向转换
			 *
			 * @return 转换后内容
			 *
			 */
			static std::string FromHexString(const uint8_t * content, std::size_t length, bool reverse = false);

			/**
			 *
			 * 转换为10进制内容
			 *
			 * @param content 处理内容
			 * @param length 处理长度
			 * @param reverse 是否反向转换
			 *
			 * @return 转换后内容
			 *
			 */
			static std::string FromHexString(const std::string & content, std::size_t length, bool reverse = false);
		};
	}
}


#endif // __TINY_TOOLKIT__UTIL__STRING__H__
