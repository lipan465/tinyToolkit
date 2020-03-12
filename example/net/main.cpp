/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"

#include "util/string.h"


class TCPSession : public net::ITCPSession
{
public:
	explicit TCPSession(bool isServer = false) : _isServer(isServer)
	{
		_title = _isServer ? "Server Session" : "Client Session";
	}

	void OnError() override
	{
		util::String::Print
		(
			"{} [{}:{}] error\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);
	}

	void OnSocket() override
	{
		util::String::Print
		(
			"{} [{}:{}] socket success\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);
	}

	void OnSocketFailed() override
	{
		util::String::Print
		(
			"{} [{}:{}] socket failed\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);
	}

	void OnBind() override
	{
		util::String::Print
		(
			"{} [{}:{}] bind success\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);
	}

	void OnBindFailed() override
	{
		util::String::Print
		(
			"{} [{}:{}] bind failed\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);
	}

	void OnConnect() override
	{
		util::String::Print
		(
			"{} [{}:{}] connect [{}:{}] success\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port,
			PeerEndpoint().host,
			PeerEndpoint().port
		);

		if (!_isServer)
		{
			Send("request server message", 22);
		}
	}

	void OnDisconnect() override
	{
		util::String::Print
		(
			"{} [{}:{}] disconnect [{}:{}]\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port,
			PeerEndpoint().host,
			PeerEndpoint().port
		);
	}

	void OnConnectFailed() override
	{
		util::String::Print
		(
			"{} [{}:{}] connect [{}:{}] failed\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port,
			PeerEndpoint().host,
			PeerEndpoint().port
		);
	}

	void OnSend() override
	{
		util::String::Print
		(
			"{} [{}:{}] send [{}:{}] success, remain {} message\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port,
			PeerEndpoint().host,
			PeerEndpoint().port,
			RemainMessageCount()
		);

		if (_isServer)
		{
			if (RemainMessageCount() == 0)
			{
				Close();
			}
		}
	}

	void OnSendFailed() override
	{
		util::String::Print
		(
			"{} [{}:{}] send [{}:{}] failed\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port,
			PeerEndpoint().host,
			PeerEndpoint().port
		);
	}

	std::size_t OnReceive(const char * buffer, std::size_t length) override
	{
		util::String::Print
		(
			"{} [{}:{}] receive [{}:{}] length={} value={}\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port,
			PeerEndpoint().host,
			PeerEndpoint().port,
			length,
			buffer
		);

		if (_isServer)
		{
			static std::string body = util::String::Join
			(
				"{\r\n",
				"\t\"code\": 200,\r\n",
				"\t\"message\": \"Success\",\r\n",
				"\t\"data\": null\r\n",
				"}\r\n"
			);

			static std::string content = util::String::Join
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

	void OnReceiveFailed() override
	{
		util::String::Print
		(
			"{} [{}:{}] receive [{}:{}] failed\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port,
			PeerEndpoint().host,
			PeerEndpoint().port
		);
	}

private:
	bool _isServer{ false };

	std::string _title{ };
};


class TCPServer : public net::ITCPServer
{
public:
	void OnError() override
	{
		util::String::Print
		(
			"{} [{}:{}] event error\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);
	}

	void OnSocket() override
	{
		util::String::Print
		(
			"{} [{}:{}] socket success\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);
	}

	void OnSocketFailed() override
	{
		util::String::Print
		(
			"{} [{}:{}] socket failed\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);
	}

	void OnBind() override
	{
		util::String::Print
		(
			"{} [{}:{}] bind success\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);
	}

	void OnBindFailed() override
	{
		util::String::Print
		(
			"{} [{}:{}] bind failed\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);
	}

	void OnListen() override
	{
		util::String::Print
		(
			"{} [{}:{}] listen success\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);
	}

	void OnListenFailed() override
	{
		util::String::Print
		(
			"{} [{}:{}] listen failed\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);
	}

	net::ITCPSession * OnAccept() override
	{
		util::String::Print
		(
			"{} [{}:{}] accept success\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);

		auto session = new TCPSession(true);

		_pool.push_back(session);

		return session;
	}

	void OnAcceptFailed() override
	{
		util::String::Print
		(
			"{} [{}:{}] accept failed\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);
	}

	void OnDisconnect() override
	{
		util::String::Print
		(
			"{} [{}:{}] disconnect\r\n",
			_title,
			LocalEndpoint().host,
			LocalEndpoint().port
		);

		for (auto &iter : _pool)
		{
			if (iter == nullptr)
			{
				continue;
			}

			iter->Close();

			delete iter;
		}
	}

protected:
	std::string _title{ "Server" };

	std::vector<TCPSession *> _pool{ };
};


static void TCP()
{
	TCPServer server;

	if (!server.Launch("127.0.0.1", 10080, 1024))
	{
		util::String::Print
		(
			"launch server listen to [{}:{}] failed\r\n",
			server.LocalEndpoint().host,
			server.LocalEndpoint().port
		);
	}

	TCPSession session;

	if (!session.Launch("127.0.0.1", 10080, 1024))
	{
		util::String::Print
		(
			"launch session connect to [{}:{}] failed\r\n",
			session.PeerEndpoint().host,
			session.PeerEndpoint().port
		);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	session.Close();

	server.Close();
}


int main(int argc, char const * argv[])
{
	(void)argc;
	(void)argv;

	TCP();

	return 0;
}
