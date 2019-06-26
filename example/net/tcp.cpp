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
 */
TCPClientSession::TCPClientSession()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Client Session Constructor");
}

/**
 *
 * 析构函数
 *
 */
TCPClientSession::~TCPClientSession()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Client Session Destructor");
}

/**
 *
 * 连接成功触发回调函数
 *
 */
void TCPClientSession::OnConnect()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Client Session [{}:{}] Connect Server Session [{}:{}] Success", LocalHost(), LocalPort(), RemoteHost(), RemotePort());
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void TCPClientSession::OnDisconnect()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Client Session [{}:{}] Disconnect : {}", LocalHost(), LocalPort(), tinyToolkit::OS::LastErrorMessage());
}

/**
 *
 * 连接失败触发回调函数
 *
 */
void TCPClientSession::OnConnectFailed()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Client Session [{}:{}] Connect Server Session [{}:{}] Failed : {}", LocalHost(), LocalPort(), RemoteHost(), RemotePort(), tinyToolkit::OS::LastErrorMessage());
}

/**
 *
 * 接收数据触发回调函数
 *
 * @param data 接收的数据缓冲区指针
 * @param size 接收的数据缓冲区长度
 *
 * @return 使用的字节数
 *
 */
std::size_t TCPClientSession::OnReceive(const char * data, std::size_t size)
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Client Session [{}:{}] Received Server Session [{}:{}] Length [{}] Message : {}", LocalHost(), LocalPort(), RemoteHost(), RemotePort(), size, data);

	return size;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 *
 * 构造函数
 *
 */
TCPServerSession::TCPServerSession()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Server Session Constructor");
}

/**
 *
 * 析构函数
 *
 */
TCPServerSession::~TCPServerSession()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Server Session Destructor");
}

/**
 *
 * 连接成功触发回调函数
 *
 */
void TCPServerSession::OnConnect()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Server Session [{}:{}] Connect Client Session [{}:{}] Success", LocalHost(), LocalPort(), RemoteHost(), RemotePort());
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void TCPServerSession::OnDisconnect()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Server Session [{}:{}] Disconnect : {}", LocalHost(), LocalPort(), tinyToolkit::OS::LastErrorMessage());
}

/**
 *
 * 连接失败触发回调函数
 *
 */
void TCPServerSession::OnConnectFailed()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Server Session [{}:{}] Connect Client Session [{}:{}] Failed : {}", LocalHost(), LocalPort(), RemoteHost(), RemotePort(), tinyToolkit::OS::LastErrorMessage());
}

/**
 *
 * 接收数据触发回调函数
 *
 * @param data 接收的数据缓冲区指针
 * @param size 接收的数据缓冲区长度
 *
 * @return 使用的字节数
 *
 */
std::size_t TCPServerSession::OnReceive(const char * data, std::size_t size)
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Server Session [{}:{}] Received Client Session [{}:{}] Length [{}] Message : {}", LocalHost(), LocalPort(), RemoteHost(), RemotePort(), size, data);

	if (strstr(data, "request"))
	{
		std::string value = tinyToolkit::String::Format("Hello TCP Client, I Will Accept Your Request");

		Send(value.c_str(), value.size());
	}

	return size;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 *
 * 构造函数
 *
 */
TCPServer::TCPServer()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Server Constructor");
}

/**
 *
 * 析构函数
 *
 */
TCPServer::~TCPServer()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Server Destructor");
}

/**
 *
 * 错误触发回调函数
 *
 */
void TCPServer::OnError()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Server [{}:{}] Error : {}", LocalHost(), LocalPort(), tinyToolkit::OS::LastErrorMessage());
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void TCPServer::OnRelease()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Server [{}:{}] Release : {}", LocalHost(), LocalPort(), tinyToolkit::OS::LastErrorMessage());

	for (auto &iter : _pool)
	{
		iter.second->Close();

		delete iter.second;
	}
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
		TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Server [{}:{}] And Client Session [{}:{}] Error : {}", LocalHost(), LocalPort(), RemoteHost(), RemotePort(), tinyToolkit::OS::LastErrorMessage());

		auto key = tinyToolkit::String::Join(RemoteHost(), ":", RemotePort());

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
		TINY_TOOLKIT_ASYNC_LOG_INFO("Session NULL");
	}
}

/**
 *
 * 会话连接触发回调函数
 *
 * @return 会话
 *
 */
tinyToolkit::ITCPSession * TCPServer::OnSessionConnect()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("TCP Server [{}:{}] Accept Client Session [{}:{}] Connect", LocalHost(), LocalPort(), RemoteHost(), RemotePort());

	auto key = tinyToolkit::String::Join(RemoteHost(), ":", RemotePort());

	auto find = _pool.find(key);

	if (find == _pool.end())
	{
		_pool.insert(std::make_pair(key, new TCPServerSession()));

		find = _pool.find(key);
	}

	return find->second;
}
