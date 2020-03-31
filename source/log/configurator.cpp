/**
 *
 *  作者: hm
 *
 *  说明: 配置器
 *
 */


#include "fileSink.h"
#include "syslogSink.h"
#include "syncLogger.h"
#include "asyncLogger.h"
#include "regexFilter.h"
#include "consoleSink.h"
#include "configurator.h"
#include "simpleLayout.h"
#include "patternLayout.h"
#include "dailyFileSink.h"
#include "priorityFilter.h"
#include "rotatingFileSink.h"
#include "priorityRangeFilter.h"

#include "../util/singleton.h"
#include "../util/filesystem.h"

#include "../container/operation.h"


namespace tinyToolkit
{
	namespace log
	{
		/**
		 *
		 * 解析文件
		 *
		 * @param path 路径
		 *
		 */
		void Configurator::ParseFile(const std::string & path)
		{
			ParseContent(util::Filesystem::Content(path));
		}

		/**
		 *
		 * 解析内容
		 *
		 * @param content 内容
		 *
		 */
		void Configurator::ParseContent(const std::string & content)
		{
			container::Operation::Swap(_sinks);
			container::Operation::Swap(_layouts);
			container::Operation::Swap(_filters);
			container::Operation::Swap(_loggers);

			container::Operation::Swap(_sinkGroup);
			container::Operation::Swap(_layoutGroup);
			container::Operation::Swap(_filterGroup);
			container::Operation::Swap(_loggerGroup);

			std::size_t length{ };

			std::string left{ };
			std::string right{ };

			std::vector<std::string> lines{ };
			std::vector<std::string> tokens{ };

			ConfiguratorContainerType sinks{ };
			ConfiguratorContainerType layouts{ };
			ConfiguratorContainerType filters{ };
			ConfiguratorContainerType loggers{ };

			auto Update = [&](std::unordered_map<std::string, std::unordered_map<std::string, std::string>> & container)
			{
				auto exits = container.find(tokens[1]);

				if (exits == container.end())
				{
					container.insert(std::make_pair(tokens[1], std::unordered_map<std::string, std::string>()));

					exits = container.find(tokens[1]);
				}

				exits->second[tokens[2]] = right;
			};

			util::String::SplitLines(content, lines);

			for (auto &iter : lines)
			{
				util::String::Trim(iter);

				length = iter.find('#');

				if (length == 0)
				{
					continue;
				}

				if (length != std::string::npos)
				{
					iter = iter.substr(0, length);
				}

				length = iter.find('=');

				if (length == std::string::npos)
				{
					continue;
				}

				left = iter.substr(0, length);
				right = iter.substr(length + 1, iter.size() - length);

				util::String::Trim(left);
				util::String::Trim(right);

				util::String::Split(left, ".", tokens);

				if (tokens.size() != 3)
				{
					continue;
				}

				if (tokens[0] == "sink")
				{
					Update(sinks);
				}
				else if (tokens[0] == "layout")
				{
					Update(layouts);
				}
				else if (tokens[0] == "filter")
				{
					Update(filters);
				}
				else if (tokens[0] == "logger")
				{
					Update(loggers);
				}

				tokens.clear();
			}

			GenerateLayout(layouts);
			GenerateFilter(filters);
			GenerateSink(sinks);
			GenerateLogger(loggers);
		}

		/**
		 *
		 * 获取节点对象
		 *
		 * @param name 名称
		 *
		 * @return 日志对象
		 *
		 */
		const std::shared_ptr<ISink> & Configurator::GetSink(const std::string & name) const
		{
			auto find = _sinks.find(name);

			if (find == _sinks.end())
			{
				static std::shared_ptr<ISink> None{ };

				return None;
			}

			return find->second;
		}

		/**
		 *
		 * 获取布局对象
		 *
		 * @param name 名称
		 *
		 * @return 日志对象
		 *
		 */
		const std::shared_ptr<ILayout> & Configurator::GetLayout(const std::string & name) const
		{
			auto find = _layouts.find(name);

			if (find == _layouts.end())
			{
				static std::shared_ptr<ILayout> None{ };

				return None;
			}

			return find->second;
		}

		/**
		 *
		 * 获取过滤器对象
		 *
		 * @param name 名称
		 *
		 * @return 日志对象
		 *
		 */
		const std::shared_ptr<IFilter> & Configurator::GetFilter(const std::string & name) const
		{
			auto find = _filters.find(name);

			if (find == _filters.end())
			{
				static std::shared_ptr<IFilter> None{ };

				return None;
			}

			return find->second;
		}

		/**
		 *
		 * 获取日志对象
		 *
		 * @param name 名称
		 *
		 * @return 日志对象
		 *
		 */
		const std::shared_ptr<ILogger> & Configurator::GetLogger(const std::string & name) const
		{
			auto find = _loggers.find(name);

			if (find == _loggers.end())
			{
				static std::shared_ptr<ILogger> None{ };

				return None;
			}

			return find->second;
		}

		/**
		 *
		 * 节点组
		 *
		 * @return 日志组
		 *
		 */
		const std::vector<std::shared_ptr<ISink>> & Configurator::SinkGroup() const
		{
			return _sinkGroup;
		}

		/**
		 *
		 * 布局组
		 *
		 * @return 日志组
		 *
		 */
		const std::vector<std::shared_ptr<ILayout>> & Configurator::LayoutGroup() const
		{
			return _layoutGroup;
		}

		/**
		 *
		 * 过滤器组
		 *
		 * @return 日志组
		 *
		 */
		const std::vector<std::shared_ptr<IFilter>> & Configurator::FilterGroup() const
		{
			return _filterGroup;
		}

		/**
		 *
		 * 日志组
		 *
		 * @return 日志组
		 *
		 */
		const std::vector<std::shared_ptr<ILogger>> & Configurator::LoggerGroup() const
		{
			return _loggerGroup;
		}

		/**
		 *
		 * 单例对象
		 *
		 * @return 单例对象
		 *
		 */
		Configurator & Configurator::Instance()
		{
			return util::Singleton<Configurator>::Instance();
		}

		/**
		 *
		 * 生成节点
		 *
		 * @param container 配置容器
		 *
		 */
		void Configurator::GenerateSink(const ConfiguratorContainerType & container)
		{
			for (auto &iter : container)
			{
				auto typeFind = iter.second.find("type");

				if (typeFind == iter.second.end())
				{
					continue;
				}

				std::shared_ptr<ISink> sink;

				if (typeFind->second == "FileSink")
				{
					auto pathFind = iter.second.find("path");
					auto truncateFind = iter.second.find("truncate");

					if (pathFind == iter.second.end() ||
						truncateFind == iter.second.end())
					{
						continue;
					}

					sink = std::make_shared<FileSink>
					(
						iter.first,
						pathFind->second,
						truncateFind->second == "true"
					);
				}
				else if (typeFind->second == "SyslogSink")
				{
					sink = std::make_shared<SyslogSink>(iter.first);
				}
				else if (typeFind->second == "ConsoleSink")
				{
					sink = std::make_shared<ConsoleSink>(iter.first);
				}
				else if (typeFind->second == "DailyFileSink")
				{
					auto pathFind = iter.second.find("path");
					auto hourFind = iter.second.find("hour");
					auto minuteFind = iter.second.find("minute");
					auto secondFind = iter.second.find("second");

					if (pathFind == iter.second.end() ||
						hourFind == iter.second.end() ||
						minuteFind == iter.second.end() ||
						secondFind == iter.second.end())
					{
						continue;
					}

					sink = std::make_shared<DailyFileSink>
					(
						iter.first,
						pathFind->second,
						strtol(hourFind->second.c_str(), nullptr, 10),
						strtol(minuteFind->second.c_str(), nullptr, 10),
						strtol(secondFind->second.c_str(), nullptr, 10)
					);
				}
				else if (typeFind->second == "RotatingFileSink")
				{
					auto pathFind = iter.second.find("path");
					auto sizeFind = iter.second.find("size");
					auto countFind = iter.second.find("count");

					if (pathFind == iter.second.end() ||
						sizeFind == iter.second.end() ||
						countFind == iter.second.end())
					{
						continue;
					}

					sink = std::make_shared<RotatingFileSink>
					(
						iter.first,
						pathFind->second,
						strtoull(sizeFind->second.c_str(), nullptr, 10),
						strtoull(countFind->second.c_str(), nullptr, 10)
					);
				}
				else
				{
					continue;
				}

				auto layoutFind = iter.second.find("layout");
				auto filterFind = iter.second.find("filter");

				if (layoutFind != iter.second.end())
				{
					std::vector<std::string> layouts{ };

					util::String::Split(layoutFind->second, " ", layouts);

					for (auto &layout : layouts)
					{
						auto from = _layouts.find(layout);

						if (from == _layouts.end())
						{
							continue;
						}

						sink->SetLayout(from->second);
					}
				}

				if (filterFind != iter.second.end())
				{
					std::vector<std::string> filters{ };

					util::String::Split(filterFind->second, " ", filters);

					for (auto &filter : filters)
					{
						auto from = _filters.find(filter);

						if (from == _filters.end())
						{
							continue;
						}

						sink->AddFilter(from->second);
					}
				}

				_sinks.insert(std::make_pair(iter.first, sink));

				_sinkGroup.push_back(sink);
			}
		}

		/**
		 *
		 * 生成布局
		 *
		 * @param container 配置容器
		 *
		 */
		void Configurator::GenerateLayout(const ConfiguratorContainerType & container)
		{
			for (auto &iter : container)
			{
				auto typeFind = iter.second.find("type");

				if (typeFind == iter.second.end())
				{
					continue;
				}

				std::shared_ptr<ILayout> layout;

				if (typeFind->second == "SimpleLayout")
				{
					layout = std::make_shared<SimpleLayout>();
				}
				else if (typeFind->second == "PatternLayout")
				{
					auto ruleFind = iter.second.find("rule");

					if (ruleFind == iter.second.end())
					{
						continue;
					}

					layout = std::make_shared<PatternLayout>(ruleFind->second);
				}
				else
				{
					continue;
				}

				_layouts.insert(std::make_pair(iter.first, layout));

				_layoutGroup.push_back(layout);
			}
		}

		/**
		 *
		 * 生成过滤器
		 *
		 * @param container 配置容器
		 *
		 */
		void Configurator::GenerateFilter(const ConfiguratorContainerType & container)
		{
			for (auto &iter : container)
			{
				auto typeFind = iter.second.find("type");
				auto ruleFind = iter.second.find("rule");

				if (typeFind == iter.second.end() ||
					ruleFind == iter.second.end())
				{
					continue;
				}

				std::shared_ptr<IFilter> filter;

				if (typeFind->second == "RegexFilter")
				{
					filter = std::make_shared<RegexFilter>(ruleFind->second);
				}
				else if (typeFind->second == "PriorityFilter")
				{
					filter = std::make_shared<PriorityFilter>(ruleFind->second);
				}
				else if (typeFind->second == "PriorityRangeFilter")
				{
					std::vector<std::string> rules{ };

					util::String::Split(ruleFind->second, ",", rules);

					if (rules.size() != 2)
					{
						continue;
					}

					for (auto &rule : rules)
					{
						util::String::Trim(rule);
					}

					filter = std::make_shared<PriorityRangeFilter>(rules[0], rules[1]);
				}
				else
				{
					continue;
				}

				_filters.insert(std::make_pair(iter.first, filter));

				_filterGroup.push_back(filter);
			}
		}

		/**
		 *
		 * 生成日志对象
		 *
		 * @param container 配置容器
		 *
		 */
		void Configurator::GenerateLogger(const ConfiguratorContainerType & container)
		{
			for (auto &iter : container)
			{
				auto typeFind = iter.second.find("type");
				auto sinkFind = iter.second.find("sink");

				if (typeFind == iter.second.end() ||
					sinkFind == iter.second.end())
				{
					continue;
				}

				std::shared_ptr<ILogger> logger;

				if (typeFind->second == "SyncLogger")
				{
					logger = std::make_shared<SyncLogger>();
				}
				else if (typeFind->second == "AsyncLogger")
				{
					logger = std::make_shared<AsyncLogger>();
				}
				else
				{
					continue;
				}

				std::vector<std::string> sinks{ };

				util::String::Split(sinkFind->second, ",", sinks);

				for (auto &sink : sinks)
				{
					util::String::Trim(sink);

					auto find = _sinks.find(sink);

					if (find == _sinks.end())
					{
						continue;
					}

					logger->AddSink(find->second);
				}

				_loggers.insert(std::make_pair(iter.first, logger));

				_loggerGroup.emplace_back(logger);
			}
		}
	}
}
