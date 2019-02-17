/**
 *
 *  作者: hm
 *
 *  说明: 模块管理器
 *
 */


#include "manager.h"

#include "../debug/trace.h"


namespace tinyToolkit
{
	/**
	 *
	 * 启动
	 *
	 */
	void LogicModuleManager::Launch()
	{
		for (auto &iter : _manager)
		{
			iter.second->Launch();
		}
	}

	/**
	 *
	 * 释放
	 *
	 */
	void LogicModuleManager::Release()
	{
		for (auto &iter : _manager)
		{
			iter.second->Release();

			delete iter.second;
		}

		dlclose(_handle);
	}

	/**
	 *
	 * 加载模块
	 *
	 * @param path 模块路径
	 *
	 * @return 是否加载成功
	 *
	 */
	bool LogicModuleManager::LoadModule(const char * path)
	{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		_handle = ::LoadLibrary(path);

		if (!handle)
		{
			TINY_TOOLKIT_DEBUG("Load [{}] : {}", path, strerror(::GetLastError()))

			return false;
		}

		auto function = reinterpret_cast<ILogicModule * (*)()>(::GetProcAddress(handle, "GetModules"));

#else

		_handle = dlopen(path, RTLD_LAZY);

		if (_handle == nullptr)
		{
			TINY_TOOLKIT_DEBUG("Load [{}] : {}", path, dlerror())

			return false;
		}

		auto function = reinterpret_cast<ILogicModule * (*)()>(dlsym(_handle, "GetModules"));

#endif

		ILogicModule * logic = function();

		while (logic)
		{
			auto find = _manager.find(logic->Name());

			if (find == _manager.end())
			{
				_manager.insert(std::make_pair(logic->Name(), logic));

				logic->Initialize();

				logic = logic->Next();
			}
		}

		return true;
	}

	/**
	 *
	 * 查找模块
	 *
	 * @param name 名称
	 *
	 * @return 模块
	 *
	 */
	ILogicModule * LogicModuleManager::Find(const std::string & name)
	{
		auto find = _manager.find(name);

		if (find == _manager.end() || find->second == nullptr)
		{
			return nullptr;
		}

		return find->second;
	}
}
