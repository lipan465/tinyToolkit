#ifndef __TINY_TOOLKIT__CRYPTO__URL__H__
#define __TINY_TOOLKIT__CRYPTO__URL__H__


/**
 *
 *  作者: hm
 *
 *  说明: url
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <string>
#
#endif


namespace tinyToolkit
{
	namespace crypto
	{
		class TINY_TOOLKIT_API Url
		{
		public:
			/**
			 *
			 * 编码
			 *
			 * @param content 内容
			 *
			 * @return 编码后内容
			 *
			 */
			static std::string Encode(const char * content);

			/**
			 *
			 * 编码
			 *
			 * @param content 内容
			 *
			 * @return 编码后内容
			 *
			 */
			static std::string Encode(const uint8_t * content);

			/**
			 *
			 * 编码
			 *
			 * @param content 内容
			 *
			 * @return 编码后内容
			 *
			 */
			static std::string Encode(const std::string & content);

			/**
			 *
			 * 编码
			 *
			 * @param content 内容
			 * @param length 长度
			 *
			 * @return 编码后内容
			 *
			 */
			static std::string Encode(const char * content, std::size_t length);

			/**
			 *
			 * 编码
			 *
			 * @param content 内容
			 * @param length 长度
			 *
			 * @return 编码后内容
			 *
			 */
			static std::string Encode(const uint8_t * content, std::size_t length);

			/**
			 *
			 * 编码
			 *
			 * @param content 内容
			 * @param length 长度
			 *
			 * @return 编码后内容
			 *
			 */
			static std::string Encode(const std::string & content, std::size_t length);

			/**
			 *
			 * 解码
			 *
			 * @param content 内容
			 *
			 * @return 解码后内容
			 *
			 */
			static std::string Decode(const char * content);

			/**
			 *
			 * 解码
			 *
			 * @param content 内容
			 *
			 * @return 解码后内容
			 *
			 */
			static std::string Decode(const uint8_t * content);

			/**
			 *
			 * 解码
			 *
			 * @param content 内容
			 *
			 * @return 解码后内容
			 *
			 */
			static std::string Decode(const std::string & content);

			/**
			 *
			 * 解码
			 *
			 * @param content 内容
			 * @param length 长度
			 *
			 * @return 解码后内容
			 *
			 */
			static std::string Decode(const char * content, std::size_t length);

			/**
			 *
			 * 解码
			 *
			 * @param content 内容
			 * @param length 长度
			 *
			 * @return 解码后内容
			 *
			 */
			static std::string Decode(const uint8_t * content, std::size_t length);

			/**
			 *
			 * 解码
			 *
			 * @param content 内容
			 * @param length 长度
			 *
			 * @return 解码后内容
			 *
			 */
			static std::string Decode(const std::string & content, std::size_t length);
		};
	}
}


#endif // __TINY_TOOLKIT__CRYPTO__URL__H__
