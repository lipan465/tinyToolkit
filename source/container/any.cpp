/**
 *
 *  作者: hm
 *
 *  说明: 任意
 *
 */


#include "any.h"


namespace tinyToolkit
{
	namespace container
	{
		/**
		 *
		 * 移动构造函数
		 *
		 * @param rhs 右值对象
		 *
		 */
		Any::Any(Any && rhs) noexcept : _content(std::move(rhs._content))
		{

		}

		/**
		 *
		 * 拷贝构造函数
		 *
		 * @param lhs 左值对象
		 *
		 */
		Any::Any(const Any & lhs) : _content(lhs._content ? lhs._content->Clone() : nullptr)
		{

		}

		/**
		 *
		 * 清理
		 *
		 */
		void Any::Clear()
		{
			_content.reset();
		}

		/**
		 *
		 * 是否有效
		 *
		 * @return 是否有效
		 *
		 */
		bool Any::IsValid()
		{
			return _content ? true : false;
		}

		/**
		 *
		 * 类型
		 *
		 * @return 类型
		 *
		 */
		const std::type_info & Any::Type()
		{
			return _content ? _content->Type() : typeid(void);
		}

		/**
		 *
		 * 重载=操作
		 *
		 * @param rhs 右值对象
		 *
		 * @return 对象
		 *
		 */
		Any & Any::operator=(Any && rhs) noexcept
		{
			_content = std::move(rhs._content);

			return *this;
		}

		/**
		 *
		 * 重载=操作
		 *
		 * @param lhs 左值对象
		 *
		 * @return 对象
		 *
		 */
		Any & Any::operator=(const Any & lhs)
		{
			if (_content != lhs._content)
			{
				_content = lhs._content ? lhs._content->Clone() : nullptr;
			}

			return *this;
		}
	}
}
