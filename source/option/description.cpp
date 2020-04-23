/**
 *
 *  作者: hm
 *
 *  说明: 描述
 *
 */


#include "description.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <stdexcept>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <cstring>
#
#endif


namespace tinyToolkit
{
	namespace option
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param name 名称
		 * @param info 信息
		 *
		 */
		DescriptionInfo::DescriptionInfo(const char * name, const char * info) : _info(info)
		{
			SetName(name);

			_isValid = false;
		}

		/**
		 *
		 * 构造函数
		 *
		 * @param name 名称
		 * @param mode 方式
		 * @param info 信息
		 * @param value 数据
		 *
		 */
		DescriptionInfo::DescriptionInfo(const char * name, const char * mode, const char * info, std::shared_ptr<SemanticValue> value) : _info(info),
		                                                                                                                                  _mode(mode),
		                                                                                                                                  _value(std::move(value))
		{
			SetName(name);

			_isValid = _value->HasValue();
		}

		/**
		 *
		 * 更新
		 *
		 */
		void DescriptionInfo::Update()
		{
			_isValid = true;
		}

		/**
		 *
		 * 更新
		 *
		 * @param value 数值
		 *
		 */
		void DescriptionInfo::Update(const std::string & value)
		{
			switch (_value->Type())
			{
				case OPTION_TYPE::BOOL:
				{
					dynamic_cast<RealValue<bool> *>(_value.get())->_value = value == "true" ? true : false;

					break;
				}

				case OPTION_TYPE::FLOAT:
				{
					dynamic_cast<RealValue<float> *>(_value.get())->_value = strtof(value.c_str(), nullptr);

					break;
				}

				case OPTION_TYPE::DOUBLE:
				{
					dynamic_cast<RealValue<double> *>(_value.get())->_value = strtod(value.c_str(), nullptr);

					break;
				}

				case OPTION_TYPE::LONG_DOUBLE:
				{
					dynamic_cast<RealValue<long double> *>(_value.get())->_value = strtold(value.c_str(), nullptr);

					break;
				}

				case OPTION_TYPE::INT8:
				{
					dynamic_cast<RealValue<int8_t> *>(_value.get())->_value = static_cast<int8_t>(strtol(value.c_str(), nullptr, 10));

					break;
				}

				case OPTION_TYPE::UINT8:
				{
					dynamic_cast<RealValue<uint8_t> *>(_value.get())->_value = static_cast<uint8_t>(strtoul(value.c_str(), nullptr, 10));

					break;
				}

				case OPTION_TYPE::INT16:
				{
					dynamic_cast<RealValue<int16_t> *>(_value.get())->_value = static_cast<int16_t>(strtol(value.c_str(), nullptr, 10));

					break;
				}

				case OPTION_TYPE::UINT16:
				{
					dynamic_cast<RealValue<uint16_t> *>(_value.get())->_value = static_cast<uint16_t>(strtoul(value.c_str(), nullptr, 10));

					break;
				}

				case OPTION_TYPE::INT32:
				{
					dynamic_cast<RealValue<int32_t> *>(_value.get())->_value = static_cast<int32_t>(strtol(value.c_str(), nullptr, 10));

					break;
				}

				case OPTION_TYPE::UINT32:
				{
					dynamic_cast<RealValue<uint32_t> *>(_value.get())->_value = static_cast<uint32_t>(strtoul(value.c_str(), nullptr, 10));

					break;
				}

				case OPTION_TYPE::INT64:
				{
					dynamic_cast<RealValue<int64_t> *>(_value.get())->_value = static_cast<int64_t>(strtoll(value.c_str(), nullptr, 10));

					break;
				}

				case OPTION_TYPE::UINT64:
				{
					dynamic_cast<RealValue<uint64_t> *>(_value.get())->_value = static_cast<uint64_t>(strtoull(value.c_str(), nullptr, 10));

					break;
				}

				case OPTION_TYPE::STRING:
				{
					dynamic_cast<RealValue<std::string> *>(_value.get())->_value = value;

					break;
				}

				case OPTION_TYPE::UNKNOWN:

				default:
				{
					throw std::runtime_error("unknown option type");

					break;
				}
			}

			_isValid = true;
		}

		/**
		 *
		 * 是否有效
		 *
		 * @return 是否有效
		 *
		 */
		bool DescriptionInfo::IsValid() const
		{
			return _isValid;
		}

		/**
		 *
		 * 是否需要数据
		 *
		 * @return 是否需要数据
		 *
		 */
		bool DescriptionInfo::IsRequired() const
		{
			return _value ? true : false;
		}

		/**
		 *
		 * 信息
		 *
		 * @return 信息
		 *
		 */
		const std::string & DescriptionInfo::Info() const
		{
			return _info;
		}

		/**
		 *
		 * 方式
		 *
		 * @return 方式
		 *
		 */
		const std::string & DescriptionInfo::Mode() const
		{
			return _mode;
		}

		/**
		 *
		 * 名称
		 *
		 * @return 名称
		 *
		 */
		const std::string & DescriptionInfo::LongName() const
		{
			return _longName;
		}

		/**
		 *
		 * 名称
		 *
		 * @return 名称
		 *
		 */
		const std::string & DescriptionInfo::ShortName() const
		{
			return _shortName;
		}

		/**
		 *
		 * 名称
		 *
		 * @return 名称
		 *
		 */
		const std::string & DescriptionInfo::OptionName() const
		{
			return _optionName;
		}

		/**
		 *
		 * 数据
		 *
		 * @return 数据
		 *
		 */
		const std::shared_ptr<SemanticValue> & DescriptionInfo::Value() const
		{
			return _value;
		}

		/**
		 *
		 * 设置名称
		 *
		 * @param name 名称
		 *
		 */
		void DescriptionInfo::SetName(const char * name)
		{
			if (name == nullptr || name[0] == '\0')
			{
				throw std::runtime_error("No option names were specified");
			}

			auto len = strlen(name);
			auto ptr = strstr(name, ",");

			if (ptr)
			{
				auto pre = ptr - name;
				auto sub = len - pre - 1;

				_longName.append(name, pre);
				_shortName.append(ptr + 1, sub);
			}
			else
			{
				_longName.append(name, len);
			}

			if (_shortName.empty())
			{
				if (!_longName.empty())
				{
					_optionName.append("--").append(_longName);
				}
			}
			else
			{
				_optionName.append("-").append(_shortName);

				if (!_longName.empty())
				{
					_optionName.append(", --").append(_longName);
				}
			}
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 *
		 * 构造函数
		 *
		 * @param caption 标题
		 *
		 */
		DescriptionGroup::DescriptionGroup(const char * caption) : _caption(caption)
		{

		}

		/**
		 *
		 * 添加选项
		 *
		 * @return 对象
		 *
		 */
		DescriptionGroup & DescriptionGroup::AddOption()
		{
			return *this;
		}

		/**
		 *
		 * 标题
		 *
		 * @return 标题
		 *
		 */
		const std::string & DescriptionGroup::Caption() const
		{
			return _caption;
		}

		/**
		 *
		 * 选项组
		 *
		 * @return 选项组
		 *
		 */
		const std::vector<std::shared_ptr<DescriptionInfo>> & DescriptionGroup::Options() const
		{
			return _options;
		}

		/**
		 *
		 * 重载()操作
		 *
		 * @param name 名称
		 * @param info 信息
		 *
		 * @return 对象
		 *
		 */
		DescriptionGroup & DescriptionGroup::operator()(const char * name, const char * info)
		{
			_options.push_back(std::make_shared<DescriptionInfo>(name, info));

			return *this;
		}

		/**
		 *
		 * 重载()操作
		 *
		 * @param name 名称
		 * @param mode 方式
		 * @param info 信息
		 * @param value 数据
		 *
		 * @return 对象
		 *
		 */
		DescriptionGroup & DescriptionGroup::operator()(const char * name, const char * mode, const char * info, const std::shared_ptr<SemanticValue> & value)
		{
			_options.push_back(std::make_shared<DescriptionInfo>(name, mode, info, value));

			return *this;
		}
	}
}
