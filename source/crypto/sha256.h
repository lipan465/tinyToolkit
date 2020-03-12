#ifndef __TINY_TOOLKIT__CRYPTO__SHA256__H__
#define __TINY_TOOLKIT__CRYPTO__SHA256__H__


/**
 *
 *  作者: hm
 *
 *  说明: sha256
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <string>
#
#endif


namespace tinyToolkit
{
	namespace crypto
	{
		class TINY_TOOLKIT_API SHA256
		{
			typedef struct
			{
				uint8_t block[2 * TINY_TOOLKIT_SHA256_BLOCK_SIZE]{ 0 };

				uint32_t hash[8]{ 0 };

				uint32_t totalLength{ 0 };
				uint32_t blockLength{ 0 };
			}Context;

		public:
			/**
			 *
			 * 构造函数
			 *
			 */
			SHA256();

			/**
			 *
			 * 重置
			 *
			 */
			void Reset();

			/**
			 *
			 * 追加
			 *
			 * @param content 内容
			 *
			 */
			void Append(const char * content);

			/**
			 *
			 * 追加
			 *
			 * @param content 内容
			 *
			 */
			void Append(const uint8_t * content);

			/**
			 *
			 * 追加
			 *
			 * @param content 内容
			 *
			 */
			void Append(const std::string & content);

			/**
			 *
			 * 追加
			 *
			 * @param content 内容
			 * @param length 长度
			 *
			 */
			void Append(const char * content, std::size_t length);

			/**
			 *
			 * 追加
			 *
			 * @param content 内容
			 * @param length 长度
			 *
			 */
			void Append(const uint8_t * content, std::size_t length);

			/**
			 *
			 * 追加
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
			 * 完成
			 *
			 * @param context 上下文
			 * @param digest 摘要
			 *
			 */
			static void Final(Context & context, uint8_t * digest);

			/**
			 *
			 * 更新
			 *
			 * @param context 上下文
			 * @param content 内容
			 * @param length 长度
			 *
			 */
			static void Update(Context & context, const uint8_t * content, uint32_t length);

			/**
			 *
			 * 转换
			 *
			 * @param context 上下文
			 * @param block 块
			 * @param number 个数
			 *
			 */
			static void Transform(Context & context, const uint8_t * block, uint32_t number);

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

			Context _context{ };

			std::string _result{ };
		};
	}
}


#endif // __TINY_TOOLKIT__CRYPTO__SHA256__H__