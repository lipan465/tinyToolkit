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
		/**
		 *
		 * /usr/sniffer/abc.txt
		 *
		 * stem()            abc                  不带扩展名的文件名
		 * filename()        abc.txt              文件名
		 * extension()       .txt                 扩展名
		 * root_name()                            根名
		 * root_path()       /                    根路径：根名+根目录
		 * parent_path()     /usr/sniffer         父路径：根路径+相对路径
		 * relative_path()   usr/sniffer/abc.txt  相对路径
		 * root_directory()  /                    根目录
		 *
		 */

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
		static bool Exists(const std::string & path)
		{
#ifdef TINY_TOOLKIT_CXX_17

			return std::filesystem::exists(path);

#else

	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			auto attribs = GetFileAttributesA(path.c_str());

			return attribs != INVALID_FILE_ATTRIBUTES && !(attribs & FILE_ATTRIBUTE_DIRECTORY);

	#else

			return access(path.c_str(), F_OK) == 0;

	#endif

#endif
		}

		/**
		 *
		 * 删除文件
		 *
		 * @param path 文件路径
		 *
		 * @return 文件是否删除
		 *
		 */
		static bool Remove(const std::string & path)
		{
#ifdef TINY_TOOLKIT_CXX_17

			return std::filesystem::remove(path);

#else

			return std::remove(path.c_str()) == 0;

#endif
		}

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
		static bool Rename(const std::string & src, const std::string & dst)
		{
#ifdef TINY_TOOLKIT_CXX_17

			std::filesystem::rename(src, dst);

			return true;

#else

			return std::rename(src.c_str(), dst.c_str()) == 0;

#endif
		}

		/**
		 *
		 * 读取文件内容
		 *
		 * @param path 待读取文件路径
		 *
		 * @return 读取结果
		 *
		 */
		static std::string ReadAll(const std::string & path)
		{
			std::ifstream ifs(path, std::ios::binary);

			if (ifs.is_open())
			{
				std::string str((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

				ifs.close();

				return str;
			}

			return { };
		}

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
		static std::size_t ReadFile(const char * path, std::vector<std::string> & container, bool keepEmpty = false)
		{
			if (path)
			{
				std::ifstream ifs(path, std::ios::binary);

				if (ifs.is_open())
				{
					std::string str{ };

					while (!ifs.eof())
					{
						std::getline(ifs, str);

						if (!ifs.good())  // 检查文件流是否正常
						{
							break;
						}

						if (keepEmpty || !str.empty())
						{
							container.push_back(str);
						}
					}

					ifs.close();
				}
			}

			return container.size();
		}

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
		static std::size_t ReadFile(const std::string & path, std::vector<std::string> & container, bool keepEmpty = false)
		{
			std::ifstream ifs(path, std::ios::binary);

			if (ifs.is_open())
			{
				std::string str{ };

				while (!ifs.eof())
				{
					std::getline(ifs, str);

					if (!ifs.good())
					{
						break;
					}

					if (keepEmpty || !str.empty())
					{
						container.push_back(str);
					}
				}

				ifs.close();
			}

			return container.size();
		}

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
		static std::vector<std::string> ReadFile(const std::string & path, bool keepEmpty = false)
		{
			std::vector<std::string> container;

			ReadFile(path, container, keepEmpty);

			return container;
		}

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
				ofs << std::forward<ValueT>(value) << std::endl;

				ofs.close();

				return true;
			}

			return false;
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
					ofs << *iter << std::endl;
				}

				ofs.close();

				return true;
			}

			return false;
		}

		/**
		 * 创建文件
		 *
		 * @param path 待创建文件路径
		 *
		 * @return 创建结果
		 *
		 */
		static bool CreateFile(const std::string & path)
		{
			std::ofstream ofs(path, std::ios::binary);

			if (ofs.is_open())
			{
				ofs.close();

				return true;
			}

			return false;
		}

		/**
		 *
		 * 文件名
		 *
		 * @param path 待处理文件路径
		 *
		 * @return 文件名
		 *
		 */
		static std::string FileName(const std::string & path)
		{
			std::size_t pos = path.rfind(TINY_TOOLKIT_FOLDER_SEP[0]);

			return pos == std::string::npos ? path : path.substr(pos + 1);
		}

		/**
		 *
		 * 文件前缀
		 *
		 * @param path 待处理文件路径
		 *
		 * @return 文件前缀
		 *
		 */
		static std::string FileSteam(const std::string & path)
		{
			std::size_t srcPos = path.rfind(TINY_TOOLKIT_FOLDER_SEP[0]);
			std::size_t dstPos = path.rfind('.');

			if (srcPos == std::string::npos)
			{
				return dstPos == std::string::npos ? path : path.substr(0, dstPos);
			}
			else
			{
				return (dstPos == std::string::npos || dstPos < srcPos) ? path.substr(srcPos + 1) : path.substr(srcPos + 1, dstPos);
			}
		}

		/**
		 *
		 * 文件扩展名
		 *
		 * @param path 待处理文件路径
		 *
		 * @return 文件扩展名
		 *
		 */
		static std::string FileExtension(const std::string & path)
		{
			std::size_t srcPos = path.rfind(TINY_TOOLKIT_FOLDER_SEP[0]);
			std::size_t dstPos = path.rfind('.');

			if (srcPos == std::string::npos)
			{
				return dstPos == std::string::npos ? "" : path.substr(dstPos);
			}
			else
			{
				return (dstPos == std::string::npos || dstPos < srcPos) ? "" : path.substr(dstPos);
			}
		}

		/**
		 *
		 * 父级路径
		 *
		 * @param path 待处理路径
		 *
		 * @return 父级路径
		 *
		 */
		static std::string FileDirectory(const std::string & path)
		{
			std::size_t pos = path.rfind(TINY_TOOLKIT_FOLDER_SEP[0]);

			return pos == std::string::npos ? "./" : path.substr(0, pos + 1);
		}

#if TINY_TOOLKIT_CXX_SUPPORT >= 17

		/**
		 *
		 * 遍历文件及目录
		 *
		 * @param path 待遍历根目录路径
		 * @param file 文件结果容器
		 * @param directory 目录结果容器
		 *
		 */
		static void Traverse(const std::string & path, std::vector<std::filesystem::path> & file, std::vector<std::filesystem::path> & directory)
		{
			for (auto &iter : std::filesystem::directory_iterator(std::filesystem::path(path)))
			{
				if (iter.is_directory())
				{
					directory.push_back(iter.path());

					Traverse(iter.path().string(), file, directory);
				}
				else
				{
					file.push_back(iter.path());
				}
			}
		}

		/**
		 *
		 * 遍历文件及目录
		 *
		 * @param path 待遍历根目录路径
		 * @param file 文件结果容器
		 * @param directory 目录结果容器
		 * @param rule 遍历规则
		 *
		 */
		static void Traverse(const std::string & path, std::vector<std::filesystem::path> & file, std::vector<std::filesystem::path> & directory, const std::regex & rule)
		{
			for (auto &iter : std::filesystem::directory_iterator(std::filesystem::path(path)))
			{
				if (iter.is_directory())
				{
					if (std::regex_match(iter.path().string(), rule))
					{
						directory.push_back(iter.path());
					}

					Traverse(iter.path().string(), file, directory);
				}
				else
				{
					if (std::regex_match(iter.path().filename().string(), rule))
					{
						file.push_back(iter.path());
					}
				}
			}
		}

		/**
		 *
		 * 遍历文件
		 *
		 * @param path 待遍历根目录路径
		 * @param container 文件结果容器
		 *
		 */
		static void TraverseFile(const std::string & path, std::vector<std::filesystem::path> & container)
		{
			for (auto &iter : std::filesystem::directory_iterator(std::filesystem::path(path)))
			{
				if (iter.is_directory())
				{
					TraverseFile(iter.path().string(), container);
				}
				else
				{
					container.push_back(iter.path());
				}
			}
		}

		/**
		 *
		 * 遍历文件
		 *
		 * @param path 待遍历根目录路径
		 * @param container 文件结果容器
		 * @param rule 遍历规则
		 *
		 */
		static void TraverseFile(const std::string & path, std::vector<std::filesystem::path> & container, const std::regex & rule)
		{
			for (auto &iter : std::filesystem::directory_iterator(std::filesystem::path(path)))
			{
				if (iter.is_directory())
				{
					TraverseFile(iter.path().string(), container, rule);
				}
				else
				{
					if (std::regex_match(iter.path().filename().string(), rule))
					{
						container.push_back(iter.path());
					}
				}
			}
		}

		/**
		 *
		 * 遍历文件
		 *
		 * @param path 待遍历根目录路径
		 *
		 * @return 文件结果容器
		 *
		 */
		static std::vector<std::filesystem::path> TraverseFile(const std::string & path)
		{
			std::vector<std::filesystem::path> container;

			TraverseFile(path, container);

			return container;
		}

		/**
		 *
		 * 遍历文件
		 *
		 * @param path 待遍历根目录路径
		 * @param rule 遍历规则
		 *
		 * @return 文件结果容器
		 *
		 */
		static std::vector<std::filesystem::path> TraverseFile(const std::string & path, const std::regex & rule)
		{
			std::vector<std::filesystem::path> container;

			TraverseFile(path, container, rule);

			return container;
		}

		/**
		 *
		 * 遍历目录
		 *
		 * @param path 待遍历根目录路径
		 * @param container 目录结果容器
		 *
		 */
		static void TraverseDirectory(const std::string & path, std::vector<std::filesystem::path> & container)
		{
			for (auto &iter : std::filesystem::directory_iterator(std::filesystem::path(path)))
			{
				if (iter.is_directory())
				{
					container.push_back(iter.path());

					TraverseDirectory(iter.path().string(), container);
				}
			}
		}

		/**
		 *
		 * 遍历目录
		 *
		 * @param path 待遍历根目录路径
		 * @param container 目录结果容器
		 * @param rule 遍历规则
		 *
		 */
		static void TraverseDirectory(const std::string & path, std::vector<std::filesystem::path> & container, const std::regex & rule)
		{
			for (auto &iter : std::filesystem::directory_iterator(std::filesystem::path(path)))
			{
				if (iter.is_directory())
				{
					if (std::regex_match(iter.path().string(), rule))
					{
						container.push_back(iter.path());
					}

					TraverseDirectory(iter.path().string(), container, rule);
				}
			}
		}

		/**
		 *
		 * 遍历目录
		 *
		 * @param path 待遍历根目录路径
		 *
		 * @return 目录结果容器
		 *
		 */
		static std::vector<std::filesystem::path> TraverseDirectory(const std::string & path)
		{
			std::vector<std::filesystem::path> container;

			TraverseDirectory(path, container);

			return container;
		}

		/**
		 *
		 * 遍历目录
		 *
		 * @param path 待遍历根目录路径
		 * @param rule 遍历规则
		 *
		 * @return 目录结果容器
		 *
		 */
		static std::vector<std::filesystem::path> TraverseDirectory(const std::string & path, const std::regex & rule)
		{
			std::vector<std::filesystem::path> container;

			TraverseDirectory(path, container, rule);

			return container;
		}

#endif
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__FILESYSTEM__H__
