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

	sOption.DefineArg("count", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	sOption.DefineArg("localHost", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	sOption.DefineArg("localPort", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	sOption.DefineArg("remoteHost", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	sOption.DefineArg("remotePort", nullptr, nullptr, tinyToolkit::Application::Name().c_str());

	sOption.DefineVersion(tinyToolkit::Application::CompileTimeString());

	sOption.Parse(argc, argv);
}


void StartApp()
{
	auto count = sOption.Has("count") ? tinyToolkit::String::Transform<uint32_t>(sOption.Get("count")) : static_cast<uint16_t>(1);

	auto localHost = sOption.Has("localHost") ? sOption.Get("localHost") : "127.0.0.1";
	auto localPort = sOption.Has("localPort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("localPort")) : static_cast<uint16_t>(1234);

	auto remoteHost = sOption.Has("remoteHost") ? sOption.Get("remoteHost") : "127.0.0.1";
	auto remotePort = sOption.Has("remotePort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("remotePort")) : static_cast<uint16_t>(4321);

	if (sOption.Has("tcp"))
	{
		if (sOption.Has("client"))
		{
			std::vector<TCPClientSession *> pool;

			for (uint32_t i = 0; i < count; ++i)
			{
				pool.push_back(new TCPClientSession(i));

				pool.back()->Launch(remoteHost.c_str(), remotePort, TINY_TOOLKIT_MB, TINY_TOOLKIT_MB);
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

			server.Launch(localHost.c_str(), localPort, TINY_TOOLKIT_MB, TINY_TOOLKIT_MB);

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

		for (uint32_t i = 0; i < count; ++i)
		{
			pool.push_back(new UDPClientSession());

			pool.back()->Launch(localHost.c_str(), localPort);
		}

		for (uint32_t j = 0; j < 100000; ++j)
		{
			for (auto &iter : pool)
			{
				iter->Send(remoteHost.c_str(), remotePort, "this is test message", 20);
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
					iter->Send(remoteHost.c_str(), remotePort, value.c_str(), value.size());
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
