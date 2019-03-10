#ifndef __TINY_TOOLKIT__UTILITIES__STRING__H__
#define __TINY_TOOLKIT__UTILITIES__STRING__H__


/**
 *
 *  作者: hm
 *
 *  说明: 字符串处理
 *
 */


#include "../common/common.h"

#include "../3rd/fmt/core.h"
#include "../3rd/fmt/time.h"
#include "../3rd/fmt/posix.h"
#include "../3rd/fmt/printf.h"
#include "../3rd/fmt/chrono.h"
#include "../3rd/fmt/format.h"
#include "../3rd/fmt/ranges.h"
#include "../3rd/fmt/locale.h"
#include "../3rd/fmt/ostream.h"
#include "../3rd/fmt/prepare.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API String
	{
	public:
		/**
		 *
		 * 去除首尾字符
		 *
		 * @param value 待处理字符串
		 * @param group 处理字符集
		 *
		 */
		static void Trim(std::string & value, const char * group = "\r\n\t ");

		/**
		 *
		 * 去除首尾字符
		 *
		 * @param value 待处理字符串
		 * @param group 处理字符集
		 *
		 * @return 处理后字符串
		 *
		 */
		static std::string Trim(const std::string & value, const char * group = "\r\n\t ");

		/**
		 *
		 * 转换小写
		 *
		 * @param value 待处理字符串
		 * @param index 偏移位置
		 *
		 */
		static void Lower(std::string & value, std::size_t index = 0);

		/**
		 *
		 * 转换小写
		 *
		 * @param value 待处理字符串
		 * @param index 偏移位置
		 *
		 * @return 处理后字符串
		 *
		 */
		static std::string Lower(const std::string & value, std::size_t index = 0);

		/**
		 *
		 * 转换大写
		 *
		 * @param value 待处理字符串
		 * @param index 偏移位置
		 *
		 */
		static void Upper(std::string & value, std::size_t index = 0);

		/**
		 *
		 * 转换大写
		 *
		 * @param value 待处理字符串
		 * @param index 偏移位置
		 *
		 * @return 处理后字符串
		 *
		 */
		static std::string Upper(const std::string & value, std::size_t index = 0);

		/**
		 *
		 * 删除字符串指定字符
		 *
		 * @param value 待查找的字符串
		 * @param key 被查找的字符串
		 * @param index 偏移位置
		 *
		 */
		static void Erase(std::string & value, char key, std::size_t index = 0);

		/**
		 *
		 * 删除字符串指定字符
		 *
		 * @param value 待查找的字符串
		 * @param key 被查找的字符串
		 * @param index 偏移位置
		 *
		 * @return 处理后字符串
		 *
		 */
		static std::string Erase(const std::string & value, char key, std::size_t index = 0);

		/**
		 *
		 * 删除字符串指定字符
		 *
		 * @param value 待查找的字符串
		 * @param key 被查找的字符串
		 * @param index 偏移位置
		 *
		 */
		static void Erase(std::string & value, const std::string & key, std::size_t index = 0);

		/**
		 *
		 * 删除字符串指定字符
		 *
		 * @param value 待查找的字符串
		 * @param key 被查找的字符串
		 * @param index 偏移位置
		 *
		 * @return 处理后字符串
		 *
		 */
		static std::string Erase(const std::string & value, const std::string & key, std::size_t index = 0);

		/**
		 *
		 * 删除字符串指定字符
		 *
		 * @param value 待处理字符串
		 * @param function 判断函数
		 * @param index 偏移位置
		 *
		 */
		static void Erase(std::string & value, const std::function<bool(char)> & function, std::size_t index = 0);

		/**
		 *
		 * 删除字符串指定字符
		 *
		 * @param value 待处理字符串
		 * @param function 判断函数
		 * @param index 偏移位置
		 *
		 * @return 处理后字符串
		 *
		 */
		static std::string Erase(const std::string & value, const std::function<bool(char)> & function, std::size_t index = 0);

		/**
		 *
		 * 字符串替换字符
		 *
		 * @param value 待处理字符串
		 * @param src 被替换字符串
		 * @param dst 待替换字符串
		 * @param index 偏移位置
		 *
		 */
		static void Replace(std::string & value, const std::string & src, const std::string & dst, std::size_t index = 0);

		/**
		 *
		 * 字符串替换字符
		 *
		 * @param value 待处理字符串
		 * @param src 被替换字符串
		 * @param dst 待替换字符串
		 * @param index 偏移位置
		 *
		 * @return 处理后字符串
		 *
		 */
		static std::string Replace(const std::string & value, const std::string & src, const std::string & dst, std::size_t index = 0);

		/**
		 *
		 * 字符串替换字符
		 *
		 * @param value 待处理字符串
		 * @param src 被替换字符串
		 * @param dst 待替换字符串
		 * @param index 偏移位置
		 *
		 */
		static void ReplaceAll(std::string & value, const std::string & src, const std::string & dst, std::size_t index = 0);

		/**
		 *
		 * 字符串替换字符
		 *
		 * @param value 待处理字符串
		 * @param src 被替换字符串
		 * @param dst 待替换字符串
		 * @param index 偏移位置
		 *
		 * @return 处理后字符串
		 *
		 */
		static std::string ReplaceAll(const std::string & value, const std::string & src, const std::string & dst, std::size_t index = 0);

		/**
		 *
		 * 是否包含被查找的字符
		 *
		 * @param value 待查找字符串
		 * @param infix 被查找字符
		 * @param index 偏移位置
		 *
		 * @return 查找结果
		 *
		 */
		static bool With(const char * value, char infix, std::size_t index = 0);

		/**
		 *
		 * 是否包含被查找的字符串
		 *
		 * @param value 待查找字符串
		 * @param infix 被查找字符串
		 * @param index 偏移位置
		 *
		 * @return 查找结果
		 *
		 */
		static bool With(const char * value, const char * infix, std::size_t index = 0);

		/**
		 *
		 * 是否包含被查找的字符
		 *
		 * @param value 待查找字符串
		 * @param infix 被查找字符
		 * @param index 偏移位置
		 *
		 * @return 查找结果
		 *
		 */
		static bool With(const std::string & value, char infix, std::size_t index = 0);

		/**
		 *
		 * 是否包含被查找的字符串
		 *
		 * @param value 待查找字符串
		 * @param infix 被查找字符串
		 * @param index 偏移位置
		 *
		 * @return 查找结果
		 *
		 */
		static bool With(const std::string & value, const char * infix, std::size_t index = 0);

		/**
		 *
		 * 是否包含被查找的字符串
		 *
		 * @param value 待查找字符串
		 * @param infix 被查找字符串
		 * @param index 偏移位置
		 *
		 * @return 查找结果
		 *
		 */
		static bool With(const std::string & value, const std::string & infix, std::size_t index = 0);

		/**
		 *
		 * 是否以字符串结尾
		 *
		 * @param value 待查找字符串
		 * @param prefix 被查找字符串
		 *
		 * @return 查找结果
		 *
		 */
		static bool EndWith(const std::string & value, const std::string & suffix);

		/**
		 *
		 * 是否字符串开头
		 *
		 * @param value 待查找字符串
		 * @param prefix 被查找字符串
		 *
		 * @return 查找结果
		 *
		 */
		static bool StartWith(const std::string & value, const std::string & prefix);

		/**
		 *
		 * 按字符串拆分
		 *
		 * @param value 待查找字符串
		 * @param key 被查找字符串
		 * @param container 结果容器
		 * @param keepEmpty 是否保留空串
		 *
		 * @return 拆分个数
		 *
		 */
		static std::size_t Split(const std::string & value, const std::string & key, std::vector<std::string> & container, bool keepEmpty = false);

		/**
		 *
		 * 按字符串拆分
		 *
		 * @param value 待查找字符串
		 * @param key 被查找字符串
		 * @param keepEmpty 是否保留空串
		 *
		 * @return 结果容器
		 *
		 */
		static std::vector<std::string> Split(const std::string & value, const std::string & key, bool keepEmpty = false);

		/**
		 *
		 * 按行拆分
		 *
		 * @param value 待查找字符串
		 * @param container 结果容器
		 * @param keepEnter 是否保留换行符
		 *
		 * @return 拆分的个数
		 *
		 */
		static std::size_t SplitLines(const std::string & value, std::vector<std::string> & container, bool keepEnter = false);

		/**
		 *
		 * 按行拆分
		 *
		 * @param value 待查找字符串
		 * @param keepEmpty 是否保留换行符
		 *
		 * @return 结果容器
		 *
		 */
		static std::vector<std::string> SplitLines(const std::string & value, bool keepEnter = false);

		/**
		 *
		 * 过滤注释
		 *
		 * @param value 待过滤数据
		 *
		 * @return 过滤后数据
		 *
		 */
		static std::string FilterNote(const std::string & value);

		/**
		 *
		 * 格式化输出字符串
		 *
		 * @tparam Args [all built-in types]
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
		 * 格式化输出字符串
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param file 输出文件
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
		 * 格式化输出字符串
		 *
		 * @tparam Args [all built-in types]
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
		 * 格式化字符串
		 *
		 * @tparam Args [all built-in types]
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
		 * @tparam Args [all built-in types]
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
		 * 拼接字符串
		 *
		 * @tparam TypeT [all built-in types]
		 * @tparam Args [all built-in types]
		 *
		 * @param value 待拼接数据
		 * @param args 待拼接数据
		 *
		 * @return 拼接后字符串
		 *
		 */
		template <typename TypeT, typename... Args>
		static std::string Join(TypeT && value, Args && ... args)
		{
			std::stringstream stream;

			JoinStream(stream, std::forward<TypeT>(value), std::forward<Args>(args)...);

			return stream.str();
		}

		/**
		*
		* 拼接字符串
		*
		* @tparam TypeT [all built-in types]
		*
		* @param stream 字符流
		* @param value 待拼接数据
		*
		*/
		template <typename TypeT>
		static void JoinStream(std::stringstream & stream, TypeT && value)
		{
			stream << std::forward<TypeT>(value);
		}

		/**
		*
		* 拼接字符串
		*
		* @tparam TypeT [all built-in types]
		* @tparam Args [all built-in types]
		*
		* @param stream 字符流
		* @param value 待拼接数据
		* @param args 待拼接数据
		*
		*/
		template <typename TypeT, typename... Args>
		static void JoinStream(std::stringstream & stream, TypeT && value, Args && ... args)
		{
			JoinStream(stream, std::forward<TypeT>(value));
			JoinStream(stream, Join(std::forward<Args>(args)...));
		}

		/**
		 *
		 * 转换16进制字符串
		 *
		 * @param value 待转换字符串
		 * @param reverse 是否反向转换
		 *
		 * @return 转换后字符串
		 *
		 */
		static std::string AsHexString(const char * value, bool reverse = false);

		/**
		 *
		 * 转换16进制字符串
		 *
		 * @param value 待转换字符串
		 * @param reverse 是否反向转换
		 *
		 * @return 转换后字符串
		 *
		 */
		static std::string AsHexString(const uint8_t * value, bool reverse = false);

		/**
		 *
		 * 转换16进制字符串
		 *
		 * @param value 待转换字符串
		 * @param reverse 是否反向转换
		 *
		 * @return 转换后字符串
		 *
		 */
		static std::string AsHexString(const std::string & value, bool reverse = false);

		/**
		 *
		 * 转换16进制字符串
		 *
		 * @param value 待转换字符串
		 * @param size 转换长度
		 * @param reverse 是否反向转换
		 *
		 * @return 转换后字符串
		 *
		 */
		static std::string AsHexString(const char * value, std::size_t size, bool reverse = false);

		/**
		 *
		 * 转换16进制字符串
		 *
		 * @param value 待转换字符串
		 * @param size 转换长度
		 * @param reverse 是否反向转换
		 *
		 * @return 转换后字符串
		 *
		 */
		static std::string AsHexString(const uint8_t * value, std::size_t size, bool reverse = false);

		/**
		 *
		 * 转换16进制字符串
		 *
		 * @param value 待转换字符串
		 * @param size 转换的长度
		 * @param reverse 是否反向转换
		 *
		 * @return 转换后字符串
		 *
		 */
		static std::string AsHexString(const std::string & value, std::size_t size, bool reverse = false);

		/**
		 *
		 * 类型转换
		 *
		 * @tparam TypeT [all types]
		 *
		 * @param value 待转换数据
		 *
		 * @return 转换后数据
		 *
		 */
		template <typename TypeT>
		static TypeT Transform(const char * value)
		{
			TypeT result = TypeT();

			std::stringstream ss(value);

			ss >> result;

			return result;
		}

		/**
		 *
		 * 类型转换
		 *
		 * @tparam TypeT [all types]
		 *
		 * @param value 待转换数据
		 *
		 * @return 转换后数据
		 *
		 */
		template <typename TypeT>
		static TypeT Transform(const std::string & value)
		{
			TypeT result = TypeT();

			std::stringstream ss(value);

			ss >> result;

			return result;
		}
	};

	class TINY_TOOLKIT_API HexString
	{
	public:
		/**
		 *
		 * 转换字符
		 *
		 * @param value 待转换字符串
		 *
		 * @return 转换后字符
		 *
		 */
		static uint8_t AsByte(const char * value);

		/**
		 *
		 * 转换字符
		 *
		 * @param value 待转换字符串
		 *
		 * @return 转换后字符
		 *
		 */
		static uint8_t AsByte(const std::string & value);

		/**
		 *
		 * 转换字符串
		 *
		 * @param value 待转换字符串
		 * @param reverse 是否反向转换
		 *
		 * @return 转换后字符串
		 *
		 */
		static std::string AsString(const char * value, bool reverse = false);

		/**
		 *
		 * 转换字符串
		 *
		 * @param value 待转换字符串
		 * @param reverse 是否反向转换
		 *
		 * @return 转换后字符串
		 *
		 */
		static std::string AsString(const std::string & value, bool reverse = false);

		/**
		 *
		 * 转换字符串
		 *
		 * @param value 待转换字符串
		 * @param size 字符串长度
		 * @param reverse 是否反向转换
		 *
		 * @return 转换后字符串
		 *
		 */
		static std::string AsString(const char * value, std::size_t size, bool reverse = false);

		/**
		 *
		 * 转换字符串
		 *
		 * @param value 待转换字符串
		 * @param size 字符串长度
		 * @param reverse 是否反向转换
		 *
		 * @return 转换后字符串
		 *
		 */
		static std::string AsString(const std::string & value, std::size_t size, bool reverse = false);
	};
};


#endif // __TINY_TOOLKIT__UTILITIES__STRING__H__
