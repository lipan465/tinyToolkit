/**
 *
 *  作者: hm
 *
 *  说明: 模块
 *
 */


#include "module.h"


namespace tinyToolkit
{
	/**
	 *
	 * 下一个模块
	 *
	 * @return 模块
	 *
	 */
	ILogicModule * ILogicModule::Next()
	{
		return _next;
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
	const std::string & ILogicModule::Name()
	{
		return _name;
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
