#ifndef __TINY_TOOLKIT__UTILITIES__STRING__H__
#define __TINY_TOOLKIT__UTILITIES__STRING__H__


/**
 *
 *  作者: hm
 *
 *  说明: 字符串处理
 *
 */


#include "../3rd/fmt/fmt.h"
#include "../common/common.h"


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
		static void Trim(std::string & value, const char * group = "\r\n\t ")
		{
			assert(group);

			std::string::size_type end   = value.find_last_not_of(group);
			std::string::size_type start = value.find_first_not_of(group);

			value = (start == std::string::npos) ? "" : value.substr(start, 1 + end - start);
		}

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
		static std::string Trim(const std::string & value, const char * group = "\r\n\t ")
		{
			assert(group);

			std::string::size_type end   = value.find_last_not_of(group);
			std::string::size_type start = value.find_first_not_of(group);

			return (start == std::string::npos) ? "" : value.substr(start, 1 + end - start);
		}

		/**
		 *
		 * 转换小写
		 *
		 * @param value 待处理字符串
		 * @param index 偏移位置
		 *
		 */
		static void Lower(std::string & value, std::size_t index = 0)
		{
			std::transform(value.begin() + index, value.end(), value.begin() + index, ::tolower);
		}

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
		static std::string Lower(const std::string & value, std::size_t index = 0)
		{
			std::string str = value;

			Lower(str, index);

			return str;
		}

		/**
		 *
		 * 转换大写
		 *
		 * @param value 待处理字符串
		 * @param index 偏移位置
		 *
		 */
		static void Upper(std::string & value, std::size_t index = 0)
		{
			std::transform(value.begin() + index, value.end(), value.begin() + index, ::toupper);
		}

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
		static std::string Upper(const std::string & value, std::size_t index = 0)
		{
			std::string str = value;

			Upper(str, index);

			return str;
		}

		/**
		 *
		 * 删除字符串指定字符
		 *
		 * @param value 待查找的字符串
		 * @param key 被查找的字符串
		 * @param index 偏移位置
		 *
		 */
		static void Erase(std::string & value, char key, std::size_t index = 0)
		{
			while ((index = value.find(key, index)) != std::string::npos)
			{
				value.erase(index, 1);
			}
		}

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
		static std::string Erase(const std::string & value, char key, std::size_t index = 0)
		{
			std::string str = value;

			Erase(str, key, index);

			return str;
		}

		/**
		 *
		 * 删除字符串指定字符
		 *
		 * @param value 待查找的字符串
		 * @param key 被查找的字符串
		 * @param index 偏移位置
		 *
		 */
		static void Erase(std::string & value, const std::string & key, std::size_t index = 0)
		{
			while ((index = value.find(key, index)) != std::string::npos)
			{
				value.erase(index, key.size());
			}
		}

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
		static std::string Erase(const std::string & value, const std::string & key, std::size_t index = 0)
		{
			std::string str = value;

			Erase(str, key, index);

			return str;
		}

		/**
		 *
		 * 删除字符串指定字符
		 *
		 * @param value 待处理字符串
		 * @param function 判断函数
		 * @param index 偏移位置
		 *
		 */
		static void Erase(std::string & value, const std::function<bool(char)> & function, std::size_t index = 0)
		{
			for (auto iter = value.begin() + index; iter != value.end();)
			{
				if (function(*iter))
				{
					value.erase(iter, iter + 1);
				}
				else
				{
					++iter;
				}
			}
		}

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
		static std::string Erase(const std::string & value, const std::function<bool(char)> & function, std::size_t index = 0)
		{
			std::string str = value;

			Erase(str, function, index);

			return str;
		}

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
		static void Replace(std::string & value, const std::string & src, const std::string & dst, std::size_t index = 0)
		{
			for (std::size_t pos = index; pos != std::string::npos; pos += dst.size())
			{
				if ((pos = value.find(src, pos)) == std::string::npos)
				{
					break;
				}
				else
				{
					value.replace(pos, src.size(), dst);
				}
			}
		}

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
		static std::string Replace(const std::string & value, const std::string & src, const std::string & dst, std::size_t index = 0)
		{
			std::string str = value;

			Replace(str, src, dst, index);

			return str;
		}

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
		static void ReplaceAll(std::string & value, const std::string & src, const std::string & dst, std::size_t index = 0)
		{
			while (true)
			{
				std::size_t pos = value.find(src, index);

				if (pos == std::string::npos)
				{
					break;
				}
				else
				{
					value.replace(pos, src.size(), dst);
				}
			}
		}

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
		static std::string ReplaceAll(const std::string & value, const std::string & src, const std::string & dst, std::size_t index = 0)
		{
			std::string str = value;

			ReplaceAll(str, src, dst, index);

			return str;
		}

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
		static bool With(const char * value, char infix, std::size_t index = 0)
		{
			assert(value);

			return std::strchr(value + index, infix) != nullptr;
		}

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
		static bool With(const char * value, const char * infix, std::size_t index = 0)
		{
			assert(value && infix);

			return strstr(value + index, infix) != nullptr;
		}

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
		static bool With(const std::string & value, char infix, std::size_t index = 0)
		{
			return value.find(infix, index) != std::string::npos;
		}

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
		static bool With(const std::string & value, const char * infix, std::size_t index = 0)
		{
			assert(infix);

			return value.find(infix, index) != std::string::npos;
		}

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
		static bool With(const std::string & value, const std::string & infix, std::size_t index = 0)
		{
			return value.find(infix, index) != std::string::npos;
		}

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
		static bool EndWith(const std::string & value, const std::string & suffix)
		{
			return value.size() >= suffix.size() && std::equal(suffix.rbegin(), suffix.rend(), value.rbegin());
		}

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
		static bool StartWith(const std::string & value, const std::string & prefix)
		{
			return value.size() >= prefix.size() && std::equal(prefix.begin(), prefix.end(), value.begin());
		}

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
		static std::size_t Split(const std::string & value, const std::string & key, std::vector<std::string> & container, bool keepEmpty = false)
		{
			if (key.empty())
			{
				container.push_back(value);
			}
			else
			{
				std::string::const_iterator beg = value.begin();
				std::string::const_iterator end;

				while (true)
				{
					end = std::search(beg, value.end(), key.begin(), key.end());

					if (beg != end)
					{
						std::string str(beg, end);

						Trim(str, "\r\n");

						if (keepEmpty || !str.empty())
						{
							container.push_back(str);
						}

						if (end == value.end())
						{
							break;
						}
					}
					else if (beg == value.end())
					{
						break;
					}

					beg = end + key.size();
				}
			}

			return container.size();
		}

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
		static std::vector<std::string> Split(const std::string & value, const std::string & key, bool keepEmpty = false)
		{
			std::vector<std::string> container;

			Split(value, key, container, keepEmpty);

			return container;
		}

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
		static std::size_t SplitLines(const std::string & value, std::vector<std::string> & container, bool keepEnter = false)
		{
			std::size_t i = 0;
			std::size_t j = 0;

			while (i < value.size())
			{
				while (i < value.size() && value[i] != '\r' && value[i] != '\n' && value[i] != '\0')
				{
					++i;
				}

				std::size_t eol = i;

				if (i < value.size())
				{
					i += (value[i] == '\r' && i + 1 < value.size() && value[i + 1] == '\n') ? 2 : 1;

					if (keepEnter)
					{
						eol = i;
					}
				}

				container.push_back(value.substr(j, eol - j));

				j = i;
			}

			return container.size();
		}

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
		static std::vector<std::string> SplitLines(const std::string & value, bool keepEnter = false)
		{
			std::vector<std::string> container;

			SplitLines(value, container, keepEnter);

			return container;
		}

		/**
		 *
		 * 过滤注释
		 *
		 * @param value 待过滤数据
		 *
		 * @return 过滤后数据
		 *
		 */
		static std::string FilterNote(const std::string & value)
		{
			enum class State : uint8_t
			{
				NORMAL,                 /// 正常代码
				SLASH,                  /// 斜杠
				NOTE_MULTI_LINE,        /// 多行注释
				NOTE_MULTI_LINE_STAR,   /// 多行注释遇到*
				NOTE_SINGLE_LINE,       /// 单行注释
				BACKSLASH,              /// 折行注释反斜线
				CHARACTER,              /// 字符
				ESCAPE_CHARACTER,       /// 字符中的转义字符
				STRING,                 /// 字符串
				ESCAPE_STRING           /// 字符串中的转义字符
			};

			std::string str;

			State state = State::NORMAL;

			for (const char c : value)
			{
				switch (state)
				{
					case State::NORMAL:  /// 正常代码
					{
						if (c == '/')
						{
							state = State::SLASH;
						}
						else
						{
							str.push_back(c);

							if (c == '\'')
							{
								state = State::CHARACTER;
							}
							else if (c == '\"')
							{
								state = State::STRING;
							}
							else
							{
								state = State::NORMAL;  /// 保持当前状态
							}
						}

						break;
					}

					case State::SLASH:  /// 斜杠
					{
						if (c == '*')
						{
							state = State::NOTE_MULTI_LINE;
						}
						else if (c == '/')
						{
							state = State::NOTE_SINGLE_LINE;
						}
						else
						{
							str.push_back('/');
							str.push_back(c);

							state = State::NORMAL;
						}

						break;
					}

					case State::NOTE_MULTI_LINE:  /// 多行注释
					{
						if (c == '*')
						{
							state = State::NOTE_MULTI_LINE_STAR;
						}
						else
						{
							state = State::NOTE_MULTI_LINE;  /// 保持当前状态
						}

						break;
					}

					case State::NOTE_MULTI_LINE_STAR:  /// 多行注释遇到*
					{
						if (c == '/')
						{
							state = State::NORMAL;  /// 注释结束
						}
						else if (c == '*')
						{
							state = State::NOTE_MULTI_LINE_STAR;  /// 保持当前状态
						}
						else
						{
							state = State::NOTE_MULTI_LINE;
						}

						break;
					}

					case State::NOTE_SINGLE_LINE:  /// 单行注释
					{
						if (c == '\\')
						{
							state = State::BACKSLASH;
						}
						else if (c == '\r' || c == '\n')
						{
							str.push_back(c);

							state = State::NORMAL;  /// 注释结束
						}
						else
						{
							state = State::NOTE_SINGLE_LINE;  /// 保持当前状态
						}

						break;
					}

					case State::BACKSLASH:  /// 折行注释反斜线
					{
						if (c == '\\' || c == '\r' || c == '\n')
						{
							state = State::BACKSLASH;  /// 保持当前状态
						}
						else
						{
							state = State::NOTE_SINGLE_LINE;
						}

						break;
					}

					case State::CHARACTER:  /// 字符
					{
						str.push_back(c);

						if (c == '\\')
						{
							state = State::ESCAPE_CHARACTER;
						}
						else if (c == '\'')
						{
							state = State::NORMAL;
						}
						else
						{
							state = State::CHARACTER;  /// 保持当前状态
						}

						break;
					}

					case State::ESCAPE_CHARACTER:  /// 字符中的转义字符
					{
						str.push_back(c);

						state = State::CHARACTER;

						break;
					}

					case State::STRING:  /// 字符串
					{
						str.push_back(c);

						if (c == '\\')
						{
							state = State::ESCAPE_STRING;
						}
						else if (c == '\"')
						{
							state = State::NORMAL;
						}
						else
						{
							state = State::STRING;  /// 保持当前状态
						}

						break;
					}

					case State::ESCAPE_STRING:  /// 字符串中的转义字符
					{
						str.push_back(c);

						state = State::STRING;

						break;
					}

					default:
					{
						break;
					}
				}
			}

			return str;
		}

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
		static std::string Splice(TypeT && value, Args &&... args)
		{
			std::stringstream stream;

			SpliceStream(stream, std::forward<TypeT>(value), std::forward<Args>(args)...);

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
		static void SpliceStream(std::stringstream & stream, TypeT && value)
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
		static void SpliceStream(std::stringstream & stream, TypeT && value, Args &&... args)
		{
			SpliceStream(stream, std::forward<TypeT>(value));
			SpliceStream(stream, Splice(std::forward<Args>(args)...));
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
		static std::string AsHexString(const char * value, bool reverse = false)
		{
			assert(value);

			return AsHexString(value, strlen(value), reverse);
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
		static std::string AsHexString(const uint8_t * value, bool reverse = false)
		{
			assert(value);

			return AsHexString(value, strlen(reinterpret_cast<const char *>(value)), reverse);
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
		static std::string AsHexString(const std::string & value, bool reverse = false)
		{
			return AsHexString(value, value.size(), reverse);
		}

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
		static std::string AsHexString(const char * value, std::size_t size, bool reverse = false)
		{
			assert(value);

			return AsHexString(reinterpret_cast<const uint8_t *>(value), size, reverse);
		}

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
		static std::string AsHexString(const uint8_t * value, std::size_t size, bool reverse = false)
		{
			assert(value);

			int64_t op   = reverse ? -1 : 1;
			int64_t init = reverse ? static_cast<int64_t>(size) - 1 : 0;
			int64_t end  = reverse ? -1 : static_cast<int64_t>(size);

			fmt::MemoryWriter w;

			for (int64_t i = init; i != end; i += op)
			{
				w << fmt::pad(fmt::hex(value[i]), 2, '0');
			}

			return w.str();
		}

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
		static std::string AsHexString(const std::string & value, std::size_t size, bool reverse = false)
		{
			return AsHexString(value.data(), size, reverse);
		}

		/**
		 *
		 * 转换本地字符串
		 *
		 * @tparam TypeT [all types]
		 *
		 * @param value 待转换数据
		 *
		 * @return 本地字符串
		 *
		 */
		template<typename TypeT>
		static std::string AsLocalString(TypeT && value)
		{
			static std::locale loc("");

			std::stringstream ss;

			ss.imbue(loc);

			ss << value;

			return ss.str();
		}

		/**
		 *
		 * 转换本地字符串
		 *
		 * @param value 待转换浮点数数据
		 * @param bit 浮点数位数
		 *
		 * @return 本地字符串
		 *
		 */
		static std::string AsLocalString(double value, int32_t bit)
		{
			static std::locale loc("");

			std::stringstream ss;

			ss.imbue(loc);

			ss << std::fixed << std::setprecision(bit) << value;

			return ss.str();
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
		static uint8_t AsByte(const char * value)
		{
			return static_cast<uint8_t>(strtol(value, nullptr, 16));
		}

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
		static std::string AsString(const char * value, bool reverse = false)
		{
			assert(value);

			return AsString(value, strlen(value), reverse);
		}

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
		static std::string AsString(const std::string & value, bool reverse = false)
		{
			return AsString(value, value.size(), reverse);
		}

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
		static std::string AsString(const char * value, std::size_t size, bool reverse = false)
		{
			int64_t op   = reverse ? -2 : 2;
			int64_t init = reverse ? static_cast<int64_t>(size) - 2 : 0;
			int64_t end  = reverse ? -2 : static_cast<int64_t>(size);

			std::string tempStr;

			for (int64_t i = init; i != end; i += op)
			{
				char buffer[3] = { value[i], value[i + 1], '\0' };

				tempStr.push_back(AsByte(buffer));
			}

			return tempStr;
		}

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
		static std::string AsString(const std::string & value, std::size_t size, bool reverse = false)
		{
			return AsString(value.data(), size, reverse);
		}
	};
};


#endif // __TINY_TOOLKIT__UTILITIES__STRING__H__
