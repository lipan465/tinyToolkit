#ifndef __TINY_TOOLKIT_EXTRA__TOOL__EVP__H__
#define __TINY_TOOLKIT_EXTRA__TOOL__EVP__H__


/**
 *
 *  作者: hm
 *
 *  说明: openssl客户端
 *
 */


#include <string>

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>


namespace tinyToolkit
{
	#if (OPENSSL_VERSION_NUMBER < 0x10100000L) || defined(LIBRESSL_VERSION_NUMBER)
	#
	#  define EVP_MD_CTX_new EVP_MD_CTX_create
	#  define EVP_MD_CTX_free EVP_MD_CTX_destroy
	#
	#endif

	class EVPClient
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param type 加密类型
		 *
		 */
		explicit EVPClient(std::string type);

		/**
		 *
		 * 构造函数
		 *
		 * @param rhs 右值对象
		 *
		 */
		EVPClient(EVPClient && rhs) noexcept;

		/**
		 *
		 * 构造函数
		 *
		 * @param lhs 左值对象
		 *
		 */
		EVPClient(const EVPClient & lhs);

		/**
		 *
		 * 析构函数
		 *
		 */
		~EVPClient();

		/**
		 *
		 * 清理内容
		 *
		 */
		void Clear();

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新的数据
		 *
		 */
		void Update(const char * value);

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新的数据
		 *
		 */
		void Update(const uint8_t * value);

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新的数据
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
		 * 16进制字符串
		 *
		 * @return 16进制字符串
		 *
		 */
		std::string Hex();

		/**
		 *
		 * 字符串
		 *
		 * @return 字符串
		 *
		 */
		std::string Str();

	protected:
		/**
		 *
		 * 释放内容
		 *
		 */
		void Free();

		/**
		 *
		 * 加密数据
		 *
		 */
		void ContextDigest();

		/**
		 *
		 * 初始化
		 *
		 */
		void Initialization();

	private:
		bool _isNew{ false };

		EVP_MD_CTX * _context{ nullptr };

		std::string _str{ };
		std::string _hex{ };
		std::string _type{ };
	};
}


#endif // __TINY_TOOLKIT_EXTRA__TOOL__EVP__H__
