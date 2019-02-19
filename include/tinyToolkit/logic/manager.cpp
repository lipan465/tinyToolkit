/**
 *
 *  作者: hm
 *
 *  说明: 逻辑模块管理器
 *
 */


#include "manager.h"

#include "../system/os.h"
#include "../debug/trace.h"
#include "../utilities/operator.h"


namespace tinyToolkit
{
	/**
	 *
	 * 析构函数
	 *
	 */
	LogicModuleManager::~LogicModuleManager()
	{
		Release();
	}

	/**
	 *
	 * 启动
	 *
	 */
	void LogicModuleManager::Launch()
	{
		for (auto &iter : _moduleManager)
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
		if (_isLoad)
		{
			_isLoad = false;

			for (auto &iter : _moduleManager)
			{
				iter.second->Release();

				delete iter.second;
			}

			for (auto &iter : _handleManager)
			{
#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

				::FreeLibrary(iter.second);

#else

				dlclose(iter.second);

#endif
			}

			Operator::Clear(_moduleManager);
			Operator::Clear(_handleManager);
		}
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

		HINSTANCE handle = ::LoadLibrary(path);

		if (!handle)
		{
			TINY_TOOLKIT_ASSERT(false, "Load [{}] : {}", path, OS::LastErrorMessage())

			return false;
		}

		auto function = reinterpret_cast<ILogicModule * (*)()>(::GetProcAddress(handle, "GetModules"));

#else

		void * handle = dlopen(path, RTLD_LAZY);

		if (handle == nullptr)
		{
			TINY_TOOLKIT_ASSERT(false, "Load [{}] : {}", path, dlerror())

			return false;
		}

		auto function = reinterpret_cast<ILogicModule * (*)()>(dlsym(handle, "GetModules"));

#endif

		ILogicModule * logic = function();

		while (logic)
		{
			auto find = _moduleManager.find(logic->Name());

			if (find == _moduleManager.end())
			{
				_moduleManager.insert(std::make_pair(logic->Name(), logic));

				logic->Initialize();

				logic = logic->Next();
			}
		}

		_isLoad = true;

		_handleManager.insert(std::make_pair(path, handle));

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
		auto find = _moduleManager.find(name);

		if (find == _moduleManager.end() || find->second == nullptr)
		{
			return nullptr;
		}

		return find->second;
	}
}
