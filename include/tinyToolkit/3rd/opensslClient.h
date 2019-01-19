#ifndef __TINY_TOOLKIT__3RD__OPENSSL_CLIENT__H__
#define __TINY_TOOLKIT__3RD__OPENSSL_CLIENT__H__


#ifdef TINY_TOOLKIT_OPENSSL_SUPPORT


/**
 *
 *  作者: hm
 *
 *  说明: openssl客户端
 *
 */


#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

#include "../common/common.h"


namespace tinyToolkit
{
	#if (OPENSSL_VERSION_NUMBER < 0x10100000L) || defined(LIBRESSL_VERSION_NUMBER)
	#
	#  define EVP_MD_CTX_new EVP_MD_CTX_create
	#  define EVP_MD_CTX_free EVP_MD_CTX_destroy
	#
	#endif

	class TINY_TOOLKIT_API EVPObject
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param type 加密类型
		 *
		 */
		explicit EVPObject(std::string type) : _type(std::move(type))
		{
			OpenSSL_add_all_digests();

			Initialization();
		}

		/**
		 *
		 * 构造函数
		 *
		 * @param rhs 右值对象
		 *
		 */
		EVPObject(EVPObject && rhs) noexcept
		{
			_context = rhs._context;

			rhs._context = nullptr;
		}

		/**
		 *
		 * 构造函数
		 *
		 * @param lhs 左值对象
		 *
		 */
		EVPObject(const EVPObject & lhs)
		{
			_isNew = true;

			_context = EVP_MD_CTX_new();

			EVP_MD_CTX_copy(_context, lhs._context);
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~EVPObject()
		{
			Free();
		}

		/**
		 *
		 * 清理内容
		 *
		 */
		void Clear()
		{
			_str.clear();
			_hex.clear();

			_isNew = false;

			Free();

			Initialization();
		}

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新的数据
		 *
		 */
		void Update(const char * value)
		{
			if (value)
			{
				Update(value, strlen(value));
			}
		}

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新的数据
		 *
		 */
		void Update(const uint8_t * value)
		{
			if (value)
			{
				Update(value, strlen(reinterpret_cast<const char *>(value)));
			}
		}

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新的数据
		 *
		 */
		void Update(const std::string & value)
		{
			Update(value, value.size());
		}

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新数据
		 * @param size 待更新数据长度
		 *
		 */
		void Update(const char * value, std::size_t size)
		{
			if (value)
			{
				EVP_DigestUpdate(_context, value, size);

				_isNew = true;
			}
		}

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新数据
		 * @param size 待更新数据长度
		 *
		 */
		void Update(const uint8_t * value, std::size_t size)
		{
			if (value)
			{
				EVP_DigestUpdate(_context, value, size);

				_isNew = true;
			}
		}

		/**
		 *
		 * 更新内容
		 *
		 * @param value 待更新数据
		 * @param size 待更新数据长度
		 *
		 */
		void Update(const std::string & value, std::size_t size)
		{
			EVP_DigestUpdate(_context, value.c_str(), size);

			_isNew = true;
		}

		/**
		 *
		 * 16进制字符串
		 *
		 * @return 16进制字符串
		 *
		 */
		std::string Hex()
		{
			ContextDigest();

			return _hex;
		}

		/**
		 *
		 * 字符串
		 *
		 * @return 字符串
		 *
		 */
		std::string Str()
		{
			ContextDigest();

			return _str;
		}

	protected:
		/**
		 *
		 * 释放内容
		 *
		 */
		void Free()
		{
			if (_context)
			{
				EVP_MD_CTX_free(_context);

				_context = nullptr;
			}
		}

		/**
		 *
		 * 初始化
		 *
		 */
		void Initialization()
		{
			_context = EVP_MD_CTX_new();

			EVP_DigestInit(_context, EVP_get_digestbyname(_type.c_str()));
		}

		/**
		 *
		 * 加密数据
		 *
		 */
		void ContextDigest()
		{
			if (_isNew)
			{
				char c;

				_isNew = false;

				EVPObject temp(*this);

				int32_t size = EVP_MD_CTX_size(temp._context);

				uint8_t digest[EVP_MAX_MD_SIZE];

				EVP_DigestFinal(temp._context, digest, nullptr);

				_hex.assign("");
				_str.assign(reinterpret_cast<const char *>(digest), static_cast<uint32_t>(size));

				for (int32_t i = 0; i < size; i++)
				{
					c = static_cast<char>((digest[i] >> 4) & 0xf);
					c = static_cast<char>((c > 9) ? c + 'a' - 10 : c + '0');

					_hex.push_back(c);

					c = static_cast<char>(digest[i] & 0xf);
					c = static_cast<char>((c > 9) ? c + 'a' - 10 : c + '0');

					_hex.push_back(c);
				}
			}
		};

	private:
		bool _isNew{ false };

		EVP_MD_CTX * _context{ nullptr };

		std::string _str{ };
		std::string _hex{ };
		std::string _type{ };
	};
}


#endif // TINY_TOOLKIT_OPENSSL_SUPPORT


#endif // __TINY_TOOLKIT__3RD__OPENSSL_CLIENT__H__
