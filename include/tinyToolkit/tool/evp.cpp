/**
 *
 *  作者: hm
 *
 *  说明: openssl客户端
 *
 */


#ifdef WITH_OPENSSL


#include "evp.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 * @param type 加密类型
	 *
	 */
	EVPClient::EVPClient(std::string type) : _type(std::move(type))
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
	EVPClient::EVPClient(EVPClient && rhs) noexcept
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
	EVPClient::EVPClient(const EVPClient & lhs)
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
	EVPClient::~EVPClient()
	{
		Free();
	}

	/**
	 *
	 * 清理内容
	 *
	 */
	void EVPClient::Clear()
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
	void EVPClient::Update(const char * value)
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
	void EVPClient::Update(const uint8_t * value)
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
	void EVPClient::Update(const std::string & value)
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
	void EVPClient::Update(const char * value, std::size_t size)
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
	void EVPClient::Update(const uint8_t * value, std::size_t size)
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
	void EVPClient::Update(const std::string & value, std::size_t size)
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
	std::string EVPClient::Hex()
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
	std::string EVPClient::Str()
	{
		ContextDigest();

		return _str;
	}

	/**
	 *
	 * 释放内容
	 *
	 */
	void EVPClient::Free()
	{
		if (_context)
		{
			EVP_MD_CTX_free(_context);

			_context = nullptr;
		}
	}

	/**
	 *
	 * 加密数据
	 *
	 */
	void EVPClient::ContextDigest()
	{
		if (_isNew)
		{
			char c;

			_isNew = false;

			EVPClient temp(*this);

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

	/**
	 *
	 * 初始化
	 *
	 */
	void EVPClient::Initialization()
	{
		_context = EVP_MD_CTX_new();

		EVP_DigestInit(_context, EVP_get_digestbyname(_type.c_str()));
	}
}


#endif // WITH_OPENSSL