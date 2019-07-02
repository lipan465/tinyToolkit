#ifndef __TINY_TOOLKIT__CRYPTO__SHA384__H__
#define __TINY_TOOLKIT__CRYPTO__SHA384__H__


/**
 *
 *  作者: hm
 *
 *  说明: sha384编码
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API SHA384
	{
		typedef struct Context
		{
			bool isComputed{ false };

			uint8_t block[256]{ 0 };

			uint64_t hash[8]{ 0 };

			uint32_t totalLength{ 0 };
			uint32_t blockLength{ 0 };
		}Context;

	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		SHA384();

		/**
		 *
		 * 构造函数
		 *
		 * @param value 待加密数据
		 *
		 */
		explicit SHA384(const char * value);

		/**
		 *
		 * 构造函数
		 *
		 * @param value 待加密数据
		 *
		 */
		explicit SHA384(const uint8_t * value);

		/**
		 *
		 * 构造函数
		 *
		 * @param value 待加密数据
		 *
		 */
		explicit SHA384(const std::string & value);

		/**
		 *
		 * 构造函数
		 *
		 * @param data 待加密数据
		 * @param size 待加密数据长度
		 *
		 */
		explicit SHA384(const char * data, std::size_t size);

		/**
		 *
		 * 构造函数
		 *
		 * @param data 待加密数据
		 * @param size 待加密数据长度
		 *
		 */
		explicit SHA384(const uint8_t * data, std::size_t size);

		/**
		 *
		 * 构造函数
		 *
		 * @param data 待加密数据
		 * @param size 待加密数据长度
		 *
		 */
		explicit SHA384(const std::string & data, std::size_t size);

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
		 * 转换
		 *
		 * @param context 内容结构
		 * @param block 待转换块
		 * @param number 待转换块个数
		 *
		 */
		static void Transform(Context & context, const uint8_t * block, uint32_t number);

	protected:
		Context _context{ };

		std::string _hex{ };
	};
}


#endif // __TINY_TOOLKIT__CRYPTO__SHA384__H__
