#ifndef __TINY_TOOLKIT__UTIL__FILESYSTEM__H__
#define __TINY_TOOLKIT__UTIL__FILESYSTEM__H__


/**
 *
 *  作者: hm
 *
 *  说明: 文件处理
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <regex>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <regex>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <regex>
#
#endif


namespace tinyToolkit
{
	namespace util
	{
		class TINY_TOOLKIT_API Filesystem
		{
		public:
			/**
			 *
			 * 是否存在
			 *
			 * @param path 文件路径
			 *
			 * @return 是否存在
			 *
			 */
			static bool Exists(const std::string & path);

			/**
			 *
			 * 是否为目录
			 *
			 * @param path 文件路径
			 *
			 * @return 是否为目录
			 *
			 */
			static bool IsDirectory(const std::string & path);

			/**
			 *
			 * 删除目录
			 *
			 * @param path 文件路径
			 *
			 * @return 是否删除成功
			 *
			 */
			static bool Remove(const std::string & path);

			/**
			 *
			 * 更改文件名称
			 *
			 * @param src 待更改文件
			 * @param dst 被更改文件
			 *
			 * @return 是否更改成功
			 *
			 */
			static bool Rename(const std::string & src, const std::string & dst);

			/**
			 *
			 * 写入文件
			 *
			 * @param path 文件路径
			 * @param content 内容
			 *
			 * @return 是否写入成功
			 *
			 */
			static bool WriteFile(const std::string & path, const std::string & content);

			/**
			 *
			 * 创建文件
			 *
			 * @param path 文件路径
			 *
			 * @return 是否创建成功
			 *
			 */
			static bool CreateFile(const std::string & path);

			/**
			 *
			 * 创建文件夹
			 *
			 * @param path 文件夹路径
			 *
			 * @return 是否创建成功
			 *
			 */
			static bool CreateDirectory(const std::string & path);

			/**
			 *
			 * 创建文件夹
			 *
			 * @param path 文件夹路径
			 *
			 * @return 是否创建成功
			 *
			 */
			static bool CreateDirectories(const std::string & path);

			/**
			 *
			 * 读取文件
			 *
			 * @param path 文件路径
			 * @param container 结果容器
			 * @param keepEmpty 是否保留空行
			 *
			 * @return 是否读取成功
			 *
			 */
			static bool ReadFile(const std::string & path, std::vector<std::string> & container, bool keepEmpty = false);

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
			 * 当前目录
			 *
			 * @return 当前目录
			 *
			 */
			static std::string CurrentDirectory();

			/**
			 *
			 * 父级目录
			 *
			 * @param path 文件路径
			 *
			 * @return 父级目录
			 *
			 */
			static std::string ParentDirectory(const std::string & path);

			/**
			 *
			 * 文件名
			 *
			 * @param path 文件路径
			 *
			 * @return 文件名
			 *
			 */
			static std::string Name(const std::string & path);

			/**
			 *
			 * 文件前缀
			 *
			 * @param path 文件路径
			 *
			 * @return 文件前缀
			 *
			 */
			static std::string Steam(const std::string & path);

			/**
			 *
			 * 文件扩展名
			 *
			 * @param path 文件路径
			 *
			 * @return 文件扩展名
			 *
			 */
			static std::string Extension(const std::string & path);

			/**
			 *
			 * 文件内容
			 *
			 * @param path 文件路径
			 *
			 * @return 文件内容
			 *
			 */
			static std::string Content(const std::string & path);

			/**
			 *
			 * 绝对路径
			 *
			 * @param path 文件路径
			 *
			 * @return 绝对路径
			 *
			 */
			static std::string Canonical(const std::string & path);

			/**
			 *
			 * 遍历文件
			 *
			 * @param path 文件路径
			 * @param container 结果容器
			 * @param subdirectory 是否遍历子目录
			 *
			 * @return 是否遍历成功
			 *
			 */
			static bool TraverseFile(const std::string & path, std::vector<std::string> & container, bool subdirectory = false);

			/**
			 *
			 * 遍历文件
			 *
			 * @param path 文件路径
			 * @param container 结果容器
			 * @param rule 匹配规则
			 * @param subdirectory 是否遍历子目录
			 *
			 * @return 是否遍历成功
			 *
			 */
			static bool TraverseFile(const std::string & path, std::vector<std::string> & container, const std::regex & rule, bool subdirectory = false);

			/**
			 *
			 * 遍历目录
			 *
			 * @param path 文件路径
			 * @param container 结果容器
			 * @param subdirectory 是否遍历子目录
			 *
			 * @return 是否遍历成功
			 *
			 */
			static bool TraverseDirectory(const std::string & path, std::vector<std::string> & container, bool subdirectory = false);

			/**
			 *
			 * 遍历目录
			 *
			 * @param path 文件路径
			 * @param container 结果容器
			 * @param rule 匹配规则
			 * @param subdirectory 是否遍历子目录
			 *
			 * @return 是否遍历成功
			 *
			 */
			static bool TraverseDirectory(const std::string & path, std::vector<std::string> & container, const std::regex & rule, bool subdirectory = false);
		};
	}
}


#endif // __TINY_TOOLKIT__UTIL__FILESYSTEM__H__
