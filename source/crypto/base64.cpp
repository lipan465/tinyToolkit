/**
 *
 *  作者: hm
 *
 *  说明: base64
 *
 */


#include "base64.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <stdexcept>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <cstring>
#  include <stdexcept>
#
#endif


namespace tinyToolkit
{
	namespace crypto
	{
		/**
		 *
		 * 编码
		 *
		 * @param content 内容
		 *
		 * @return 编码后内容
		 *
		 */
		std::string Base64::Encode(const char * content)
		{
			if (content == nullptr)
			{
				return { };
			}

			return Encode(content, ::strlen(content));
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
		std::string Base64::Encode(const uint8_t * content)
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
		std::string Base64::Encode(const std::string & content)
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
		std::string Base64::Encode(const char * content, std::size_t length)
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
		std::string Base64::Encode(const uint8_t * content, std::size_t length)
		{
			if (content == nullptr || length == 0)
			{
				return { };
			}

			static char EncodeTable[64] =
			{
				'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
				'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
				'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
				'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
				'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
				'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
				'w', 'x', 'y', 'z', '0', '1', '2', '3',
				'4', '5', '6', '7', '8', '9', '+', '/'
			};

			std::string str{ };

			while (length > 2)
			{
				str += EncodeTable[                              content[0] >> 2 ];
				str += EncodeTable[((content[0] & 0x03) << 4) + (content[1] >> 4)];
				str += EncodeTable[((content[1] & 0x0f) << 2) + (content[2] >> 6)];
				str += EncodeTable[  content[2] & 0x3f];

				length -= 3;
				content += 3;
			}

			if (length > 0)
			{
				str += EncodeTable[content[0] >> 2];

				if (length % 3 == 1)
				{
					str += EncodeTable[(content[0] & 0x03) << 4];
					str += "==";
				}
				else if (length % 3 == 2)
				{
					str += EncodeTable[((content[0] & 0x03) << 4) + (content[1] >> 4)];
					str += EncodeTable[ (content[1] & 0x0f) << 2];
					str += "=";
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
		std::string Base64::Encode(const std::string & content, std::size_t length)
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
		std::string Base64::Decode(const char * content)
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
		std::string Base64::Decode(const uint8_t * content)
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
		std::string Base64::Decode(const std::string & content)
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
		std::string Base64::Decode(const char * content, std::size_t length)
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
		std::string Base64::Decode(const uint8_t * content, std::size_t length)
		{
			if (content == nullptr || length == 0)
			{
				return { };
			}

			static char DecodeTable[256] =
			{
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
				52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1,  0, -1, -1,
				-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
				15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
				-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
				41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
			};

			char ch;

			int32_t bin = 0;

			std::size_t i = 0;
			std::size_t pos;

			std::string str{ };

			while ((ch = *content++) != '\0' && i <= length)
			{
				pos = i % 4;

				if (ch == '=')
				{
					/**
					 *
					 * 先说明一个概念 : 在解码时, 4个字符为一组进行一轮字符匹配.
					 *
					 * 两个条件 :
					 * 1、如果某一轮匹配的第二个是 "=" 且第三个字符不是 "=" , 说明这个带解析字符串不合法, 直接返回空
					 * 2、如果当前 "=" 不是第二个字符, 且后面的字符只包含空白符, 则说明这个这个条件合法, 可以继续.
					 *
					 */
					if (*content != '=' && pos == 1)
					{
						throw std::runtime_error("Decrypt data is invalid");
					}

					continue;
				}

				ch = DecodeTable[static_cast<uint8_t>(ch)];

				if (ch < 0)
				{
					continue;  /// 空格或者其它分隔符属于不合法的字符, 直接跳过
				}

				switch (pos)
				{
					case 0:
					{
						bin = ch << 2;

						break;
					}

					case 1:
					{
						bin |= ch >> 4;

						str.push_back(static_cast<char>(bin));

						bin = (ch & 0x0f) << 4;

						break;
					}

					case 2:
					{
						bin |= ch >> 2;

						str.push_back(static_cast<char>(bin));

						bin = (ch & 0x03) << 6;

						break;
					}

					case 3:
					{
						bin |= ch;

						str.push_back(static_cast<char>(bin));

						break;
					}

					default:
					{
						break;
					}
				}

				++i;
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
		std::string Base64::Decode(const std::string & content, std::size_t length)
		{
			if (length == 0)
			{
				return { };
			}

			return Decode(content.c_str(), length);
		}
	}
}
