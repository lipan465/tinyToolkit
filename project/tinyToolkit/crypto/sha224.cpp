/**
 *
 *  作者: hm
 *
 *  说明: sha224编码
 *
 */


#include "sha224.h"

#include "../utilities/string.h"
#include "../utilities/operator.h"


namespace tinyToolkit
{
	#define SHA224_BLOCK_SIZE  (512 / 8)
	#define SHA224_DIGEST_SIZE (224 / 8)


	////////////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 *
	 * 构造函数
	 *
	 */
	SHA224::SHA224()
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
	SHA224::SHA224(const char * value) : SHA224()
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
	SHA224::SHA224(const uint8_t * value) : SHA224()
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
	SHA224::SHA224(const std::string & value) : SHA224()
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
	SHA224::SHA224(const char * data, std::size_t size) : SHA224()
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
	SHA224::SHA224(const uint8_t * data, std::size_t size) : SHA224()
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
	SHA224::SHA224(const std::string & data, std::size_t size) : SHA224()
	{
		Update(data, size);
	}

	/**
	 *
	 * 重置
	 *
	 */
	void SHA224::Reset()
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
	void SHA224::Update(const char * value)
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
	void SHA224::Update(const uint8_t * value)
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
	void SHA224::Update(const std::string & value)
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
	void SHA224::Update(const char * value, std::size_t size)
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
	void SHA224::Update(const uint8_t * value, std::size_t size)
	{
		if (value == nullptr || size == 0)
		{
			return;
		}

		UpdateDigest(_context, value, static_cast<uint32_t>(size));

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
	void SHA224::Update(const std::string & value, std::size_t size)
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
	const std::string & SHA224::Hex()
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
	void SHA224::Initialization()
	{
		_context.hash[0] = 0xC1059ED8;
		_context.hash[1] = 0x367CD507;
		_context.hash[2] = 0x3070DD17;
		_context.hash[3] = 0xF70E5939;
		_context.hash[4] = 0xFFC00B31;
		_context.hash[5] = 0x68581511;
		_context.hash[6] = 0x64F98FA7;
		_context.hash[7] = 0xBEFA4FA4;
	}

	/**
	 *
	 * 转换加密后的数据
	 *
	 */
	void SHA224::ContextDigest()
	{
		if (!_context.isComputed)
		{
			_context.isComputed = true;

			SHA224 temp(*this);

			uint8_t digest[SHA224_DIGEST_SIZE]{ 0 };

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
	void SHA224::FinalDigest(Context & context, uint8_t * digest)
	{
		#define UNPACK32(x, str)                      \
		{                                             \
			*((str) + 3) = (uint8_t) ((x)      );     \
			*((str) + 2) = (uint8_t) ((x) >>  8);     \
			*((str) + 1) = (uint8_t) ((x) >> 16);     \
			*((str) + 0) = (uint8_t) ((x) >> 24);     \
		}

		uint32_t totalLength = (context.totalLength + context.blockLength) << 3;
		uint32_t blockNumber = (1 + ((SHA224_BLOCK_SIZE - 9) < (context.blockLength % SHA224_BLOCK_SIZE)));
		uint32_t blockLength = blockNumber << 6;

		memset(&context.block[context.blockLength], 0, blockLength - context.blockLength);

		context.block[context.blockLength] = 0x80;

		UNPACK32(totalLength, context.block + blockLength - 4)

		Transform(context, context.block, blockNumber);

		for (int32_t i = 0 ; i < 7; i++)
		{
			UNPACK32(context.hash[i], digest + (i << 2))
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
	void SHA224::UpdateDigest(Context & context, const uint8_t * value, uint32_t length)
	{
		uint32_t fill = SHA224_BLOCK_SIZE - context.blockLength;

		if (fill > length)
		{
			fill = length;
		}

		memcpy(&context.block[context.blockLength], value, fill);

		if (context.blockLength + length < SHA224_BLOCK_SIZE)
		{
			context.blockLength += length;

			return;
		}

		uint32_t overLength = length - fill;
		uint32_t blockNumber = overLength / SHA224_BLOCK_SIZE;

		const uint8_t * overValue = value + fill;

		Transform(context, context.block, 1);
		Transform(context, overValue, blockNumber);

		fill = overLength % SHA224_BLOCK_SIZE;

		memcpy(context.block, &overValue[blockNumber << 6], fill);

		context.blockLength  = fill;
		context.totalLength += (blockNumber + 1) << 6;
	}

	/**
	 *
	 * 转换
	 *
	 * @param context 内容结构
	 * @param block 待转换块
	 * @param number 待转换块个数
	 *
	 */
	void SHA224::Transform(Context & context, const uint8_t * block, uint32_t number)
	{
		#define CH(x, y, z) ((x & y) ^ (~x & z))
		#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

		#define SHFR(x, n) (x >> n)
		#define ROTR(x, n) ((x >> n) | (x << ((sizeof(x) << 3) - n)))
		#define ROTL(x, n) ((x << n) | (x >> ((sizeof(x) << 3) - n)))

		#define PACK32(str, x)                        \
		{                                             \
			*(x) =	((uint32_t) *((str) + 3)      ) | \
					((uint32_t) *((str) + 2) <<  8) | \
					((uint32_t) *((str) + 1) << 16) | \
					((uint32_t) *((str) + 0) << 24);  \
		}

		#define SHA224_F1(x) (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
		#define SHA224_F2(x) (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))
		#define SHA224_F3(x) (ROTR(x,  7) ^ ROTR(x, 18) ^ SHFR(x,  3))
		#define SHA224_F4(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHFR(x, 10))

		#define SHA224_SCR(i)                        \
		{                                            \
			Y[i] = SHA224_F4(Y[i -  2]) + Y[i -  7]  \
				 + SHA224_F3(Y[i - 15]) + Y[i - 16]; \
		}

		static uint32_t SHA224_KEY[64] =
		{
			0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
			0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
			0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
			0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
			0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
			0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
			0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,
			0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
			0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
			0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
			0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
			0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
			0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
			0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
			0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
			0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
		};

		uint32_t X[8]{ 0 };
		uint32_t Y[64]{ 0 };

		for (uint32_t i = 0; i < number; i++)
		{
			const uint8_t * blockValue = block + (i << 6);

			for (int32_t j = 0; j < 16; j++)
			{
				PACK32(&blockValue[j << 2], &Y[j])
			}

			for (int32_t j = 16; j < 64; j++)
			{
				SHA224_SCR(j)
			}

			for (int32_t j = 0; j < 8; j++)
			{
				X[j] = context.hash[j];
			}

			for (int32_t j = 0; j < 64; j++)
			{
				uint32_t t1 = X[7] + SHA224_F2(X[4]) + CH(X[4], X[5], X[6]) + SHA224_KEY[j] + Y[j];
				uint32_t t2 = SHA224_F1(X[0]) + MAJ(X[0], X[1], X[2]);

				X[7] = X[6];
				X[6] = X[5];
				X[5] = X[4];
				X[4] = X[3] + t1;
				X[3] = X[2];
				X[2] = X[1];
				X[1] = X[0];
				X[0] = t1 + t2;
			}

			for (int32_t j = 0; j < 8; j++)
			{
				context.hash[j] += X[j];
			}
		}
	}
}
