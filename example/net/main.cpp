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
	auto type = sOption.Has("type") ? sOption.Get("type") : "rudp";
	auto mode = sOption.Has("mode") ? sOption.Get("mode") : "client";

	auto count = sOption.Has("count") ? tinyToolkit::String::Transform<uint32_t>(sOption.Get("count")) : static_cast<uint16_t>(1);

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

	auto localHost = sOption.Has("localHost") ? sOption.Get("localHost") : "0.0.0.0";
	auto localPort = sOption.Has("localPort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("localPort")) : static_cast<uint16_t>(1234);

	auto remoteHost = sOption.Has("remoteHost") ? sOption.Get("remoteHost") : "192.168.2.71";
	auto remotePort = sOption.Has("remotePort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("remotePort")) : static_cast<uint16_t>(1234);

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

	auto localHost = sOption.Has("localHost") ? sOption.Get("localHost") : "0.0.0.0";
	auto localPort = sOption.Has("localPort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("localPort")) : static_cast<uint16_t>(1234);

	auto remoteHost = sOption.Has("remoteHost") ? sOption.Get("remoteHost") : "192.168.2.70";
	auto remotePort = sOption.Has("remotePort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("remotePort")) : static_cast<uint16_t>(1234);

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE

	auto localHost = sOption.Has("localHost") ? sOption.Get("localHost") : "0.0.0.0";
	auto localPort = sOption.Has("localPort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("localPort")) : static_cast<uint16_t>(1234);

	auto remoteHost = sOption.Has("remoteHost") ? sOption.Get("remoteHost") : "192.168.0.71";
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

				if (pool.back()->Launch(remoteHost, remotePort, TINY_TOOLKIT_MB))
				{
					TINY_TOOLKIT_SYNC_LOG_INFO("launch tcp client success");
				}
				else
				{
					TINY_TOOLKIT_SYNC_LOG_INFO("launch tcp client failed");
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

			if (server.Launch(localHost, localPort, TINY_TOOLKIT_MB))
			{
				TINY_TOOLKIT_SYNC_LOG_INFO("launch tcp server success");
			}
			else
			{
				TINY_TOOLKIT_SYNC_LOG_INFO("launch tcp server failed");
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
		std::vector<UDPClientSession *> pool;

		for (uint32_t i = 0; i < count; ++i)
		{
			pool.push_back(new UDPClientSession());

			if (pool.back()->Launch(localHost, localPort, remoteHost, remotePort, TINY_TOOLKIT_MB))
			{
				TINY_TOOLKIT_SYNC_LOG_INFO("launch udp client success");
			}
			else
			{
				TINY_TOOLKIT_SYNC_LOG_INFO("launch udp client failed");
			}
		}

		for (uint32_t j = 0; j < 100000; ++j)
		{
			std::string value = tinyToolkit::String::Format("this is test message [{}]", j);

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
	else if (type == "rudp")
	{
		std::vector<RUDPClientSession *> pool;

		for (uint32_t i = 0; i < count; ++i)
		{
			pool.push_back(new RUDPClientSession());

			if (pool.back()->Launch(localHost, localPort, remoteHost, remotePort, TINY_TOOLKIT_MB))
			{
				TINY_TOOLKIT_SYNC_LOG_INFO("launch rudp client success");
			}
			else
			{
				TINY_TOOLKIT_SYNC_LOG_INFO("launch rudp client failed");
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
}


int main(int argc, char const * argv[])
{
	ParseOption(argc, argv);

	tinyToolkit::Signal::RegisterIgnore();
	tinyToolkit::Signal::RegisterStackTrace(tinyToolkit::Backtrace::Print);

	tinyToolkit::SyncLogger::Instance().AddSink<tinyToolkit::ConsoleLogSink>("console");
	tinyToolkit::SyncLogger::Instance().SetSinkLayout<tinyToolkit::SimpleLogLayout>();
	tinyToolkit::SyncLogger::Instance().EnableSinkAutoFlush();

	StartApp();

	return 0;
}
