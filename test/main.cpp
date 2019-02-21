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
	tinyToolkit::OptionManager::Instance().DefineArg("mode",  nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("type",  nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("host",  nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("port",  nullptr, nullptr, tinyToolkit::Application::Name().c_str());
	tinyToolkit::OptionManager::Instance().DefineArg("size",  nullptr, nullptr, tinyToolkit::Application::Name().c_str());

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

				if (session->Launch(host, port))
				{
					tinyToolkit::String::Print("客户端[{}:{}]启动成功\r\n", host, port);
				}
				else
				{
					tinyToolkit::String::Print("客户端[{}:{}]启动失败 : {}\r\n", host, port,
											   tinyToolkit::OS::LastErrorMessage());
				}

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

			if (server.Launch(host, port))
			{
				tinyToolkit::String::Print("服务器[{}:{}]启动成功\r\n", host, port);
			}
			else
			{
				tinyToolkit::String::Print("服务器[{}:{}]启动失败 : {}\r\n", host, port, tinyToolkit::OS::LastErrorMessage());
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
		if (mode == "client")
		{
			std::vector<UDPClientSession *> pool;

			for (uint32_t i = 0; i < size; ++i)
			{
				auto session = new UDPClientSession(i);

				if (session->Launch(host, port))
				{
					tinyToolkit::String::Print("客户端[{}:{}]启动成功\r\n", host, port);
				}
				else
				{
					tinyToolkit::String::Print("客户端[{}:{}]启动失败 : {}\r\n", host, port, tinyToolkit::OS::LastErrorMessage());
				}

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
			UDPServer server;

			if (server.Launch(host, port))
			{
				tinyToolkit::String::Print("服务器[{}:{}]启动成功\r\n", host, port);
			}
			else
			{
				tinyToolkit::String::Print("服务器[{}:{}]启动失败 : {}\r\n", host, port, tinyToolkit::OS::LastErrorMessage());
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
}


int main(int argc, char const * argv[])
{
	ParseOption(argc, argv);

	tinyToolkit::Signal::RegisterIgnore();
	tinyToolkit::Signal::RegisterStackTrace(tinyToolkit::Backtrace::Print);

	StartApp();

	return 0;
}
