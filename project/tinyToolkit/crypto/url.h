#ifndef __TINY_TOOLKIT__CRYPTO__URL__H__
#define __TINY_TOOLKIT__CRYPTO__URL__H__


/**
 *
 *  作者: hm
 *
 *  说明: url编码
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API UrlTransform
	{
	public:
		/**
		 *
		 * 编码
		 *
		 * @param value 待编码内容
		 *
		 * @return 编码后内容
		 *
		 */
		static std::string Encode(const char * value);

		/**
		 *
		 * 编码
		 *
		 * @param value 待编码内容
		 *
		 * @return 编码后内容
		 *
		 */
		static std::string Encode(const uint8_t * value);

		/**
		 *
		 * 编码
		 *
		 * @param value 待编码内容
		 *
		 * @return 编码后内容
		 *
		 */
		static std::string Encode(const std::string & value);

		/**
		 *
		 * 编码
		 *
		 * @param value 待编码内容
		 * @param length 待编码内容长度
		 *
		 * @return 编码后内容
		 *
		 */
		static std::string Encode(const char * value, std::size_t length);

		/**
		 *
		 * 编码
		 *
		 * @param value 待编码内容
		 * @param length 待编码内容长度
		 *
		 * @return 编码后内容
		 *
		 */
		static std::string Encode(const uint8_t * value, std::size_t length);

		/**
		 *
		 * 编码
		 *
		 * @param value 待编码内容
		 * @param length 待编码内容长度
		 *
		 * @return 编码后内容
		 *
		 */
		static std::string Encode(const std::string & value, std::size_t length);

		/**
		 *
		 * 解码
		 *
		 * @param value 待解码内容
		 *
		 * @return 解码后内容
		 *
		 */
		static std::string Decode(const char * value);

		/**
		 *
		 * 解码
		 *
		 * @param value 待解码内容
		 *
		 * @return 解码后内容
		 *
		 */
		static std::string Decode(const uint8_t * value);

		/**
		 *
		 * 解码
		 *
		 * @param value 待解码内容
		 *
		 * @return 解码后内容
		 *
		 */
		static std::string Decode(const std::string & value);

		/**
		 *
		 * 解码
		 *
		 * @param value 待解码内容
		 * @param length 待解码内容长度
		 *
		 * @return 解码后内容
		 *
		 */
		static std::string Decode(const char * value, std::size_t length);

		/**
		 *
		 * 解码
		 *
		 * @param value 待解码内容
		 * @param length 待解码内容长度
		 *
		 * @return 解码后内容
		 *
		 */
		static std::string Decode(const uint8_t * value, std::size_t length);

		/**
		 *
		 * 解码
		 *
		 * @param value 待解码内容
		 * @param length 待解码内容长度
		 *
		 * @return 解码后内容
		 *
		 */
		static std::string Decode(const std::string & value, std::size_t length);
	};
}


#endif // __TINY_TOOLKIT__CRYPTO__URL__H__
