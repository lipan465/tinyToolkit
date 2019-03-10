/**
 *
 *  作者: hm
 *
 *  说明: tcp
 *
 */


#include "tcp.h"


/**
 *
 * 构造函数
 *
 * @param id 序号
 *
 */
TCPClientSession::TCPClientSession(int32_t id) : _id(id)
{

}

/**
 *
 * 析构函数
 *
 */
TCPClientSession::~TCPClientSession()
{
	tinyToolkit::String::Print("Client session destructor\r\n");
}

/**
 *
 * 连接成功触发回调函数
 *
 */
void TCPClientSession::OnConnect()
{
	tinyToolkit::String::Print("Client session [{}:{}] connect server session [{}:{}] success\r\n", _localHost, _localPort, _remoteHost, _remotePort);

	std::string value = tinyToolkit::String::Format("Hello TCP server, my client number is {}", _id);

	Send(value.c_str(), value.size());
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void TCPClientSession::OnDisconnect()
{
	tinyToolkit::String::Print("Client session [{}:{}] disconnect : {}\r\n", _localHost, _localPort, strerror(errno));
}

/**
 *
 * 连接失败触发回调函数
 *
 */
void TCPClientSession::OnConnectFailed()
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
std::size_t TCPClientSession::OnReceive(const char * data, std::size_t size)
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
TCPServerSession::TCPServerSession(int32_t id) : _id(id)
{

}

/**
 *
 * 析构函数
 *
 */
TCPServerSession::~TCPServerSession()
{
	tinyToolkit::String::Print("Server session destructor\r\n");
}

/**
 *
 * 连接成功触发回调函数
 *
 */
void TCPServerSession::OnConnect()
{
	tinyToolkit::String::Print("Server session [{}:{}] connect client session [{}:{}] success\r\n", _localHost, _localPort, _remoteHost, _remotePort);

	std::string value = tinyToolkit::String::Format("Hello TCP client, my server number is {}", _id);

	Send(value.c_str(), value.size());
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void TCPServerSession::OnDisconnect()
{
	tinyToolkit::String::Print("Server session [{}:{}] disconnect : {}\r\n", _localHost, _localPort, strerror(errno));
}

/**
 *
 * 连接失败触发回调函数
 *
 */
void TCPServerSession::OnConnectFailed()
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
std::size_t TCPServerSession::OnReceive(const char * data, std::size_t size)
{
	tinyToolkit::String::Print("Server session [{}:{}] received client session [{}:{}] lengeth [{}] message : {}\r\n", _localHost, _localPort, _remoteHost, _remotePort, size, data);

	if (strstr(data, "request"))
	{
		std::string value = tinyToolkit::String::Format("Hello TCP client , i will accept your request");

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
TCPServer::~TCPServer()
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
tinyToolkit::ITCPSession * TCPServer::OnNewConnect(const std::string & host, uint16_t port)
{
	tinyToolkit::String::Print("Server [{}:{}] accept client [{}:{}] session request\r\n", _host, _port, host, port);

	auto key = tinyToolkit::String::Join(host, ":", port);

	auto find = _pool.find(key);

	if (find == _pool.end())
	{
		_pool.insert(std::make_pair(key, new TCPServerSession(port)));

		find = _pool.find(key);
	}

	return find->second;
}

/**
 *
 * 会话错误触发回调函数
 *
 */
void TCPServer::OnSessionError(tinyToolkit::ITCPSession * session)
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
void TCPServer::OnError()
{
	tinyToolkit::String::Print("Server [{}:{}] error : {}\r\n", _host, _port, strerror(errno));
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void TCPServer::OnRelease()
{
	tinyToolkit::String::Print("Server [{}:{}] disconnect : {}\r\n", _host, _port, strerror(errno));

	for (auto &iter : _pool)
	{
		iter.second->Close();

		delete iter.second;
	}
}
