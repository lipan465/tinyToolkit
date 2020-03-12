/**
 *
 *  作者: hm
 *
 *  说明: hmac_sha1
 *
 */


#include "hmac_sha1.h"

#include "../util/string.h"

#include "../container/operation.h"


namespace tinyToolkit
{
	namespace crypto
	{
		/**
		 *
		 * 构造函数
		 *
		 */
		HMAC_SHA1::HMAC_SHA1()
		{
			Reset();
		}

		/**
		 *
		 * 重置
		 *
		 */
		void HMAC_SHA1::Reset()
		{
			_isComputed = false;

			memset(reinterpret_cast<void *>(_digest),   0, sizeof(_digest));
			memset(reinterpret_cast<void *>(&_context), 0, sizeof(_context));

			container::Operation::Swap(_result);

			Initialization(_context);

			_sha1.Reset();
		}

		/**
		 *
		 * 设置密钥
		 *
		 * @param key 密钥
		 *
		 */
		void HMAC_SHA1::SetKey(const char * key)
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
		 * @param key 密钥
		 *
		 */
		void HMAC_SHA1::SetKey(const uint8_t * key)
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
		 * @param key 密钥
		 *
		 */
		void HMAC_SHA1::SetKey(const std::string & key)
		{
			SetKey(key, key.size());
		}

		/**
		 *
		 * 设置密钥
		 *
		 * @param key 密钥
		 * @param length 长度
		 *
		 */
		void HMAC_SHA1::SetKey(const char * key, std::size_t length)
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
		 * @param key 密钥
		 * @param length 长度
		 *
		 */
		void HMAC_SHA1::SetKey(const uint8_t * key, std::size_t length)
		{
			if (key == nullptr || length == 0)
			{
				return;
			}

			if (length > TINY_TOOLKIT_HMAC_PAD_64_SIZE)
			{
				SHA1 sha1{ };

				sha1.Append(key, length);

				for (int i = 0; i < TINY_TOOLKIT_HMAC_PAD_64_SIZE; ++i)
				{
					_context.iPad[i] = sha1.Digest()[i] ^ 0x36;
					_context.oPad[i] = sha1.Digest()[i] ^ 0x5c;
				}
			}
			else
			{
				memset(_context.iPad, 0x36, TINY_TOOLKIT_HMAC_PAD_64_SIZE);
				memset(_context.oPad, 0x5c, TINY_TOOLKIT_HMAC_PAD_64_SIZE);

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
		 * @param key 密钥
		 * @param length 长度
		 *
		 */
		void HMAC_SHA1::SetKey(const std::string & key, std::size_t length)
		{
			if (length == 0)
			{
				return;
			}

			SetKey(key.c_str(), length);
		}

		/**
		 *
		 * 追加内容
		 *
		 * @param content 内容
		 *
		 */
		void HMAC_SHA1::Append(const char * content)
		{
			if (content == nullptr)
			{
				return;
			}

			Append(content, strlen(content));
		}

		/**
		 *
		 * 追加内容
		 *
		 * @param content 内容
		 *
		 */
		void HMAC_SHA1::Append(const uint8_t * content)
		{
			if (content == nullptr)
			{
				return;
			}

			Append(content, strlen(reinterpret_cast<const char *>(content)));
		}

		/**
		 *
		 * 追加内容
		 *
		 * @param content 内容
		 *
		 */
		void HMAC_SHA1::Append(const std::string & content)
		{
			Append(content, content.size());
		}

		/**
		 *
		 * 追加内容
		 *
		 * @param content 内容
		 * @param length 长度
		 *
		 */
		void HMAC_SHA1::Append(const char * content, std::size_t length)
		{
			if (content == nullptr || length == 0)
			{
				return;
			}

			Append(reinterpret_cast<const uint8_t *>(content), length);
		}

		/**
		 *
		 * 追加内容
		 *
		 * @param content 内容
		 * @param length 长度
		 *
		 */
		void HMAC_SHA1::Append(const uint8_t * content, std::size_t length)
		{
			if (content == nullptr || length == 0)
			{
				return;
			}

			_sha1.Append(content, length);

			_isComputed = false;
		}

		/**
		 *
		 * 追加内容
		 *
		 * @param content 内容
		 * @param length 长度
		 *
		 */
		void HMAC_SHA1::Append(const std::string & content, std::size_t length)
		{
			if (length == 0)
			{
				return;
			}

			Append(content.c_str(), length);
		}

		/**
		 *
		 * 摘要
		 *
		 * @return 摘要
		 *
		 */
		const uint8_t * HMAC_SHA1::Digest()
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
		const std::string & HMAC_SHA1::Result()
		{
			Generate();

			return _result;
		}

		/**
		 *
		 * 生成
		 *
		 */
		void HMAC_SHA1::Generate()
		{
			if (_isComputed)
			{
				return;
			}

			_isComputed = true;

			SHA1 sha1{ };

			sha1.Append(_context.oPad, sizeof(_context.oPad));
			sha1.Append(_sha1.Digest(), TINY_TOOLKIT_SHA1_DIGEST_SIZE);

			memcpy(_digest, sha1.Digest(), TINY_TOOLKIT_SHA1_DIGEST_SIZE);

			_result.assign(util::String::AsHexString(_digest, sizeof(_digest), false));
		}

		/**
		 *
		 * 初始化
		 *
		 * @param context 上下文
		 *
		 */
		void HMAC_SHA1::Initialization(Context & context)
		{
			memset(context.iPad, 0x36, sizeof(_context.iPad));
			memset(context.oPad, 0x5c, sizeof(_context.oPad));
		}
	}
}
