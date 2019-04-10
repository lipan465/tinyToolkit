#ifndef __EXAMPLE__NET__UDP__H__
#define __EXAMPLE__NET__UDP__H__


/**
 *
 *  作者: hm
 *
 *  说明: udp
 *
 */


#include "tinyToolkit/tinyToolkit.h"


class UDPClientSession : public tinyToolkit::IUDPSession
{
public:
	/**
	 *
	 * 构造函数
	 *
	 */
	explicit UDPClientSession() = default;

	/**
	 *
	 * 析构函数
	 *
	 */
	~UDPClientSession() override;

	/**
	 *
	 * 连接成功触发回调函数
	 *
	 */
	void OnConnect() override;

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnDisconnect() override;

	/**
	 *
	 * 连接失败触发回调函数
	 *
	 */
	void OnConnectFailed() override;

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
	std::size_t OnReceive(const char * data, std::size_t size) override;
};


#endif // __EXAMPLE__NET__UDP__H__
