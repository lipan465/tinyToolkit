#ifndef __TINY_TOOLKIT__UTILITIES__LOCK__H__
#define __TINY_TOOLKIT__UTILITIES__LOCK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 锁处理
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API FakeMutex
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		FakeMutex() = default;

		/**
		 *
		 * 析构函数
		 *
		 */
		~FakeMutex() = default;

		/**
		 *
		 * 锁定
		 *
		 */
		void lock()
		{

		}

		/**
		 *
		 * 解锁
		 *
		 */
		void unlock()
		{

		}

		/**
		 *
		 * 尝试锁定
		 *
		 * @return 是否锁定成功
		 *
		 */
		bool try_lock()
		{
			return true;
		}

		/**
		 *
		 * 尝试锁定一段时间
		 *
		 * @tparam TypeT [all time types]
		 *
		 * @param time 待锁定的时间
		 *
		 * @return 是否锁定成功
		 *
		 */
		template <typename TypeT>
		bool try_lock_for(const TypeT & time)
		{
			return true;
		}

		/**
		 *
		 * 尝试锁定到某个时间
		 *
		 * @tparam TypeT [all time types]
		 *
		 * @param time 锁定到期的时间
		 *
		 * @return 是否锁定成功
		 *
		 */
		template <typename TypeT>
		bool try_lock_util(const TypeT & time)
		{
			return true;
		}
	};

	template <typename TypeT>
	class TINY_TOOLKIT_API FakeAtomic
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		FakeAtomic() = default;

		/**
		 *
		 * 析构函数
		 *
		 */
		~FakeAtomic() = default;

		/**
		 *
		 * 构造函数
		 *
		 * @param val 初始值
		 *
		 */
		explicit FakeAtomic(TypeT val) : value(val)
		{

		}

		/**
		 *
		 * 赋值
		 *
		 * @param val 待赋值数据
		 *
		 */
		void store(TypeT val)
		{
			value = val;
		}

		/**
		 *
		 * 取值
		 *
		 * @return 获取的数据
		 *
		 */
		TypeT load(std::memory_order = std::memory_order_seq_cst) const
		{
			return value;
		}

	public:
		TypeT value{ };
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__LOCK__H__
