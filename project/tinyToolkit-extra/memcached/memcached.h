#ifndef __TINY_TOOLKIT_EXTRA__TOOL__MEMCACHED__H__
#define __TINY_TOOLKIT_EXTRA__TOOL__MEMCACHED__H__


/**
 *
 *  作者: hm
 *
 *  说明: memcached客户端
 *
 */


#include <ctime>
#include <string>

#include <libmemcached/memcached.h>


namespace tinyToolkit
{
	class MemcachedClient
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		~MemcachedClient();

		/**
		 *
		 * 断开连接
		 *
		 */
		void DisConnect();

		/**
		 *
		 * 建立连接
		 *
		 * @param host 主机地址
		 * @param port 端口
		 *
		 * @return 是否连接成功
		 *
		 */
		bool Connect(const char * host = "127.0.0.1", uint16_t port = 11211);

		/**
		 *
		 * 获取关键字对应的值
		 *
		 * @param key 关键字
		 * @param value 获取的值
		 *
		 * @return 是否获取成功
		 *
		 */
		bool GetValue(const std::string & key, std::string & value);

		/**
		 *
		 * 存储关键字对应的值
		 *
		 * @param key 关键字
		 * @param value 待存储的值
		 *
		 * @return 是否存储成功
		 *
		 */
		bool SetValue(const std::string & key, const std::string & value);

		/**
		 *
		 * 设置期限
		 *
		 * @param expiration 期限
		 *
		 */
		void SetExpiration(std::time_t expiration);

		/**
		 *
		 * 删除关键字对应的值
		 *
		 * @param key 关键字
		 *
		 * @return 是否删除成功
		 *
		 */
		bool DeleteValue(const std::string & key);

		/**
		 *
		 * 最后一次错误信息
		 *
		 * @return 错误信息
		 *
		 */
		const std::string & LastErrorMessage() const;

	protected:
		std::time_t _expiration{ 0 };

		std::string _errorMsg{ };

		memcached_st * _memcached{ nullptr };
	};
}


#endif // __TINY_TOOLKIT_EXTRA__TOOL__MEMCACHED__H__
