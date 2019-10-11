/**
 *
 *  作者: hm
 *
 *  说明: hmac-sha224编码
 *
 */


#include "hmac_sha224.h"

#include "../utilities/string.h"
#include "../utilities/operator.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 */
	HMAC_SHA224::HMAC_SHA224()
	{
		Reset();
	}

	/**
	 *
	 * 重置
	 *
	 */
	void HMAC_SHA224::Reset()
	{
		_isComputed = false;

		memset(_digest, 0, sizeof(_digest));
		memset(reinterpret_cast<void *>(&_context), 0, sizeof(_context));

		Operator::Clear(_result);

		Initialization(_context);

		_sha224.Reset();
	}

	/**
	 *
	 * 设置密钥
	 *
	 * @param key 待设置密钥
	 *
	 */
	void HMAC_SHA224::SetKey(const char * key)
	{
		if (key == nullptr)
		{
			return;
		}

		SetKey(key, strlen(key));
	}

	/**
	 *
	 * 设置密钥
	 *
	 * @param key 待设置密钥
	 *
	 */
	void HMAC_SHA224::SetKey(const uint8_t * key)
	{
		if (key == nullptr)
		{
			return;
		}

		SetKey(key, strlen(reinterpret_cast<const char *>(key)));
	}

	/**
	 *
	 * 设置密钥
	 *
	 * @param key 待设置密钥
	 *
	 */
	void HMAC_SHA224::SetKey(const std::string & key)
	{
		SetKey(key, key.size());
	}

	/**
	 *
	 * 设置密钥
	 *
	 * @param key 待设置密钥
	 * @param length 待设置密钥长度
	 *
	 */
	void HMAC_SHA224::SetKey(const char * key, std::size_t length)
	{
		if (key == nullptr || length == 0)
		{
			return;
		}

		SetKey(reinterpret_cast<const uint8_t *>(key), length);
	}

	/**
	 *
	 * 设置密钥
	 *
	 * @param key 待设置密钥
	 * @param length 待设置密钥长度
	 *
	 */
	void HMAC_SHA224::SetKey(const uint8_t * key, std::size_t length)
	{
		if (key == nullptr || length == 0)
		{
			return;
		}

		if (length > HMAC_PAD_64_SIZE)
		{
			SHA224 sha224;

			sha224.Append(key, length);

			for (int i = 0; i < HMAC_PAD_64_SIZE; ++i)
			{
				_context.iPad[i] = sha224.Digest()[i] ^ 0x36;
				_context.oPad[i] = sha224.Digest()[i] ^ 0x5c;
			}
		}
		else
		{
			memset(_context.iPad, 0x36, HMAC_PAD_64_SIZE);
			memset(_context.oPad, 0x5c, HMAC_PAD_64_SIZE);

			for (std::size_t i = 0; i < length; ++i)
			{
				_context.iPad[i] = key[i] ^ 0x36;
				_context.oPad[i] = key[i] ^ 0x5c;
			}
		}

		Append(_context.iPad, sizeof(_context.iPad));
	}

	/**
	 *
	 * 设置密钥
	 *
	 * @param key 待设置密钥
	 * @param length 待设置密钥长度
	 *
	 */
	void HMAC_SHA224::SetKey(const std::string & key, std::size_t length)
	{
		SetKey(key.c_str(), length);
	}

	/**
	 *
	 * 追加内容
	 *
	 * @param value 待追加内容
	 *
	 */
	void HMAC_SHA224::Append(const char * value)
	{
		if (value == nullptr)
		{
			return;
		}

		Append(value, strlen(value));
	}

	/**
	 *
	 * 追加内容
	 *
	 * @param value 待追加内容
	 *
	 */
	void HMAC_SHA224::Append(const uint8_t * value)
	{
		if (value == nullptr)
		{
			return;
		}

		Append(value, strlen(reinterpret_cast<const char *>(value)));
	}

	/**
	 *
	 * 追加内容
	 *
	 * @param value 待追加内容
	 *
	 */
	void HMAC_SHA224::Append(const std::string & value)
	{
		Append(value, value.size());
	}

	/**
	 *
	 * 追加内容
	 *
	 * @param value 待追加内容
	 * @param length 待追加内容长度
	 *
	 */
	void HMAC_SHA224::Append(const char * value, std::size_t length)
	{
		if (value == nullptr || length == 0)
		{
			return;
		}

		Append(reinterpret_cast<const uint8_t *>(value), length);
	}

	/**
	 *
	 * 追加内容
	 *
	 * @param value 待追加内容
	 * @param length 待追加内容长度
	 *
	 */
	void HMAC_SHA224::Append(const uint8_t * value, std::size_t length)
	{
		if (value == nullptr || length == 0)
		{
			return;
		}

		_sha224.Append(value, length);

		_isComputed = false;
	}

	/**
	 *
	 * 追加内容
	 *
	 * @param value 待追加内容
	 * @param length 待追加内容长度
	 *
	 */
	void HMAC_SHA224::Append(const std::string & value, std::size_t size)
	{
		Append(value.c_str(), size);
	}

	/**
	 *
	 * 摘要
	 *
	 * @return 摘要
	 *
	 */
	const uint8_t * HMAC_SHA224::Digest()
	{
		Generate();

		return _digest;
	}

	/**
	 *
	 * 摘要
	 *
	 * @return 摘要
	 *
	 */
	const std::string & HMAC_SHA224::Result()
	{
		Generate();

		return _result;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 生成摘要
	 *
	 */
	void HMAC_SHA224::Generate()
	{
		if (!_isComputed)
		{
			_isComputed = true;

			SHA224 sha224;

			sha224.Append(_context.oPad, sizeof(_context.oPad));
			sha224.Append(_sha224.Digest(), SHA224_DIGEST_SIZE);

			memcpy(_digest, sha224.Digest(), SHA224_DIGEST_SIZE);

			_result.assign(String::AsHexString(_digest, sizeof(_digest), false));
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 初始化
	 *
	 * @param context 内容结构
	 *
	 */
	void HMAC_SHA224::Initialization(Context & context)
	{
		memset(context.iPad, 0x36, sizeof(_context.iPad));
		memset(context.oPad, 0x5c, sizeof(_context.oPad));
	}
}
