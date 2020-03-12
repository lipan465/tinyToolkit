#ifndef __TINY_TOOLKIT__NET__ADAPTOR__H__
#define __TINY_TOOLKIT__NET__ADAPTOR__H__


/**
 *
 *  作者: hm
 *
 *  说明: 适配器
 *
 */


#include "context.h"
#include "endpoint.h"


namespace tinyToolkit
{
	namespace net
	{
		class TINY_TOOLKIT_API Adaptor
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 */
			Adaptor() = default;

			/**
			 *
			 * 构造函数
			 *
			 * @param socket 套接字
			 *
			 */
			explicit Adaptor(TINY_TOOLKIT_SOCKET_TYPE socket);

			/**
			 *
			 * 析构函数
			 *
			 */
			virtual ~Adaptor();

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
			 * 是否有效
			 *
			 * @return 是否有效
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
			 * 设置是否堵塞
			 *
			 * @param on 状态
			 *
			 * @return 是否设置成功
			 *
			 */
			bool SetBlock(bool on);

			/**
			 *
			 * 设置是否开启Nagle算法
			 *
			 * @param on 状态
			 *
			 * @return 是否设置成功
			 *
			 */
			bool SetDelay(bool on);

			/**
			 *
			 * 设置是否触发中断信号
			 *
			 * @param on 状态
			 *
			 * @return 是否设置成功
			 *
			 */
			bool SetSignal(bool on);

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
			 * 绑定地址
			 *
			 * @param endpoint 端点
			 *
			 * @return 绑定结果
			 *
			 */
			int32_t Bind(const Endpoint & endpoint);

			/**
			 *
			 * 监听地址
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
			int32_t Accept(Context * context = nullptr);

			/**
			 *
			 * 连接
			 *
			 * @param endpoint 端点
			 * @param context 上下文
			 *
			 * @return 连接结果
			 *
			 */
			int32_t Connect(const Endpoint & endpoint, Context * context = nullptr);

			/**
			 *
			 * 发送
			 *
			 * @param buffer 内容
			 * @param length 长度
			 * @param context 上下文
			 *
			 * @return 发送字节数
			 *
			 */
			int32_t Send(void * buffer, std::size_t length, Context * context = nullptr);

			/**
			 *
			 * 接收
			 *
			 * @param buffer 内容
			 * @param length 长度
			 * @param context 上下文
			 *
			 * @return 接收字节数
			 *
			 */
			int32_t Receive(void * buffer, std::size_t length, Context * context = nullptr);

			/**
			 *
			 * 套接字
			 *
			 * @return 套接字
			 *
			 */
			TINY_TOOLKIT_SOCKET_TYPE Socket() const;

		protected:
			TINY_TOOLKIT_SOCKET_TYPE _socket{ TINY_TOOLKIT_SOCKET_INVALID };
		};

		class TINY_TOOLKIT_API TCPAdaptor : public Adaptor
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @return 构造函数
			 *
			 */
			TCPAdaptor();

			/**
			 *
			 * 析构函数
			 *
			 */
			~TCPAdaptor() override = default;
		};

		class TINY_TOOLKIT_API UDPAdaptor : public Adaptor
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @return 构造函数
			 *
			 */
			UDPAdaptor();

			/**
			 *
			 * 析构函数
			 *
			 */
			~UDPAdaptor() override = default;
		};
	}
}


#endif // __TINY_TOOLKIT__NET__ADAPTOR__H__
