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
	tinyToolkit::String::Print("客户端会话析构\r\n");
}

/**
 *
 * 接收数据触发回调函数
 *
 * @param data 接收的数据缓冲区
 * @param size 接收的数据缓冲区长度
 *
 */
void UDPClientSession::OnReceive(const char * data, std::size_t size)
{
	tinyToolkit::String::Print("客户端会话[{}:{}]接收到服务器会话[{}:{}]长度为[{}]的信息 : {}\r\n", _localHost, _localPort, _remoteHost, _remotePort, size, data);
}

/**
 *
 * 连接成功触发回调函数
 *
 */
void UDPClientSession::OnConnect()
{
	tinyToolkit::String::Print("客户端会话[{}:{}]连接服务器会话[{}:{}]成功\r\n", _localHost, _localPort, _remoteHost, _remotePort);

	std::string value = tinyToolkit::String::Format("udp服务器你好, 我的编号为{}", _id);

	Send(value.c_str(), value.size());
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void UDPClientSession::OnDisconnect()
{
	tinyToolkit::String::Print("客户端会话[{}:{}]断开连接 : {}\r\n", _localHost, _localPort, strerror(errno));
}

/**
 *
 * 连接失败触发回调函数
 *
 */
void UDPClientSession::OnConnectFailed()
{
	tinyToolkit::String::Print("客户端会话[{}:{}]连接服务器会话[{}:{}]失败 : {}\r\n", _localHost, _localPort, _remoteHost, _remotePort, strerror(errno));
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
	tinyToolkit::String::Print("服务器会话析构\r\n");
}

/**
 *
 * 接收数据触发回调函数
 *
 * @param data 接收的数据缓冲区
 * @param size 接收的数据缓冲区长度
 *
 */
void UDPServerSession::OnReceive(const char * data, std::size_t size)
{
	tinyToolkit::String::Print("服务器会话[{}:{}]接收到客户端会话[{}:{}]长度为[{}]的信息 : {}\r\n", _localHost, _localPort, _remoteHost, _remotePort, size, data);

	if (strstr(data, "request"))
	{
		std::string value = tinyToolkit::String::Format("udp客户端你好, 请求已收到");

		Send(value.c_str(), value.size());
	}
}

/**
 *
 * 连接成功触发回调函数
 *
 */
void UDPServerSession::OnConnect()
{
	tinyToolkit::String::Print("服务器会话[{}:{}]连接客户端会话[{}:{}]成功\r\n", _localHost, _localPort, _remoteHost, _remotePort);

	std::string value = tinyToolkit::String::Format("udp客户端你好, 我的编号为{}", _id);

	Send(value.c_str(), value.size());
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void UDPServerSession::OnDisconnect()
{
	tinyToolkit::String::Print("服务器会话[{}:{}]断开连接 : {}\r\n", _localHost, _localPort, strerror(errno));
}

/**
 *
 * 连接失败触发回调函数
 *
 */
void UDPServerSession::OnConnectFailed()
{
	tinyToolkit::String::Print("服务器会话[{}:{}]连接客户端会话[{}:{}]失败 : {}\r\n", _localHost, _localPort, _remoteHost, _remotePort, strerror(errno));
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 *
 * 析构函数
 *
 */
UDPServer::~UDPServer()
{
	tinyToolkit::String::Print("服务器析构\r\n");
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
	tinyToolkit::String::Print("服务器[{}:{}]接收到客户端[{}:{}]会话请求\r\n", _host, _port, host, port);

	auto key = tinyToolkit::String::Splice(host, ":", port);

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
		tinyToolkit::String::Print("服务器[{}:{}]与客户端[{}:{}]会话错误 : {}\r\n", _host, _port, session->_remoteHost, session->_remotePort, strerror(errno));

		auto key = tinyToolkit::String::Splice(session->_remoteHost, ":", session->_remotePort);

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
	tinyToolkit::String::Print("服务器[{}:{}]异常 : {}\r\n", _host, _port, strerror(errno));
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void UDPServer::OnRelease()
{
	tinyToolkit::String::Print("服务器[{}:{}]断开连接 : {}\r\n", _host, _port, strerror(errno));

	for (auto &iter : _pool)
	{
		iter.second->Close();

		delete iter.second;
	}
}
