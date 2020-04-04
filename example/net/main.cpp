/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"

#include "pool/task.h"
#include "util/string.h"


template <typename SessionTypeT>
static void InitializeSession(SessionTypeT & session)
{
	session.OnError([&]()
	{
		util::String::Print
		(
			"Session [{}:{}] error\r\n",
			session.LocalEndpoint().host,
			session.LocalEndpoint().port
		);
	});

	session.OnDisconnect([&]()
	{
		 util::String::Print
		 (
		 	"Session [{}:{}] disconnect [{}:{}]\r\n",
		    session.LocalEndpoint().host,
		    session.LocalEndpoint().port,
		    session.PeerEndpoint().host,
		    session.PeerEndpoint().port
		 );
	});

	session.OnBind([&](bool status)
	{
		util::String::Print
		(
			"Session [{}:{}] bind {}\r\n",
			session.LocalEndpoint().host,
			session.LocalEndpoint().port,
			status ? "success" : "failed"
		);
	});

	session.OnSend([&](bool status)
	{
		util::String::Print
		(
			"Session [{}:{}] send [{}:{}] {}, remain {} message\r\n",
			session.LocalEndpoint().host,
			session.LocalEndpoint().port,
			session.PeerEndpoint().host,
			session.PeerEndpoint().port,
			status ? "success" : "failed",
			session.RemainMessageCount()
		);
	});

	session.OnSocket([&](bool status)
	{
		util::String::Print
		(
			"Session [{}:{}] socket {}\r\n",
			session.LocalEndpoint().host,
			session.LocalEndpoint().port,
			status ? "success" : "failed"
		);
	});

	session.OnConnect([&](bool status)
	{
		util::String::Print
		(
			"Session [{}:{}] connect [{}:{}] {}\r\n",
			session.LocalEndpoint().host,
			session.LocalEndpoint().port,
			session.PeerEndpoint().host,
			session.PeerEndpoint().port,
			status ? "success" : "failed"
		);

		if (status)
		{
			session.Send("request server message", 22);
		}
	});

	session.OnReceive([&](bool status, const char * buffer, std::size_t length) -> std::size_t
	{
		util::String::Print
		(
			"Session [{}:{}] receive [{}:{}] {}\r\n",
			session.LocalEndpoint().host,
			session.LocalEndpoint().port,
			session.PeerEndpoint().host,
			session.PeerEndpoint().port,
			status ? buffer : "failed"
		);

		return length;
	});
}


template <typename ServerTypeT, typename SessionTypeT>
static void InitializeServer(ServerTypeT & server, std::vector<SessionTypeT *> & sessionPool)
{
	server.OnError([&]()
	{
		util::String::Print
		(
			"Server [{}:{}] error\r\n",
			server.LocalEndpoint().host,
			server.LocalEndpoint().port
		);
	});

	server.OnShutdown([&]()
	{
		util::String::Print
		(
			"Server [{}:{}] shutdown\r\n",
			server.LocalEndpoint().host,
			server.LocalEndpoint().port
		);

		for (auto &session : sessionPool)
		{
			session->Close();

			delete session;
		}
	});

	server.OnBind([&](bool status)
	{
		util::String::Print
		(
			"Server [{}:{}] bind {}\r\n",
			server.LocalEndpoint().host,
			server.LocalEndpoint().port,
			status ? "success" : "failed"
		);
	});

	server.OnSocket([&](bool status)
	{
		util::String::Print
		(
			"Server [{}:{}] socket {}\r\n",
			server.LocalEndpoint().host,
			server.LocalEndpoint().port,
			status ? "success" : "failed"
		);
	});

	server.OnListen([&](bool status)
	{
		util::String::Print
		(
			"Server [{}:{}] listen {}\r\n",
			server.LocalEndpoint().host,
			server.LocalEndpoint().port,
			status ? "success" : "failed"
		);
	});

	server.OnAccept([&](bool status) -> SessionTypeT *
	{
		util::String::Print
		(
			"Server [{}:{}] accept {}\r\n",
			server.LocalEndpoint().host,
			server.LocalEndpoint().port,
			status ? "success" : "failed"
		);

		if (!status)
		{
			return nullptr;
		}

		auto * session = new SessionTypeT;

		session->OnSend([session](bool)
		{
			if (session->RemainMessageCount() == 0)
			{
				session->Close();
			}
		});

		session->OnReceive([session](bool result, const char * buffer, std::size_t length) -> std::size_t
		{
			util::String::Print
			(
				"ServerSession [{}:{}] receive [{}:{}] {}\r\n",
				session->LocalEndpoint().host,
				session->LocalEndpoint().port,
				session->PeerEndpoint().host,
				session->PeerEndpoint().port,
				result ? buffer : "failed"
			);

			if (result)
			{
				session->Send("respond server message", 22);
			}

			return length;
		});

		sessionPool.push_back(session);

		return session;
	});
}


static void TCP()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		net::TCPServer server{ };

		std::vector<net::TCPSession *> sessionPool{ };

		InitializeServer(server, sessionPool);

		if (!server.Launch("0.0.0.0", 10080, 1024))
		{
			util::String::Print
			(
				"launch server listen to [{}:{}] failed\r\n",
				server.LocalEndpoint().host,
				server.LocalEndpoint().port
			);
		}

		pool::TaskPool pool{ };

		pool.Launch();

		for (int i = 0; i < 8; ++i)
		{
			pool.AddTask([]()
	         {
		         net::TCPSession session;

		         InitializeSession(session);

	             if (!session.Launch("127.0.0.1", 10080, 1024))
	             {
		             util::String::Print
		             (
			             "launch session connect to [{}:{}] failed\r\n",
			             session.PeerEndpoint().host,
			             session.PeerEndpoint().port
		             );
	             }

	             std::this_thread::sleep_for(std::chrono::milliseconds(500));

	             session.Close();
	         });
		}

		pool.Wait();

		server.Close();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void UDP()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		net::UDPServer server{ };

		std::vector<net::UDPSession *> sessionPool{ };

		InitializeServer(server, sessionPool);

		if (!server.Launch("0.0.0.0", 10080, 1024))
		{
			util::String::Print
			(
				"launch server listen to [{}:{}] failed\r\n",
				server.LocalEndpoint().host,
				server.LocalEndpoint().port
			);
		}

		pool::TaskPool pool{ };

		pool.Launch();

		for (int i = 0; i < 8; ++i)
		{
			pool.AddTask([]()
		    {
				net::UDPSession session;

			    InitializeSession(session);

				if (!session.Launch("127.0.0.1", 10080, 1024))
				{
					util::String::Print
					(
						"launch session connect to [{}:{}] failed\r\n",
						session.PeerEndpoint().host,
						session.PeerEndpoint().port
					);
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(500));

			    session.Close();
		    });
		}

		pool.Wait();

		server.Close();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


int main(int argc, char const * argv[])
{
	(void)argc;
	(void)argv;

	TCP();
	UDP();

	return 0;
}
