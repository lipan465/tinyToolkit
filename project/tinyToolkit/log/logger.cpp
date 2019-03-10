/**
 *
 *  作者: hm
 *
 *  说明: 日志管理器
 *
 */


#include "logger.h"

#include "../system/application.h"


namespace tinyToolkit
{
	/**
	 *
	 * 构造函数
	 *
	 */
	ILogger::ILogger() : _name(Application::Name())
	{

	}

	/**
	 *
	 * 构造函数
	 *
	 * @param name 日志名称
	 *
	 */
	ILogger::ILogger(std::string name) : _name(std::move(name))
	{

	}

	/**
	 *
	 * 删除节点
	 *
	 * @param name 节点名称
	 *
	 */
	void ILogger::DelSink(const std::string & name)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		_container.erase(name);
	}

	/**
	 *
	 * 删除节点
	 *
	 * @param sink 节点对象
	 *
	 */
	void ILogger::DelSink(const std::shared_ptr<ILogSink> & sink)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		_container.erase(sink->Name());
	}

	/**
	 *
	 * 关闭节点
	 *
	 */
	void ILogger::CloseSink()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		for (auto &iter : _container)
		{
			iter.second->Close();
		}
	}

	/**
	 *
	 * 刷新节点
	 *
	 */
	void ILogger::FlushSink()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		for (auto &iter : _container)
		{
			iter.second->Flush();
		}
	}

	/**
	 *
	 * 重新打开节点
	 *
	 */
	void ILogger::ReopenSink()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		for (auto &iter : _container)
		{
			iter.second->Reopen();
		}
	}

	/**
	 *
	 * 设置日志节点自动刷新
	 *
	 */
	void ILogger::EnableSinkAutoFlush()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		for (auto &iter : _container)
		{
			iter.second->EnableAutoFlush();
		}
	}

	/**
	 *
	 * 禁止日志节点自动刷新
	 *
	 */
	void ILogger::DisableSinkAutoFlush()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		for (auto &iter : _container)
		{
			iter.second->DisableAutoFlush();
		}
	}

	/**
	 *
	 * 消息名称
	 *
	 * @return 消息名称
	 *
	 */
	const std::string & ILogger::Name()
	{
		return _name;
	}

	/**
	 *
	 * 日志节点
	 *
	 * @param name 节点名称
	 *
	 * @return 日志节点
	 *
	 */
	const std::shared_ptr<ILogSink> & ILogger::FindSink(const std::string & name)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		static std::shared_ptr<ILogSink> nullSink = nullptr;

		auto iter = _container.find(name);

		return iter == _container.end() ? nullSink : _container[name];
	}
}
