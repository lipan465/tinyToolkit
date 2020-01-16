/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


bool gIsServer = false;


class TCPSession : public tinyToolkit::ITCPSession
{
public:
	/**
	 *
	 * 缓存堵塞触发回调函数
	 *
	 * @return 是否关闭连接
	 *
	 */
	bool OnCacheFull() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] cache full\r\n",
			LocalAddress().host,
			LocalAddress().port
		);

		return true;
	}

	/**
	 *
	 * 事件错误触发回调函数
	 *
	 */
	void OnEventError() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] event error\r\n",
			LocalAddress().host,
			LocalAddress().port
		);
	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnDisconnect() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] disconnect [{}:{}]\r\n",
			LocalAddress().host,
			LocalAddress().port,
			PeerAddress().host,
			PeerAddress().port
		);
	}

	/**
	 *
	 * 发送成功触发回调函数
	 *
	 */
	void OnSend() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] send [{}:{}] success, remain {} message\r\n",
			LocalAddress().host,
			LocalAddress().port,
			PeerAddress().host,
			PeerAddress().port,
			RemainMessageCount()
		);

		if (gIsServer)
		{
			if (RemainMessageCount() == 0)
			{
				Close();
			}
		}
	}

	/**
	 *
	 * 发送失败触发回调函数
	 *
	 */
	void OnSendFailed() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] send [{}:{}] failed\r\n",
			LocalAddress().host,
			LocalAddress().port,
			PeerAddress().host,
			PeerAddress().port
		);
	}

	/**
	 *
	 * 监听连接触发回调函数
	 *
	 */
	void OnBind() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] bind success\r\n",
			LocalAddress().host,
			LocalAddress().port
		);
	}

	/**
	 *
	 * 监听失败触发回调函数
	 *
	 */
	void OnBindFailed() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] bind failed\r\n",
			LocalAddress().host,
			LocalAddress().port
		);
	}

	/**
	 *
	 * 套接字生成触发回调函数
	 *
	 */
	void OnSocket() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] socket success\r\n",
			LocalAddress().host,
			LocalAddress().port
		);
	}

	/**
	 *
	 * 套接字错误触发回调函数
	 *
	 */
	void OnSocketFailed() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] socket failed\r\n",
			LocalAddress().host,
			LocalAddress().port
		);
	}

	/**
	 *
	 * 连接成功触发回调函数
	 *
	 */
	void OnConnect() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] connect [{}:{}] success\r\n",
			LocalAddress().host,
			LocalAddress().port,
			PeerAddress().host,
			PeerAddress().port
		);
	}

	/**
	 *
	 * 连接失败触发回调函数
	 *
	 */
	void OnConnectFailed() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] connect [{}:{}] failed\r\n",
			LocalAddress().host,
			LocalAddress().port,
			PeerAddress().host,
			PeerAddress().port
		);
	}

	/**
	 *
	 * 接收失败触发回调函数
	 *
	 */
	void OnReceiveFailed() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] receive [{}:{}] failed\r\n",
			LocalAddress().host,
			LocalAddress().port,
			PeerAddress().host,
			PeerAddress().port
		);
	}

	/**
	 *
	 * 接收数据触发回调函数
	 *
	 * @param buffer 数据缓冲区指针
	 * @param length 数据缓冲区长度
	 *
	 * @return 使用的字节数
	 *
	 */
	std::size_t OnReceive(const char * buffer, std::size_t length) override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] receive [{}:{}] length={} value={}\r\n",
			LocalAddress().host,
			LocalAddress().port,
			PeerAddress().host,
			PeerAddress().port,
			length,
			buffer
		);

		if (gIsServer)
		{
			static std::string body = tinyToolkit::String::Join
			(
				"{\r\n",
				"\t\"code\": 200,\r\n",
				"\t\"message\": \"Success\",\r\n",
				"\t\"data\": null\r\n",
				"}\r\n"
			);

			static std::string content = tinyToolkit::String::Join
			(
				"HTTP/1.1 200 OK\r\n",
				"Server: licenseServiceCenter/0.1.0\r\n",
				"Connection: Close\r\n",
				"Content-Type: application/json; charset=utf-8\r\n",
				"Content-Length: " + std::to_string(body.size()) + "\r\n",
				"\r\n",
				body
			);

			Send(content.c_str(), content.size());
		}

		return length;
	}
};


class TCPServer : public tinyToolkit::ITCPServer
{
public:
	/**
	 *
	 * 事件错误触发回调函数
	 *
	 */
	void OnEventError() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] event error\r\n",
			LocalAddress().host,
			LocalAddress().port
		);
	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnDisconnect() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] disconnect\r\n",
			LocalAddress().host,
			LocalAddress().port
		);

		for (auto &iter : _pool)
		{
			iter->Close();

			delete iter;
		}
	}

	/**
	 *
	 * 监听连接触发回调函数
	 *
	 */
	void OnBind() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] bind success\r\n",
			LocalAddress().host,
			LocalAddress().port
		);
	}

	/**
	 *
	 * 监听失败触发回调函数
	 *
	 */
	void OnBindFailed() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] bind failed\r\n",
			LocalAddress().host,
			LocalAddress().port
		);
	}

	/**
	 *
	 * 套接字生成触发回调函数
	 *
	 */
	void OnSocket() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] socket success\r\n",
			LocalAddress().host,
			LocalAddress().port
		);
	}

	/**
	 *
	 * 套接字错误触发回调函数
	 *
	 */
	void OnSocketFailed() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] socket failed\r\n",
			LocalAddress().host,
			LocalAddress().port
		);
	}

	/**
	 *
	 * 监听连接触发回调函数
	 *
	 */
	void OnListen() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] listen success\r\n",
			LocalAddress().host,
			LocalAddress().port
		);
	}

	/**
	 *
	 * 监听失败触发回调函数
	 *
	 */
	void OnListenFailed() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] listen failed\r\n",
			LocalAddress().host,
			LocalAddress().port
		);
	}

	/**
	 *
	 * 连接失败触发回调函数
	 *
	 */
	void OnAcceptFailed() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] accept failed\r\n",
			LocalAddress().host,
			LocalAddress().port
		);
	}

	/**
	 *
	 * 会话连接触发回调函数
	 *
	 * @return 会话
	 *
	 */
	tinyToolkit::ITCPSession * OnAccept() override
	{
		tinyToolkit::String::Print
		(
			"[{}:{}] accept success\r\n",
			LocalAddress().host,
			LocalAddress().port
		);

		auto session = new TCPSession();

		_pool.push_back(session);

		return session;
	}

protected:
	std::vector<TCPSession *> _pool{ };
};


/**
 *
 * 启动tcp服务
 *
 */
void LaunchTCPServer(const char * localHost, uint16_t localPort)
{
	TCPServer server;

	if (!server.Launch(localHost, localPort, 1024))
	{
		tinyToolkit::String::Print
		(
			"launch server listen to [{}:{}] failed\r\n",
			localHost,
			localPort
		);
	}

	while (true)
	{
		std::string value{ };

		std::getline(std::cin, value);

		if (value == "q")
		{
			server.Close();

			break;
		}
	}
}


/**
 *
 * 启动tcp会话
 *
 * @param peerHost 目标地址
 * @param peerPort 目标端口
 * @param count 会话个数
 *
 */
void LaunchTCPSession(const char * peerHost, uint16_t peerPort, uint32_t count)
{
	std::vector<TCPSession *> sessionVec;

	tinyToolkit::ApplicationPool<TCPSession> sessionPool;

	for (uint32_t i = 0; i < count; ++i)
	{
		auto session = new TCPSession();

		if (!session->Launch(peerHost, peerPort, 1024))
		{
			tinyToolkit::String::Print
			(
				"launch session connect to [{}:{}] failed\r\n",
				peerHost,
				peerPort
			);

			delete session;
		}

		sessionVec.push_back(session);
	}

	while (true)
	{
		std::string value{ };

		std::getline(std::cin, value);

		if (value == "q")
		{
			for (auto &iter : sessionVec)
			{
				iter->Close();

				delete iter;
			}

			break;
		}
		else
		{
			for (auto &iter : sessionVec)
			{
				iter->Send(value.c_str(), value.size());
			}
		}
	}
}


int main(int argc, char const * argv[])
{
	uint16_t port  = 0;
	uint32_t count = 1;

	const char * type = nullptr;
	const char * host = nullptr;

	for (int32_t i = 1; i < argc; ++i)
	{
		const char * value = argv[i];

		if (strncmp(value, "--", 2) != 0)
		{
			throw std::invalid_argument("Arg invalid : " + std::string(value));
		}

		const char * find = strstr(value + 2, "=");

		if (find == nullptr)
		{
			throw std::invalid_argument("Arg need input : " + std::string(value));
		}

		const char * val = find + 1;
		const char * opt = value + 2;

		if (strncmp(opt, "type", 4) == 0)
		{
			type = val;
		}
		else if (strncmp(opt, "host", 4) == 0)
		{
			host = val;
		}
		else if (strncmp(opt, "port", 4) == 0)
		{
			port = static_cast<uint16_t>(strtoul(val, nullptr, 10));
		}
		else if (strncmp(opt, "count", 5) == 0)
		{
			count = static_cast<uint32_t>(strtoul(val, nullptr, 10));
		}
	}

	if (type == nullptr || host == nullptr)
	{
		return 0;
	}

	if (strncmp(type, "client", 6) == 0)
	{
		gIsServer = false;

		LaunchTCPSession(host, port, count);
	}
	else if (strncmp(type, "server", 6) == 0)
	{
		gIsServer = true;

		LaunchTCPServer(host, port);
	}

	return 0;
}
