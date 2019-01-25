/**
 *
 *  作者: hm
 *
 *  说明: 复制操作
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
	 * 加密
	 *
	 * @param value 待加密数据
	 *
	 * @return 加密后数据
	 *
	 */
	std::string UrlTransform::Encode(const char * value)
	{
		assert(value);

		return Encode(value, strlen(value));
	}

	/**
	 *
	 * 加密
	 *
	 * @param value 待加密数据
	 *
	 * @return 加密后数据
	 *
	 */
	std::string UrlTransform::Encode(const uint8_t * value)
	{
		assert(value);

		return Encode(value, strlen(reinterpret_cast<const char *>(value)));
	}

	/**
	 *
	 * 加密
	 *
	 * @param value 待加密数据
	 *
	 * @return 加密后数据
	 *
	 */
	std::string UrlTransform::Encode(const std::string & value)
	{
		return Encode(value, value.size());
	}

	/**
	 *
	 * 加密
	 *
	 * @param value 待加密数据
	 * @param size 待加密数据长度
	 *
	 * @return 加密后数据
	 *
	 */
	std::string UrlTransform::Encode(const char * value, std::size_t size)
	{
		assert(value);

		return Encode(reinterpret_cast<const uint8_t *>(value), size);
	}

	/**
	 *
	 * 加密
	 *
	 * @param value 待加密数据
	 * @param size 待加密数据长度
	 *
	 * @return 加密后数据
	 *
	 */
	std::string UrlTransform::Encode(const uint8_t * value, std::size_t size)
	{
		assert(value);

		std::string tempStr;

		for (size_t i = 0; i < size; ++i)
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
	 * 加密
	 *
	 * @param value 待加密数据
	 * @param size 待加密数据长度
	 *
	 * @return 加密后数据
	 *
	 */
	std::string UrlTransform::Encode(const std::string & value, std::size_t size)
	{
		return Encode(value.c_str(), size);
	}

	/**
	 *
	 * 解密
	 *
	 * @param value 待解密数据
	 *
	 * @return 解密后数据
	 *
	 */
	std::string UrlTransform::Decode(const char * value)
	{
		assert(value);

		return Decode(value, strlen(value));
	}

	/**
	 *
	 * 解密
	 *
	 * @param value 待解密数据
	 *
	 * @return 解密后数据
	 *
	 */
	std::string UrlTransform::Decode(const uint8_t * value)
	{
		assert(value);

		return Decode(value, strlen(reinterpret_cast<const  char *>(value)));
	}

	/**
	 *
	 * 解密
	 *
	 * @param value 待解密数据
	 *
	 * @return 解密后数据
	 *
	 */
	std::string UrlTransform::Decode(const std::string & value)
	{
		return Decode(value, value.size());
	}

	/**
	 *
	 * 解密
	 *
	 * @param value 待解密数据
	 * @param size 待解密数据长度
	 *
	 * @return 解密后数据
	 *
	 */
	std::string UrlTransform::Decode(const char * value, std::size_t size)
	{
		assert(value);

		return Decode(reinterpret_cast<const uint8_t *>(value), size);
	}

	/**
	 *
	 * 解密
	 *
	 * @param value 待解密数据
	 * @param size 待解密数据长度
	 *
	 * @return 解密后数据
	 *
	 */
	std::string UrlTransform::Decode(const uint8_t * value, std::size_t size)
	{
		assert(value);

		std::string tempStr;

		for (std::size_t i = 0; i < size; ++i)
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
	 * 解密
	 *
	 * @param value 待解密数据
	 * @param size 待解密数据长度
	 *
	 * @return 解密后数据
	 *
	 */
	std::string UrlTransform::Decode(const std::string & value, std::size_t size)
	{
		return Decode(value.c_str(), size);
	}
}
