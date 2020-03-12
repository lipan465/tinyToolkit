/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void Url()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		std::string encode = crypto::Url::Encode("https://www.baidu.com/?p1=123&p2=uuid");
		std::string decode = crypto::Url::Decode(encode);

		std::cout << "encode : " << encode << std::endl;
		std::cout << "decode : " << decode << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void MD5()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		crypto::MD5 md5;

		md5.Append("123");
		md5.Append("abc");
		md5.Append(";'.");
		md5.Append("#$%");

		std::cout << "result : " << md5.Result() << std::endl;

		md5.Reset();

		md5.Append("666");
		md5.Append("xyz");
		md5.Append("[]/");
		md5.Append("!~@");

		std::cout << "result : " << md5.Result() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void SHA1()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		crypto::SHA1 sha1;

		sha1.Append("123");
		sha1.Append("abc");
		sha1.Append(";'.");
		sha1.Append("#$%");

		std::cout << "result : " << sha1.Result() << std::endl;

		sha1.Reset();

		sha1.Append("666");
		sha1.Append("xyz");
		sha1.Append("[]/");
		sha1.Append("!~@");

		std::cout << "result : " << sha1.Result() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void SHA224()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		crypto::SHA224 sha224;

		sha224.Append("123");
		sha224.Append("abc");
		sha224.Append(";'.");
		sha224.Append("#$%");

		std::cout << "result : " << sha224.Result() << std::endl;

		sha224.Reset();

		sha224.Append("666");
		sha224.Append("xyz");
		sha224.Append("[]/");
		sha224.Append("!~@");

		std::cout << "result : " << sha224.Result() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void SHA256()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		crypto::SHA256 sha256;

		sha256.Append("123");
		sha256.Append("abc");
		sha256.Append(";'.");
		sha256.Append("#$%");

		std::cout << "result : " << sha256.Result() << std::endl;

		sha256.Reset();

		sha256.Append("666");
		sha256.Append("xyz");
		sha256.Append("[]/");
		sha256.Append("!~@");

		std::cout << "result : " << sha256.Result() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void SHA384()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		crypto::SHA384 sha384;

		sha384.Append("123");
		sha384.Append("abc");
		sha384.Append(";'.");
		sha384.Append("#$%");

		std::cout << "result : " << sha384.Result() << std::endl;

		sha384.Reset();

		sha384.Append("666");
		sha384.Append("xyz");
		sha384.Append("[]/");
		sha384.Append("!~@");

		std::cout << "result : " << sha384.Result() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void SHA512()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		crypto::SHA512 sha512;

		sha512.Append("123");
		sha512.Append("abc");
		sha512.Append(";'.");
		sha512.Append("#$%");

		std::cout << "result : " << sha512.Result() << std::endl;

		sha512.Reset();

		sha512.Append("666");
		sha512.Append("xyz");
		sha512.Append("[]/");
		sha512.Append("!~@");

		std::cout << "result : " << sha512.Result() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void Base64()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		std::string encode = crypto::Base64::Encode("https://www.baidu.com/?p1=123&p2=uuid");
		std::string decode = crypto::Base64::Decode(encode);

		std::cout << "encode : " << encode << std::endl;
		std::cout << "decode : " << decode << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void HMAC_MD5()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		crypto::HMAC_MD5 hmac_md5;

		hmac_md5.SetKey("123");
		hmac_md5.Append("123");
		hmac_md5.Append("abc");
		hmac_md5.Append(";'.");
		hmac_md5.Append("#$%");

		std::cout << "result : " << hmac_md5.Result() << std::endl;

		hmac_md5.Reset();

		hmac_md5.SetKey("456");
		hmac_md5.Append("666");
		hmac_md5.Append("xyz");
		hmac_md5.Append("[]/");
		hmac_md5.Append("!~@");

		std::cout << "result : " << hmac_md5.Result() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void HMAC_SHA1()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		crypto::HMAC_SHA1 hmac_sha1;

		hmac_sha1.SetKey("123");
		hmac_sha1.Append("123");
		hmac_sha1.Append("abc");
		hmac_sha1.Append(";'.");
		hmac_sha1.Append("#$%");

		std::cout << "result : " << hmac_sha1.Result() << std::endl;

		hmac_sha1.Reset();

		hmac_sha1.SetKey("456");
		hmac_sha1.Append("666");
		hmac_sha1.Append("xyz");
		hmac_sha1.Append("[]/");
		hmac_sha1.Append("!~@");

		std::cout << "result : " << hmac_sha1.Result() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void HMAC_SHA224()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		crypto::HMAC_SHA224 hmac_sha224;

		hmac_sha224.SetKey("123");
		hmac_sha224.Append("123");
		hmac_sha224.Append("abc");
		hmac_sha224.Append(";'.");
		hmac_sha224.Append("#$%");

		std::cout << "result : " << hmac_sha224.Result() << std::endl;

		hmac_sha224.Reset();

		hmac_sha224.SetKey("456");
		hmac_sha224.Append("666");
		hmac_sha224.Append("xyz");
		hmac_sha224.Append("[]/");
		hmac_sha224.Append("!~@");

		std::cout << "result : " << hmac_sha224.Result() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void HMAC_SHA256()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		crypto::HMAC_SHA256 hmac_sha256;

		hmac_sha256.SetKey("123");
		hmac_sha256.Append("123");
		hmac_sha256.Append("abc");
		hmac_sha256.Append(";'.");
		hmac_sha256.Append("#$%");

		std::cout << "result : " << hmac_sha256.Result() << std::endl;

		hmac_sha256.Reset();

		hmac_sha256.SetKey("456");
		hmac_sha256.Append("666");
		hmac_sha256.Append("xyz");
		hmac_sha256.Append("[]/");
		hmac_sha256.Append("!~@");

		std::cout << "result : " << hmac_sha256.Result() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void HMAC_SHA384()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		crypto::HMAC_SHA384 hmac_sha384;

		hmac_sha384.SetKey("123");
		hmac_sha384.Append("123");
		hmac_sha384.Append("abc");
		hmac_sha384.Append(";'.");
		hmac_sha384.Append("#$%");

		std::cout << "result : " << hmac_sha384.Result() << std::endl;

		hmac_sha384.Reset();

		hmac_sha384.SetKey("456");
		hmac_sha384.Append("666");
		hmac_sha384.Append("xyz");
		hmac_sha384.Append("[]/");
		hmac_sha384.Append("!~@");

		std::cout << "result : " << hmac_sha384.Result() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void HMAC_SHA512()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		crypto::HMAC_SHA512 hmac_sha512;

		hmac_sha512.SetKey("123");
		hmac_sha512.Append("123");
		hmac_sha512.Append("abc");
		hmac_sha512.Append(";'.");
		hmac_sha512.Append("#$%");

		std::cout << "result : " << hmac_sha512.Result() << std::endl;

		hmac_sha512.Reset();

		hmac_sha512.SetKey("456");
		hmac_sha512.Append("666");
		hmac_sha512.Append("xyz");
		hmac_sha512.Append("[]/");
		hmac_sha512.Append("!~@");

		std::cout << "result : " << hmac_sha512.Result() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char const * argv[])
{
	(void)argc;
	(void)argv;

	Url();
	MD5();
	SHA1();
	SHA224();
	SHA256();
	SHA384();
	SHA512();
	Base64();
	HMAC_MD5();
	HMAC_SHA1();
	HMAC_SHA224();
	HMAC_SHA256();
	HMAC_SHA384();
	HMAC_SHA512();

	return 0;
}
