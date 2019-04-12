/**
 *
 *  作者: hm
 *
 *  说明: 可靠udp
 *
 */


#include "rudp.h"


/**
 *
 * 析构函数
 *
 */
RUDPClientSession::~RUDPClientSession()
{
	TINY_TOOLKIT_SYNC_LOG_INFO("RUDP Client session destructor");
}

/**
 *
 * 连接成功触发回调函数
 *
 */
void RUDPClientSession::OnConnect()
{
	TINY_TOOLKIT_SYNC_LOG_INFO("RUDP Client session [{}:{}] create success", LocalHost(), LocalPort());
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void RUDPClientSession::OnDisconnect()
{
	TINY_TOOLKIT_SYNC_LOG_INFO("RUDP Client session [{}:{}] disconnect : {}", LocalHost(), LocalPort(), strerror(errno));
}

/**
 *
 * 连接失败触发回调函数
 *
 */
void RUDPClientSession::OnConnectFailed()
{
	TINY_TOOLKIT_SYNC_LOG_INFO("RUDP Client session [{}:{}] create failed : {}", LocalHost(), LocalPort(), strerror(errno));
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
std::size_t RUDPClientSession::OnReceive(const char * data, std::size_t size)
{
	TINY_TOOLKIT_SYNC_LOG_INFO("RUDP Client session [{}:{}] received server session [{}:{}] length [{}] message : {}", LocalHost(), LocalPort(), RemoteHost(), RemotePort(), data, size);

	return size;
}