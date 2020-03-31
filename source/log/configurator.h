#ifndef __TINY_TOOLKIT__LOG__CONFIGURATOR__H__
#define __TINY_TOOLKIT__LOG__CONFIGURATOR__H__


/**
 *
 *  作者: hm
 *
 *  说明: 配置器
 *
 */


#include "logger.h"


namespace tinyToolkit
{
	namespace log
	{
		class Configurator
		{
			using ConfiguratorContainerType = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;

		public:
			/**
			 *
			 * 解析文件
			 *
			 * @param path 路径
			 *
			 */
			void ParseFile(const std::string & path);

			/**
			 *
			 * 解析内容
			 *
			 * @param content 内容
			 *
			 */
			void ParseContent(const std::string & content);

			/**
			 *
			 * 获取节点对象
			 *
			 * @param name 名称
			 *
			 * @return 日志对象
			 *
			 */
			const std::shared_ptr<ISink> & GetSink(const std::string & name) const;

			/**
			 *
			 * 获取布局对象
			 *
			 * @param name 名称
			 *
			 * @return 日志对象
			 *
			 */
			const std::shared_ptr<ILayout> & GetLayout(const std::string & name) const;

			/**
			 *
			 * 获取过滤器对象
			 *
			 * @param name 名称
			 *
			 * @return 日志对象
			 *
			 */
			const std::shared_ptr<IFilter> & GetFilter(const std::string & name) const;

			/**
			 *
			 * 获取日志对象
			 *
			 * @param name 名称
			 *
			 * @return 日志对象
			 *
			 */
			const std::shared_ptr<ILogger> & GetLogger(const std::string & name) const;

			/**
			 *
			 * 节点组
			 *
			 * @return 日志组
			 *
			 */
			const std::vector<std::shared_ptr<ISink>> & SinkGroup() const;

			/**
			 *
			 * 布局组
			 *
			 * @return 日志组
			 *
			 */
			const std::vector<std::shared_ptr<ILayout>> & LayoutGroup() const;

			/**
			 *
			 * 过滤器组
			 *
			 * @return 日志组
			 *
			 */
			const std::vector<std::shared_ptr<IFilter>> & FilterGroup() const;

			/**
			 *
			 * 日志组
			 *
			 * @return 日志组
			 *
			 */
			const std::vector<std::shared_ptr<ILogger>> & LoggerGroup() const;

			/**
			 *
			 * 单例对象
			 *
			 * @return 单例对象
			 *
			 */
			static Configurator & Instance();

		private:
			/**
			 *
			 * 生成节点
			 *
			 * @param container 配置容器
			 *
			 */
			void GenerateSink(const ConfiguratorContainerType & container);

			/**
			 *
			 * 生成布局
			 *
			 * @param container 配置容器
			 *
			 */
			void GenerateLayout(const ConfiguratorContainerType & container);

			/**
			 *
			 * 生成过滤器
			 *
			 * @param container 配置容器
			 *
			 */
			void GenerateFilter(const ConfiguratorContainerType & container);

			/**
			 *
			 * 生成日志对象
			 *
			 * @param container 配置容器
			 *
			 */
			void GenerateLogger(const ConfiguratorContainerType & container);

		private:
			std::vector<std::shared_ptr<ISink>> _sinkGroup{ };
			std::vector<std::shared_ptr<ILayout>> _layoutGroup{ };
			std::vector<std::shared_ptr<IFilter>> _filterGroup{ };
			std::vector<std::shared_ptr<ILogger>> _loggerGroup{ };

			std::unordered_map<std::string, std::shared_ptr<ISink>> _sinks{ };
			std::unordered_map<std::string, std::shared_ptr<ILayout>> _layouts{ };
			std::unordered_map<std::string, std::shared_ptr<IFilter>> _filters{ };
			std::unordered_map<std::string, std::shared_ptr<ILogger>> _loggers{ };
		};
	}
}


#endif // __TINY_TOOLKIT__LOG__CONFIGURATOR__H__
