#ifndef __TINY_TOOLKIT__NET__SOCKET__H__
#define __TINY_TOOLKIT__NET__SOCKET__H__


/**
 *
 *  作者: hm
 *
 *  说明: socket
 *
 */


#include "context.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API NetSocket
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		NetSocket() = default;

		/**
		 *
		 * 构造函数
		 *
		 * @param socket 句柄
		 *
		 */
		explicit NetSocket(TINY_TOOLKIT_SOCKET_TYPE socket);

		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~NetSocket();

		/**
		 *
		 * 关闭
		 *
		 * @return 是否关闭成功
		 *
		 */
		bool Close();

		/**
		 *
		 * 是否是有效句柄
		 *
		 * @return 是否是有效句柄
		 *
		 */
		bool IsValid();

		/**
		 *
		 * 设置是否开启延时关闭
		 *
		 * @param on 状态
		 * @param timeout 超时时长
		 *
		 * @return 是否设置成功
		 *
		 */
		bool SetLinger(bool on, int32_t timeout);

		/**
		 *
		 * 设置是否非堵塞
		 *
		 * @param on 状态
		 *
		 * @return 是否设置成功
		 *
		 */
		bool SetNoBlock(bool on);

		/**
		 *
		 * 设置是否关闭Nagle算法
		 *
		 * @param on 状态
		 *
		 * @return 是否设置成功
		 *
		 */
		bool SetNoDelay(bool on);

		/**
		 *
		 * 设置是否启用端口复用
		 *
		 * @param on 状态
		 *
		 * @return 是否设置成功
		 *
		 */
		bool SetReusePort(bool on);

		/**
		 *
		 * 设置是否启用地址复用
		 *
		 * @param on 状态
		 *
		 * @return 是否设置成功
		 *
		 */
		bool SetReuseAddress(bool on);

		/**
		 *
		 * 绑定
		 *
		 * @param host 地址
		 * @param port 端口
		 *
		 * @return 绑定结果
		 *
		 */
		int32_t Bind(const char * host, uint16_t port);

		/**
		 *
		 * 监听
		 *
		 * @param backlog 上限
		 *
		 * @return 监听结果
		 *
		 */
		int32_t Listen(int32_t backlog);

		/**
		 *
		 * 接受连接
		 *
		 * @param context 上下文
		 *
		 * @return 接受结果
		 *
		 */
		int32_t Accept(NetContext * context = nullptr);

		/**
		 *
		 * 连接
		 *
		 * @param host 地址
		 * @param port 端口
		 * @param context 上下文
		 *
		 * @return 连接结果
		 *
		 */
		int32_t Connect(const char * host, uint16_t port, NetContext * context = nullptr);

		/**
		 *
		 * 发送
		 *
		 * @param buffer 待发送缓冲区指针
		 * @param length 待发送缓冲区长度
		 * @param context 上下文
		 *
		 * @return 发送字节数
		 *
		 */
		int32_t Send(const char * buffer, std::size_t length, NetContext * context = nullptr);

		/**
		 *
		 * 接收
		 *
		 * @param buffer 待接收缓冲区指针
		 * @param length 待接收缓冲区长度
		 * @param context 上下文
		 *
		 * @return 接收字节数
		 *
		 */
		int32_t Receive(char * buffer, std::size_t length, NetContext * context = nullptr);

		/**
		 *
		 * 句柄
		 *
		 * @return 句柄
		 *
		 */
		TINY_TOOLKIT_SOCKET_TYPE Handle();

	protected:
		TINY_TOOLKIT_SOCKET_TYPE _socket{ TINY_TOOLKIT_SOCKET_INVALID };
	};

	class TINY_TOOLKIT_API TCPSocket : public NetSocket
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @return 构造函数
		 *
		 */
		TCPSocket();

		/**
		 *
		 * 析构函数
		 *
		 */
		~TCPSocket() override = default;
	};

	class TINY_TOOLKIT_API UDPSocket : public NetSocket
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @return 构造函数
		 *
		 */
		UDPSocket();

		/**
		 *
		 * 析构函数
		 *
		 */
		~UDPSocket() override = default;
	};
}


#endif // __TINY_TOOLKIT__NET__SOCKET__H__
