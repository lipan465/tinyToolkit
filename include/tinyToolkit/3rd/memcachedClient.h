#ifndef __TINY_TOOLKIT__3RD__MEMCACHED_CLIENT__H__
#define __TINY_TOOLKIT__3RD__MEMCACHED_CLIENT__H__


#ifdef TINY_TOOLKIT_MEMCACHED_SUPPORT


/**
 *
 *  作者: hm
 *
 *  说明: memcached客户端
 *
 */


#include <libmemcached/memcached.h>

#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Memcached
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		~Memcached()
		{
			DisConnect();
		}

		/**
		 *
		 * 断开连接
		 *
		 */
		void DisConnect()
		{
			if (_memcached)
			{
				memcached_free(_memcached);

				_memcached = nullptr;
			}
		}

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
		bool Connect(const char * host = "127.0.0.1", uint16_t port = 11211)
		{
			if (_memcached)
			{
				_errorMsg = "MEMCACHED_ALREADY_INITIALIZE";

				return false;
			}

			_memcached = memcached_create(nullptr);

			if (_memcached == nullptr)
			{
				_errorMsg = "MEMCACHED_CREATE_FAILED";

				return false;
			}

			memcached_return ret;

			memcached_server_st * server = memcached_server_list_append(nullptr, host, port, &ret);

			if (server == nullptr)
			{
				_errorMsg = memcached_strerror(_memcached, ret);

				return false;
			}

			memcached_server_push(_memcached, server);
			memcached_server_list_free(server);

			/// 使用NO-BLOCK, 防止memcached倒掉时挂死
			ret = memcached_behavior_set(_memcached, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);

			if (ret != MEMCACHED_SUCCESS)
			{
				_errorMsg = memcached_strerror(_memcached, ret);

				return false;
			}

			/// 使用二进制协议传输
			ret = memcached_behavior_set(_memcached, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, 1);

			if (ret != MEMCACHED_SUCCESS)
			{
				_errorMsg = memcached_strerror(_memcached, ret);

				return false;
			}

			ret = memcached_behavior_set(_memcached, MEMCACHED_BEHAVIOR_DISTRIBUTION, MEMCACHED_DISTRIBUTION_CONSISTENT);

			if (ret != MEMCACHED_SUCCESS)
			{
				_errorMsg = memcached_strerror(_memcached, ret);

				return false;
			}

			ret = memcached_behavior_set(_memcached, MEMCACHED_BEHAVIOR_RETRY_TIMEOUT, 20);

			if (ret != MEMCACHED_SUCCESS)
			{
				_errorMsg = memcached_strerror(_memcached, ret);

				return false;
			}

			ret = memcached_behavior_set(_memcached, MEMCACHED_BEHAVIOR_SERVER_FAILURE_LIMIT, 5);

			if (ret != MEMCACHED_SUCCESS)
			{
				_errorMsg = memcached_strerror(_memcached, ret);

				return false;
			}

			ret = memcached_behavior_set(_memcached, MEMCACHED_BEHAVIOR_AUTO_EJECT_HOSTS, 1);

			if (ret != MEMCACHED_SUCCESS)
			{
				_errorMsg = memcached_strerror(_memcached, ret);

				return false;
			}

			return true;
		}

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
		bool GetValue(const std::string & key, std::string & value)
		{
			assert(_memcached);

			uint32_t flag;
			std::size_t len;
			memcached_return ret;

			char * tempStr = memcached_get(_memcached, key.c_str(), key.size(), &len, &flag, &ret);

			if (ret == MEMCACHED_SUCCESS)
			{
				if (tempStr)
				{
					value = tempStr;
				}
			}
			else
			{
				_errorMsg = memcached_strerror(_memcached, ret);
			}

			if (tempStr)
			{
				free(tempStr);
			}

			return (ret == MEMCACHED_SUCCESS);
		}

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
		bool SetValue(const std::string & key, const std::string & value)
		{
			assert(_memcached);

			uint32_t flags = 0;

			auto ret = memcached_set(_memcached, key.c_str(), key.size(), value.c_str(), value.size(), _expiration, flags);

			if (ret != MEMCACHED_SUCCESS)
			{
				_errorMsg = memcached_strerror(_memcached, ret);
			}

			return (ret == MEMCACHED_SUCCESS);
		}

		/**
		 *
		 * 设置期限
		 *
		 * @param expiration 期限
		 *
		 */
		void SetExpiration(const std::time_t expiration)
		{
			_expiration = expiration;
		}

		/**
		 *
		 * 删除关键字对应的值
		 *
		 * @param key 关键字
		 *
		 * @return 是否删除成功
		 *
		 */
		bool DeleteValue(const std::string & key)
		{
			assert(_memcached);

			auto ret = memcached_delete(_memcached, key.c_str(), key.size(), _expiration);

			if (ret != MEMCACHED_SUCCESS)
			{
				_errorMsg = memcached_strerror(_memcached, ret);
			}

			return (ret == MEMCACHED_SUCCESS);
		}

		/**
		 *
		 * 最后一次错误信息
		 *
		 * @return 错误信息
		 *
		 */
		const std::string & LastErrorMessage() const
		{
			return _errorMsg;
		}

	protected:
		std::time_t _expiration{ 0 };

		std::string _errorMsg{ };

		memcached_st * _memcached{ nullptr };
	};
}


#endif // TINY_TOOLKIT_MEMCACHED_SUPPORT


#endif // __TINY_TOOLKIT__3RD__MEMCACHED_CLIENT__H__
