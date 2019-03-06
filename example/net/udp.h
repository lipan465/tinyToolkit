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
	 * @param id 序号
	 *
	 */
	explicit UDPClientSession(int32_t id);

	/**
	 *
	 * 析构函数
	 *
	 */
	~UDPClientSession() override;

	/**
	 *
	 * 接收数据触发回调函数
	 *
	 * @param data 接收的数据缓冲区
	 * @param size 接收的数据缓冲区长度
	 *
	 */
	void OnReceive(const char * data, std::size_t size) override;

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

private:
	int32_t _id{ 0 };
};


class UDPServerSession : public tinyToolkit::IUDPSession
{
public:
	/**
	 *
	 * 构造函数
	 *
	 * @param id 序号
	 *
	 */
	explicit UDPServerSession(int32_t id);

	/**
	 *
	 * 析构函数
	 *
	 */
	~UDPServerSession() override;

	/**
	 *
	 * 接收数据触发回调函数
	 *
	 * @param data 接收的数据缓冲区
	 * @param size 接收的数据缓冲区长度
	 *
	 */
	void OnReceive(const char * data, std::size_t size) override;

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

private:
	int32_t _id{ 0 };
};


class UDPServer : public tinyToolkit::IUDPServer
{
public:
	/**
	 *
	 * 析构函数
	 *
	 */
	~UDPServer() override;

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
	tinyToolkit::IUDPSession * OnNewConnect(const std::string & host, uint16_t port) override;

	/**
	 *
	 * 会话错误触发回调函数
	 *
	 */
	void OnSessionError(tinyToolkit::IUDPSession * session) override;

	/**
	 *
	 * 错误触发回调函数
	 *
	 */
	void OnError() override;

	/**
	 *
	 * 断开连接触发回调函数
	 *
	 */
	void OnRelease() override;

protected:
	std::unordered_map<std::string, tinyToolkit::IUDPSession *> _pool{ };
};


#endif // __EXAMPLE__NET__UDP__H__
