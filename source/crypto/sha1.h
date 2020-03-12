#ifndef __TINY_TOOLKIT__CRYPTO__SHA1__H__
#define __TINY_TOOLKIT__CRYPTO__SHA1__H__


/**
 *
 *  作者: hm
 *
 *  说明: sha1
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
		class TINY_TOOLKIT_API SHA1
		{
			typedef struct Context
			{
				bool isCorrupted{ false };

				int16_t messageBlockIndex{ 0 };

				uint8_t messageBlock[2 * TINY_TOOLKIT_SHA1_BLOCK_SIZE]{ 0 };

				uint32_t hash[5]{ 0 };
				uint32_t lengthLow{ 0 };
				uint32_t lengthHigh{ 0 };
			}Context;

		public:
			/**
			 *
			 * 构造函数
			 *
			 */
			SHA1();

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
			 *
			 */
			static void Transform(Context & context);

			/**
			 *
			 * 填充
			 *
			 * @param context 上下文
			 *
			 */
			static void PadMessage(Context & context);

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

			uint8_t _digest[TINY_TOOLKIT_SHA1_DIGEST_SIZE]{ 0 };

			Context _context{ };

			std::string _result{ };
		};
	}
}


#endif // __TINY_TOOLKIT__CRYPTO__SHA1__H__
