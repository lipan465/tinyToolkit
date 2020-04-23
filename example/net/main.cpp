/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"

#include "pool/task.h"
#include "pool/object.h"

#include "util/time.h"
#include "util/string.h"


class TCPServer : public tinyToolkit::net::ITCPServer
{
	class Session : public tinyToolkit::net::ITCPSession
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		~Session() override = default;

	private:
		/**
		 *
		 * 接收数据
		 *
		 * @param buffer 内容
		 * @param length 长度
		 *
		 * @return 偏移长度
		 *
		 */
		std::size_t OnReceive(const char * buffer, std::size_t length) override
		{
			Send(buffer, length);

			return length;
		}
	};

public:
	/**
	 *
	 * 析构函数
	 *
	 */
	~TCPServer() override = default;

private:
	/**
	 *
	 * 事件错误
	 *
	 */
	void OnError() override
	{
		util::String::Print("Service [{}:{}] error\r\n", LocalEndpoint().host, LocalEndpoint().port);
	}

	/**
	 *
	 * 关闭连接
	 *
	 */
	void OnShutdown() override
	{
		util::String::Print("Service [{}:{}] shutdown\r\n", LocalEndpoint().host, LocalEndpoint().port);

		_pool.Release();
	}

	/**
	 *
	 * 接收会话
	 *
	 * @return 会话对象
	 *
	 */
	tinyToolkit::net::ITCPSession * OnAccept() override
	{
		auto session = _pool.Borrow();

		session->SetReceiveCacheSize(1024);

		return session;
	}

private:
	pool::ObjectPool<TCPServer::Session> _pool{ };
};


class UDPServer : public tinyToolkit::net::IUDPServer
{
	class Session : public tinyToolkit::net::IUDPSession
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		~Session() override = default;

	private:
		/**
		 *
		 * 接收数据
		 *
		 * @param buffer 内容
		 * @param length 长度
		 *
		 * @return 偏移长度
		 *
		 */
		std::size_t OnReceive(const char * buffer, std::size_t length) override
		{
			Send(buffer, length);

			return length;
		}
	};

public:
	/**
	 *
	 * 析构函数
	 *
	 */
	~UDPServer() override = default;

private:
	/**
	 *
	 * 事件错误
	 *
	 */
	void OnError() override
	{
		util::String::Print("Service [{}:{}] error\r\n", LocalEndpoint().host, LocalEndpoint().port);
	}

	/**
	 *
	 * 关闭连接
	 *
	 */
	void OnShutdown() override
	{
		util::String::Print("Service [{}:{}] shutdown\r\n", LocalEndpoint().host, LocalEndpoint().port);

		_pool.Release();
	}

	/**
	 *
	 * 接收会话
	 *
	 * @return 会话对象
	 *
	 */
	tinyToolkit::net::IUDPSession * OnAccept() override
	{
		auto session = _pool.Borrow();

		session->SetReceiveCacheSize(1024);

		return session;
	}

private:
	pool::ObjectPool<UDPServer::Session> _pool{ };
};


class TCPSession : public tinyToolkit::net::ITCPSession
{
public:
	/**
	 *
	 * 析构函数
	 *
	 */
	~TCPSession() override = default;

private:
	/**
	 *
	 * 事件错误
	 *
	 */
	void OnError() override
	{
		util::String::Print("Session [{}:{}] error\r\n", LocalEndpoint().host, LocalEndpoint().port);
	}

	/**
	 *
	 * 会话连接
	 *
	 */
	void OnConnect() override
	{
		util::String::Print
		(
			"Session [{}:{}] connect [{}:{}]\r\n",
			LocalEndpoint().host,
			LocalEndpoint().port,
			PeerEndpoint().host,
			PeerEndpoint().port
		);

		Send("Hello TCP", 9);

		_sTime = util::Time::Milliseconds();
	}

	/**
	 *
	 * 接收数据
	 *
	 * @param buffer 内容
	 * @param length 长度
	 *
	 * @return 偏移长度
	 *
	 */
	std::size_t OnReceive(const char * buffer, std::size_t length) override
	{
		_length += length;

		if (++_count == 10000)
		{
			_eTime = util::Time::Milliseconds();
			_inter = _eTime - _sTime;

			util::String::Print
			(
				"Session [{}:{}] receive [{}:{}] count {}, length {}, time use {}ms, qps {}\n",
				LocalEndpoint().host,
				LocalEndpoint().port,
				PeerEndpoint().host,
				PeerEndpoint().port,
				_count,
				_length,
				_inter,
				(_count * 1000) / _inter
			);

			Close();
		}

		Send(buffer, length);

		return length;
	}

private:
	std::time_t _sTime{ 0 };
	std::time_t _eTime{ 0 };
	std::time_t _inter{ 0 };

	std::size_t _count{ 0 };
	std::size_t _length{ 0 };
};


class UDPSession : public tinyToolkit::net::IUDPSession
{
public:
	/**
	 *
	 * 析构函数
	 *
	 */
	~UDPSession() override = default;

private:
	/**
	 *
	 * 事件错误
	 *
	 */
	void OnError() override
	{
		util::String::Print("Session [{}:{}] error\r\n", LocalEndpoint().host, LocalEndpoint().port);
	}

	/**
	 *
	 * 会话连接
	 *
	 */
	void OnConnect() override
	{
		util::String::Print
		(
			"Session [{}:{}] connect [{}:{}]\r\n",
			LocalEndpoint().host,
			LocalEndpoint().port,
			PeerEndpoint().host,
			PeerEndpoint().port
		);

		Send("Hello UDP", 9);

		_sTime = util::Time::Milliseconds();
	}

	/**
	 *
	 * 接收数据
	 *
	 * @param buffer 内容
	 * @param length 长度
	 *
	 * @return 偏移长度
	 *
	 */
	std::size_t OnReceive(const char * buffer, std::size_t length) override
	{
		_length += length;

		if (++_count == 10000)
		{
			_eTime = util::Time::Milliseconds();
			_inter = _eTime - _sTime;

			util::String::Print
			(
				"Session [{}:{}] Receive [{}:{}] count {}, length {}, time use {}ms, qps {}\n",
				LocalEndpoint().host,
				LocalEndpoint().port,
				PeerEndpoint().host,
				PeerEndpoint().port,
				_count,
				_length,
				_inter,
				(_count * 1000) / _inter
			);

			Close();
		}

		Send(buffer, length);

		return length;
	}

private:
	std::time_t _sTime{ 0 };
	std::time_t _eTime{ 0 };
	std::time_t _inter{ 0 };

	std::size_t _count{ 0 };
	std::size_t _length{ 0 };
};


static void TCP()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		TCPServer server{ };

		if (!server.Listen("0.0.0.0", 10080))
		{
			util::String::Print
			(
				"launch server listen to [{}:{}] failed\r\n",
				server.LocalEndpoint().host,
				server.LocalEndpoint().port
			);
		}

		pool::TaskPool pool{ };

		pool.Launch();

		for (std::size_t i = 0; i < pool.ThreadSize(); ++i)
		{
			pool.AddTask([]()
	        {
		         TCPSession session{ };

		         session.SetReceiveCacheSize(1024);

	             if (!session.Connect("127.0.0.1", 10080))
	             {
		             util::String::Print
		             (
			             "launch session connect to [{}:{}] failed\r\n",
			             session.PeerEndpoint().host,
			             session.PeerEndpoint().port
		             );
	             }

	             while (session.IsValid())
	             {
		             std::this_thread::sleep_for(std::chrono::milliseconds(500));
	             }

	             session.Close();
	        });
		}

		pool.Wait();

		server.Close();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void UDP()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << TINY_TOOLKIT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
#if TINY_TOOLKIT_PLATFORM != TINY_TOOLKIT_PLATFORM_WINDOWS

		UDPServer server{ };

		if (!server.Listen("0.0.0.0", 10080))
		{
			util::String::Print
			(
				"launch server listen to [{}:{}] failed\r\n",
				server.LocalEndpoint().host,
				server.LocalEndpoint().port
			);
		}

		pool::TaskPool pool{ };

		pool.Launch();

		for (std::size_t i = 0; i < pool.ThreadSize(); ++i)
		{
			pool.AddTask([]()
		    {
				UDPSession session;

			    session.SetReceiveCacheSize(1024);

				if (!session.Connect("127.0.0.1", 10080))
				{
					util::String::Print
					(
						"launch session connect to [{}:{}] failed\r\n",
						session.PeerEndpoint().host,
						session.PeerEndpoint().port
					);
				}

			    while (session.IsValid())
			    {
				    std::this_thread::sleep_for(std::chrono::milliseconds(500));
			    }

			    session.Close();
		    });
		}

		pool.Wait();

		server.Close();

#endif
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


int main(int argc, char const * argv[])
{
	(void)argc;
	(void)argv;

	TCP();
	UDP();

	return 0;
}
