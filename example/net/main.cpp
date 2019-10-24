/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void UDPClient()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		class UDPSession : public tinyToolkit::IUDPSession
		{
		public:
			UDPSession()
			{
				std::cout << "UDP Session Constructor" << std::endl;
			}

			~UDPSession() override
			{
				std::cout << "UDP Session Destructor" << std::endl;
			}

			void OnConnect() override
			{
				std::cout << "UDP Session ["
						  << LocalHost() << ":" << LocalPort() << "] Connect ["
						  << RemoteHost() << ":" << RemotePort() << "] Success" << std::endl;
			}

			void OnDisconnect() override
			{
				std::cout << "UDP Session [" << LocalHost() << ":" << LocalPort() << "] Disconnect" << std::endl;
			}

			void OnConnectFailed() override
			{
				std::cout << "UDP Session ["
						  << LocalHost() << ":" << LocalPort() << "] Connect ["
						  << RemoteHost() << ":" << RemotePort() << "] Failed : " 
						  << tinyToolkit::OS::LastErrorMessage() << std::endl;
			}

			std::size_t OnReceive(const char * data, std::size_t size) override
			{
				std::cout << "UDP Session ["
						  << LocalHost() << ":" << LocalPort() << "] Received ["
						  << RemoteHost() << ":" << RemotePort() << "] Length [" << size << "] Message : " << data << std::endl;

				return size;
			}
		};

		UDPSession session;

		if (!session.Launch("0.0.0.0", 10086, "127.0.0.1", 20086, 4096))
		{
			std::cout << "Launch TCP Client Failed" << std::endl;

			return;
		}

		while (true)
		{
			std::string value{ };

			std::getline(std::cin, value);

			if (value[0] == 'q')
			{
				session.Close();

				return;
			}
			else
			{
				session.Send(value.c_str(), value.size());
			}
		}
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void UDPServer()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		class UDPSession : public tinyToolkit::IUDPSession
		{
		public:
			UDPSession()
			{
				std::cout << "UDP Session Constructor" << std::endl;
			}

			~UDPSession() override
			{
				std::cout << "UDP Session Destructor" << std::endl;
			}

			void OnConnect() override
			{
				std::cout << "UDP Session ["
						  << LocalHost() << ":" << LocalPort() << "] Connect ["
						  << RemoteHost() << ":" << RemotePort() << "] Success" << std::endl;
			}

			void OnDisconnect() override
			{
				std::cout << "UDP Session [" << LocalHost() << ":" << LocalPort() << "] Disconnect" << std::endl;
			}

			void OnConnectFailed() override
			{
				std::cout << "UDP Session ["
						  << LocalHost() << ":" << LocalPort() << "] Connect ["
						  << RemoteHost() << ":" << RemotePort() << "] Failed : "
						  << tinyToolkit::OS::LastErrorMessage() << std::endl;
			}

			std::size_t OnReceive(const char * data, std::size_t size) override
			{
				std::cout << "UDP Session ["
						  << LocalHost() << ":" << LocalPort() << "] Received ["
						  << RemoteHost() << ":" << RemotePort() << "] Length [" << size << "] Message : " << data << std::endl;

				return size;
			}
		};

		UDPSession session;

		if (!session.Launch("0.0.0.0", 20086, "127.0.0.1", 10086, 4096))
		{
			std::cout << "Launch TCP Client Failed" << std::endl;

			return;
		}

		while (true)
		{
			std::string value{ };

			std::getline(std::cin, value);

			if (value[0] == 'q')
			{
				session.Close();

				return;
			}
			else
			{
				session.Send(value.c_str(), value.size());
			}
		}
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void TCPClient()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		static std::vector<std::shared_ptr<tinyToolkit::ITCPSession>> sSessionList;

		class TCPSession : public tinyToolkit::ITCPSession
		{
		public:
			TCPSession() = default;

			~TCPSession() override = default;

			void OnConnect() override
			{
				std::cout << "TCP Session ["
						  << LocalHost() << ":" << LocalPort() << "] Connect ["
						  << RemoteHost() << ":" << RemotePort() << "] Success" << std::endl;
			}

			void OnDisconnect() override
			{
				std::cout << "TCP Session [" << LocalHost() << ":" << LocalPort() << "] Disconnect" << std::endl;
			}

			void OnConnectFailed() override
			{
				std::cout << "TCP Session ["
						  << LocalHost() << ":" << LocalPort() << "] Connect ["
						  << RemoteHost() << ":" << RemotePort() << "] Failed : "
						  << tinyToolkit::OS::LastErrorMessage() << std::endl;
			}

			std::size_t OnReceive(const char * data, std::size_t size) override
			{
				std::cout << "TCP Session ["
						  << LocalHost() << ":" << LocalPort() << "] Received ["
						  << RemoteHost() << ":" << RemotePort() << "] Length [" << size << "] Message : " << data << std::endl;

				return size;
			}
		};

		for (int i = 0; i < 666; ++i)
		{
			sSessionList.push_back(std::make_shared<TCPSession>());

			if (!sSessionList.back()->Launch("127.0.0.1", 10086, 4096))
			{
				std::cout << "Launch TCP Client Failed : " << i << std::endl;

				break;
			}
		}

		while (true)
		{
			std::string value{ };

			std::getline(std::cin, value);

			if (value[0] == 'q')
			{
				for (auto &iter : sSessionList)
				{
					iter->Close();
				}

				return;
			}
			else
			{
				for (auto &iter : sSessionList)
				{
					iter->Send(value.c_str(), value.size());
				}
			}
		}
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void TCPServer()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		static std::unordered_map<int32_t, std::shared_ptr<tinyToolkit::ITCPSession>> sSessionList;

		class TCPSession : public tinyToolkit::ITCPSession
		{
		public:
			explicit TCPSession(int32_t id) : _id(id)
			{

			}

			~TCPSession() override = default;

			void OnConnect() override
			{

			}

			void OnDisconnect() override
			{
				std::cout << "TCP Session [" << LocalHost() << ":" << LocalPort() << "] Disconnect" << std::endl;

				auto find = sSessionList.find(_id);

				if (find != sSessionList.end())
				{
					sSessionList.erase(find);
				}
			}

			void OnConnectFailed() override
			{

			}

			std::size_t OnReceive(const char * data, std::size_t size) override
			{
				std::cout << "TCP Session ["
						  << LocalHost() << ":" << LocalPort() << "] Received ["
						  << RemoteHost() << ":" << RemotePort() << "] Length [" << size << "] Message : " << data << std::endl;

				return size;
			}

		public:
			int32_t _id{ 0 };
		};

		class TCPServer : public tinyToolkit::ITCPServer
		{
		public:
			TCPServer() = default;

			~TCPServer() override = default;

			void OnError() override
			{
				std::cout << "TCP Server [" << LocalHost() << ":" << LocalPort() << "] Error : " 
						  << tinyToolkit::OS::LastErrorMessage() << std::endl;
			}

			void OnRelease() override
			{
				std::cout << "TCP Server [" << LocalHost() << ":" << LocalPort() << "] Release" << std::endl;

				for (auto iter = sSessionList.begin(); iter != sSessionList.end();)
				{
					auto temp = iter;

					++iter;

					temp->second->Close();
				}
			}

			void OnSessionError(tinyToolkit::ITCPSession * session) override
			{
				std::cout << "TCP Server ["
						  << LocalHost() << ":" << LocalPort() << " And Session ["
						  << RemoteHost() << ":" << RemotePort() << "] Error : "
						  << tinyToolkit::OS::LastErrorMessage() << std::endl;

				if (session == nullptr)
				{
					std::cout << "session NULL" << std::endl;
				}
			}

			tinyToolkit::ITCPSession * OnSessionConnect() override
			{
				std::cout << "TCP Server ["
						  << LocalHost() << ":" << LocalPort() << "] Accept Session ["
						  << RemoteHost() << ":" << RemotePort() << "] Connect" << std::endl;

				static int32_t count = 0;

				auto session = std::make_shared<TCPSession>(++count);

				sSessionList.insert(std::make_pair(session->_id, session));

				return session.get();
			}
		};

		TCPServer server;

		if (!server.Launch("0.0.0.0", 10086, 4096))
		{
			std::cout << "Launch TCP Server Failed" << std::endl;
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
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char const * argv[])
{
	if (argc < 3)
	{
		return 0;
	}

	if (strncmp(argv[1], "tcp", 3) == 0)
	{
		if (strncmp(argv[2], "server", 6) == 0)
		{
			TCPServer();
		}
		else if (strncmp(argv[2], "client", 6) == 0)
		{
			TCPClient();
		}
	}
	else if (strncmp(argv[1], "udp", 3) == 0)
	{
		if (strncmp(argv[2], "server", 6) == 0)
		{
			UDPServer();
		}
		else if (strncmp(argv[2], "client", 6) == 0)
		{
			UDPClient();
		}
	}

	return 0;
}
