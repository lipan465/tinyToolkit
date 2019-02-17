#ifndef __TINY_TOOLKIT__LOGIC__MODULE__H__
#define __TINY_TOOLKIT__LOGIC__MODULE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 模块
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API ILogicModule
	{
	public:
		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~ILogicModule() = default;

		/**
		 *
		 * 启动
		 *
		 * @return 是否启动成功
		 *
		 */
		virtual bool Launch() = 0;

		/**
		 *
		 * 释放
		 *
		 * @return 是否释放成功
		 *
		 */
		virtual bool Release() = 0;

		/**
		 *
		 * 初始化
		 *
		 * @return 是否初始化成功
		 *
		 */
		virtual bool Initialize() = 0;

		/**
		 *
		 * 设置名称
		 *
		 * @param name 名称
		 *
		 */
		void SetName(std::string name);

		/**
		 *
		 * 设置下一个模块
		 *
		 * @param module 下一个模块
		 *
		 */
		void SetNext(ILogicModule * module);

		/**
		 *
		 * 下一个模块
		 *
		 * @return 模块
		 *
		 */
		ILogicModule * Next();

		/**
		 *
		 * 下一个模块
		 *
		 * @return 模块
		 *
		 */
		ILogicModule * Next() const;

		/**
		 *
		 * 名称
		 *
		 * @return 名称
		 *
		 */
		const std::string & Name();

		/**
		 *
		 * 名称
		 *
		 * @return 名称
		 *
		 */
		const std::string & Name() const;

	protected:
		std::string _name{ };

		ILogicModule * _next{ nullptr };
	};
}


#endif // __TINY_TOOLKIT__LOGIC__MODULE__H__
