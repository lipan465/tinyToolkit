#ifndef __EXAMPLE__TEST__CRYPTO__H__
#define __EXAMPLE__TEST__CRYPTO__H__


#include "test.h"


class Crypto
{
public:
	static void Test()
	{
		{
			std::cout << "-------------------------------------------------- md5 --------------------------------------------------" << std::endl;

			tinyToolkit::MD5 md5;

			md5.Update("123");
			FUNCTION_TEST(md5.Size())
			FUNCTION_TEST(md5.Value())
			FUNCTION_TEST(md5.Hex16())
			FUNCTION_TEST(md5.Hex32())

			md5.Update("哈哈哈");
			FUNCTION_TEST(md5.Size())
			FUNCTION_TEST(md5.Value())
			FUNCTION_TEST(md5.Hex16())
			FUNCTION_TEST(md5.Hex32())

			md5.Reset();
			md5.Update("123");
			FUNCTION_TEST(md5.Size())
			FUNCTION_TEST(md5.Value())
			FUNCTION_TEST(md5.Hex16())
			FUNCTION_TEST(md5.Hex32())

			md5.Reset();
			md5.Update("哈哈哈");
			FUNCTION_TEST(md5.Size())
			FUNCTION_TEST(md5.Value())
			FUNCTION_TEST(md5.Hex16())
			FUNCTION_TEST(md5.Hex32())
		}

		{
			std::cout << "-------------------------------------------------- url --------------------------------------------------" << std::endl;

			FUNCTION_TEST(tinyToolkit::UrlTransform::Encode("123哈哈哈"))
			FUNCTION_TEST(tinyToolkit::UrlTransform::Decode(tinyToolkit::UrlTransform::Encode("123哈哈哈")))
		}

		{
			std::cout << "-------------------------------------------------- base64 --------------------------------------------------" << std::endl;

			FUNCTION_TEST(tinyToolkit::Base64::Encode("123哈哈哈"))
			FUNCTION_TEST(tinyToolkit::Base64::Decode(tinyToolkit::Base64::Encode("123哈哈哈")))
		}
	}
};


#endif // __EXAMPLE__TEST__CRYPTO__H__
