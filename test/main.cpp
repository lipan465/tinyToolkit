/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"

#include "tinyToolkit/net/epoll.h"


class TestClient : public tinyToolkit::ITCPSession
{
public:
	explicit TestClient(int32_t id) : _id(id)
	{

	}

	~TestClient() override
	{
		std::cout << "id = " << _id << ", " << TINY_TOOLKIT_FUNC << std::endl;

		Close();
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
		std::cout << "id = " << _id << ", " << TINY_TOOLKIT_FUNC << std::endl;
		std::cout << "receive : " << *((int32_t *)data) << std::endl;

		if (++_count < 3)
		{
			std::cout << "send count : " << _count << std::endl;

			Send(&_id, sizeof(_id));
		}
	}

	/**
	 *
	 * 连接成功触发回调函数
	 *
	 */
	void OnConnect() override
	{
		std::cout << "id = " << _id << ", " << TINY_TOOLKIT_FUNC << std::endl;

		std::string value = tinyToolkit::String::Splice("GET / HTTP/1.1\r\n"
														"Host: 192.168.10.135\r\n"
														"User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0\r\n"
														"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
														"Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
														"Accept-Encoding: gzip, deflate\r\n"
														"Connection: keep-alive\r\n\r\n");

		Send(&_id, sizeof(_id));
//		Send(value);
	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnDisconnect() override
	{
		std::cout << "id = " << _id << ", " << TINY_TOOLKIT_FUNC << std::endl;
	}

	/**
	 *
	 * 连接失败触发回调函数
	 *
	 */
	void OnConnectFailed() override
	{
		std::cout << "id = " << _id << ", " << TINY_TOOLKIT_FUNC << ", " << strerror(errno) << std::endl;
	}

protected:
	int32_t _id{ 0 };

	int32_t _count{ 0 };
};


class TestServer : public tinyToolkit::ITCPServer
{
public:
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
		std::cout << TINY_TOOLKIT_FUNC << ", host=" << host << ", port=" << port << std::endl;

		return new TestClient(port);
	}

	/**
	 *
	 * 会话错误触发回调函数
	 *
	 */
	void OnSessionError(const tinyToolkit::ITCPSession * session) override
	{
		std::cout << TINY_TOOLKIT_FUNC << ", host=" << session->_host << ", port=" << session->_port << ", error=" << strerror(errno) << std::endl;
	}

	/**
	 *
	 * 错误触发回调函数
	 *
	 */
	void OnError() override
	{
		std::cout << TINY_TOOLKIT_FUNC << ", error=" << strerror(errno) << std::endl;
	}

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnRelease() override
	{
		std::cout << TINY_TOOLKIT_FUNC << std::endl;
	}
};


void ParseOption(int argc, char const * argv[])
{
	tinyToolkit::OptionManager::Instance().DefineArg("type", nullptr, "client", tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("host", nullptr, "192.168.2.172", tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("port", nullptr, "6666", tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("count", nullptr, "1", tinyToolkit::Application::Name().c_str());

	tinyToolkit::OptionManager::Instance().DefineVersion(tinyToolkit::Application::CompileTimeString());

	tinyToolkit::OptionManager::Instance().Parse(argc, argv);
}


int main(int argc, char const * argv[])
{
	ParseOption(argc, argv);

	auto port = tinyToolkit::String::Transform<uint16_t>(tinyToolkit::OptionManager::Instance().Get("port"));
	auto count = tinyToolkit::String::Transform<uint32_t>(tinyToolkit::OptionManager::Instance().Get("count"));

	std::string type = tinyToolkit::OptionManager::Instance().Get("type");
	std::string host = tinyToolkit::OptionManager::Instance().Get("host");

	if (type == "server")
	{
		auto server = new TestServer();

		if (tinyToolkit::NetWorkManager::Instance().LaunchTCPServer(server, host, port))
		{
			std::cout << "[TCPServer] success" << std::endl;
		}
		else
		{
			std::cout << "[TCPServer] failed" << std::endl;
		}

		while (true)
		{
			if (getchar() == 'q')
			{
				break;
			}
		}

		delete server;
	}
	else
	{
		std::vector<TestClient *> list;

		for (uint32_t i = 0; i < count; ++i)
		{
			list.push_back(new TestClient(i));

			if (tinyToolkit::NetWorkManager::Instance().LaunchTCPClient(list.back(), host, port))
			{
				std::cout << "[TCPClient] success" << std::endl;
			}
			else
			{
				std::cout << "[TCPClient] failed" << std::endl;
			}
		}

		while (true)
		{
			if (getchar() == 'q')
			{
				break;
			}
		}

		for (auto &iter : list)
		{
			iter->Close();

			delete iter;
		}
	}
}