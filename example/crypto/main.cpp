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
		std::string encode = tinyToolkit::UrlTransform::Encode("https://www.baidu.com/?p1=123&p2=uuid");
		std::string decode = tinyToolkit::UrlTransform::Decode(encode);

		std::cout << "encode : " << encode << std::endl;
		std::cout << "decode : " << decode << std::endl;
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
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
		tinyToolkit::MD5 md5;

		md5.Update("123");
		md5.Update("abc");
		md5.Update(";'.");
		md5.Update("#$%");

		std::cout << "str : " << "123abc;'.#$%" << std::endl;
		std::cout << "hex : " << md5.Hex() << std::endl;

		md5.Reset();

		md5.Update("666");
		md5.Update("xyz");
		md5.Update("[]/");
		md5.Update("!~@");

		std::cout << "str : " << "666xyz[]/!~@" << std::endl;
		std::cout << "hex : " << md5.Hex() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
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
		tinyToolkit::SHA1 sha1;

		sha1.Update("123");
		sha1.Update("abc");
		sha1.Update(";'.");
		sha1.Update("#$%");

		std::cout << "str : " << "123abc;'.#$%" << std::endl;
		std::cout << "hex : " << sha1.Hex() << std::endl;

		sha1.Reset();

		sha1.Update("666");
		sha1.Update("xyz");
		sha1.Update("[]/");
		sha1.Update("!~@");

		std::cout << "str : " << "666xyz[]/!~@" << std::endl;
		std::cout << "hex : " << sha1.Hex() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
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
		tinyToolkit::SHA224 sha224;

		sha224.Update("123");
		sha224.Update("abc");
		sha224.Update(";'.");
		sha224.Update("#$%");

		std::cout << "str : " << "123abc;'.#$%" << std::endl;
		std::cout << "hex : " << sha224.Hex() << std::endl;

		sha224.Reset();

		sha224.Update("666");
		sha224.Update("xyz");
		sha224.Update("[]/");
		sha224.Update("!~@");

		std::cout << "str : " << "666xyz[]/!~@" << std::endl;
		std::cout << "hex : " << sha224.Hex() << std::endl;
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
		tinyToolkit::SHA256 sha256;

		sha256.Update("123");
		sha256.Update("abc");
		sha256.Update(";'.");
		sha256.Update("#$%");

		std::cout << "str : " << "123abc;'.#$%" << std::endl;
		std::cout << "hex : " << sha256.Hex() << std::endl;

		sha256.Reset();

		sha256.Update("666");
		sha256.Update("xyz");
		sha256.Update("[]/");
		sha256.Update("!~@");

		std::cout << "str : " << "666xyz[]/!~@" << std::endl;
		std::cout << "hex : " << sha256.Hex() << std::endl;
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
		tinyToolkit::SHA384 sha384;

		sha384.Update("123");
		sha384.Update("abc");
		sha384.Update(";'.");
		sha384.Update("#$%");

		std::cout << "str : " << "123abc;'.#$%" << std::endl;
		std::cout << "hex : " << sha384.Hex() << std::endl;

		sha384.Reset();

		sha384.Update("666");
		sha384.Update("xyz");
		sha384.Update("[]/");
		sha384.Update("!~@");

		std::cout << "str : " << "666xyz[]/!~@" << std::endl;
		std::cout << "hex : " << sha384.Hex() << std::endl;
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
		tinyToolkit::SHA512 sha512;

		sha512.Update("123");
		sha512.Update("abc");
		sha512.Update(";'.");
		sha512.Update("#$%");

		std::cout << "str : " << "123abc;'.#$%" << std::endl;
		std::cout << "hex : " << sha512.Hex() << std::endl;

		sha512.Reset();

		sha512.Update("666");
		sha512.Update("xyz");
		sha512.Update("[]/");
		sha512.Update("!~@");

		std::cout << "str : " << "666xyz[]/!~@" << std::endl;
		std::cout << "hex : " << sha512.Hex() << std::endl;
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
		std::string encode = tinyToolkit::Base64::Encode("https://www.baidu.com/?p1=123&p2=uuid");
		std::string decode = tinyToolkit::Base64::Decode(encode);

		std::cout << "encode : " << encode << std::endl;
		std::cout << "decode : " << decode << std::endl;
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
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

	return 0;
}
