/**
 *
 *  作者: hm
 *
 *  说明: 字符串处理
 *
 */


#include "string.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <regex>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <regex>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <regex>
#
#endif


namespace tinyToolkit
{
	namespace util
	{
		/**
		 *
		 * 转换小写
		 *
		 * @param content 处理内容
		 * @param offset 偏移位置
		 *
		 */
		void String::Lower(std::string & content, std::size_t offset)
		{
			std::transform(content.begin() + offset, content.end(), content.begin() + offset, ::tolower);
		}

		/**
		 *
		 * 转换大写
		 *
		 * @param content 处理内容
		 * @param offset 偏移位置
		 *
		 */
		void String::Upper(std::string & content, std::size_t offset)
		{
			std::transform(content.begin() + offset, content.end(), content.begin() + offset, ::toupper);
		}

		/**
		 *
		 * 删除字符
		 *
		 * @param content 处理内容
		 * @param key 匹配字符
		 * @param offset 偏移位置
		 *
		 */
		void String::Erase(std::string & content, char key, std::size_t offset)
		{
			while ((offset = content.find(key, offset)) != std::string::npos)
			{
				content.erase(offset, 1);
			}
		}

		/**
		 *
		 * 删除内容
		 *
		 * @param content 处理内容
		 * @param key 匹配内容
		 * @param offset 偏移位置
		 *
		 */
		void String::Erase(std::string & content, const std::string & key, std::size_t offset)
		{
			while ((offset = content.find(key, offset)) != std::string::npos)
			{
				content.erase(offset, key.size());
			}
		}

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
		void String::Split(const std::string & content, const std::string & key, std::vector<std::string> & container, bool keepEmpty)
		{
			if (key.empty())
			{
				container.push_back(content);

				return;
			}

			std::string::const_iterator beg = content.begin();
			std::string::const_iterator end;

			while (true)
			{
				end = std::search(beg, content.end(), key.begin(), key.end());

				if (beg != end)
				{
					std::string str(beg, end);

					if (keepEmpty || !str.empty())
					{
						container.push_back(str);
					}

					if (end == content.end())
					{
						break;
					}
				}

				beg = end + key.size();

				if (beg == content.end())
				{
					break;
				}
			}
		}

		/**
		 *
		 * 按行拆分内容
		 *
		 * @param content 处理内容
		 * @param container 结果容器
		 * @param keepEnter 是否保留换行符
		 *
		 */
		void String::SplitLines(const std::string & content, std::vector<std::string> & container, bool keepEnter)
		{
			std::size_t i = 0;
			std::size_t j = 0;
			std::size_t e = 0;

			while (i < content.size())
			{
				while (i < content.size() && content[i] != '\r' && content[i] != '\n' && content[i] != '\0')
				{
					++i;
				}

				e = i;

				if (i < content.size())
				{
					i += (content[i] == '\r' && (i + 1) < content.size() && content[i + 1] == '\n') ? 2 : 1;

					if (keepEnter)
					{
						e = i;
					}
				}

				container.push_back(content.substr(j, e - j));

				j = i;
			}
		}

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
		void String::Replace(std::string & content, const std::string & src, const std::string & dst, std::size_t offset)
		{
			for (std::size_t pos = offset; pos != std::string::npos; pos += dst.size())
			{
				if ((pos = content.find(src, pos)) == std::string::npos)
				{
					break;
				}

				content.replace(pos, src.size(), dst);
			}
		}

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
		void String::ReplaceLoop(std::string & content, const std::string & src, const std::string & dst, std::size_t offset)
		{
			std::size_t pos;

			while (true)
			{
				if ((pos = content.find(src, offset)) == std::string::npos)
				{
					break;
				}

				content.replace(pos, src.size(), dst);
			}
		}

		/**
		 *
		 * 是否为ip
		 *
		 * @param content 处理内容
		 *
		 * @return 是否为ip
		 *
		 */
		bool String::IsIP(const char * content)
		{
			static std::regex rule
			(
				"^(([01]?[0-9]{1,2}|2[0-4][0-9]|25[0-5])(\\.([01]?[0-9]{1,2}|2[0-4][0-9]|25[0-5])){3}|([0-9a-fA-F]{1,4}:)+:?([0-9a-fA-F]{1,4}:)*[0-9a-fA-F]{1,4})$"
			);

			return std::regex_match(content, rule);
		}

		/**
		 *
		 * 是否为ip
		 *
		 * @param content 处理内容
		 *
		 * @return 是否为ip
		 *
		 */
		bool String::IsIP(const std::string & content)
		{
			static std::regex rule
			(
				"^(([01]?[0-9]{1,2}|2[0-4][0-9]|25[0-5])(\\.([01]?[0-9]{1,2}|2[0-4][0-9]|25[0-5])){3}|([0-9a-fA-F]{1,4}:)+:?([0-9a-fA-F]{1,4}:)*[0-9a-fA-F]{1,4})$"
			);

			return std::regex_match(content, rule);
		}

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
		bool String::With(const char * content, char key, std::size_t offset)
		{
			if (content == nullptr)
			{
				return false;
			}

			return strchr(content + offset, key) != nullptr;
		}

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
		bool String::With(const char * content, const char * key, std::size_t offset)
		{
			if (key == nullptr || content == nullptr)
			{
				return false;
			}

			return strstr(content + offset, key) != nullptr;
		}

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
		bool String::With(const char * content, const std::string & key, std::size_t offset)
		{
			return With(content, key.c_str(), offset);
		}

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
		bool String::With(const std::string & content, char key, std::size_t offset)
		{
			return content.find(key, offset) != std::string::npos;
		}

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
		bool String::With(const std::string & content, const char * key, std::size_t offset)
		{
			if (key == nullptr)
			{
				return false;
			}

			return content.find(key, offset) != std::string::npos;
		}

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
		bool String::With(const std::string & content, const std::string & key, std::size_t offset)
		{
			return content.find(key, offset) != std::string::npos;
		}

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
		bool String::EndWith(const std::string & content, const std::string & suffix)
		{
			return content.size() >= suffix.size() && std::equal(suffix.rbegin(), suffix.rend(), content.rbegin());
		}

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
		bool String::StartWith(const std::string & content, const std::string & prefix)
		{
			return content.size() >= prefix.size() && std::equal(prefix.begin(), prefix.end(), content.begin());
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
		std::string String::Trim(const std::string & content, const char * pattern)
		{
			if (pattern == nullptr)
			{
				return content;
			}

			auto end   = content.find_last_not_of(pattern);
			auto start = content.find_first_not_of(pattern);

			return (start == std::string::npos) ? "" : content.substr(start, 1 + end - start);
		}

		/**
		 *
		 * 过滤注释
		 *
		 * @param content 处理内容
		 *
		 * @return 过滤后内容
		 *
		 */
		std::string String::FilterNote(const std::string & content)
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

			std::string str{ };

			State state = State::NORMAL;

			for (const char c : content)
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
		 * 收缩json
		 *
		 * @param content 处理内容
		 *
		 * @return 收缩后数据
		 *
		 */
		std::string String::ReduceJson(const std::string & content)
		{
			std::size_t tag = 0;
			std::size_t length = content.size();

			std::string str{ };

			for (std::size_t i = 0; i < length; ++i)
			{
				auto & key = content[i];

				if ((tag & 1) == 0)
				{
					if (key == ' ' || key == '\t' || key == '\r' || key == '\n')
					{
						continue;
					}

					if (key == ',' || key == '[' || key == ']' || key == '{' || key == '}')
					{
						str += key;

						continue;
					}
				}

				if (key == '\"')
				{
					++tag;
				}

				str += key;
			}

			return str;
		}

		/**
		 *
		 * 美化json
		 *
		 * @param content 处理内容
		 *
		 * @return 格式化后数据
		 *
		 */
		std::string String::BeautifyJson(const std::string & content)
		{
			auto tmp = 0;
			auto tag = static_cast<int32_t>(0);
			auto count = static_cast<int32_t>(0);
			auto length = static_cast<int32_t>(content.size());

			std::string str{ };

			static auto AppendIndent = [&]()
			{
				for (int32_t i = 0; i < count; ++i)
				{
					str += "\t";
				}
			};

			for (int32_t i = 0; i < length; ++i)
			{
				auto & key = content[i];

				if ((tag & 1) == 0)
				{
					if (key == ' ' || key == '\t' || key == '\r' || key == '\n')
					{
						continue;
					}

					if (key == ',')
					{
						str += key;
						str += TINY_TOOLKIT_EOL;

						AppendIndent();

						continue;
					}

					if (key == '[' || key == '{')
					{
						tmp = i;

						while ((tmp - 1) >= 0)
						{
							--tmp;

							if (content[tmp] == ' ')
							{
								continue;
							}

							if (content[tmp] == ':')
							{
								str += TINY_TOOLKIT_EOL;

								AppendIndent();
							}

							break;
						}

						str += key;
						str += TINY_TOOLKIT_EOL;

						++count;

						AppendIndent();

						continue;
					}

					if (key == ']' || key == '}')
					{
						tmp = i;

						while ((tmp - 1) >= 0)
						{
							--tmp;

							if (content[tmp] == ' ')
							{
								continue;
							}

							if (content[tmp] != ']' && content[tmp] != '}')
							{
								str += TINY_TOOLKIT_EOL;
							}

							break;
						}

						--count;

						AppendIndent();

						str += key;

						while ((i + 1) < length)
						{
							if (content[i + 1] == ' ')
							{
								++i;

								continue;
							}

							if (content[i + 1] != ',')
							{
								str += TINY_TOOLKIT_EOL;
							}

							break;
						}

						continue;
					}
				}

				if (key == '\"')
				{
					if (i - 1 > 0)
					{
						if (content[i - 1] != '\\')
						{
							++tag;
						}
					}
					else
					{
						++tag;
					}
				}

				str += key;
			}

			return str;
		}

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
		std::string String::AsHexString(const char * content, bool reverse)
		{
			if (content == nullptr)
			{
				return { };
			}

			return AsHexString(content, strlen(content), reverse);
		}

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
		std::string String::AsHexString(const uint8_t * content, bool reverse)
		{
			if (content == nullptr)
			{
				return { };
			}

			return AsHexString(content, strlen(reinterpret_cast<const char *>(content)), reverse);
		}

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
		std::string String::AsHexString(const std::string & content, bool reverse)
		{
			return AsHexString(content, content.size(), reverse);
		}

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
		std::string String::AsHexString(const char * content, std::size_t length, bool reverse)
		{
			if (content == nullptr || length == 0)
			{
				return { };
			}

			return AsHexString(reinterpret_cast<const uint8_t *>(content), length, reverse);
		}

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
		std::string String::AsHexString(const uint8_t * content, std::size_t length, bool reverse)
		{
			if (content == nullptr || length == 0)
			{
				return { };
			}

			int64_t op   = reverse ? -1 : 1;
			int64_t init = reverse ? static_cast<int64_t>(length) - 1 : 0;
			int64_t end  = reverse ? -1 : static_cast<int64_t>(length);

			std::string str{ };

			for (int64_t i = init; i != end; i += op)
			{
				char buffer[3]{ 0 };

				snprintf(buffer, sizeof(buffer), "%02x", content[i]);

				str.append(buffer, 2);
			}

			return str;
		}

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
		std::string String::AsHexString(const std::string & content, std::size_t length, bool reverse)
		{
			if (length == 0)
			{
				return { };
			}

			return AsHexString(content.c_str(), length, reverse);
		}

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
		std::string String::FromHexString(const char * content, bool reverse)
		{
			if (content == nullptr)
			{
				return { };
			}

			return FromHexString(content, strlen(content), reverse);
		}

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
		std::string String::FromHexString(const uint8_t * content, bool reverse)
		{
			if (content == nullptr)
			{
				return { };
			}

			return FromHexString(content, strlen(reinterpret_cast<const char *>(content)), reverse);
		}

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
		std::string String::FromHexString(const std::string & content, bool reverse)
		{
			return FromHexString(content, content.size(), reverse);
		}

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
		std::string String::FromHexString(const char * content, std::size_t length, bool reverse)
		{
			if (content == nullptr || length == 0)
			{
				return { };
			}

			int64_t op   = reverse ? -2 : 2;
			int64_t init = reverse ? static_cast<int64_t>(length) - 2 : 0;
			int64_t end  = reverse ? -2 : static_cast<int64_t>(length);

			std::string str{ };

			for (int64_t i = init; i != end; i += op)
			{
				char buffer[3] = { content[i], content[i + 1], '\0' };

				str.push_back(static_cast<uint8_t>(strtol(buffer, nullptr, 16)));
			}

			return str;
		}

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
		std::string String::FromHexString(const uint8_t * content, std::size_t length, bool reverse)
		{
			if (content == nullptr || length == 0)
			{
				return { };
			}

			return FromHexString(reinterpret_cast<const char *>(content), length, reverse);
		}

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
		std::string String::FromHexString(const std::string & content, std::size_t length, bool reverse)
		{
			if (length == 0)
			{
				return { };
			}

			return FromHexString(content.c_str(), length, reverse);
		}
	}
}
