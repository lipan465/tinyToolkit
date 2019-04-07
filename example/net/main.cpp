/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


#define sOption	tinyToolkit::OptionManager::Instance()


void ParseOption(int argc, char const * argv[])
{
	sOption.Define("tcp", nullptr, tinyToolkit::Application::Name().c_str());
	sOption.Define("udp", nullptr, tinyToolkit::Application::Name().c_str());

	sOption.Define("client", nullptr, tinyToolkit::Application::Name().c_str());
	sOption.Define("server", nullptr, tinyToolkit::Application::Name().c_str());

	sOption.DefineArg("host", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	sOption.DefineArg("port", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	sOption.DefineArg("size", nullptr, nullptr, tinyToolkit::Application::Name().c_str());

	sOption.DefineVersion(tinyToolkit::Application::CompileTimeString());

	sOption.Parse(argc, argv);
}


void StartApp()
{
	auto host = sOption.Has("host") ? sOption.Get("host") : "127.0.0.1";
	auto port = sOption.Has("port") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("port")) : 1234;
	auto size = sOption.Has("size") ? tinyToolkit::String::Transform<uint32_t>(sOption.Get("size")) : 1;

	if (sOption.Has("tcp"))
	{
		if (sOption.Has("client"))
		{
			std::vector<TCPClientSession *> pool;

			for (uint32_t i = 0; i < size; ++i)
			{
				pool.push_back(new TCPClientSession(i));

				pool.back()->Launch(host, port, TINY_TOOLKIT_MB, TINY_TOOLKIT_MB);
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
		else if (sOption.Has("server"))
		{
			TCPServer server;

			server.Launch(host, port, TINY_TOOLKIT_MB, TINY_TOOLKIT_MB);

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
//	else if (sOption.Has("udp"))
	{
		std::vector<UDPClientSession *> pool;

		for (uint32_t i = 0; i < size; ++i)
		{
			pool.push_back(new UDPClientSession());

			pool.back()->Launch(host, port);
		}

		for (uint32_t j = 0; j < 100000; ++j)
		{
			for (auto &iter : pool)
			{
				iter->Send("127.0.0.1", 4321, "this is test message", 20);
			}
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
					iter->Send("127.0.0.1", 4321, value.c_str(), value.size());
				}
			}
		}
	}
}


int main(int argc, char const * argv[])
{
	ParseOption(argc, argv);

	tinyToolkit::Signal::RegisterIgnore();
	tinyToolkit::Signal::RegisterStackTrace(tinyToolkit::Backtrace::Print);

	StartApp();

	return 0;
}
