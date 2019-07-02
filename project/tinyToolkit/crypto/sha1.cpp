/**
 *
 *  作者: hm
 *
 *  说明: sha1编码
 *
 */


#include "sha1.h"

#include "../utilities/string.h"
#include "../utilities/operator.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 */
	SHA1::SHA1()
	{
		Initialization();
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param value 待加密数据
	 *
	 */
	SHA1::SHA1(const char * value) : SHA1()
	{
		if (value)
		{
			Update(value);
		}
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param value 待加密数据
	 *
	 */
	SHA1::SHA1(const uint8_t * value) : SHA1()
	{
		if (value)
		{
			Update(value);
		}
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param value 待加密数据
	 *
	 */
	SHA1::SHA1(const std::string & value) : SHA1()
	{
		Update(value);
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param data 待加密数据
	 * @param size 待加密数据长度
	 *
	 */
	SHA1::SHA1(const char * data, std::size_t size) : SHA1()
	{
		Update(data, size);
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param data 待加密数据
	 * @param size 待加密数据长度
	 *
	 */
	SHA1::SHA1(const uint8_t * data, std::size_t size) : SHA1()
	{
		Update(data, size);
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param data 待加密数据
	 * @param size 待加密数据长度
	 *
	 */
	SHA1::SHA1(const std::string & data, std::size_t size) : SHA1()
	{
		Update(data, size);
	}

	/**
	 *
	 * 重置
	 *
	 */
	void SHA1::Reset()
	{
		Operator::Clear(_hex);

		memset(reinterpret_cast<void *>(&_context), 0, sizeof(Context));

		Initialization();
	}

	/**
	 *
	 * 更新内容
	 *
	 * @param value 待更新数据
	 *
	 */
	void SHA1::Update(const char * value)
	{
		Update(value, strlen(value));
	}

	/**
	 *
	 * 更新内容
	 *
	 * @param value 待更新数据
	 *
	 */
	void SHA1::Update(const uint8_t * value)
	{
		Update(value, strlen(reinterpret_cast<const char *>(value)));
	}

	/**
	 *
	 * 更新内容
	 *
	 * @param value 待更新数据
	 *
	 */
	void SHA1::Update(const std::string & value)
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
	void SHA1::Update(const char * value, std::size_t size)
	{
		Update(reinterpret_cast<const uint8_t *>(value), size);
	}

	/**
	 *
	 * 更新内容
	 *
	 * @param value 待更新数据
	 * @param size 待更新数据长度
	 *
	 */
	void SHA1::Update(const uint8_t * value, std::size_t size)
	{
		if (value == nullptr || size == 0 || _context.isCorrupted)
		{
			return;
		}

		UpdateDigest(_context, value, size);

		_context.isComputed = false;
	}

	/**
	 *
	 * 更新内容
	 *
	 * @param value 待更新数据
	 * @param size 待更新数据长度
	 *
	 */
	void SHA1::Update(const std::string & value, std::size_t size)
	{
		Update(value.c_str(), size);
	}

	/**
	 *
	 * 转换后的16进制字符串
	 *
	 * @return 16进制字符串
	 *
	 */
	const std::string & SHA1::Hex()
	{
		ContextDigest();

		return _hex;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 初始化
	 *
	 */
	void SHA1::Initialization()
	{
		_context.hash[0] = 0x67452301;
		_context.hash[1] = 0xEFCDAB89;
		_context.hash[2] = 0x98BADCFE;
		_context.hash[3] = 0x10325476;
		_context.hash[4] = 0xC3D2E1F0;
	}

	/**
	 *
	 * 转换加密后的数据
	 *
	 */
	void SHA1::ContextDigest()
	{
		if (!_context.isComputed)
		{
			_context.isComputed = true;

			SHA1 temp(*this);

			uint8_t digest[20]{ 0 };

			FinalDigest(temp._context, digest);

			_hex.assign(String::AsHexString(digest, sizeof(digest), false));
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 结束加密
	 *
	 * @param context 内容结构
	 * @param digest 存储摘要
	 *
	 */
	void SHA1::FinalDigest(Context & context, uint8_t * digest)
	{
		PadMessage(context);

		for (int32_t i = 0; i < 20; ++i)
		{
			digest[i] = context.hash[i >> 2] >> 8 * (3 - (i & 0x03));
		}
	}

	/**
	 *
	 * 更新加密内容
	 *
	 * @param context 内容结构
	 * @param value 加密数据
	 * @param length 加密数据长度
	 *
	 */
	void SHA1::UpdateDigest(Context & context, const uint8_t * value, std::size_t length)
	{
		while (length-- && !context.isCorrupted)
		{
			context.messageBlock[context.messageBlockIndex++] = (*value & 0xFF);

			context.lengthLow += 8;
			context.lengthLow &= 0xFFFFFFFF;

			if (context.lengthLow == 0)
			{
				context.lengthHigh += 1;
				context.lengthHigh &= 0xFFFFFFFF;

				if (context.lengthHigh == 0)
				{
					context.isCorrupted = true;
				}
			}

			if (context.messageBlockIndex == 64)
			{
				ProcessMessageBlock(context);
			}

			++value;
		}
	}

	/**
	 *
	 * 填充
	 *
	 * @param context 内容结构
	 *
	 */
	void SHA1::PadMessage(Context & context)
	{
		if (context.messageBlockIndex > 55)
		{
			context.messageBlock[context.messageBlockIndex++] = 0x80;

			while (context.messageBlockIndex < 64)
			{
				context.messageBlock[context.messageBlockIndex++] = 0;
			}

			ProcessMessageBlock(context);

			while (context.messageBlockIndex < 56)
			{
				context.messageBlock[context.messageBlockIndex++] = 0;
			}
		}
		else
		{
			context.messageBlock[context.messageBlockIndex++] = 0x80;

			while (context.messageBlockIndex < 56)
			{
				context.messageBlock[context.messageBlockIndex++] = 0;
			}
		}

		context.messageBlock[56] = context.lengthHigh >> 24;
		context.messageBlock[57] = context.lengthHigh >> 16;
		context.messageBlock[58] = context.lengthHigh >> 8;
		context.messageBlock[59] = context.lengthHigh;
		context.messageBlock[60] = context.lengthLow >> 24;
		context.messageBlock[61] = context.lengthLow >> 16;
		context.messageBlock[62] = context.lengthLow >> 8;
		context.messageBlock[63] = context.lengthLow;

		ProcessMessageBlock(context);
	}

	/**
	 *
	 * 转换
	 *
	 * @param context 内容结构
	 *
	 */
	void SHA1::ProcessMessageBlock(Context & context)
	{
		const uint32_t K[] =
		{
			0x5A827999,
			0x6ED9EBA1,
			0x8F1BBCDC,
			0xCA62C1D6
		};

		#define CIRCULAR_SHIFT(bits, word) (((word) << (bits)) | ((word) >> (32-(bits))))

		uint32_t W[80];


		for (int32_t t = 0; t < 16; t++)
		{
			W[t]  = context.messageBlock[t * 4 + 0] << 24;
			W[t] |= context.messageBlock[t * 4 + 1] << 16;
			W[t] |= context.messageBlock[t * 4 + 2] << 8;
			W[t] |= context.messageBlock[t * 4 + 3];
		}

		for (int32_t t = 16; t < 80; t++)
		{
			W[t] = CIRCULAR_SHIFT(1, W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
		}

		uint32_t A = context.hash[0];
		uint32_t B = context.hash[1];
		uint32_t C = context.hash[2];
		uint32_t D = context.hash[3];
		uint32_t E = context.hash[4];

		for (int32_t t = 0; t < 20; t++)
		{
			uint32_t temp =  CIRCULAR_SHIFT(5, A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];

			E = D;
			D = C;
			C = CIRCULAR_SHIFT(30, B);
			B = A;
			A = temp;
		}

		for (int32_t t = 20; t < 40; t++)
		{
			uint32_t temp = CIRCULAR_SHIFT(5, A) + (B ^ C ^ D) + E + W[t] + K[1];

			E = D;
			D = C;
			C = CIRCULAR_SHIFT(30, B);
			B = A;
			A = temp;
		}

		for (int32_t t = 40; t < 60; t++)
		{
			uint32_t temp = CIRCULAR_SHIFT(5, A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];

			E = D;
			D = C;
			C = CIRCULAR_SHIFT(30, B);
			B = A;
			A = temp;
		}

		for (int32_t t = 60; t < 80; t++)
		{
			uint32_t temp = CIRCULAR_SHIFT(5,A) + (B ^ C ^ D) + E + W[t] + K[3];

			E = D;
			D = C;
			C = CIRCULAR_SHIFT(30, B);
			B = A;
			A = temp;
		}

		context.hash[0] += A;
		context.hash[1] += B;
		context.hash[2] += C;
		context.hash[3] += D;
		context.hash[4] += E;

		context.messageBlockIndex = 0;
	}
}
