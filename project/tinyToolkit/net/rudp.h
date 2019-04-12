#ifndef __TINY_TOOLKIT__NET__R_UDP__H__
#define __TINY_TOOLKIT__NET__R_UDP__H__


/**
 *
 *  作者: hm
 *
 *  说明: 可靠udp
 *
 */


#include "ikcp.h"
#include "cache.h"
#include "server.h"
#include "message.h"

#include "../container/queue.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API RUDPSessionPipe : public IRUDPPipe
	{
		#define TINY_TOOLKIT_REQUEST_CONNECT_UUID_MESSAGE	"TINY_TOOLKIT_REQUEST_CONNECT_UUID:"
		#define TINY_TOOLKIT_RESPOND_CONNECT_UUID_MESSAGE	"TINY_TOOLKIT_RESPOND_CONNECT_UUID:"

		class UDPClientSession : public tinyToolkit::IUDPSession
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param session 会话对象指针
			 *
			 */
			explicit UDPClientSession(RUDPSessionPipe * pipe) : _pipe(pipe)
			{

			}

			/**
			 *
			 * 析构函数
			 *
			 */
			~UDPClientSession() override = default;

			/**
			 *
			 * 连接成功触发回调函数
			 *
			 */
			void OnConnect() override
			{

			}

			/**
			 *
			 * 断开连接触发回调函数
			 *
			 */
			void OnDisconnect() override
			{
				_pipe->_session->OnDisconnect();
			}

			/**
			 *
			 * 连接失败触发回调函数
			 *
			 */
			void OnConnectFailed() override
			{
				_pipe->_session->OnConnectFailed();
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
			std::size_t OnReceive(const char * data, std::size_t size) override
			{
				if (_pipe->_isConnectStage)
				{
					if (size >= sizeof(TINY_TOOLKIT_RESPOND_CONNECT_UUID_MESSAGE) &&
						memcmp(data, TINY_TOOLKIT_RESPOND_CONNECT_UUID_MESSAGE, sizeof(TINY_TOOLKIT_RESPOND_CONNECT_UUID_MESSAGE) - 1) == 0)
					{
						_pipe->_kcp = ikcp_create(strtol(data + sizeof(TINY_TOOLKIT_RESPOND_CONNECT_UUID_MESSAGE) - 1, nullptr, 10), (void *)_pipe);

						_pipe->_kcp->output = &RUDPSessionPipe::Output;

						/**
						 *
						 * 启动快速模式
						 *
						 * 第二个参数 nodelay 启用以后若干常规加速将启动
						 * 第三个参数 interval 为内部处理时钟, 默认设置为 10ms
						 * 第四个参数 resend 为快速重传指标, 设置为2
						 * 第五个参数 nc 为是否禁用常规流控, 这里禁止
						 *
						 */
						ikcp_nodelay(_pipe->_kcp, 1, 2, 1, 1);

						_pipe->_isConnect = true;
						_pipe->_isConnectStage = false;

						_pipe->_session->OnConnect();
					}
				}
				else if (_pipe->_isConnect)
				{
					if (ikcp_input(_pipe->_kcp, data, size) == 0)
					{
						static char buffer[TINY_TOOLKIT_SOCKET_TEMP_SIZE]{ 0 };

						bool status = false;

						while (true)
						{
							int32_t bytes = ikcp_recv(_pipe->_kcp, buffer, sizeof(buffer));

							if (bytes < 0)
							{
								break;
							}
							else
							{
								status = true;

								buffer[bytes] = '\0';

								if (_pipe->_cache.Push(buffer, bytes))
								{
									_pipe->_session->Close();

									return size;
								}
							}
						}

						if (status)
						{
							_pipe->_cache.Reduced(_pipe->_session->OnReceive(_pipe->_cache.Value(), _pipe->_cache.Length()));
						}
					}
				}

				return size;
			}

		private:
			RUDPSessionPipe * _pipe{ nullptr };
		};

	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param session 会话
		 *
		 */
		explicit RUDPSessionPipe(IRUDPSession * session);

		/**
		 *
		 * 启动
		 *
		 * @return 是否启动成功
		 *
		 */
		bool Launch();

		/**
		 *
		 * 关闭会话
		 *
		 */
		void Close() override;

		/**
		 *
		 * 发送数据
		 *
		 * @param data 待发送数据指针
		 * @param size 待发送数据长度
		 *
		 */
		void Send(const void * data, std::size_t size) override;

	private:
		/**
		 *
		 * app线程逻辑函数
		 *
		 */
		void ThreadProcess();

		/**
		 *
		 * kcp输出函数
		 *
		 * @param data 待发送的数据指针
		 * @param size 待发送的数据长度
		 * @param kcp 对象
		 * @param user 用户数据
		 *
		 * @return 调用状态
		 *
		 */
		static int32_t Output(const char * data, int32_t size, ikcpcb * kcp, void * user);

	private:
		bool _status{ false };
		bool _isConnect{ false };
		bool _isConnectStage{ false };

		ikcpcb * _kcp{ nullptr };

		NetCache _cache;

		std::thread _thread{ };

		IRUDPSession * _session{ nullptr };

		std::shared_ptr<IUDPSession> _udpSession{ };

		LockQueue<std::shared_ptr<NetMessage>> _sendQueue{ };
	};
}


#endif // __TINY_TOOLKIT__NET__R_UDP__H__
