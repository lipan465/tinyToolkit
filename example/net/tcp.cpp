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
	TINY_TOOLKIT_SYNC_LOG_INFO("TCP client session destructor");
}

/**
 *
 * 连接成功触发回调函数
 *
 */
void TCPClientSession::OnConnect()
{
	TINY_TOOLKIT_SYNC_LOG_INFO("TCP client session [{}:{}] connect server session [{}:{}] success", LocalHost(), LocalPort(), RemoteHost(), RemotePort());

	for (uint32_t i = 0; i < 100000; ++i)
	{
		std::string value = tinyToolkit::String::Format("Hello TCP server, message number [{}]", i);

		Send(value.c_str(), value.size());
	}
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void TCPClientSession::OnDisconnect()
{
	TINY_TOOLKIT_SYNC_LOG_INFO("TCP client session [{}:{}] disconnect : {}", LocalHost(), LocalPort(), strerror(errno));
}

/**
 *
 * 连接失败触发回调函数
 *
 */
void TCPClientSession::OnConnectFailed()
{
	TINY_TOOLKIT_SYNC_LOG_INFO("TCP client session [{}:{}] connect server session [{}:{}] failed : {}", LocalHost(), LocalPort(), RemoteHost(), RemotePort(), strerror(errno));
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
	TINY_TOOLKIT_SYNC_LOG_INFO("TCP client session [{}:{}] received server session [{}:{}] lengeth [{}] message : {}", LocalHost(), LocalPort(), RemoteHost(), RemotePort(), size, data);

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
	TINY_TOOLKIT_SYNC_LOG_INFO("TCP server session destructor");
}

/**
 *
 * 连接成功触发回调函数
 *
 */
void TCPServerSession::OnConnect()
{
	TINY_TOOLKIT_SYNC_LOG_INFO("TCP server session [{}:{}] connect client session [{}:{}] success", LocalHost(), LocalPort(), RemoteHost(), RemotePort());

	for (uint32_t i = 0; i < 100000; ++i)
	{
		std::string value = tinyToolkit::String::Format("Hello TCP client, message number [{}]", i);

		Send(value.c_str(), value.size());
	}
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void TCPServerSession::OnDisconnect()
{
	TINY_TOOLKIT_SYNC_LOG_INFO("TCP server session [{}:{}] disconnect : {}", LocalHost(), LocalPort(), strerror(errno));
}

/**
 *
 * 连接失败触发回调函数
 *
 */
void TCPServerSession::OnConnectFailed()
{
	TINY_TOOLKIT_SYNC_LOG_INFO("TCP server session [{}:{}] connect client session [{}:{}] failed : {}", LocalHost(), LocalPort(), RemoteHost(), RemotePort(), strerror(errno));
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
	TINY_TOOLKIT_SYNC_LOG_INFO("TCP server session [{}:{}] received client session [{}:{}] lengeth [{}] message : {}", LocalHost(), LocalPort(), RemoteHost(), RemotePort(), size, data);

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
	TINY_TOOLKIT_SYNC_LOG_INFO("TCP server destructor");
}

/**
 *
 * 错误触发回调函数
 *
 */
void TCPServer::OnError()
{
	TINY_TOOLKIT_SYNC_LOG_INFO("TCP server [{}:{}] error : {}", LocalHost(), LocalPort(), strerror(errno));
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void TCPServer::OnRelease()
{
	TINY_TOOLKIT_SYNC_LOG_INFO("TCP server [{}:{}] disconnect : {}", LocalHost(), LocalPort(), strerror(errno));

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
		TINY_TOOLKIT_SYNC_LOG_INFO("TCP server [{}:{}] and client [{}:{}] session error : {}", LocalHost(), LocalPort(), RemoteHost(), RemotePort(), strerror(errno));

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
		TINY_TOOLKIT_SYNC_LOG_INFO("session null");
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
	TINY_TOOLKIT_SYNC_LOG_INFO("TCP server [{}:{}] accept client [{}:{}] session request", LocalHost(), LocalPort(), RemoteHost(), RemotePort());

	auto key = tinyToolkit::String::Join(RemoteHost(), ":", RemotePort());

	auto find = _pool.find(key);

	if (find == _pool.end())
	{
		_pool.insert(std::make_pair(key, new TCPServerSession(RemotePort())));

		find = _pool.find(key);
	}

	return find->second;
}