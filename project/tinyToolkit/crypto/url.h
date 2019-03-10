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
		 * 加密
		 *
		 * @param value 待加密数据
		 *
		 * @return 加密后数据
		 *
		 */
		static std::string Encode(const char * value);

		/**
		 *
		 * 加密
		 *
		 * @param value 待加密数据
		 *
		 * @return 加密后数据
		 *
		 */
		static std::string Encode(const uint8_t * value);

		/**
		 *
		 * 加密
		 *
		 * @param value 待加密数据
		 *
		 * @return 加密后数据
		 *
		 */
		static std::string Encode(const std::string & value);

		/**
		 *
		 * 加密
		 *
		 * @param value 待加密数据
		 * @param size 待加密数据长度
		 *
		 * @return 加密后数据
		 *
		 */
		static std::string Encode(const char * value, std::size_t size);

		/**
		 *
		 * 加密
		 *
		 * @param value 待加密数据
		 * @param size 待加密数据长度
		 *
		 * @return 加密后数据
		 *
		 */
		static std::string Encode(const uint8_t * value, std::size_t size);

		/**
		 *
		 * 加密
		 *
		 * @param value 待加密数据
		 * @param size 待加密数据长度
		 *
		 * @return 加密后数据
		 *
		 */
		static std::string Encode(const std::string & value, std::size_t size);

		/**
		 *
		 * 解密
		 *
		 * @param value 待解密数据
		 *
		 * @return 解密后数据
		 *
		 */
		static std::string Decode(const char * value);

		/**
		 *
		 * 解密
		 *
		 * @param value 待解密数据
		 *
		 * @return 解密后数据
		 *
		 */
		static std::string Decode(const uint8_t * value);

		/**
		 *
		 * 解密
		 *
		 * @param value 待解密数据
		 *
		 * @return 解密后数据
		 *
		 */
		static std::string Decode(const std::string & value);

		/**
		 *
		 * 解密
		 *
		 * @param value 待解密数据
		 * @param size 待解密数据长度
		 *
		 * @return 解密后数据
		 *
		 */
		static std::string Decode(const char * value, std::size_t size);

		/**
		 *
		 * 解密
		 *
		 * @param value 待解密数据
		 * @param size 待解密数据长度
		 *
		 * @return 解密后数据
		 *
		 */
		static std::string Decode(const uint8_t * value, std::size_t size);

		/**
		 *
		 * 解密
		 *
		 * @param value 待解密数据
		 * @param size 待解密数据长度
		 *
		 * @return 解密后数据
		 *
		 */
		static std::string Decode(const std::string & value, std::size_t size);
	};
}


#endif // __TINY_TOOLKIT__CRYPTO__URL__H__
