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
 * 析构函数
 *
 */
UDPClientSession::~UDPClientSession()
{
	tinyToolkit::String::Print("UDP Client session destructor\r\n");
}

/**
 *
 * 连接成功触发回调函数
 *
 */
void UDPClientSession::OnConnect()
{
	tinyToolkit::String::Print("UDP Client session [{}:{}] create success\r\n", _localHost, _localPort);
}

/**
 *
 * 断开连接触发回调函数
 *
 */
void UDPClientSession::OnDisconnect()
{
	tinyToolkit::String::Print("UDP Client session [{}:{}] disconnect : {}\r\n", _localHost, _localPort, strerror(errno));
}

/**
 *
 * 连接失败触发回调函数
 *
 */
void UDPClientSession::OnConnectFailed()
{
	tinyToolkit::String::Print("UDP Client session [{}:{}] create failed : {}\r\n", _localHost, _localPort, strerror(errno));
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
	tinyToolkit::String::Print("UDP Client session [{}:{}] received server session [{}:{}] length [{}] message : {}\r\n", _localHost, _localPort, _remoteHost, _remotePort, size, data);

	return size;
}
