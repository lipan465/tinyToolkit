/**
 *
 *  作者: hm
 *
 *  说明: 逻辑模块
 *
 */


#include "module.h"


namespace tinyToolkit
{
	/**
	 *
	 * 设置名称
	 *
	 * @param name 名称
	 *
	 */
	void ILogicModule::SetName(std::string name)
	{
		_name = std::move(name);
	}

	/**
	 *
	 * 设置下一个模块
	 *
	 * @param module 下一个模块
	 *
	 */
	void ILogicModule::SetNext(ILogicModule * module)
	{
		_next = module;
	}

	/**
	 *
	 * 下一个模块
	 *
	 * @return 模块
	 *
	 */
	ILogicModule * ILogicModule::Next() const
	{
		return _next;
	}

	/**
	 *
	 * 名称
	 *
	 * @return 名称
	 *
	 */
	const std::string & ILogicModule::Name() const
	{
		return _name;
	}
}
