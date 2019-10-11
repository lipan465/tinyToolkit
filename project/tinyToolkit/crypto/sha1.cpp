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
		Initialization(_context);
	}

	/**
	 *
	 * 重置
	 *
	 */
	void SHA1::Reset()
	{
		_isComputed = false;

		memset(_digest, 0, sizeof(_digest));
		memset(reinterpret_cast<void *>(&_context), 0, sizeof(_context));

		Operator::Clear(_result);

		Initialization(_context);
	}

	/**
	 *
	 * 追加内容
	 *
	 * @param value 待追加内容
	 *
	 */
	void SHA1::Append(const char * value)
	{
		if (value == nullptr)
		{
			return;
		}

		Append(value, strlen(value));
	}

	/**
	 *
	 * 追加内容
	 *
	 * @param value 待追加内容
	 *
	 */
	void SHA1::Append(const uint8_t * value)
	{
		if (value == nullptr)
		{
			return;
		}

		Append(value, strlen(reinterpret_cast<const char *>(value)));
	}

	/**
	 *
	 * 追加内容
	 *
	 * @param value 待追加内容
	 *
	 */
	void SHA1::Append(const std::string & value)
	{
		Append(value, value.size());
	}

	/**
	 *
	 * 追加内容
	 *
	 * @param value 待追加内容
	 * @param length 待追加内容长度
	 *
	 */
	void SHA1::Append(const char * value, std::size_t length)
	{
		if (value == nullptr || length == 0 || _context.isCorrupted)
		{
			return;
		}

		Append(reinterpret_cast<const uint8_t *>(value), length);
	}

	/**
	 *
	 * 追加内容
	 *
	 * @param value 待追加内容
	 * @param length 待追加内容长度
	 *
	 */
	void SHA1::Append(const uint8_t * value, std::size_t length)
	{
		if (value == nullptr || length == 0 || _context.isCorrupted)
		{
			return;
		}

		Update(_context, value, length);

		_isComputed = false;
	}

	/**
	 *
	 * 追加内容
	 *
	 * @param value 待追加内容
	 * @param length 待追加内容长度
	 *
	 */
	void SHA1::Append(const std::string & value, std::size_t length)
	{
		Append(value.c_str(), length);
	}

	/**
	 *
	 * 摘要
	 *
	 * @return 摘要
	 *
	 */
	const uint8_t * SHA1::Digest()
	{
		Generate();

		return _digest;
	}

	/**
	 *
	 * 摘要
	 *
	 * @return 摘要
	 *
	 */
	const std::string & SHA1::Result()
	{
		Generate();

		return _result;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 生成
	 *
	 */
	void SHA1::Generate()
	{
		if (!_isComputed)
		{
			_isComputed = true;

			Context context{ };

			memcpy(&context, &_context, sizeof(context));

			Final(context, _digest);

			_result.assign(String::AsHexString(_digest, sizeof(_digest), false));
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 完成
	 *
	 * @param context 内容结构
	 * @param digest 存储摘要
	 *
	 */
	void SHA1::Final(Context & context, uint8_t * digest)
	{
		PadMessage(context);

		for (int32_t i = 0; i < 20; ++i)
		{
			digest[i] = context.hash[i >> 2] >> 8 * (3 - (i & 0x03));
		}
	}

	/**
	 *
	 * 更新
	 *
	 * @param context 内容结构
	 * @param value 待更新内容
	 * @param length 待更新内容长度
	 *
	 */
	void SHA1::Update(Context & context, const uint8_t * value, std::size_t length)
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

	/**
	 *
	 * 初始化
	 *
	 * @param context 内容结构
	 *
	 */
	void SHA1::Initialization(Context & context)
	{
		context.hash[0] = 0x67452301;
		context.hash[1] = 0xEFCDAB89;
		context.hash[2] = 0x98BADCFE;
		context.hash[3] = 0x10325476;
		context.hash[4] = 0xC3D2E1F0;
	}
}
