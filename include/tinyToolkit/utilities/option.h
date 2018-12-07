#ifndef __TINY_TOOLKIT__UTILITIES__OPTION__H__
#define __TINY_TOOLKIT__UTILITIES__OPTION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 选项管理
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API OptionValue
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param description 描述
		 *
		 */
		explicit OptionValue(const char * description)
		{
			_isArg = false;

			if (description)
			{
				_description = description;
			}
		}

		/**
		 *
		 * 构造函数
		 *
		 * @param description 描述
		 * @param value 数值
		 *
		 */
		OptionValue(const char * description, const char * value) : OptionValue(description)
		{
			_isArg = true;

			if (value)
			{
				_value = value;

				_hasValue = true;
			}
		}

		/**
		 *
		 * 是否是参数
		 *
		 * @return 是否是参数
		 *
		 */
		bool IsArg() const
		{
			return _isArg;
		}

		/**
		 *
		 * 是否有数值
		 *
		 * @return 是否有数值
		 *
		 */
		bool HasValue() const
		{
			return _hasValue;
		}

		/**
		 *
		 * 数值
		 *
		 * @return 数值
		 *
		 */
		const std::string & Value() const
		{
			return _value;
		}

		/**
		 *
		 * 描述
		 *
		 * @return 描述
		 *
		 */
		const std::string & Description() const
		{
			return _description;
		}

	protected:
		bool _isArg{ false };
		bool _hasValue{ false };

		std::string _value{ };
		std::string _description{ };
	};

	class TINY_TOOLKIT_API OptionGroup
	{
	public:
		/**
		 *
		 * 添加选项
		 *
		 * @param option 选项
		 * @param description 描述
		 *
		 * @return 添加状态
		 *
		 */
		bool AddOption(const char * option, const char * description)
		{
			if (HasOption(option))
			{
				return false;
			}

			_options.insert(std::make_pair(option, std::make_shared<OptionValue>(description)));

			return true;
		}

		/**
		 *
		 * 添加选项
		 *
		 * @param option 选项
		 * @param description 描述
		 * @param value 数值
		 *
		 * @return 添加状态
		 *
		 */
		bool AddOption(const char * option, const char * description, const char * value)
		{
			if (HasOption(option))
			{
				return false;
			}

			_options.insert(std::make_pair(option, std::make_shared<OptionValue>(description, value)));

			return true;
		}

		/**
		 *
		 * 选项容器
		 *
		 * @return 选项容器
		 *
		 */
		const std::unordered_map<std::string, std::shared_ptr<OptionValue>> & Options() const
		{
			return _options;
		}

	protected:
		/**
		 *
		 * 是否存在选项
		 *
		 * @param option 待查找选项
		 *
		 * @return 是否存在
		 *
		 */
		bool HasOption(const char * option)
		{
			return _options.find(option) != _options.end();
		}

	protected:
		std::unordered_map<std::string, std::shared_ptr<OptionValue>> _options{ };
	};

	class TINY_TOOLKIT_API OptionManager
	{
	public:
		/**
		 *
		 * 是否为空
		 *
		 * @return 是否为空
		 *
		 */
		bool Empty()
		{
			return _parse.empty();
		}

		/**
		 *
		 * 是否存在选项
		 *
		 * @param option 待查询选项
		 *
		 * @return 是否存在
		 *
		 */
		bool Has(const char * option)
		{
			return _parse.find(option) != _parse.end();
		}

		/**
		 *
		 * 是否存在选项
		 *
		 * @param option 待查询选项
		 *
		 * @return 是否存在
		 *
		 */
		bool Has(const std::string & option)
		{
			return _parse.find(option) != _parse.end();
		}

		/**
		 *
		 * 定义选项
		 *
		 * @param option 选项
		 * @param description 描述
		 * @param groupName 组名
		 *
		 */
		void Define(const char * option, const char * description, const char * groupName = nullptr)
		{
			if (GetGroup(groupName ? groupName : "Allowed options")->AddOption(option, description))
			{
				UpdateAlignment(option, { });

				_options.insert(std::make_pair(option, std::make_shared<OptionValue>(description)));
			}
		}

		/**
		 *
		 * 定义参数选项
		 *
		 * @param option 选项
		 * @param description 描述
		 * @param value 数值
		 * @param groupName 组名
		 *
		 */
		void DefineArg(const char * option, const char * description, const char * value = nullptr, const char * groupName = nullptr)
		{
			if (GetGroup(groupName ? groupName : "Allowed options")->AddOption(option, description, value))
			{
				UpdateAlignment(option, value);

				_options.insert(std::make_pair(option, std::make_shared<OptionValue>(description, value)));
			}
		}

		/**
		 *
		 * 定义版本号
		 *
		 * @param version 版本号
		 *
		 */
		void DefineVersion(std::string version)
		{
			_version = std::move(version);
		}

		/**
		 *
		 * 解析选项
		 *
		 * @param argc 解析选项个数
		 * @param argv 解析选项流
		 *
		 */
		void Parse(int32_t argc, char ** argv)
		{
			Define("help",    "display help message",    "Help options");
			Define("version", "display version message", "Version options");

			std::string opt;
			std::string val;

			for (int32_t i = 1; i < argc; ++i)
			{
				const char * value = argv[i];

				if (strncmp(value, "--", 2) != 0)
				{
					throw std::invalid_argument("Arg invalid : " + std::string(value));
				}

				const char * find = strstr(value + 2, "=");

				if (find)
				{
					val.assign(find + 1);
					opt.assign(value + 2, find);
				}
				else
				{
					opt.assign(value + 2);
				}

				auto iter = _options.find(opt);

				if (_options.end() == iter)
				{
					throw std::invalid_argument("Arg invalid : " + std::string(value));
				}

				if (iter->second->IsArg())
				{
					if (find == nullptr)
					{
						throw std::invalid_argument("Arg need input : " + std::string(value));
					}

					_parse.insert(std::make_pair(opt, std::make_shared<OptionValue>(iter->second->Description().c_str(), val.c_str())));
				}
				else
				{
					_parse.insert(std::make_pair(opt, std::make_shared<OptionValue>(iter->second->Description().c_str())));
				}
			}

			ComposeDefaultOptions();

			if (Has("help"))
			{
				PrintHelp();
			}

			if (Has("version"))
			{
				PrintVersion();
			}
		}

		/**
		 *
		 * 获取数值
		 *
		 * @param option 待查找选项
		 *
		 * @return 数值
		 *
		 */
		const std::string & Get(const std::string & option)
		{
			if (!Has(option))
			{
				throw std::invalid_argument("Option invalid : " + option);
			}

			return _parse[option]->Value();
		};

	protected:
		/**
		 *
		 * 更新显示对齐
		 *
		 * @param option 对齐的选项
		 * @param value 对齐的数值
		 *
		 */
		void UpdateAlignment(const char * option, const char * value)
		{
			if (value)
			{
				std::size_t size = strlen(value);

				if (_valSize < size)
				{
					_valSize = size;
				}
			}

			if (option)
			{
				std::size_t size = strlen(option);

				if (_optSize < size)
				{
					_optSize = size;
				}
			}
		}

		/**
		 *
		 * 整合默认值选项
		 *
		 */
		void ComposeDefaultOptions()
		{
			for (auto &iter : _options)
			{
				if (!iter.second->IsArg() || !iter.second->HasValue() || Has(iter.first))
				{
					continue;
				}

				_parse.insert(iter);
			}
		}

		/**
		 *
		 * 输出帮助信息
		 *
		 */
		void PrintHelp()
		{
			for (auto &group : _groups)
			{
				std::cout << std::endl << group.first << ":" << std::endl;

				for (auto &iter : group.second->Options())
				{
					std::cout << std::setw(static_cast<int32_t>(_optSize)) << std::right << iter.first << "    ";

					if (iter.second->IsArg())
					{
						auto value = std::any_cast<std::string>(iter.second->Value());

						std::cout << "arg";

						if (value.empty())
						{
							std::cout << "  ";
						}
						else
						{
							std::cout << "(" << value << ")";
						}

						auto size = static_cast<int32_t>(_valSize - value.size());

						if (size > 0)
						{
							std::cout << std::setw(size) << std::right << " ";
						}
					}
					else
					{
						std::cout << std::setw(static_cast<int32_t>(_valSize + 5)) << std::right << " ";
					}

					std::cout << "    " << iter.second->Description() << std::endl;
				}
			}

			std::cout << std::endl;

			exit(0);
		}

		/**
		 *
		 * 输出版本信息
		 *
		 */
		void PrintVersion()
		{
			std::cout << _version << std::endl;

			exit(0);
		}

		/**
		 *
		 * 获取选项组
		 *
		 * @param name 待查找组名
		 *
		 * @return 选项组
		 *
		 */
		std::shared_ptr<OptionGroup> GetGroup(const char * groupName)
		{
			auto iter = _groups.find(groupName);

			if (iter == _groups.end())
			{
				auto group = std::make_shared<OptionGroup>();

				_groups.insert(std::make_pair(groupName, group));

				return group;
			}
			else
			{
				return iter->second;
			}
		}

	protected:
		std::string _version{ TINY_TOOLKIT_VERSION_STRING };

		std::size_t _optSize{ 0 };
		std::size_t _valSize{ 0 };

		std::unordered_map<std::string, std::shared_ptr<OptionValue>> _parse{ };
		std::unordered_map<std::string, std::shared_ptr<OptionValue>> _options{ };
		std::unordered_map<std::string, std::shared_ptr<OptionGroup>> _groups{ };
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__OPTION__H__
