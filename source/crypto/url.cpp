/**
 *
 *  作者: hm
 *
 *  说明: url
 *
 */


#include "url.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <cstring>
#
#endif


namespace tinyToolkit
{
	namespace crypto
	{
		/**
		 *
		 * 字符对应的16进制
		 *
		 * @param tag 字符
		 *
		 * @return 16进制字符
		 *
		 */
		static uint8_t AsHex(uint8_t tag)
		{
			return static_cast<uint8_t>(tag > 9 ? tag - 10 + 'A': tag + '0');
		}

		/**
		 *
		 * 16进制对应的字符
		 *
		 * @param tag 字符
		 *
		 * @return 字符
		 *
		 */
		static uint8_t AsChar(uint8_t tag)
		{
			return static_cast<uint8_t>(::isdigit(tag) == 0 ? tag + 10 - 'A' : tag - '0');
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////


		/**
		 *
		 * 编码
		 *
		 * @param content 内容
		 *
		 * @return 编码后内容
		 *
		 */
		std::string Url::Encode(const char * content)
		{
			if (content == nullptr)
			{
				return { };
			}

			return Encode(content, strlen(content));
		}

		/**
		 *
		 * 编码
		 *
		 * @param content 内容
		 *
		 * @return 编码后内容
		 *
		 */
		std::string Url::Encode(const uint8_t * content)
		{
			if (content == nullptr)
			{
				return { };
			}

			return Encode(content, strlen(reinterpret_cast<const char *>(content)));
		}

		/**
		 *
		 * 编码
		 *
		 * @param content 内容
		 *
		 * @return 编码后内容
		 *
		 */
		std::string Url::Encode(const std::string & content)
		{
			return Encode(content, content.size());
		}

		/**
		 *
		 * 编码
		 *
		 * @param content 内容
		 * @param length 长度
		 *
		 * @return 编码后内容
		 *
		 */
		std::string Url::Encode(const char * content, std::size_t length)
		{
			if (content == nullptr || length == 0)
			{
				return { };
			}

			return Encode(reinterpret_cast<const uint8_t *>(content), length);
		}

		/**
		 *
		 * 编码
		 *
		 * @param content 内容
		 * @param length 长度
		 *
		 * @return 编码后内容
		 *
		 */
		std::string Url::Encode(const uint8_t * content, std::size_t length)
		{
			if (content == nullptr || length == 0)
			{
				return { };
			}

			std::string str{ };

			for (size_t i = 0; i < length; ++i)
			{
				uint8_t byte = content[i];

				if (isalnum(byte))
				{
					str += byte;
				}
				else if (byte == ' ')
				{
					str += "+";
				}
				else if (byte == '-' || byte == '_' || byte == '.' || byte == '!' ||
				         byte == '~' || byte == '*' || byte == '(' || byte == ')')
				{
					str += byte;
				}
				else
				{
					str += "%";
					str += AsHex(static_cast<uint8_t>(byte >> 4));
					str += AsHex(static_cast<uint8_t>(byte % 16));
				}
			}

			return str;
		}

		/**
		 *
		 * 编码
		 *
		 * @param content 内容
		 * @param length 长度
		 *
		 * @return 编码后内容
		 *
		 */
		std::string Url::Encode(const std::string & content, std::size_t length)
		{
			if (length == 0)
			{
				return { };
			}

			return Encode(content.c_str(), length);
		}

		/**
		 *
		 * 解码
		 *
		 * @param content 内容
		 *
		 * @return 解码后内容
		 *
		 */
		std::string Url::Decode(const char * content)
		{
			if (content == nullptr)
			{
				return { };
			}

			return Decode(content, strlen(content));
		}

		/**
		 *
		 * 解码
		 *
		 * @param content 内容
		 *
		 * @return 解码后内容
		 *
		 */
		std::string Url::Decode(const uint8_t * content)
		{
			if (content == nullptr)
			{
				return { };
			}

			return Decode(content, strlen(reinterpret_cast<const char *>(content)));
		}

		/**
		 *
		 * 解码
		 *
		 * @param content 内容
		 *
		 * @return 解码后内容
		 *
		 */
		std::string Url::Decode(const std::string & content)
		{
			return Decode(content, content.size());
		}

		/**
		 *
		 * 解码
		 *
		 * @param content 内容
		 * @param length 长度
		 *
		 * @return 解码后内容
		 *
		 */
		std::string Url::Decode(const char * content, std::size_t length)
		{
			if (content == nullptr || length == 0)
			{
				return { };
			}

			return Decode(reinterpret_cast<const uint8_t *>(content), length);
		}

		/**
		 *
		 * 解码
		 *
		 * @param content 内容
		 * @param length 长度
		 *
		 * @return 解码后内容
		 *
		 */
		std::string Url::Decode(const uint8_t * content, std::size_t length)
		{
			if (content == nullptr || length == 0)
			{
				return { };
			}

			std::string str{ };

			for (std::size_t i = 0; i < length; ++i)
			{
				if (content[i] == '+')
				{
					str.push_back(' ');
				}
				else if (content[i] == '%')
				{
					uint8_t high = AsChar(content[++i]);
					uint8_t low  = AsChar(content[++i]);

					str.push_back(static_cast<char>(high * 16 + low));
				}
				else
				{
					str.push_back(content[i]);
				}
			}

			return str;
		}

		/**
		 *
		 * 解码
		 *
		 * @param content 内容
		 * @param length 长度
		 *
		 * @return 解码后内容
		 *
		 */
		std::string Url::Decode(const std::string & content, std::size_t length)
		{
			if (length == 0)
			{
				return { };
			}

			return Decode(content.c_str(), length);
		}
	}
}
