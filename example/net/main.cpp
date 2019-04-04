/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


void ParseOption(int argc, char const * argv[])
{
	tinyToolkit::OptionManager::Instance().DefineArg("mode",  nullptr, "client",		tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("type",  nullptr, "udp",			tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("host",  nullptr, "192.168.2.71",	tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("port",  nullptr, "1234",			tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("size",  nullptr, "3",				tinyToolkit::Application::Name().c_str());

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
			std::vector<TCPClientSession *> pool;

			for (uint32_t i = 0; i < size; ++i)
			{
				auto session = new TCPClientSession(i);

				session->Launch(host, port, TINY_TOOLKIT_MB, TINY_TOOLKIT_MB);

				pool.push_back(session);
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
	else if (type == "udp")
	{
		std::vector<UDPClientSession *> pool;

		for (uint32_t i = 0; i < size; ++i)
		{
			auto session = new UDPClientSession(i);

			session->Launch(host, port);

			pool.push_back(session);
		}

		auto time1 = tinyToolkit::Time::Milliseconds();

		for (int j = 0; j < 10000; ++j)
		{
			for (auto &iter : pool)
			{
				iter->Send("192.168.2.70", 1234, "this is test message", 20);
			}
		}

		auto time2 = tinyToolkit::Time::Milliseconds();

		std::cout << "time : " << time2 - time1 << std::endl;

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
					iter->Send("192.168.2.70", 1234, value.c_str(), value.size());
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
