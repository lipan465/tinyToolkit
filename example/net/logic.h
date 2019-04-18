#ifndef __EXAMPLE__NET__LOGIC__H__
#define __EXAMPLE__NET__LOGIC__H__


/**
 *
 *  作者: hm
 *
 *  说明: 逻辑
 *
 */


#include "tcp.h"
#include "udp.h"


class Logic
{
public:
	/**
	 *
	 * 启动
	 *
	 */
	static void Launch();

	/**
	 *
	 * 启动tcp客户端
	 *
	 * @param remoteHost 远端地址
	 * @param remotePort 远端端口
	 * @param clientCount 客户端个数
	 *
	 */
	static void LaunchTCPClient(const std::string & remoteHost, uint16_t remotePort, uint32_t clientCount);

	/**
	 *
	 * 启动tcp服务器
	 *
	 * @param localHost 主机地址
	 * @param localPort 主机端口
	 *
	 */
	static void LaunchTCPServer(const std::string & localHost, uint16_t localPort);

	/**
	 *
	 * 启动udp客户端
	 *
	 * @param localHost 主机地址
	 * @param localPort 主机端口
	 * @param remoteHost 远端地址
	 * @param remotePort 远端端口
	 * @param clientCount 客户端个数
	 *
	 */
	static void LaunchUDPClient(const std::string & localHost, uint16_t localPort, const std::string & remoteHost, uint16_t remotePort, uint32_t clientCount);
};


#endif // __EXAMPLE__NET__LOGIC__H__
