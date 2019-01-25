#ifndef __TINY_TOOLKIT__TOOL__HTTP__H__
#define __TINY_TOOLKIT__TOOL__HTTP__H__


/**
 *
 *  作者: hm
 *
 *  说明: http客户端
 *
 */


#include <curl/curl.h>

#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API HTTPProxy
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		~HTTPProxy();

		/**
		 *
		 * 清理
		 *
		 */
		void Clear();

	public:
		bool enable{ false };

		std::string host{ };
		std::string user{ };
		std::string pass{ };
		std::string login{ };

		curl_proxytype type{ CURLPROXY_HTTP };
	};

	class TINY_TOOLKIT_API HTTPSession
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		~HTTPSession();

		/**
		 *
		 * 清理
		 *
		 */
		void Clear();

	public:
		uint32_t count{ 0 };
		uint32_t verbose{ 0 };
		uint32_t errorCount{ 0 };
		uint32_t failedCount{ 0 };
		uint32_t successCount{ 0 };

		std::time_t timeout{ 0 };
		std::time_t receiveTimeout { 0 };

		std::string url{ };
		std::string user{ };
		std::string pass{ };
		std::string login{ };
		std::string caFile{ };
		std::string params{ };
		std::string cookie{ };
		std::string userAgent{ };
		std::string requestOption{ };

		struct curl_slist * headers{ nullptr };
	};

	class TINY_TOOLKIT_API HTTPResponse
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		~HTTPResponse();

		/**
		 *
		 * 清理
		 *
		 */
		void Clear();

	public:
		uint32_t code{ 0 };

		std::string header{ };
		std::string content{ };

		std::double_t totalTime{ 0.0 };
		std::double_t contentLength{ 0.0 };
	};

	class TINY_TOOLKIT_API HTTPClient
	{
		enum class HTTP_OPTION_TYPE : uint8_t
		{
			GET,
			PUT,
			POST,
			OTHER,
		};

	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		HTTPClient() = default;

		/**
		 *
		 * 构造函数
		 *
		 * @param url 链接
		 *
		 */
		explicit HTTPClient(std::string url);

		/**
		 *
		 * 析构函数
		 *
		 */
		~HTTPClient();

		/**
		 *
		 * get操作
		 *
		 * @return 操作结果
		 *
		 */
		bool Get();

		/**
		 *
		 * put操作
		 *
		 * @return 操作结果
		 *
		 */
		bool Put();

		/**
		 *
		 * post操作
		 *
		 * @return 操作结果
		 *
		 */
		bool Post();

		/**
		 *
		 * 请求操作
		 *
		 * @return 操作结果
		 *
		 */
		bool Request();

		/**
		 *
		 * 最后一次错误信息
		 *
		 * @return 最后一次错误信息
		 *
		 */
		const char * LastError() const;

		/**
		 *
		 * 代理信息
		 *
		 * @return 代理信息
		 *
		 */
		const HTTPProxy & Proxy() const;

		/**
		 *
		 * 会话信息
		 *
		 * @return 会话信息
		 *
		 */
		const HTTPSession & Session() const;

		/**
		 *
		 * 响应信息
		 *
		 * @return 响应信息
		 *
		 */
		const HTTPResponse & Response() const;

		/**
		 *
		 * 设置链接
		 *
		 * @param url 链接
		 *
		 */
		void SetUrl(std::string url);

		/**
		 *
		 * 设置登陆信息
		 *
		 * @param user 帐号
		 * @param pass 密码
		 *
		 */
		void SetLogin(std::string user, std::string pass);

		/**
		 *
		 * 设置代理信息
		 *
		 * @param host 主机
		 * @param user 帐号
		 * @param pass 密码
		 * @param type 类型
		 *
		 */
		void SetProxy(std::string host, std::string user, std::string pass, curl_proxytype type = CURLPROXY_HTTP);

		/**
		 *
		 * 设置参数
		 *
		 * @param param 参数
		 *
		 */
		void SetParam(std::string param);

		/**
		 *
		 * 设置ca证书路径
		 *
		 * @param caFile ca证书路径
		 *
		 */
		void SetCAFile(std::string caFile);

		/**
		 *
		 * 设置cookie
		 *
		 * @param cookie cookie
		 *
		 */
		void SetCookie(std::string cookie);

		/**
		 *
		 * 设置头部信息
		 *
		 * @param header 头部信息
		 *
		 */
		void SetHeader(const char * header);

		/**
		 *
		 * 设置用户代理
		 *
		 * @param userAgent 用户代理
		 *
		 */
		void SetUserAgent(std::string userAgent);

		/**
		 *
		 * 设置连接超时时间
		 *
		 * @param timeout 连接超时时间
		 *
		 */
		void SetConnectTimeout(std::time_t timeout);

		/**
		 *
		 * 设置接收数据超时时间
		 *
		 * @param timeout 接收数据超时时间
		 *
		 */
		void SetReceiveTimeout(std::time_t timeout);

		/**
		 *
		 * 设置请求方式
		 *
		 * @param requestOption 请求方式
		 *
		 */
		void SetRequestOption(std::string requestOption);

		/**
		 * 添加参数
		 *
		 * @param param 参数
		 *
		 */
		void AddParam(const std::string & param);

		/**
		 *
		 * 添加头部信息
		 *
		 * @param header 头部信息
		 *
		 */
		void AddHeader(const char * header);

		/**
		 *
		 * 清理
		 *
		 */
		void Clear();

		/**
		 *
		 * 清理代理信息
		 *
		 */
		void ClearProxy();

		/**
		 *
		 * 清理会话信息
		 *
		 */
		void ClearSession();

		/**
		 *
		 * 清理响应信息
		 *
		 */
		void ClearResponse();

		/**
		 *
		 * 启用详细信息
		 *
		 */
		void EnableVerbose();

		/**
		 *
		 * 禁用详细信息
		 *
		 */
		void DisableVerbose();

		/**
		 *
		 * 注销全局参数
		 *
		 */
		static void GlobalRelease();

		/**
		 *
		 * 初始化全局参数
		 *
		 * @return 是否初始化成功
		 *
		 */
		static bool GlobalInitialize();

	protected:
		/**
		 *
		 * 初始化
		 *
		 * @return 初始化状态
		 *
		 */
		bool Initialize();

		/**
		 *
		 * 设置代理信息
		 *
		 */
		void SetProxy();

		/**
		 *
		 * 设置会话信息
		 *
		 * @param option 操作类型
		 *
		 */
		void SetSession(HTTP_OPTION_TYPE option);

		/**
		 *
		 * 设置共享DNS解析
		 *
		 * @param curl 待设置句柄
		 *
		 */
		void SetShareDNSCache(CURL * curl);

		/**
		 *
		 * 解析响应代码
		 *
		 */
		void ParseResponseCode();

		/**
		 *
		 * 解析响应数据长度
		 *
		 */
		void ParseResponseLength();

		/**
		 *
		 * http操作
		 *
		 * @param option 操作类型
		 *
		 * @return 操作结果
		 *
		 */
		bool Launch(HTTP_OPTION_TYPE option);

		/**
		 *
		 * 响应回调函数
		 *
		 * @param buffer 响应数据缓冲区
		 * @param size 响应数据块大小
		 * @param nmemb 响应数据块个数
		 * @param stream 存储响应数据地址
		 *
		 * @return 响应数据大小
		 *
		 */
		static std::size_t WriteFunction(void * buffer, std::size_t size, std::size_t nmemb, void * stream);

		/**
		 *
		 * 响应头部信息回调函数
		 *
		 * @param buffer 响应数据缓冲区
		 * @param size 响应数据块大小
		 * @param nmemb 响应数据块个数
		 * @param stream 存储响应数据地址
		 *
		 * @return 响应数据大小
		 *
		 */
		static std::size_t HeaderFunction(void * buffer, std::size_t size, std::size_t nmemb, void * stream);

	protected:
		bool _isFirstWrite{ true };

		CURL * _curl{ nullptr };

		CURLcode _errorCode{ CURLE_OK };

		HTTPProxy _proxy{ };
		HTTPSession _session{ };
		HTTPResponse _response{ };
	};
}


#endif // __TINY_TOOLKIT__TOOL__HTTP__H__
