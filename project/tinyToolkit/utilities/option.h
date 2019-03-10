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
		explicit OptionValue(const char * description);

		/**
		 *
		 * 构造函数
		 *
		 * @param description 描述
		 * @param value 数值
		 *
		 */
		OptionValue(const char * description, const char * value);

		/**
		 *
		 * 是否是参数
		 *
		 * @return 是否是参数
		 *
		 */
		bool IsArg() const;

		/**
		 *
		 * 是否有数值
		 *
		 * @return 是否有数值
		 *
		 */
		bool HasValue() const;

		/**
		 *
		 * 数值
		 *
		 * @return 数值
		 *
		 */
		const std::string & Value() const;

		/**
		 *
		 * 描述
		 *
		 * @return 描述
		 *
		 */
		const std::string & Description() const;

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
		bool AddOption(const char * option, const char * description);

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
		bool AddOption(const char * option, const char * description, const char * value);

		/**
		 *
		 * 选项容器
		 *
		 * @return 选项容器
		 *
		 */
		const std::unordered_map<std::string, std::shared_ptr<OptionValue>> & Options() const;

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
		bool HasOption(const char * option);

	protected:
		std::unordered_map<std::string, std::shared_ptr<OptionValue>> _options{ };
	};

	class TINY_TOOLKIT_API OptionManager
	{
	public:
		/**
		 *
		 * 获取单例
		 *
		 * @return 单例对象引用
		 *
		 */
		static OptionManager & Instance();

		/**
		 *
		 * 是否为空
		 *
		 * @return 是否为空
		 *
		 */
		bool Empty();

		/**
		 *
		 * 是否存在选项
		 *
		 * @param option 待查询选项
		 *
		 * @return 是否存在
		 *
		 */
		bool Has(const char * option);

		/**
		 *
		 * 是否存在选项
		 *
		 * @param option 待查询选项
		 *
		 * @return 是否存在
		 *
		 */
		bool Has(const std::string & option);

		/**
		 *
		 * 定义选项
		 *
		 * @param option 选项
		 * @param description 描述
		 * @param groupName 组名
		 *
		 */
		void Define(const char * option, const char * description, const char * groupName = nullptr);

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
		void DefineArg(const char * option, const char * description, const char * value = nullptr, const char * groupName = nullptr);

		/**
		 *
		 * 定义版本号
		 *
		 * @param version 版本号
		 *
		 */
		void DefineVersion(std::string version);

		/**
		 *
		 * 解析选项
		 *
		 * @param argc 解析选项个数
		 * @param argv 解析选项流
		 *
		 */
		void Parse(int32_t argc, char const ** argv);

		/**
		 *
		 * 获取数值
		 *
		 * @param option 待查找选项
		 *
		 * @return 数值
		 *
		 */
		const std::string & Get(const std::string & option);

	protected:
		/**
		 *
		 * 更新显示对齐
		 *
		 * @param option 对齐的选项
		 * @param value 对齐的数值
		 *
		 */
		void UpdateAlignment(const char * option, const char * value);

		/**
		 *
		 * 整合默认值选项
		 *
		 */
		void ComposeDefaultOptions();

		/**
		 *
		 * 输出帮助信息
		 *
		 */
		void PrintHelp();

		/**
		 *
		 * 输出版本信息
		 *
		 */
		void PrintVersion();

		/**
		 *
		 * 获取选项组
		 *
		 * @param name 待查找组名
		 *
		 * @return 选项组
		 *
		 */
		std::shared_ptr<OptionGroup> GetGroup(const char * groupName);

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
