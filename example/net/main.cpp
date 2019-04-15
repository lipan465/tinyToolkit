/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


#define sOption	tinyToolkit::OptionManager::Instance()


////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 *
 * 启动tcp客户端
 *
 * @param remoteHost 远端地址
 * @param remotePort 远端端口
 * @param clientCount 客户端个数
 *
 */
void LaunchTCPClient(const std::string & remoteHost, uint16_t remotePort, uint32_t clientCount)
{
	std::vector<std::shared_ptr<TCPClientSession>> pool;

	for (uint32_t i = 0; i < clientCount; ++i)
	{
		pool.push_back(std::make_shared<TCPClientSession>());

		if (pool.back()->Launch(remoteHost, remotePort, TINY_TOOLKIT_MB))
		{
			TINY_TOOLKIT_SYNC_LOG_INFO("Launch TCP Client Success");
		}
		else
		{
			TINY_TOOLKIT_SYNC_LOG_INFO("Launch TCP Client Failed");
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
			}

			return;
		}
		else
		{
			for (auto & iter : pool)
			{
				iter->Send(value.c_str(), value.size());
			}
		}
	}
}


/**
 *
 * 启动tcp服务器
 *
 * @param localHost 主机地址
 * @param localPort 主机端口
 *
 */
void LaunchTCPServer(const std::string & localHost, uint16_t localPort)
{
	TCPServer server;

	if (server.Launch(localHost, localPort, TINY_TOOLKIT_MB))
	{
		TINY_TOOLKIT_SYNC_LOG_INFO("Launch TCP Server Success");
	}
	else
	{
		TINY_TOOLKIT_SYNC_LOG_INFO("Launch TCP Server Failed");
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


/**
 *
 * 启动udp客户端
 *
 * @param localHost 主机地址
 * @param localPort 主机端口
 * @param remoteHost 远端地址
 * @param remotePort 远端端口
 * @param clientCount 客户端个数
 *
 */
void LaunchUDPClient(const std::string & localHost, uint16_t localPort, const std::string & remoteHost, uint16_t remotePort, uint32_t clientCount)
{
	std::vector<UDPClientSession *> pool;

	for (uint32_t i = 0; i < clientCount; ++i)
	{
		pool.push_back(new UDPClientSession());

		if (pool.back()->Launch(localHost, localPort, remoteHost, remotePort, TINY_TOOLKIT_MB))
		{
			TINY_TOOLKIT_SYNC_LOG_INFO("Launch UDP Client Success");
		}
		else
		{
			TINY_TOOLKIT_SYNC_LOG_INFO("Launch UDP Client Failed");
		}
	}

	for (uint32_t j = 0; j < 1000; ++j)
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


////////////////////////////////////////////////////////////////////////////////////////////////////


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
	TINY_TOOLKIT_SYNC_LOG_INFO("Start App : {}", 0);

	auto type = sOption.Has("type") ? sOption.Get("type") : "udp";
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
			LaunchTCPClient(remoteHost, remotePort, count);
		}
		else if (mode == "server")
		{
			LaunchTCPServer(localHost, localPort);
		}
	}
	else if (type == "udp")
	{
		LaunchUDPClient(localHost, localPort, remoteHost, remotePort, count);
	}
}


void CloseApp(int32_t signalNo = 0)
{
	TINY_TOOLKIT_SYNC_LOG_INFO("Close App : {}", signalNo);
}


void KillApp(int32_t signalNo = 0)
{
	TINY_TOOLKIT_SYNC_LOG_INFO("Kill App : {}", signalNo);

	CloseApp(signalNo);

	exit(0);
}


int main(int argc, char const * argv[])
{
	ParseOption(argc, argv);

	tinyToolkit::Signal::RegisterIgnore();
	tinyToolkit::Signal::RegisterTerminate(KillApp);
	tinyToolkit::Signal::RegisterStackTrace(tinyToolkit::Backtrace::Print);

	tinyToolkit::SyncLogger::Instance().AddSink<tinyToolkit::ConsoleLogSink>("console");
	tinyToolkit::SyncLogger::Instance().SetSinkLayout<tinyToolkit::SimpleLogLayout>();
	tinyToolkit::SyncLogger::Instance().EnableSinkAutoFlush();

	StartApp();
	CloseApp();

	return 0;
}
