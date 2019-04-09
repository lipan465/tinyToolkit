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
	sOption.DefineArg("type", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	sOption.DefineArg("mode", nullptr, nullptr, tinyToolkit::Application::Name().c_str());
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
	auto type = sOption.Has("type") ? sOption.Get("type") : "tcp";
	auto mode = sOption.Has("mode") ? sOption.Get("mode") : "client";

	auto count = sOption.Has("count") ? tinyToolkit::String::Transform<uint32_t>(sOption.Get("count")) : static_cast<uint16_t>(1);

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

	auto localHost = sOption.Has("localHost") ? sOption.Get("localHost") : "192.168.2.70";
	auto localPort = sOption.Has("localPort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("localPort")) : static_cast<uint16_t>(1234);

	auto remoteHost = sOption.Has("remoteHost") ? sOption.Get("remoteHost") : "192.168.2.71";
	auto remotePort = sOption.Has("remotePort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("remotePort")) : static_cast<uint16_t>(4321);

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

	auto localHost = sOption.Has("localHost") ? sOption.Get("localHost") : "192.168.2.71";
	auto localPort = sOption.Has("localPort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("localPort")) : static_cast<uint16_t>(4321);

	auto remoteHost = sOption.Has("remoteHost") ? sOption.Get("remoteHost") : "192.168.2.70";
	auto remotePort = sOption.Has("remotePort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("remotePort")) : static_cast<uint16_t>(1234);

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

	auto localHost = sOption.Has("localHost") ? sOption.Get("localHost") : "127.0.0.1";
	auto localPort = sOption.Has("localPort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("localPort")) : static_cast<uint16_t>(4321);

	auto remoteHost = sOption.Has("remoteHost") ? sOption.Get("remoteHost") : "127.0.0.1";
	auto remotePort = sOption.Has("remotePort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("remotePort")) : static_cast<uint16_t>(1234);

#endif

	if (type == "tcp")
	{
		if (mode == "client")
		{
			std::vector<TCPClientSession *> pool;

			for (uint32_t i = 0; i < count; ++i)
			{
				pool.push_back(new TCPClientSession(i));

				if (pool.back()->Launch(remoteHost.c_str(), remotePort, 100 * TINY_TOOLKIT_MB, TINY_TOOLKIT_MB))
				{
					std::cout << "launch tcp client success" << std::endl;
				}
				else
				{
					std::cout << "launch tcp client failed" << std::endl;
				}
			}

			for (uint32_t j = 0; j < 100000; ++j)
			{
				std::string value = tinyToolkit::String::Format("[{}]", j);

				for (auto &iter : pool)
				{
					iter->Send(value.c_str(), value.size());
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
						iter->Send(value.c_str(), value.size());
					}
				}
			}
		}
		else if (mode == "server")
		{
			TCPServer server;

			if (server.Launch(localHost.c_str(), localPort, TINY_TOOLKIT_MB, TINY_TOOLKIT_MB))
			{
				std::cout << "launch tcp server success" << std::endl;
			}
			else
			{
				std::cout << "launch tcp server failed" << std::endl;
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
	else
	{
		std::vector<UDPClientSession *> pool;

		for (uint32_t i = 0; i < count; ++i)
		{
			pool.push_back(new UDPClientSession());

			if (pool.back()->Launch(localHost.c_str(), localPort))
			{
				std::cout << "launch udp client success" << std::endl;
			}
			else
			{
				std::cout << "launch udp client failed" << std::endl;
			}
		}

		for (uint32_t j = 0; j < 100000; ++j)
		{
			std::string value = tinyToolkit::String::Format("this is test message [{}]", j);

			for (auto &iter : pool)
			{
				iter->Send(remoteHost.c_str(), remotePort, value.c_str(), value.size());
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
