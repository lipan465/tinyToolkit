/**
 *
 *  作者: hm
 *
 *  说明: 通讯会话
 *
 */


#include "session.h"
#include "manager.h"


namespace tinyToolkit
{
	/**
	 *
	 * 启动
	 *
	 * @param host 主机地址
	 * @param port 主机端口
	 *
	 * @return 是否启动成功
	 *
	 */
	bool ITCPSession::Launch(const std::string & host, uint16_t port)
	{
		return NetWorkManager::Instance().LaunchTCPClient(this, host, port);
	}

	/**
	 *
	 * 关闭会话
	 *
	 */
	void ITCPSession::Close()
	{
		if (_pipe)
		{
			_pipe->Close();
		}
	}

	/**
	 *
	 * 发送数据
	 *
	 * @param value 待发送数据
	 * @param size 待发送数据长度
	 *
	 */
	void ITCPSession::Send(const void * value, std::size_t size)
	{
		if (_pipe)
		{
			_pipe->Send(value, size);
		}
	}
}
