/**
 *
 *  作者: hm
 *
 *  说明: json处理
 *
 */


#include "json.h"


namespace tinyToolkit
{
	/**
	 *
	 * 收缩
	 *
	 * @param value 待格收缩数据
	 *
	 * @return 收缩后数据
	 *
	 */
	std::string Json::Reduce(const std::string & value)
	{
		std::size_t tag = 0;
		std::size_t length = value.size();

		std::string str{ };

		for (std::size_t i = 0; i < length; ++i)
		{
			auto & key = value[i];

			if ((tag & 1) == 0)
			{
				if (key == ',')
				{
					str += key;

					continue;
				}
				else if ((key == '[') || (key == '{'))
				{
					str += key;

					continue;
				}
				else if (((key == ']') || (key == '}')))
				{
					str += key;

					continue;
				}
				else if ((key == ' ') || (key == '\t') || (key == '\r') || (key == '\n'))
				{
					continue;
				}
			}

			if (key == '\"')
			{
				++tag;
			}

			str += key;
		}

		return str;
	}

	/**
	 *
	 * 格式化
	 *
	 * @param value 待格式化数据
	 *
	 * @return 格式化后数据
	 *
	 */
	std::string Json::Format(const std::string & value)
	{
		std::size_t tag = 0;
		std::size_t count = 0;
		std::size_t length = value.size();

		std::string str{ };

		auto AppendIndent = [&]()
		{
			for (std::size_t i = 0; i < count; ++i)
			{
				str += "\t";
			}
		};

		for (std::size_t i = 0; i < length; ++i)
		{
			auto & key = value[i];

			if ((tag & 1) == 0)
			{
				if (key == ',')
				{
					str += key;
					str += TINY_TOOLKIT_EOL;

					AppendIndent();

					continue;
				}
				else if ((key == '[') || (key == '{'))
				{
					if (((i - 1) > 0) && (value[i - 1] == ':'))
					{
						str += TINY_TOOLKIT_EOL;

						AppendIndent();
					}

					str += key;
					str += TINY_TOOLKIT_EOL;

					++count;

					AppendIndent();

					continue;
				}
				else if (((key == ']') || (key == '}')))
				{
					if (((i - 1) > 0) && ((value[i - 1] != ']') && (value[i - 1] != '}')))
					{
						str += TINY_TOOLKIT_EOL;
					}

					--count;

					AppendIndent();

					str += key;

					if (((i + 1) < length) && (value[i + 1] != ','))
					{
						str += TINY_TOOLKIT_EOL;
					}

					continue;
				}
				else if ((key == ' ') || (key == '\t') || (key == '\r') || (key == '\n'))
				{
					continue;
				}
			}

			if (key == '\"')
			{
				if (i - 1 > 0)
				{
					if (value[i - 1] != '\\')
					{
						++tag;
					}
				}
				else
				{
					++tag;
				}
			}

			str += key;
		}

		return str;
	}
}
