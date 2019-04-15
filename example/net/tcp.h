#ifndef __EXAMPLE__NET__TCP__H__
#define __EXAMPLE__NET__TCP__H__


/**
 *
 *  作者: hm
 *
 *  说明: tcp
 *
 */


#include "tinyToolkit/tinyToolkit.h"


class TCPClientSession : public tinyToolkit::ITCPSession
{
public:
	/**
	 *
	 * 构造函数
	 *
	 */
	TCPClientSession();

	/**
	 *
	 * 析构函数
	 *
	 */
	~TCPClientSession() override;

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
	 * @param data 接收的数据缓冲区指针
	 * @param size 接收的数据缓冲区长度
	 *
	 * @return 使用的字节数
	 *
	 */
	std::size_t OnReceive(const char * data, std::size_t size) override;
};


class TCPServerSession : public tinyToolkit::ITCPSession
{
public:
	/**
	 *
	 * 构造函数
	 *
	 */
	TCPServerSession();

	/**
	 *
	 * 析构函数
	 *
	 */
	~TCPServerSession() override;

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
	 * @param data 接收的数据缓冲区指针
	 * @param size 接收的数据缓冲区长度
	 *
	 * @return 使用的字节数
	 *
	 */
	std::size_t OnReceive(const char * data, std::size_t size) override;
};


class TCPServer : public tinyToolkit::ITCPServer
{
public:
	/**
	 *
	 * 构造函数
	 *
	 */
	TCPServer();

	/**
	 *
	 * 析构函数
	 *
	 */
	~TCPServer() override;

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

	/**
	 *
	 * 会话错误触发回调函数
	 *
	 */
	void OnSessionError(tinyToolkit::ITCPSession * session) override;

	/**
	 *
	 * 会话连接触发回调函数
	 *
	 * @return 会话
	 *
	 */
	tinyToolkit::ITCPSession * OnSessionConnect() override;

protected:
	std::unordered_map<std::string, tinyToolkit::ITCPSession *> _pool{ };
};


#endif // __EXAMPLE__NET__TCP__H__
