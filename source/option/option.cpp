/**
 *
 *  作者: hm
 *
 *  说明: 解析器
 *
 */


#include "option.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <sstream>
#  include <iomanip>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <sstream>
#  include <iomanip>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <cstring>
#  include <sstream>
#  include <iomanip>
#
#endif


namespace tinyToolkit
{
	namespace option
	{
		/**
		 *
		 * 解析
		 *
		 * @param argc 选项个数
		 * @param argv 选项数组
		 *
		 */
		void Option::Parse(int argc, const char * argv[])
		{
			std::string opt{ };
			std::string val{ };
			std::string err{ };

			for (int32_t i = 1; i < argc; ++i)
			{
				const char * value = argv[i];
				const char * found = strstr(value, "=");

				std::unordered_map<std::string, DescriptionInfo *>::iterator iter{ };

				if (strncmp(value, "--", 2) == 0)
				{
					if (found)
					{
						val.assign(found + 1);
						opt.assign(value + 2, found);
					}
					else
					{
						opt.assign(value + 2);
					}

					err = "--" + opt;

					if (opt.empty())
					{
						throw std::invalid_argument("Option is empty : " + err);
					}

					iter = _longOptions.find(opt);

					if (iter == _longOptions.end())
					{
						throw std::invalid_argument("Undefined option : " + err);
					}
				}
				else if (strncmp(value, "-", 1) == 0)
				{
					if (found)
					{
						val.assign(found + 1);
						opt.assign(value + 1, found);
					}
					else
					{
						opt.assign(value + 1);
					}

					err = "-" + opt;

					if (opt.empty())
					{
						throw std::invalid_argument("Option is empty : " + err);
					}

					iter = _shortOptions.find(opt);

					if (iter == _shortOptions.end())
					{
						throw std::invalid_argument("Undefined option : " + err);
					}
				}
				else
				{
					throw std::invalid_argument("Invalid option : " + std::string(value));
				}

				if (iter->second->IsRequired())
				{
					if (found == nullptr)
					{
						throw std::invalid_argument("Option require input value : " + err);
					}

					iter->second->Update(val);
				}
				else
				{
					if (found)
					{
						throw std::invalid_argument("Option do not require input value : " + err);
					}

					iter->second->Update();
				}
			}
		}

		/**
		 *
		 * 添加描述组
		 *
		 * @param group 描述组
		 *
		 */
		void Option::AddDescriptionGroup(const std::shared_ptr<DescriptionGroup> & group)
		{
			for (auto &iter : _groups)
			{
				if (iter == group)
				{
					throw std::runtime_error("Multiple add description group : " + group->Caption());
				}
			}

			_groups.push_back(group);

			for (auto &iter : group->Options())
			{
				if (!iter->LongName().empty())
				{
					if (!_options.insert(std::make_pair(iter->LongName(), iter.get())).second ||
					    !_longOptions.insert(std::make_pair(iter->LongName(), iter.get())).second)
					{
						throw std::runtime_error("Multiple definition : " + iter->LongName());
					}
				}

				if (!iter->ShortName().empty())
				{
					if (!_options.insert(std::make_pair(iter->ShortName(), iter.get())).second ||
					    !_shortOptions.insert(std::make_pair(iter->ShortName(), iter.get())).second)
					{
						throw std::runtime_error("Multiple definition : " + iter->ShortName());
					}
				}

				if (iter->IsRequired())
				{
					if (iter->Value()->HasDefault())
					{
						if (_modeWidth < (iter->Mode().size() + iter->Value()->Content().size() + 11))
						{
							_modeWidth = iter->Mode().size() + iter->Value()->Content().size() + 11;
						}
					}
					else
					{
						if (_modeWidth < iter->Mode().size())
						{
							_modeWidth = iter->Mode().size();
						}
					}
				}

				if (_optionWidth < iter->OptionName().size())
				{
					_optionWidth = iter->OptionName().size();
				}
			}
		}

		/**
		 *
		 * 是否存在
		 *
		 * @param option 选项
		 *
		 * @return 是否存在
		 *
		 */
		bool Option::Exits(const std::string & option) const
		{
			auto find = _options.find(option);

			if (find == _options.end())
			{
				return false;
			}

			return find->second->IsValid();
		}

		/**
		 *
		 * 详细信息
		 *
		 * @return 详细信息
		 *
		 */
		std::string Option::Verbose()
		{
			std::stringstream stream{ };

			for (auto &group : _groups)
			{
				stream << std::endl
				       << group->Caption()
				       << ":"
				       << std::endl;

				for (auto &option : group->Options())
				{
					stream << "  "
					       << std::setw(static_cast<int32_t>(_optionWidth))
					       << std::right
					       << option->OptionName()
					       << "   ";

					std::size_t width = _modeWidth;

					if (option->IsRequired())
					{
						stream << option->Mode();

						width -= option->Mode().size();

						if (option->Value()->HasDefault())
						{
							stream << " (default="
							       << option->Value()->Content()
							       << ")";

							width -= option->Value()->Content().size() + 11;
						}
					}

					if (width > 0)
					{
						stream << std::setw(width)
						       << std::right
						       << " ";
					}

					stream << "   "
					       << option->Info()
					       << std::endl;
				}
			}

			return stream.str();
		}
	}
}
