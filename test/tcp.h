#ifndef __TEST__TCP__H__
#define __TEST__TCP__H__


/**
 *
 *  作者: hm
 *
 *  说明: tcp
 *
 */


#include "../include/tinyToolkit/tinyToolkit.h"


class TCPClientSession : public tinyToolkit::ITCPSession
{
public:
	/**
	 *
	 * 构造函数
	 *
	 * @param id 序号
	 *
	 */
	explicit TCPClientSession(int32_t id);

	/**
	 *
	 * 析构函数
	 *
	 */
	~TCPClientSession() override;

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


class TCPServerSession : public tinyToolkit::ITCPSession
{
public:
	/**
	 *
	 * 构造函数
	 *
	 * @param id 序号
	 *
	 */
	explicit TCPServerSession(int32_t id);

	/**
	 *
	 * 析构函数
	 *
	 */
	~TCPServerSession() override;

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


class TCPServer : public tinyToolkit::ITCPServer
{
public:
	/**
	 *
	 * 析构函数
	 *
	 */
	~TCPServer() override;

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
	tinyToolkit::ITCPSession * OnNewConnect(const std::string & host, uint16_t port) override;

	/**
	 *
	 * 会话错误触发回调函数
	 *
	 */
	void OnSessionError(tinyToolkit::ITCPSession * session) override;

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
	std::unordered_map<std::string, tinyToolkit::ITCPSession *> _pool{ };
};


#endif // __TEST__TCP__H__
