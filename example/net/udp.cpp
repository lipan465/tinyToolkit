/**
 *
 *  作者: hm
 *
 *  说明: tcp
 *
 */


#include "udp.h"


/**
 *
 * 构造函数
 *
 * @param id 序号
 *
 */
UDPClientSession::UDPClientSession(int32_t id) : _id(id)
{

}

/**
 *
 * 析构函数
 *
 */
UDPClientSession::~UDPClientSession()
{
	tinyToolkit::String::Print("Client session destructor\r\n");
}

/**
 *
 * 连接成功触发回调函数
 *
 */
void UDPClientSession::OnConnect()
{
	GetAddress();

	tinyToolkit::String::Print("Client session [{}:{}] connect server session [{}:{}] success\r\n", _localHost, _localPort, _remoteHost, _remotePort);

	std::string value = tinyToolkit::String::Format("Hello UDP server, my client number is {}", _id);

	Send(value.c_str(), value.size());
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void UDPClientSession::OnDisconnect()
{
	tinyToolkit::String::Print("Client session [{}:{}] disconnect : {}\r\n", _localHost, _localPort, strerror(errno));
}

/**
 *
 * 连接失败触发回调函数
 *
 */
void UDPClientSession::OnConnectFailed()
{
	tinyToolkit::String::Print("Client session [{}:{}] connect server session [{}:{}] failed : {}\r\n", _localHost, _localPort, _remoteHost, _remotePort, strerror(errno));
}

/**
 *
 * 接收数据触发回调函数
 *
 * @param data 接收的数据缓冲区
 * @param size 接收的数据缓冲区长度
 *
 * @return 使用的字节数
 *
 */
std::size_t UDPClientSession::OnReceive(const char * data, std::size_t size)
{
	tinyToolkit::String::Print("Client session [{}:{}] received server session [{}:{}] lengeth [{}] message : {}\r\n", _localHost, _localPort, _remoteHost, _remotePort, size, data);

	return size;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 *
 * 构造函数
 *
 * @param id 序号
 *
 */
UDPServerSession::UDPServerSession(int32_t id) : _id(id)
{

}

/**
 *
 * 析构函数
 *
 */
UDPServerSession::~UDPServerSession()
{
	tinyToolkit::String::Print("Server session destructor\r\n");
}

/**
 *
 * 连接成功触发回调函数
 *
 */
void UDPServerSession::OnConnect()
{
	GetAddress();

	tinyToolkit::String::Print("Server session [{}:{}] connect client session [{}:{}] success\r\n", _localHost, _localPort, _remoteHost, _remotePort);

	std::string value = tinyToolkit::String::Format("Hello UDP client, my server number is {}", _id);

	Send(value.c_str(), value.size());
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void UDPServerSession::OnDisconnect()
{
	tinyToolkit::String::Print("Server session [{}:{}] disconnect : {}\r\n", _localHost, _localPort, strerror(errno));
}

/**
 *
 * 连接失败触发回调函数
 *
 */
void UDPServerSession::OnConnectFailed()
{
	tinyToolkit::String::Print("Server session [{}:{}] connect client session [{}:{}] failed : {}\r\n", _localHost, _localPort, _remoteHost, _remotePort, strerror(errno));
}

/**
 *
 * 接收数据触发回调函数
 *
 * @param data 接收的数据缓冲区
 * @param size 接收的数据缓冲区长度
 *
 * @return 使用的字节数
 *
 */
std::size_t UDPServerSession::OnReceive(const char * data, std::size_t size)
{
	tinyToolkit::String::Print("Server session [{}:{}] received client session [{}:{}] lengeth [{}] message : {}\r\n", _localHost, _localPort, _remoteHost, _remotePort, size, data);

	if (strstr(data, "request"))
	{
		std::string value = tinyToolkit::String::Format("Hello UDP client , i will accept your request");

		Send(value.c_str(), value.size());
	}

	return size;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 *
 * 析构函数
 *
 */
UDPServer::~UDPServer()
{
	tinyToolkit::String::Print("Server destructor\r\n");
}

/**
 *
 * 新连接触发回调函数
 *
 * @param host 主机地址
 * @param port 主机端口
 *
 * @return 会话
 *
 */
tinyToolkit::IUDPSession * UDPServer::OnNewConnect(const std::string & host, uint16_t port)
{
	tinyToolkit::String::Print("Server [{}:{}] accept client [{}:{}] session request\r\n", _host, _port, host, port);

	auto key = tinyToolkit::String::Join(host, ":", port);

	auto find = _pool.find(key);

	if (find == _pool.end())
	{
		_pool.insert(std::make_pair(key, new UDPServerSession(port)));

		find = _pool.find(key);
	}

	return find->second;
}

/**
 *
 * 会话错误触发回调函数
 *
 */
void UDPServer::OnSessionError(tinyToolkit::IUDPSession * session)
{
	if (session)
	{
		tinyToolkit::String::Print("Server [{}:{}] and client [{}:{}] session error : {}\r\n", _host, _port, session->_remoteHost, session->_remotePort, strerror(errno));

		auto key = tinyToolkit::String::Join(session->_remoteHost, ":", session->_remotePort);

		auto find = _pool.find(key);

		if (find != _pool.end())
		{
			find->second->Close();

			delete find->second;

			_pool.erase(find);
		}
	}
	else
	{
		std::cout << "session null" << std::endl;
	}
}

/**
 *
 * 错误触发回调函数
 *
 */
void UDPServer::OnError()
{
	tinyToolkit::String::Print("Server [{}:{}] error : {}\r\n", _host, _port, strerror(errno));
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void UDPServer::OnRelease()
{
	tinyToolkit::String::Print("Server [{}:{}] disconnect : {}\r\n", _host, _port, strerror(errno));

	for (auto &iter : _pool)
	{
		iter.second->Close();

		delete iter.second;
	}
}
