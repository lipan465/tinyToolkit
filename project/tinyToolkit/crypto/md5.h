#ifndef __TINY_TOOLKIT__CRYPTO__MD5__H__
#define __TINY_TOOLKIT__CRYPTO__MD5__H__


/**
 *
 *  作者: hm
 *
 *  说明: md5编码
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API MD5
	{
		typedef struct Context
		{
			uint32_t count[2]{ 0 };
			uint32_t state[4]{ 0 };
			uint8_t  buffer[64]{ 0 };
		}Context;

	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		MD5();

		/**
		 *
		 * 构造函数
		 *
		 * @param value 待加密数据
		 *
		 */
		explicit MD5(const char * value);

		/**
		 *
		 * 构造函数
		 *
		 * @param value 待加密数据
		 *
		 */
		explicit MD5(const uint8_t * value);

		/**
		 *
		 * 构造函数
		 *
		 * @param value 待加密数据
		 *
		 */
		explicit MD5(const std::string & value);

		/**
		 *
		 * 构造函数
		 *
		 * @param data 待加密数据
		 * @param size 待加密数据长度
		 *
		 */
		explicit MD5(const char * data, std::size_t size);

		/**
		 *
		 * 构造函数
		 *
		 * @param data 待加密数据
		 * @param size 待加密数据长度
		 *
		 */
		explicit MD5(const uint8_t * data, std::size_t size);

		/**
		 *
		 * 构造函数
		 *
		 * @param data 待加密数据
		 * @param size 待加密数据长度
		 *
		 */
		explicit MD5(const std::string & data, std::size_t size);

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
		 * 数据大小
		 *
		 * @return 数据大小
		 *
		 */
		std::size_t Size() const;

		/**
		 *
		 * 转换后的16位16进制字符串
		 *
		 * @return 16进制字符串
		 *
		 */
		const std::string & Hex16();

		/**
		 *
		 * 转换后的32位16进制字符串
		 *
		 * @return 16进制字符串
		 *
		 */
		const std::string & Hex32();

		/**
		 *
		 * 待转换字符串
		 *
		 * @return 字符串
		 *
		 */
		const std::string & Value() const;

	protected:
		/**
		 *
		 * 初始化
		 *
		 */
		void Initialization();

		/**
		 *
		 * 更新加密内容
		 *
		 * @param context 内容结构
		 * @param value 加密数据
		 * @param length 加密数据长度
		 *
		 */
		void UpdateDigest(Context & context, const uint8_t * value, std::size_t length);

		/**
		 *
		 * 结束加密
		 *
		 * @param context 内容结构
		 * @param data 存储数据
		 *
		 */
		void FinalDigest(Context & context, uint8_t data[16]);

		/**
		 *
		 * 转换加密后的数据
		 *
		 */
		void ContextDigest();

		/**
		 *
		 * 转换
		 *
		 * @param context 内容结构
		 * @param data 待转换数据
		 *
		 */
		void Transform(Context & context, const uint8_t data[64]);

		/**
		 *
		 * 编码
		 *
		 * @param input 待编码数据
		 * @param output 编码结果
		 * @param len 编码数据长度
		 *
		 */
		void Encode(const uint32_t * input, uint8_t * output, std::size_t len);

		/**
		 *
		 * 解码
		 *
		 * @param input 待解码数据
		 * @param output 解码结果
		 * @param len 解码数据长度
		 *
		 */
		void Decode(const uint8_t * input, uint32_t * output, std::size_t len);

	protected:
		bool _isNew{ false };

		Context _context{ };

		std::size_t _size{ 0 };

		std::string _hex16{ };
		std::string _hex32{ };
		std::string _value{ };
	};
}


#endif // __TINY_TOOLKIT__CRYPTO__MD5__H__