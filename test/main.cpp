/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


class TCPClientSession : public tinyToolkit::ITCPSession
{
public:
	/**
	 *
	 * 构造函数
	 *
	 * @param id 序号
	 *
	 */
	explicit TCPClientSession(int32_t id) : _id(id)
	{

	}

	/**
	 *
	 * 析构函数
	 *
	 */
	~TCPClientSession() override
	{
		tinyToolkit::String::Print("客户端会话析构\r\n");
	}

	/**
	 *
	 * 接收数据触发回调函数
	 *
	 * @param data 接收的数据缓冲区
	 * @param size 接收的数据缓冲区长度
	 *
	 */
	void OnReceive(const char * data, std::size_t size) override
	{
		tinyToolkit::String::Print("客户端会话[{}:{}]接收到服务器会话[{}:{}]长度为[{}]的信息 : {}\r\n", _localHost, _localPort, _remoteHost, _remotePort, size, data);
	}

	/**
	 *
	 * 连接成功触发回调函数
	 *
	 */
	void OnConnect() override
	{
		tinyToolkit::String::Print("客户端会话[{}:{}]连接服务器会话[{}:{}]成功\r\n", _localHost, _localPort, _remoteHost, _remotePort);

		std::string value = tinyToolkit::String::Format("tcp服务器你好, 我的编号为{}", _id);

		Send(value.c_str(), value.size());
	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnDisconnect() override
	{
		tinyToolkit::String::Print("客户端会话[{}:{}]断开连接\r\n", _localHost, _localPort);
	}

	/**
	 *
	 * 连接失败触发回调函数
	 *
	 */
	void OnConnectFailed() override
	{
		tinyToolkit::String::Print("客户端会话[{}:{}]连接服务器会话[{}:{}]失败\r\n", _localHost, _localPort, _remoteHost, _remotePort);
	}

private:
	int32_t _id{ 0 };
};


class TCPServerSession : public tinyToolkit::ITCPSession
{
public:
	/**
	 *
	 * 构造函数
	 *
	 * @param id 序号
	 *
	 */
	explicit TCPServerSession(int32_t id) : _id(id)
	{

	}

	/**
	 *
	 * 析构函数
	 *
	 */
	~TCPServerSession() override
	{
		tinyToolkit::String::Print("服务器会话析构\r\n");
	}

	/**
	 *
	 * 接收数据触发回调函数
	 *
	 * @param data 接收的数据缓冲区
	 * @param size 接收的数据缓冲区长度
	 *
	 */
	void OnReceive(const char * data, std::size_t size) override
	{
		tinyToolkit::String::Print("服务器会话[{}:{}]接收到客户端会话[{}:{}]长度为[{}]的信息 : {}\r\n", _localHost, _localPort, _remoteHost, _remotePort, size, data);

		if (strstr(data, "request"))
		{
			std::string value = tinyToolkit::String::Format("tcp客户端你好, 请求已收到");

			Send(value.c_str(), value.size());
		}
	}

	/**
	 *
	 * 连接成功触发回调函数
	 *
	 */
	void OnConnect() override
	{
		tinyToolkit::String::Print("服务器会话[{}:{}]连接客户端会话[{}:{}]成功\r\n", _localHost, _localPort, _remoteHost, _remotePort);

		std::string value = tinyToolkit::String::Format("tcp客户端你好, 我的编号为{}", _id);

		Send(value.c_str(), value.size());
	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnDisconnect() override
	{
		tinyToolkit::String::Print("服务器会话[{}:{}]断开连接\r\n", _localHost, _localPort);
	}

	/**
	 *
	 * 连接失败触发回调函数
	 *
	 */
	void OnConnectFailed() override
	{
		tinyToolkit::String::Print("服务器会话[{}:{}]连接客户端会话[{}:{}]失败\r\n", _localHost, _localPort, _remoteHost, _remotePort);
	}

private:
	int32_t _id{ 0 };
};


class TCPServer : public tinyToolkit::ITCPServer
{
public:
	/**
	 *
	 * 析构函数
	 *
	 */
	~TCPServer() override
	{
		tinyToolkit::String::Print("服务器析构\r\n");
	}

	/**
	 *
	 * 新连接触发回调函数
	 *
	 * @param host 主机地址
	 * @param port 主机端口
	 *
	 * @return 会话
	 *
	 */
	tinyToolkit::ITCPSession * OnNewConnect(const std::string & host, uint16_t port) override
	{
		tinyToolkit::String::Print("服务器[{}:{}]接收到客户端[{}:{}]会话请求\r\n", _host, _port, host, port);

		auto key = tinyToolkit::String::Splice(host, ":", port);

		auto find = _pool.find(key);

		if (find == _pool.end())
		{
			_pool.insert(std::make_pair(key, new TCPServerSession(port)));

			find = _pool.find(key);
		}

		return find->second;
	}

	/**
	 *
	 * 会话错误触发回调函数
	 *
	 */
	void OnSessionError(tinyToolkit::ITCPSession * session) override
	{
		tinyToolkit::String::Print("服务器[{}:{}]与客户端[{}:{}]会话错误\r\n", _host, _port, session->_remoteHost, session->_remotePort);

		auto key = tinyToolkit::String::Splice(session->_remoteHost, ":", session->_remotePort);

		auto find = _pool.find(key);

		if (find != _pool.end())
		{
			find->second->Close();

			delete find->second;

			_pool.erase(find);
		}
	}

	/**
	 *
	 * 错误触发回调函数
	 *
	 */
	void OnError() override
	{
		tinyToolkit::String::Print("服务器[{}:{}]异常\r\n", _host, _port);
	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnRelease() override
	{
		tinyToolkit::String::Print("服务器[{}:{}]断开连接\r\n", _host, _port);

		for (auto &iter : _pool)
		{
			iter.second->Close();

			delete iter.second;
		}
	}

protected:
	std::unordered_map<std::string, tinyToolkit::ITCPSession *> _pool{ };
};


class UDPClientSession : public tinyToolkit::IUDPSession
{
public:
	/**
	 *
	 * 构造函数
	 *
	 * @param id 序列号
	 *
	 */
	explicit UDPClientSession(int32_t id) : _id(id)
	{

	}

	/**
	 *
	 * 析构函数
	 *
	 */
	~UDPClientSession() override
	{
		tinyToolkit::String::Print("客户端会话析构\r\n");
	}

	/**
	 *
	 * 接收数据触发回调函数
	 *
	 * @param host 接收的主机地址
	 * @param port 接收的主机端口
	 * @param data 接收的数据缓冲区
	 * @param size 接收的数据缓冲区长度
	 *
	 */
	void OnReceive(const char * host, uint16_t port, const char * data, std::size_t size) override
	{
		tinyToolkit::String::Print("客户端会话[{}:{}]接收到服务器会话[{}:{}]长度为[{}]的信息 : {}\r\n", _localHost, _localPort, host, port, size, data);
	}

	/**
	 *
	 * 连接成功触发回调函数
	 *
	 */
	void OnConnect() override
	{
		tinyToolkit::String::Print("客户端会话[{}:{}]连接服务器会话[{}:{}]成功\r\n", _localHost, _localPort, _remoteHost, _remotePort);

		std::string value = tinyToolkit::String::Format("udp服务器你好, 我的编号为{}", _id);

		Send(_remoteHost.c_str(), _remotePort, value.c_str(), value.size());
	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnDisconnect() override
	{
		tinyToolkit::String::Print("客户端会话[{}:{}]断开连接\r\n", _id);
	}

	/**
	 *
	 * 连接失败触发回调函数
	 *
	 */
	void OnConnectFailed() override
	{
		tinyToolkit::String::Print("客户端会话[{}:{}]连接服务器会话[{}:{}]失败\r\n", _localHost, _localPort, _remoteHost, _remotePort);
	}

private:
	int32_t _id{ 0 };
};


class UDPServerSession : public tinyToolkit::IUDPSession
{
public:
	/**
	 *
	 * 构造函数
	 *
	 * @param id 序号
	 *
	 */
	explicit UDPServerSession(int32_t id) : _id(id)
	{

	}

	/**
	 *
	 * 析构函数
	 *
	 */
	~UDPServerSession() override
	{
		tinyToolkit::String::Print("服务器会话析构\r\n");
	}

	/**
	 *
	 * 接收数据触发回调函数
	 *
	 * @param host 接收的主机地址
	 * @param port 接收的主机端口
	 * @param data 接收的数据缓冲区
	 * @param size 接收的数据缓冲区长度
	 *
	 */
	void OnReceive(const char * host, uint16_t port, const char * data, std::size_t size) override
	{
		tinyToolkit::String::Print("服务器会话[{}:{}]接收到客户端会话[{}:{}]长度为[{}]的信息 : {}\r\n", _localHost, _localPort, host, port, size, data);

		if (strstr(data, "request"))
		{
			std::string value = tinyToolkit::String::Format("udp客户端你好, 请求已收到");

			Send(host, port, value.c_str(), value.size());
		}
	}

	/**
	 *
	 * 连接成功触发回调函数
	 *
	 */
	void OnConnect() override
	{
		tinyToolkit::String::Print("服务器会话[{}:{}]连接客户端会话[{}:{}]成功\r\n", _localHost, _localPort, _remoteHost, _remotePort);

		std::string value = tinyToolkit::String::Format("udp客户端你好, 我的编号为{}", _id);

		Send(_remoteHost.c_str(), _remotePort, value.c_str(), value.size());
	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnDisconnect() override
	{
		tinyToolkit::String::Print("服务器会话[{}:{}]断开连接\r\n", _localHost, _localPort);
	}

	/**
	 *
	 * 连接失败触发回调函数
	 *
	 */
	void OnConnectFailed() override
	{
		tinyToolkit::String::Print("服务器会话[{}:{}]连接客户端会话[{}:{}]失败\r\n", _localHost, _localPort, _remoteHost, _remotePort);
	}

private:
	int32_t _id{ 0 };
};


class UDPServer : public tinyToolkit::IUDPServer
{
public:
	/**
	 *
	 * 析构函数
	 *
	 */
	~UDPServer() override
	{
		tinyToolkit::String::Print("服务器析构\r\n");

		for (auto &iter : _pool)
		{
			iter.second->Close();

			delete iter.second;
		}
	}

	/**
	 *
	 * 新连接触发回调函数
	 *
	 * @param host 主机地址
	 * @param port 主机端口
	 *
	 * @return 会话
	 *
	 */
	tinyToolkit::IUDPSession * OnNewConnect(const std::string & host, uint16_t port) override
	{
		tinyToolkit::String::Print("服务器[{}:{}]接收到客户端[{}:{}]会话请求\r\n", _host, _port, host, port);

		auto key = tinyToolkit::String::Splice(host, ":", port);

		auto find = _pool.find(key);

		if (find == _pool.end())
		{
			_pool.insert(std::make_pair(key, new UDPServerSession(port)));

			find = _pool.find(key);
		}

		return find->second;
	}

	/**
	 *
	 * 会话错误触发回调函数
	 *
	 */
	void OnSessionError(tinyToolkit::IUDPSession * session) override
	{
		tinyToolkit::String::Print("服务器[{}:{}]与客户端[{}:{}]会话错误\r\n", _host, _port, session->_remoteHost, session->_remotePort);

		auto key = tinyToolkit::String::Splice(session->_remoteHost, ":", session->_remotePort);

		auto find = _pool.find(key);

		if (find != _pool.end())
		{
			find->second->Close();

			delete find->second;

			_pool.erase(find);
		}
	}

	/**
	 *
	 * 错误触发回调函数
	 *
	 */
	void OnError() override
	{
		tinyToolkit::String::Print("服务器[{}:{}]异常\r\n", _host, _port);
	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnRelease() override
	{
		tinyToolkit::String::Print("服务器[{}:{}]断开连接\r\n", _host, _port);

		for (auto &iter : _pool)
		{
			iter.second->Close();

			delete iter.second;
		}
	}

private:
	std::unordered_map<std::string, tinyToolkit::IUDPSession *> _pool{ };
};


void ParseOption(int argc, char const * argv[])
{
	tinyToolkit::OptionManager::Instance().DefineArg("mode",  nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("type",  nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("host",  nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("port",  nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("size",  nullptr, nullptr, tinyToolkit::Application::Name().c_str());

	tinyToolkit::OptionManager::Instance().DefineVersion(tinyToolkit::Application::CompileTimeString());

	tinyToolkit::OptionManager::Instance().Parse(argc, argv);
}


void StartApp()
{
	if (tinyToolkit::OptionManager::Instance().Empty())
	{
		TCPServer server;

		if (server.Launch("127.0.0.1", 1234))
		{
			tinyToolkit::String::Print("服务器[{}:{}]启动成功\r\n", "127.0.0.1", 1234);
		}
		else
		{
			tinyToolkit::String::Print("服务器[{}:{}]启动失败\r\n", "127.0.0.1", 1234);
		}

		while (true)
		{
			if (getchar() == 'q')
			{
				server.Close();

				return;
			}
		}
	}

	auto mode = tinyToolkit::OptionManager::Instance().Get("mode");
	auto type = tinyToolkit::OptionManager::Instance().Get("type");
	auto host = tinyToolkit::OptionManager::Instance().Get("host");
	auto port = tinyToolkit::String::Transform<uint16_t>(tinyToolkit::OptionManager::Instance().Get("port"));
	auto size = tinyToolkit::String::Transform<uint32_t>(tinyToolkit::OptionManager::Instance().Get("size"));

	if (type == "tcp")
	{
		if (mode == "client")
		{
			std::vector<TCPClientSession *> pool;

			for (uint32_t i = 0; i < size; ++i)
			{
				auto session = new TCPClientSession(i);

				if (session->Launch(host, port))
				{
					tinyToolkit::String::Print("客户端[{}:{}]启动成功\r\n", host, port);
				}
				else
				{
					tinyToolkit::String::Print("客户端[{}:{}]启动失败\r\n", host, port);
				}

				pool.push_back(session);
			}

			while (true)
			{
				std::string value{ };

				std::getline(std::cin, value);

				if (value[0] == 'q')
				{
					for (auto &iter : pool)
					{
						iter->Close();

						delete iter;
					}

					return;
				}
				else
				{
					for (auto &iter : pool)
					{
						iter->Send(value.c_str(), value.size());
					}
				}
			}
		}
		else if (mode == "server")
		{
			TCPServer server;

			if (server.Launch(host, port))
			{
				tinyToolkit::String::Print("服务器[{}:{}]启动成功\r\n", host, port);
			}
			else
			{
				tinyToolkit::String::Print("服务器[{}:{}]启动失败\r\n", host, port);
			}

			while (true)
			{
				if (getchar() == 'q')
				{
					server.Close();

					return;
				}
			}
		}
	}
	else if (type == "udp")
	{
		if (mode == "client")
		{
			std::vector<UDPClientSession *> pool;

			for (uint32_t i = 0; i < size; ++i)
			{
				auto session = new UDPClientSession(i);

				if (session->Launch(host, port))
				{
					tinyToolkit::String::Print("客户端[{}:{}]启动成功\r\n", host, port);
				}
				else
				{
					tinyToolkit::String::Print("客户端[{}:{}]启动失败\r\n", host, port);
				}

				pool.push_back(session);
			}

			while (true)
			{
				std::string value{ };

				std::getline(std::cin, value);

				if (value[0] == 'q')
				{
					for (auto &iter : pool)
					{
						iter->Close();

						delete iter;
					}

					return;
				}
				else
				{
					for (auto &iter : pool)
					{
						iter->Send(iter->_remoteHost.c_str(), iter->_remotePort, value.c_str(), value.size());
					}
				}
			}
		}
		else if (mode == "server")
		{
			UDPServer server;

			if (server.Launch(host, port))
			{
				tinyToolkit::String::Print("服务器[{}:{}]启动成功\r\n", host, port);
			}
			else
			{
				tinyToolkit::String::Print("服务器[{}:{}]启动失败\r\n", host, port);
			}

			while (true)
			{
				if (getchar() == 'q')
				{
					server.Close();

					return;
				}
			}
		}
	}
}


int main(int argc, char const * argv[])
{
	ParseOption(argc, argv);

	tinyToolkit::Signal::RegisterIgnore();
	tinyToolkit::Signal::RegisterStackTrace();

	StartApp();

	return 0;
}
