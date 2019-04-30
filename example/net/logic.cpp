/**
 *
 *  作者: hm
 *
 *  说明: 逻辑
 *
 */


#include "logic.h"


#define sOption	tinyToolkit::OptionManager::Instance()


/**
 *
 * 启动
 *
 */
void Logic::Launch()
{
	auto type = sOption.Has("type") ? sOption.Get("type") : "udp";
	auto mode = sOption.Has("mode") ? sOption.Get("mode") : "client";

	auto count = sOption.Has("count") ? tinyToolkit::String::Transform<uint32_t>(sOption.Get("count")) : static_cast<uint32_t>(1);

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

	auto localHost = sOption.Has("localHost") ? sOption.Get("localHost") : "0.0.0.0";
	auto localPort = sOption.Has("localPort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("localPort")) : static_cast<uint16_t>(1234);

	auto remoteHost = sOption.Has("remoteHost") ? sOption.Get("remoteHost") : "192.168.2.71";
	auto remotePort = sOption.Has("remotePort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("remotePort")) : static_cast<uint16_t>(1234);

#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX

	auto localHost = sOption.Has("localHost") ? sOption.Get("localHost") : "0.0.0.0";
	auto localPort = sOption.Has("localPort") ? tinyToolkit::String::Transform<uint16_t>(sOption.Get("localPort")) : static_cast<uint16_t>(1234);

	auto remoteHost = sOption.Has("remoteHost") ? sOption.Get("remoteHost") : "192.168.2.171";
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

/**
 *
 * 启动tcp客户端
 *
 * @param remoteHost 远端地址
 * @param remotePort 远端端口
 * @param clientCount 客户端个数
 *
 */
void Logic::LaunchTCPClient(const std::string & remoteHost, uint16_t remotePort, uint32_t clientCount)
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
void Logic::LaunchTCPServer(const std::string & localHost, uint16_t localPort)
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
void Logic::LaunchUDPClient(const std::string & localHost, uint16_t localPort, const std::string & remoteHost, uint16_t remotePort, uint32_t clientCount)
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
