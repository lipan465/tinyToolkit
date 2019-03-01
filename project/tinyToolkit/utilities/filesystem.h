#ifndef __TINY_TOOLKIT__UTILITIES__FILESYSTEM__H__
#define __TINY_TOOLKIT__UTILITIES__FILESYSTEM__H__


/**
 *
 *  作者: hm
 *
 *  说明: 文件处理
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API Filesystem
	{
	public:
		/**
		 *
		 * 文件是否存在
		 *
		 * @param path 文件路径
		 *
		 * @return 文件是否存在
		 *
		 */
		static bool Exists(const std::string & path);

		/**
		 *
		 * 删除文件
		 *
		 * @param path 文件路径
		 *
		 * @return 文件是否删除
		 *
		 */
		static bool Remove(const std::string & path);

		/**
		 *
		 * 更改文件名称
		 *
		 * @param src 待更改文件路径
		 * @param dst 被更改文件路径
		 *
		 * @return 文件是否更改
		 *
		 */
		static bool Rename(const std::string & src, const std::string & dst);

		/**
		 *
		 * 是否为目录
		 *
		 * @param path 待检测文件路径
		 *
		 * @return 是否为目录
		 *
		 */
		static bool IsDirectory(const std::string & path);

		/**
		 *
		 * 文件大小
		 *
		 * @param path 文件路径
		 *
		 * @return 文件大小
		 *
		 */
		static std::size_t Size(const std::string & path);

		/**
		 *
		 * 文件内容
		 *
		 * @param path 待读取文件路径
		 *
		 * @return 文件内容
		 *
		 */
		static std::string Content(const std::string & path);

		/**
		 *
		 * 读取文件内容
		 *
		 * @param path 待读取文件路径
		 * @param container 结果容器
		 * @param keepEmpty 是否保留空行
		 *
		 * @return 读取行数
		 *
		 */
		static std::size_t ReadFile(const std::string & path, std::vector<std::string> & container, bool keepEmpty = false);

		/**
		 *
		 * 读取文件内容
		 *
		 * @param path 待读取文件路径
		 * @param keepEmpty 是否保留空行
		 *
		 * @return 结果容器
		 *
		 */
		static std::vector<std::string> ReadFile(const std::string & path, bool keepEmpty = false);

		/**
		 *
		 * 写入文件
		 *
		 * @tparam ValueTypeT [all built-in types]
		 *
		 * @param path 待写入文件路径
		 * @param value 待写入数据
		 *
		 * @return 写入结果
		 *
		 */
		template<typename ValueT>
		static bool WriteFile(const std::string & path, ValueT && value)
		{
			std::ofstream ofs(path, std::ios::binary);

			if (ofs.is_open())
			{
				ofs << std::forward<ValueT>(value);

				ofs.close();

				return true;
			}
			else
			{
				throw std::runtime_error("Failed open file : " + path);
			}
		}

		/**
		 *
		 * 写入文件
		 *
		 * @tparam It [container iterator]
		 *
		 * @param path 待写入文件路径
		 * @param begin 待写入数据迭代器头部
		 * @param end 待写入数据迭代器尾部
		 *
		 * @return 写入结果
		 *
		 */
		template<typename It>
		static bool WriteFile(const std::string & path, const It & begin, const It & end)
		{
			std::ofstream ofs(path, std::ios::binary);

			if (ofs.is_open())
			{
				for (auto iter = begin; iter != end; ++iter)
				{
					ofs << *iter << TINY_TOOLKIT_EOL;
				}

				ofs.close();

				return true;
			}
			else
			{
				throw std::runtime_error("Failed open file : " + path);
			}
		}

		/**
		 * 创建文件
		 *
		 * @param path 待创建文件路径
		 *
		 * @return 创建结果
		 *
		 */
		static bool CreateFile(const std::string & path);

		/**
		 *
		 * 创建文件夹
		 *
		 * @param path 待创建文件路径
		 *
		 * @return 创建结果
		 *
		 */
		static bool CreateDirectory(const std::string & path);

		/**
		 *
		 * 创建层级目录
		 *
		 * @param path 待创建目录路径
		 *
		 * @return 创建结果
		 *
		 */
		static bool CreateDirectories(const std::string & path);

		/**
		 *
		 * 当前目录
		 *
		 * @return 当前目录
		 *
		 */
		static std::string CurrentDirectory();

		/**
		 *
		 * 文件名
		 *
		 * @param path 待处理文件路径
		 *
		 * @return 文件名
		 *
		 */
		static std::string Name(const std::string & path);

		/**
		 *
		 * 文件前缀
		 *
		 * @param path 待处理文件路径
		 *
		 * @return 文件前缀
		 *
		 */
		static std::string Steam(const std::string & path);

		/**
		 *
		 * 绝对路径
		 *
		 * @param path 待处理路径
		 *
		 * @return 绝对路径
		 *
		 */
		static std::string Canonical(const std::string & path);

		/**
		 *
		 * 文件扩展名
		 *
		 * @param path 待处理文件路径
		 *
		 * @return 文件扩展名
		 *
		 */
		static std::string Extension(const std::string & path);

		/**
		 *
		 * 父级路径
		 *
		 * @param path 待处理路径
		 *
		 * @return 父级路径
		 *
		 */
		static std::string ParentDirectory(const std::string & path);

		/**
		 *
		 * 遍历文件
		 *
		 * @param path 待遍历根目录路径
		 * @param container 文件结果容器
		 * @param subdirectory 是否遍历子目录
		 *
		 */
		static void TraverseFile(const std::string & path, std::vector<std::string> & container, bool subdirectory = false);

		/**
		 *
		 * 遍历文件
		 *
		 * @param path 待遍历根目录路径
		 * @param container 文件结果容器
		 * @param rule 遍历规则
		 * @param subdirectory 是否遍历子目录
		 *
		 */
		static void TraverseFile(const std::string & path, std::vector<std::string> & container, const std::regex & rule, bool subdirectory = false);

		/**
		 *
		 * 遍历文件
		 *
		 * @param path 待遍历根目录路径
		 * @param subdirectory 是否遍历子目录
		 *
		 * @return 文件结果容器
		 *
		 */
		static std::vector<std::string> TraverseFile(const std::string & path, bool subdirectory = false);

		/**
		 *
		 * 遍历文件
		 *
		 * @param path 待遍历根目录路径
		 * @param rule 遍历规则
		 * @param subdirectory 是否遍历子目录
		 *
		 * @return 文件结果容器
		 *
		 */
		static std::vector<std::string> TraverseFile(const std::string & path, const std::regex & rule, bool subdirectory = false);

		/**
		 *
		 * 遍历目录
		 *
		 * @param path 待遍历根目录路径
		 * @param container 目录结果容器
		 * @param subdirectory 是否遍历子目录
		 *
		 */
		static void TraverseDirectory(const std::string & path, std::vector<std::string> & container, bool subdirectory = false);

		/**
		 *
		 * 遍历目录
		 *
		 * @param path 待遍历根目录路径
		 * @param container 目录结果容器
		 * @param rule 遍历规则
		 * @param subdirectory 是否遍历子目录
		 *
		 */
		static void TraverseDirectory(const std::string & path, std::vector<std::string> & container, const std::regex & rule, bool subdirectory = false);

		/**
		 *
		 * 遍历目录
		 *
		 * @param path 待遍历根目录路径
		 * @param subdirectory 是否遍历子目录
		 *
		 * @return 目录结果容器
		 *
		 */
		static std::vector<std::string> TraverseDirectory(const std::string & path, bool subdirectory = false);

		/**
		 *
		 * 遍历目录
		 *
		 * @param path 待遍历根目录路径
		 * @param rule 遍历规则
		 * @param subdirectory 是否遍历子目录
		 *
		 * @return 目录结果容器
		 *
		 */
		static std::vector<std::string> TraverseDirectory(const std::string & path, const std::regex & rule, bool subdirectory = false);
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__FILESYSTEM__H__
