/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


class TCPClient : public tinyToolkit::ITCPSession
{
public:
	/**
	 *
	 * 构造函数
	 *
	 * @param id 序列号
	 *
	 */
	explicit TCPClient(int32_t id) : _id(id)
	{

	}

	/**
	 *
	 * 析构函数
	 *
	 */
	~TCPClient() override
	{
		tinyToolkit::String::Print("客户端析构\r\n");
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
		tinyToolkit::String::Print("客户端[{}]接收到服务器[{}:{}]长度为[{}]的信息 : {}\r\n", _id, _host, _port, size, data);
	}

	/**
	 *
	 * 连接成功触发回调函数
	 *
	 */
	void OnConnect() override
	{
		tinyToolkit::String::Print("客户端[{}]连接服务器[{}:{}]成功\r\n", _id, _host, _port);

		std::string value = tinyToolkit::String::Format("你好, 我的tcp编号为{}", _id);

		Send(value.c_str(), value.size());
	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnDisconnect() override
	{
		tinyToolkit::String::Print("客户端[{}]断开连接\r\n", _id);
	}

	/**
	 *
	 * 连接失败触发回调函数
	 *
	 */
	void OnConnectFailed() override
	{
		tinyToolkit::String::Print("客户端[{}]连接服务器[{}:{}]失败\r\n", _id, _host, _port);
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

		for (auto &iter : _pool)
		{
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
	tinyToolkit::ITCPSession * OnNewConnect(const std::string & host, uint16_t port) override
	{
		tinyToolkit::String::Print("服务器[{}:{}]接收到客户端[{}:{}]会话请求\r\n", _host, _port, host, port);

		auto key = tinyToolkit::String::Splice(host, ":", port);

		auto find = _pool.find(key);

		if (find == _pool.end())
		{
			_pool.insert(std::make_pair(key, new TCPClient(port)));

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
		tinyToolkit::String::Print("服务器[{}:{}]与客户端[{}:{}]会话错误\r\n", _host, _port, session->_host, session->_port);
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
	}

protected:
	std::unordered_map<std::string, tinyToolkit::ITCPSession *> _pool;
};


class UDPClient : public tinyToolkit::IUDPSession
{
public:
	/**
	 *
	 * 构造函数
	 *
	 * @param id 序列号
	 *
	 */
	explicit UDPClient(int32_t id) : _id(id)
	{

	}

	/**
	 *
	 * 析构函数
	 *
	 */
	~UDPClient() override
	{
		tinyToolkit::String::Print("客户端析构\r\n");
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
		tinyToolkit::String::Print("客户端[{}]接收到服务器[{}:{}]长度为[{}]的信息 : {}\r\n", _id, host, port, size, data);
	}

	/**
	 *
	 * 连接成功触发回调函数
	 *
	 */
	void OnConnect() override
	{
		tinyToolkit::String::Print("客户端[{}]连接服务器[{}:{}]成功\r\n", _id, _host, _port);

		std::string value = tinyToolkit::String::Format("你好, 我的udp编号为{}", _id);

		Send(_host.c_str(), _port, value.c_str(), value.size());
	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnDisconnect() override
	{
		tinyToolkit::String::Print("客户端[{}]断开连接\r\n", _id);
	}

	/**
	 *
	 * 连接失败触发回调函数
	 *
	 */
	void OnConnectFailed() override
	{
		tinyToolkit::String::Print("客户端[{}]连接服务器[{}:{}]失败\r\n", _id, _host, _port);
	}

private:
	int32_t _id{ 0 };
};


class UDPServer : public tinyToolkit::IUDPServer
{
public:
	/**
	 *
	 * 构造函数
	 *
	 * @param id 序列号
	 *
	 */
	explicit UDPServer(int32_t id) : _id(id)
	{

	}

	/**
	 *
	 * 析构函数
	 *
	 */
	~UDPServer() override
	{
		tinyToolkit::String::Print("服务器析构\r\n");
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
		tinyToolkit::String::Print("服务器[{}]接收到客户端[{}:{}]长度为[{}]的信息 : {}\r\n", _id, host, port, size, data);

		static int32_t count = 0;

		if (++count > 1)
		{
			std::string value = tinyToolkit::String::Format("你好, 我的udp编号为{}", _id);

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
		tinyToolkit::String::Print("服务器[{}]连接客户端[{}:{}]成功\r\n", _id, _host, _port);
	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnDisconnect() override
	{
		tinyToolkit::String::Print("服务器[{}]断开连接\r\n", _id);
	}

	/**
	 *
	 * 连接失败触发回调函数
	 *
	 */
	void OnConnectFailed() override
	{
		tinyToolkit::String::Print("服务器[{}]连接客户端[{}:{}]失败\r\n", _id, _host, _port);
	}

private:
	int32_t _id{ 0 };
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
	auto mode = tinyToolkit::OptionManager::Instance().Get("mode");
	auto type = tinyToolkit::OptionManager::Instance().Get("type");
	auto host = tinyToolkit::OptionManager::Instance().Get("host");
	auto port = tinyToolkit::String::Transform<uint16_t>(tinyToolkit::OptionManager::Instance().Get("port"));
	auto size = tinyToolkit::String::Transform<uint32_t>(tinyToolkit::OptionManager::Instance().Get("size"));

	if (type == "tcp")
	{
		if (mode == "client")
		{
			tinyToolkit::ApplicationPool<TCPClient> pool;

			for (uint32_t i = 0; i < size; ++i)
			{
				if (pool.Create(i)->Launch(host, port))
				{
					tinyToolkit::String::Print("客户端[{}:{}]启动成功\r\n", host, port);
				}
				else
				{
					tinyToolkit::String::Print("客户端[{}:{}]启动失败\r\n", host, port);
				}
			}

			while (true)
			{
				if (getchar() == 'q')
				{
					break;
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
					break;
				}
			}
		}
	}
	else if (type == "udp")
	{
		if (mode == "client")
		{
			tinyToolkit::ApplicationPool<UDPClient> pool;

			for (uint32_t i = 0; i < size; ++i)
			{
				if (pool.Create(i)->Launch(host, port))
				{
					tinyToolkit::String::Print("客户端[{}:{}]启动成功\r\n", host, port);
				}
				else
				{
					tinyToolkit::String::Print("客户端[{}:{}]启动失败\r\n", host, port);
				}
			}

			while (true)
			{
				if (getchar() == 'q')
				{
					break;
				}
			}
		}
		else if (mode == "server")
		{
			UDPServer server(10086);

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
					break;
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
