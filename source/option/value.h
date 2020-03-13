#ifndef __TINY_TOOLKIT__OPTION__VALUE__H__
#define __TINY_TOOLKIT__OPTION__VALUE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 内容
 *
 */


#include "traits.h"

#include "../container/any.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <string>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#
#
#endif


namespace tinyToolkit
{
	namespace option
	{
		class TINY_TOOLKIT_API SemanticValue
		{
		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			virtual ~SemanticValue() = default;

			/**
			 *
			 * 是否有数据
			 *
			 * @return 是否有数据
			 *
			 */
			virtual bool HasValue() = 0;

			/**
			 *
			 * 是否有默认值
			 *
			 * @return 是否有默认值
			 *
			 */
			virtual bool HasDefault() = 0;

			/**
			 *
			 * 类型
			 *
			 * @return 类型
			 *
			 */
			virtual OPTION_TYPE Type() = 0;

			/**
			 *
			 * 内容
			 *
			 * @return 内容
			 *
			 */
			virtual std::string Content() = 0;
		};

		template<class ValueTypeT>
		class TINY_TOOLKIT_API RealValue : public SemanticValue
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 */
			RealValue() : _type(Traits<ValueTypeT>::type)
			{

			}

			/**
			 *
			 * 移动构造函数
			 *
			 * @param rhs 右值对象
			 *
			 */
			RealValue(RealValue && rhs) noexcept : _value(std::move(rhs._value)),
			                                       _default(std::move(rhs._default)),
			                                       _type(rhs._type)
			{

			}

			/**
			 *
			 * 拷贝构造函数
			 *
			 * @param lhs 左值对象
			 *
			 */
			RealValue(const RealValue & lhs) : _value(lhs._value),
			                                   _default(lhs._default),
			                                   _type(lhs._type)
			{

			}

			/**
			 *
			 * 构造函数
			 *
			 * @param value 默认数据
			 *
			 */
			explicit RealValue(ValueTypeT && value) : _value(std::forward<ValueTypeT>(value)),
			                                          _default(std::forward<ValueTypeT>(value)),
			                                          _type(Traits<ValueTypeT>::type)
			{

			}

			/**
			 *
			 * 析构函数
			 *
			 */
			~RealValue() override = default;

			/**
			 *
			 * 是否有数据
			 *
			 * @return 是否有数据
			 *
			 */
			bool HasValue() override
			{
				return _value.IsValid();
			}

			/**
			 *
			 * 是否有默认值
			 *
			 * @return 是否有默认值
			 *
			 */
			bool HasDefault() override
			{
				return _default.IsValid();
			}

			/**
			 *
			 * 类型
			 *
			 * @return 类型
			 *
			 */
			OPTION_TYPE Type() override
			{
				return _type;
			}

			/**
			 *
			 * 内容
			 *
			 * @return 内容
			 *
			 */
			std::string Content() override
			{
				return ToString<ValueTypeT>(_default.Get<ValueTypeT>());
			}

		private:
			/**
			 *
			 * 转换字符串
			 *
			 * @param value 处理内容
			 *
			 * @return 转换后内容
			 *
			 */
			template<typename TypeT>
			typename std::enable_if
			<
				std::is_same<TypeT, bool>::value, std::string
			>::type
			ToString(TypeT value)
			{
				return value ? "true" : "false";
			}

			/**
			 *
			 * 转换字符串
			 *
			 * @param value 处理内容
			 *
			 * @return 转换后内容
			 *
			 */
			template<typename TypeT>
			typename std::enable_if
			<
				std::is_same<TypeT, float>::value ||
				std::is_same<TypeT, double>::value ||
				std::is_same<TypeT, long double>::value ||
				std::is_same<TypeT, int8_t>::value ||
				std::is_same<TypeT, uint8_t>::value ||
				std::is_same<TypeT, int16_t>::value ||
				std::is_same<TypeT, uint16_t>::value ||
				std::is_same<TypeT, int32_t>::value ||
				std::is_same<TypeT, uint32_t>::value ||
				std::is_same<TypeT, int64_t>::value ||
				std::is_same<TypeT, uint64_t>::value,
				std::string
			>::type
			ToString(TypeT value)
			{
				return std::to_string(value);
			}

			/**
			 *
			 * 转换字符串
			 *
			 * @param value 处理内容
			 *
			 * @return 转换后内容
			 *
			 */
			template<typename TypeT>
			typename std::enable_if
			<
				std::is_same<TypeT, std::string>::value, std::string
			>::type
			ToString(TypeT value)
			{
				return value;
			}

		public:
			container::Any _value{ };
			container::Any _default{ };

			OPTION_TYPE _type{ OPTION_TYPE::UNKNOWN };
		};

		template<typename ValueTypeT>
		std::shared_ptr<RealValue<ValueTypeT>> Value()
		{
			return std::make_shared<RealValue<ValueTypeT>>();
		}

		template<typename ValueTypeT>
		std::shared_ptr<RealValue<ValueTypeT>> Value(ValueTypeT && value)
		{
			return std::make_shared<RealValue<ValueTypeT>>(std::forward<ValueTypeT>(value));
		}
	}
}


#endif // __TINY_TOOLKIT__OPTION__VALUE__H__
