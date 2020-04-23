#ifndef __TINY_TOOLKIT__OPTION__DESCRIPTION__H__
#define __TINY_TOOLKIT__OPTION__DESCRIPTION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 描述
 *
 */


#include "value.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <vector>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <vector>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <vector>
#
#endif


namespace tinyToolkit
{
	namespace option
	{
		class TINY_TOOLKIT_API DescriptionInfo
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param name 名称
			 * @param info 信息
			 *
			 */
			DescriptionInfo(const char * name, const char * info);

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
			DescriptionInfo(const char * name, const char * mode, const char * info, std::shared_ptr<SemanticValue> value);

			/**
			 *
			 * 更新
			 *
			 */
			void Update();

			/**
			 *
			 * 更新
			 *
			 * @param value 数值
			 *
			 */
			void Update(const std::string & value);

			/**
			 *
			 * 是否有效
			 *
			 * @return 是否有效
			 *
			 */
			bool IsValid() const;

			/**
			 *
			 * 是否需要数据
			 *
			 * @return 是否需要数据
			 *
			 */
			bool IsRequired() const;

			/**
			 *
			 * 信息
			 *
			 * @return 信息
			 *
			 */
			const std::string & Info() const;

			/**
			 *
			 * 方式
			 *
			 * @return 方式
			 *
			 */
			const std::string & Mode() const;

			/**
			 *
			 * 名称
			 *
			 * @return 名称
			 *
			 */
			const std::string & LongName() const;

			/**
			 *
			 * 名称
			 *
			 * @return 名称
			 *
			 */
			const std::string & ShortName() const;

			/**
			 *
			 * 名称
			 *
			 * @return 名称
			 *
			 */
			const std::string & OptionName() const;

			/**
			 *
			 * 数据
			 *
			 * @return 数据
			 *
			 */
			const std::shared_ptr<SemanticValue> & Value() const;

		private:
			/**
			 *
			 * 设置名称
			 *
			 * @param name 名称
			 *
			 */
			void SetName(const char * name);

		private:
			bool _isValid{ false };

			std::string _info{ };
			std::string _mode{ };
			std::string _longName{ };
			std::string _shortName{ };
			std::string _optionName{ };

			std::shared_ptr<SemanticValue> _value{ };
		};

		class TINY_TOOLKIT_API DescriptionGroup
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param caption 标题
			 *
			 */
			explicit DescriptionGroup(const char * caption);

			/**
			 *
			 * 添加选项
			 *
			 * @return 对象
			 *
			 */
			DescriptionGroup & AddOption();

			/**
			 *
			 * 标题
			 *
			 * @return 标题
			 *
			 */
			const std::string & Caption() const;

			/**
			 *
			 * 选项组
			 *
			 * @return 选项组
			 *
			 */
			const std::vector<std::shared_ptr<DescriptionInfo>> & Options() const;

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
			DescriptionGroup & operator()(const char * name, const char * info);

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
			DescriptionGroup & operator()(const char * name, const char * mode, const char * info, const std::shared_ptr<SemanticValue> & value);

		private:
			std::string _caption{ };

			std::vector<std::shared_ptr<DescriptionInfo>> _options{ };
		};
	}
}


#endif // __TINY_TOOLKIT__OPTION__DESCRIPTION__H__
