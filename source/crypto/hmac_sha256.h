#ifndef __TINY_TOOLKIT__CRYPTO__HMAC_SHA256__H__
#define __TINY_TOOLKIT__CRYPTO__HMAC_SHA256__H__


/**
 *
 *  作者: hm
 *
 *  说明: hmac_sha256
 *
 */


#include "sha256.h"


namespace tinyToolkit
{
	namespace crypto
	{
		class TINY_TOOLKIT_API HMAC_SHA256
		{
			typedef struct Context
			{
				uint8_t iPad[TINY_TOOLKIT_HMAC_PAD_64_SIZE]{ 0 };
				uint8_t oPad[TINY_TOOLKIT_HMAC_PAD_64_SIZE]{ 0 };
			}Context;

		public:
			/**
			 *
			 * 构造函数
			 *
			 */
			HMAC_SHA256();

			/**
			 *
			 * 重置
			 *
			 */
			void Reset();

			/**
			 *
			 * 设置密钥
			 *
			 * @param key 密钥
			 *
			 */
			void SetKey(const char * key);

			/**
			 *
			 * 设置密钥
			 *
			 * @param key 密钥
			 *
			 */
			void SetKey(const uint8_t * key);

			/**
			 *
			 * 设置密钥
			 *
			 * @param key 密钥
			 *
			 */
			void SetKey(const std::string & key);

			/**
			 *
			 * 设置密钥
			 *
			 * @param key 密钥
			 * @param length 长度
			 *
			 */
			void SetKey(const char * key, std::size_t length);

			/**
			 *
			 * 设置密钥
			 *
			 * @param key 密钥
			 * @param length 长度
			 *
			 */
			void SetKey(const uint8_t * key, std::size_t length);

			/**
			 *
			 * 设置密钥
			 *
			 * @param key 密钥
			 * @param length 长度
			 *
			 */
			void SetKey(const std::string & key, std::size_t length);

			/**
			 *
			 * 追加内容
			 *
			 * @param content 内容
			 *
			 */
			void Append(const char * content);

			/**
			 *
			 * 追加内容
			 *
			 * @param content 内容
			 *
			 */
			void Append(const uint8_t * content);

			/**
			 *
			 * 追加内容
			 *
			 * @param content 内容
			 *
			 */
			void Append(const std::string & content);

			/**
			 *
			 * 追加内容
			 *
			 * @param content 内容
			 * @param length 长度
			 *
			 */
			void Append(const char * content, std::size_t length);

			/**
			 *
			 * 追加内容
			 *
			 * @param content 内容
			 * @param length 长度
			 *
			 */
			void Append(const uint8_t * content, std::size_t length);

			/**
			 *
			 * 追加内容
			 *
			 * @param content 内容
			 * @param length 长度
			 *
			 */
			void Append(const std::string & content, std::size_t length);

			/**
			 *
			 * 摘要
			 *
			 * @return 摘要
			 *
			 */
			const uint8_t * Digest();

			/**
			 *
			 * 摘要
			 *
			 * @return 摘要
			 *
			 */
			const std::string & Result();

		private:
			/**
			 *
			 * 生成
			 *
			 */
			void Generate();

			/**
			 *
			 * 初始化
			 *
			 * @param context 上下文
			 *
			 */
			static void Initialization(Context & context);

		private:
			bool _isComputed{ false };

			uint8_t _digest[TINY_TOOLKIT_SHA256_DIGEST_SIZE]{ 0 };

			SHA256 _sha256{ };

			Context _context{ };

			std::string _result{ };
		};
	}
}


#endif // __TINY_TOOLKIT__CRYPTO__HMAC_SHA256__H__
