/**
 *
 *  作者: hm
 *
 *  说明: url编码
 *
 */


#include "url.h"


namespace tinyToolkit
{
	/**
	 *
	 * 字符对应的16进制
	 *
	 * @param value 待转换字符
	 *
	 * @return 16进制字符
	 *
	 */
	static uint8_t AsHex(uint8_t value)
	{
		return static_cast<uint8_t>(value > 9 ? value - 10 + 'A': value + '0');
	}

	/**
	 *
	 * 16进制对应的字符
	 *
	 * @param value 待转换字符
	 *
	 * @return 字符
	 *
	 */
	static uint8_t AsChar(uint8_t value)
	{
		return static_cast<uint8_t>(::isdigit(value) == 0 ? value + 10 - 'A' : value - '0');
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 编码
	 *
	 * @param value 待编码内容
	 *
	 * @return 编码后内容
	 *
	 */
	std::string UrlTransform::Encode(const char * value)
	{
		assert(value);

		return Encode(value, strlen(value));
	}

	/**
	 *
	 * 编码
	 *
	 * @param value 待编码内容
	 *
	 * @return 编码后内容
	 *
	 */
	std::string UrlTransform::Encode(const uint8_t * value)
	{
		assert(value);

		return Encode(value, strlen(reinterpret_cast<const char *>(value)));
	}

	/**
	 *
	 * 编码
	 *
	 * @param value 待编码内容
	 *
	 * @return 编码后内容
	 *
	 */
	std::string UrlTransform::Encode(const std::string & value)
	{
		return Encode(value, value.size());
	}

	/**
	 *
	 * 编码
	 *
	 * @param value 待编码内容
	 * @param length 待编码内容长度
	 *
	 * @return 编码后内容
	 *
	 */
	std::string UrlTransform::Encode(const char * value, std::size_t length)
	{
		assert(value);

		return Encode(reinterpret_cast<const uint8_t *>(value), length);
	}

	/**
	 *
	 * 编码
	 *
	 * @param value 待编码内容
	 * @param length 待编码内容长度
	 *
	 * @return 编码后内容
	 *
	 */
	std::string UrlTransform::Encode(const uint8_t * value, std::size_t length)
	{
		assert(value);

		std::string tempStr;

		for (size_t i = 0; i < length; ++i)
		{
			uint8_t byte = value[i];

			if (isalnum(byte))
			{
				tempStr += byte;
			}
			else if (byte == ' ')
			{
				tempStr += "+";
			}
			else if (byte == '-' || byte == '_' || byte == '.' || byte == '!' ||
					 byte == '~' || byte == '*' || byte == '(' || byte == ')')
			{
				tempStr += byte;
			}
			else
			{
				tempStr += "%";
				tempStr += AsHex(static_cast<uint8_t>(byte >> 4));
				tempStr += AsHex(static_cast<uint8_t>(byte % 16));
			}
		}

		return tempStr;
	}

	/**
	 *
	 * 编码
	 *
	 * @param value 待编码内容
	 * @param length 待编码内容长度
	 *
	 * @return 编码后内容
	 *
	 */
	std::string UrlTransform::Encode(const std::string & value, std::size_t length)
	{
		return Encode(value.c_str(), length);
	}

	/**
	 *
	 * 解码
	 *
	 * @param value 待解码内容
	 *
	 * @return 解码后内容
	 *
	 */
	std::string UrlTransform::Decode(const char * value)
	{
		assert(value);

		return Decode(value, strlen(value));
	}

	/**
	 *
	 * 解码
	 *
	 * @param value 待解码内容
	 *
	 * @return 解码后内容
	 *
	 */
	std::string UrlTransform::Decode(const uint8_t * value)
	{
		assert(value);

		return Decode(value, strlen(reinterpret_cast<const  char *>(value)));
	}

	/**
	 *
	 * 解码
	 *
	 * @param value 待解码内容
	 *
	 * @return 解码后内容
	 *
	 */
	std::string UrlTransform::Decode(const std::string & value)
	{
		return Decode(value, value.size());
	}

	/**
	 *
	 * 解码
	 *
	 * @param value 待解码内容
	 * @param length 待解码内容长度
	 *
	 * @return 解码后内容
	 *
	 */
	std::string UrlTransform::Decode(const char * value, std::size_t length)
	{
		assert(value);

		return Decode(reinterpret_cast<const uint8_t *>(value), length);
	}

	/**
	 *
	 * 解码
	 *
	 * @param value 待解码内容
	 * @param length 待解码内容长度
	 *
	 * @return 解码后内容
	 *
	 */
	std::string UrlTransform::Decode(const uint8_t * value, std::size_t length)
	{
		assert(value);

		std::string tempStr;

		for (std::size_t i = 0; i < length; ++i)
		{
			if (value[i] == '+')
			{
				tempStr.push_back(' ');
			}
			else if (value[i] == '%')
			{
				uint8_t high = AsChar(value[++i]);
				uint8_t low  = AsChar(value[++i]);

				tempStr.push_back(static_cast<char>(high * 16 + low));
			}
			else
			{
				tempStr.push_back(value[i]);
			}
		}

		return tempStr;
	}

	/**
	 *
	 * 解码
	 *
	 * @param value 待解码内容
	 * @param length 待解码内容长度
	 *
	 * @return 解码后内容
	 *
	 */
	std::string UrlTransform::Decode(const std::string & value, std::size_t size)
	{
		return Decode(value.c_str(), size);
	}
}
