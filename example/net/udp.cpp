/**
 *
 *  作者: hm
 *
 *  说明: udp
 *
 */


#include "udp.h"


/**
 *
 * 构造函数
 *
 */
UDPClientSession::UDPClientSession()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("UDP Client Session Constructor");
}

/**
 *
 * 析构函数
 *
 */
UDPClientSession::~UDPClientSession()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("UDP Client Session Destructor");
}

/**
 *
 * 连接成功触发回调函数
 *
 */
void UDPClientSession::OnConnect()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("UDP Client Session [{}:{}] Connect Success", LocalHost(), LocalPort());
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void UDPClientSession::OnDisconnect()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("UDP Client Session [{}:{}] Disconnect : {}", LocalHost(), LocalPort(), tinyToolkit::OS::LastErrorMessage());
}

/**
 *
 * 连接失败触发回调函数
 *
 */
void UDPClientSession::OnConnectFailed()
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("UDP Client Session [{}:{}] Connect Failed : {}", LocalHost(), LocalPort(), tinyToolkit::OS::LastErrorMessage());
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
std::size_t UDPClientSession::OnReceive(const char * data, std::size_t size)
{
	TINY_TOOLKIT_ASYNC_LOG_INFO("UDP Client Session [{}:{}] Received Other Session [{}:{}] Length [{}] Message : {}", LocalHost(), LocalPort(), RemoteHost(), RemotePort(), size, data);

	return size;
}
