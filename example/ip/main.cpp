/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void Socket()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		{
			auto sock = ip::Socket::PollSocket();

			std::cout << sock << std::endl;

			if (ip::Socket::IsValidPoll(sock))
			{
				ip::Socket::ClosePoll(sock);
			}
		}

		{
			auto sock = ip::Socket::TCPSocketV4();

			std::cout << sock << std::endl;

			if (ip::Socket::IsValid(sock))
			{
				auto peerEndpoint = ip::Socket::PeerEndpointV4(sock);
				auto localEndpoint = ip::Socket::LocalEndpointV4(sock);

				std::cout << peerEndpoint.first << std::endl;
				std::cout << peerEndpoint.second << std::endl;

				std::cout << localEndpoint.first << std::endl;
				std::cout << localEndpoint.second << std::endl;

				ip::Socket::SetLinger(sock, true, 10);
				ip::Socket::SetBlock(sock, false);
				ip::Socket::SetDelay(sock, false);
				ip::Socket::SetReusePort(sock, true);
				ip::Socket::SetReuseAddress(sock, true);
				ip::Socket::SetSendTimeout(sock, 10);
				ip::Socket::SetReceiveTimeout(sock, 10);
				ip::Socket::SetSendBufferSize(sock, 1024);
				ip::Socket::SetReceiveBufferSize(sock, 2048);

				std::cout << ip::Socket::Family(sock) << std::endl;
				std::cout << ip::Socket::ReadableSize(sock) << std::endl;
				std::cout << ip::Socket::SendBufferSize(sock) << std::endl;
				std::cout << ip::Socket::ReceiveBufferSize(sock) << std::endl;

				ip::Socket::Close(sock);
			}
		}

		{
			auto sock = ip::Socket::UDPSocketV4();

			std::cout << sock << std::endl;

			if (ip::Socket::IsValid(sock))
			{
				auto peerEndpoint = ip::Socket::PeerEndpointV4(sock);
				auto localEndpoint = ip::Socket::LocalEndpointV4(sock);

				std::cout << peerEndpoint.first << std::endl;
				std::cout << peerEndpoint.second << std::endl;

				std::cout << localEndpoint.first << std::endl;
				std::cout << localEndpoint.second << std::endl;

				ip::Socket::SetBlock(sock, false);
				ip::Socket::SetReusePort(sock, true);
				ip::Socket::SetReuseAddress(sock, true);
				ip::Socket::SetSendTimeout(sock, 10);
				ip::Socket::SetReceiveTimeout(sock, 10);
				ip::Socket::SetSendBufferSize(sock, 1024);
				ip::Socket::SetReceiveBufferSize(sock, 2048);

				std::cout << ip::Socket::Family(sock) << std::endl;
				std::cout << ip::Socket::ReadableSize(sock) << std::endl;
				std::cout << ip::Socket::SendBufferSize(sock) << std::endl;
				std::cout << ip::Socket::ReceiveBufferSize(sock) << std::endl;

				ip::Socket::Close(sock);
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
	(void)argc;
	(void)argv;

	Socket();

	return 0;
}
