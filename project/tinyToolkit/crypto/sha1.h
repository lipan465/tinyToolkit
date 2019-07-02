#ifndef __TINY_TOOLKIT__CRYPTO__SHA1__H__
#define __TINY_TOOLKIT__CRYPTO__SHA1__H__


/**
 *
 *  作者: hm
 *
 *  说明: sha1编码
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API SHA1
	{
		typedef struct Context
		{
			bool isComputed{ false };
			bool isCorrupted{ false };

			int16_t messageBlockIndex{ 0 };

			uint8_t messageBlock[64]{ 0 };

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
		 * 构造函数
		 *
		 * @param value 待加密数据
		 *
		 */
		explicit SHA1(const char * value);

		/**
		 *
		 * 构造函数
		 *
		 * @param value 待加密数据
		 *
		 */
		explicit SHA1(const uint8_t * value);

		/**
		 *
		 * 构造函数
		 *
		 * @param value 待加密数据
		 *
		 */
		explicit SHA1(const std::string & value);

		/**
		 *
		 * 构造函数
		 *
		 * @param data 待加密数据
		 * @param size 待加密数据长度
		 *
		 */
		explicit SHA1(const char * data, std::size_t size);

		/**
		 *
		 * 构造函数
		 *
		 * @param data 待加密数据
		 * @param size 待加密数据长度
		 *
		 */
		explicit SHA1(const uint8_t * data, std::size_t size);

		/**
		 *
		 * 构造函数
		 *
		 * @param data 待加密数据
		 * @param size 待加密数据长度
		 *
		 */
		explicit SHA1(const std::string & data, std::size_t size);

		/**
		 *
		 * 重置
		 *
		 */
		void Reset();

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新数据
		 *
		 */
		void Update(const char * value);

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新数据
		 *
		 */
		void Update(const uint8_t * value);

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新数据
		 *
		 */
		void Update(const std::string & value);

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新数据
		 * @param size 待更新数据长度
		 *
		 */
		void Update(const char * value, std::size_t size);

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新数据
		 * @param size 待更新数据长度
		 *
		 */
		void Update(const uint8_t * value, std::size_t size);

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新数据
		 * @param size 待更新数据长度
		 *
		 */
		void Update(const std::string & value, std::size_t size);

		/**
		 *
		 * 转换后的16进制字符串
		 *
		 * @return 16进制字符串
		 *
		 */
		const std::string & Hex();

	protected:
		/**
		 *
		 * 初始化
		 *
		 */
		void Initialization();

		/**
		 *
		 * 转换加密后的数据
		 *
		 */
		void ContextDigest();

	protected:
		/**
		 *
		 * 结束加密
		 *
		 * @param context 内容结构
		 * @param digest 存储摘要
		 *
		 */
		static void FinalDigest(Context & context, uint8_t * digest);

		/**
		 *
		 * 更新加密内容
		 *
		 * @param context 内容结构
		 * @param value 加密数据
		 * @param length 加密数据长度
		 *
		 */
		static void UpdateDigest(Context & context, const uint8_t * value, std::size_t length);

		/**
		 *
		 * 填充
		 *
		 * @param context 内容结构
		 *
		 */
		static void PadMessage(Context & context);

		/**
		 *
		 * 转换
		 *
		 * @param context 内容结构
		 *
		 */
		static void ProcessMessageBlock(Context & context);

	protected:
		Context _context{ };

		std::string _hex{ };
	};
}



#endif // __TINY_TOOLKIT__CRYPTO__SHA1__H__
