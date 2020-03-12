/**
 *
 *  作者: hm
 *
 *  说明: 文件处理
 *
 */


#include "filesystem.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <fstream>
#
#  include <io.h>
#  include <direct.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <fstream>
#
#  include <unistd.h>
#  include <dirent.h>
#
#  include <sys/stat.h>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <fstream>
#
#  include <unistd.h>
#  include <dirent.h>
#
#  include <sys/stat.h>
#
#endif


namespace tinyToolkit
{
	namespace util
	{
		/**
		 *
		 * 是否存在
		 *
		 * @param path 文件路径
		 *
		 * @return 是否存在
		 *
		 */
		bool Filesystem::Exists(const std::string & path)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			return ::_access(path.c_str(), S_OK) == 0;

		#else

			return ::access(path.c_str(), F_OK) == 0;

		#endif
		}

		/**
		 *
		 * 是否为目录
		 *
		 * @param path 文件路径
		 *
		 * @return 是否为目录
		 *
		 */
		bool Filesystem::IsDirectory(const std::string & path)
		{
			if (!Exists(path))
			{
				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			return ::GetFileAttributesA(path.c_str()) & FILE_ATTRIBUTE_DIRECTORY;

		#else

			struct stat status{ };

			if (::stat(path.c_str(), &status) == -1)
			{
				return false;
			}

			return S_ISDIR(status.st_mode);

		#endif
		}

		/**
	 *
	 * 删除目录
	 *
	 * @param path 文件路径
	 *
	 * @return 是否删除成功
	 *
	 */
		bool Filesystem::Remove(const std::string & path)
		{
			if (!Exists(path))
			{
				return true;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (!IsDirectory(path))
			{
				return ::DeleteFile(path.c_str()) == TRUE;
			}

			std::string dir{ };

			if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
			{
				dir.append(path);
				dir.append("*.*");
			}
			else
			{
				dir.append(path);
				dir.append(TINY_TOOLKIT_PATH_SEP);
				dir.append("*.*");
			}

			WIN32_FIND_DATA finder{ };

			HANDLE hFind = ::FindFirstFile(dir.c_str(), &finder);

			if (hFind == INVALID_HANDLE_VALUE)
			{
				return false;
			}

			do
			{
				if (strcmp(finder.cFileName, ".") == 0 || strcmp(finder.cFileName, "..") == 0)
				{
					continue;
				}

				std::string value{ };

				if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
				{
					value.append(path);
					value.append(finder.cFileName);
				}
				else
				{
					value.append(path);
					value.append(TINY_TOOLKIT_PATH_SEP);
					value.append(finder.cFileName);
				}

				if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (!Remove(value))
					{
						return false;
					}
				}
				else
				{
					if (::DeleteFile(value.c_str()) == FALSE)
					{
						return false;
					}
				}
			}
			while (::FindNextFile(hFind, &finder));

			if (::RemoveDirectory(path.c_str()) == FALSE)
			{
				return false;
			}

		#else

			if (!IsDirectory(path))
			{
				return std::remove(path.c_str()) == 0;
			}

			DIR * dir = ::opendir(path.c_str());

			if (dir == nullptr)
			{
				return false;
			}

			struct dirent * dirEvent = ::readdir(dir);

			while (dirEvent)
			{
				if (dirEvent->d_name[0] != '.')
				{
					std::string value{ };

					if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
					{
						value.append(path);
						value.append(dirEvent->d_name);
					}
					else
					{
						value.append(path);
						value.append(TINY_TOOLKIT_PATH_SEP);
						value.append(dirEvent->d_name);
					}

					if (IsDirectory(value))
					{
						if (!Remove(value))
						{
							::closedir(dir);

							return false;
						}
					}
					else
					{
						if (std::remove(value.c_str()) == -1)
						{
							::closedir(dir);

							return false;
						}
					}
				}

				dirEvent = ::readdir(dir);
			}

			if (std::remove(path.c_str()) == -1)
			{
				::closedir(dir);

				return false;
			}

			::closedir(dir);

		#endif

			return true;
		}

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
		bool Filesystem::Rename(const std::string & src, const std::string & dst)
		{
			return std::rename(src.c_str(), dst.c_str()) == 0;
		}

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
		bool Filesystem::WriteFile(const std::string & path, const std::string & content)
		{
			std::ofstream ofs(path, std::ios::binary);

			if (!ofs.is_open())
			{
				return false;
			}

			ofs << content;

			ofs.close();

			return true;
		}

		/**
		 *
		 * 创建文件
		 *
		 * @param path 文件路径
		 *
		 * @return 是否创建成功
		 *
		 */
		bool Filesystem::CreateFile(const std::string & path)
		{
			std::ofstream ofs(path, std::ios::binary);

			if (!ofs.is_open())
			{
				return false;
			}

			ofs.close();

			return true;
		}

		/**
		 *
		 * 创建文件夹
		 *
		 * @param path 文件夹路径
		 *
		 * @return 是否创建成功
		 *
		 */
		bool Filesystem::CreateDirectory(const std::string & path)
		{
		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (::_mkdir(path.c_str()) == -1)
			{
				return Exists(path);
			}

		#else

			if (::mkdir(path.c_str(), 0777) == -1)
			{
				return Exists(path);
			}

		#endif

			return true;
		}

		/**
		 *
		 * 创建文件夹
		 *
		 * @param path 文件夹路径
		 *
		 * @return 是否创建成功
		 *
		 */
		bool Filesystem::CreateDirectories(const std::string & path)
		{
			std::size_t size = path.size();

			for (std::size_t i = 0; i < size; ++i)
			{
				if (i == (size - 1))
				{
					if (!CreateDirectory(path))
					{
						return false;
					}
				}
				else if (path[i] == TINY_TOOLKIT_PATH_SEP[0])
				{
					if (i == 0)
					{
						continue;
					}

					if (!CreateDirectory(path.substr(0, i)))
					{
						return false;
					}
				}
			}

			return true;
		}

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
		bool Filesystem::ReadFile(const std::string & path, std::vector<std::string> & container, bool keepEmpty)
		{
			std::ifstream ifs(path, std::ios::binary);

			if (!ifs.is_open())
			{
				return false;
			}

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

			return true;
		}

		/**
		 *
		 * 文件大小
		 *
		 * @param path 文件路径
		 *
		 * @return 文件大小
		 *
		 */
		std::size_t Filesystem::Size(const std::string & path)
		{
			std::ifstream ifs(path, std::ifstream::ate | std::ifstream::binary);

			if (!ifs.is_open())
			{
				return 0;
			}

			std::size_t size = static_cast<std::size_t>(ifs.tellg());

			ifs.close();

			return size;
		}

		/**
		 *
		 * 当前目录
		 *
		 * @return 当前目录
		 *
		 */
		std::string Filesystem::CurrentDirectory()
		{
			char directory[TINY_TOOLKIT_PATH_MAX + 1]{ 0 };

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (::_getcwd(directory, TINY_TOOLKIT_PATH_MAX))
			{
				return directory;
			}

		#else

			if (::getcwd(directory, TINY_TOOLKIT_PATH_MAX))
			{
				return directory;
			}

		#endif

			return TINY_TOOLKIT_PATH_DEF;
		}

		/**
		 *
		 * 父级目录
		 *
		 * @param path 文件路径
		 *
		 * @return 父级目录
		 *
		 */
		std::string Filesystem::ParentDirectory(const std::string & path)
		{
			std::size_t pos = path.rfind(TINY_TOOLKIT_PATH_SEP[0]);

			if (pos == std::string::npos)
			{
				return CurrentDirectory();
			}

			return path.substr(0, pos);
		}

		/**
		 *
		 * 文件名
		 *
		 * @param path 文件路径
		 *
		 * @return 文件名
		 *
		 */
		std::string Filesystem::Name(const std::string & path)
		{
			std::size_t pos = path.rfind(TINY_TOOLKIT_PATH_SEP[0]);

			if (pos == std::string::npos)
			{
				return path;
			}

			return path.substr(pos + 1);
		}

		/**
		 *
		 * 文件前缀
		 *
		 * @param path 文件路径
		 *
		 * @return 文件前缀
		 *
		 */
		std::string Filesystem::Steam(const std::string & path)
		{
			std::size_t srcPos = path.rfind(TINY_TOOLKIT_PATH_SEP[0]);
			std::size_t dstPos = path.rfind('.');

			if (srcPos == std::string::npos)
			{
				if (dstPos == std::string::npos)
				{
					return path;
				}

				return path.substr(0, dstPos);
			}

			if (dstPos == std::string::npos || dstPos < srcPos)
			{
				return path.substr(srcPos + 1);
			}

			return path.substr(srcPos + 1, dstPos - srcPos - 1);
		}

		/**
		 *
		 * 文件扩展名
		 *
		 * @param path 文件路径
		 *
		 * @return 文件扩展名
		 *
		 */
		std::string Filesystem::Extension(const std::string & path)
		{
			std::size_t srcPos = path.rfind(TINY_TOOLKIT_PATH_SEP[0]);
			std::size_t dstPos = path.rfind('.');

			if (srcPos == std::string::npos)
			{
				if (dstPos == std::string::npos)
				{
					return { };
				}

				return path.substr(dstPos);
			}

			if (dstPos == std::string::npos || dstPos < srcPos)
			{
				return { };
			}

			return path.substr(dstPos);
		}

		/**
		 *
		 * 文件内容
		 *
		 * @param path 文件路径
		 *
		 * @return 文件内容
		 *
		 */
		std::string Filesystem::Content(const std::string & path)
		{
			std::ifstream ifs(path, std::ios::binary);

			if (!ifs.is_open())
			{
				return { };
			}

			std::string str
			(
				(std::istreambuf_iterator<char>(ifs)),
				(std::istreambuf_iterator<char>())
			);

			ifs.close();

			return str;
		}

		/**
		 *
		 * 绝对路径
		 *
		 * @param path 文件路径
		 *
		 * @return 绝对路径
		 *
		 */
		std::string Filesystem::Canonical(const std::string & path)
		{
			char directory[TINY_TOOLKIT_PATH_MAX + 1]{ 0 };

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			if (::_fullpath(directory, path.c_str(), TINY_TOOLKIT_PATH_MAX))
			{
				return directory;
			}

		#else

			if (::realpath(path.c_str(), directory))
			{
				return directory;
			}

		#endif

			return TINY_TOOLKIT_PATH_DEF;
		}

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
		bool Filesystem::TraverseFile(const std::string & path, std::vector<std::string> & container, bool subdirectory)
		{
			if (!Exists(path))
			{
				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			std::string dir{ };

			if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
			{
				dir.append(path);
				dir.append("*.*");
			}
			else
			{
				dir.append(path);
				dir.append(TINY_TOOLKIT_PATH_SEP);
				dir.append("*.*");
			}

			WIN32_FIND_DATA finder{ };

			HANDLE hFind = ::FindFirstFile(dir.c_str(), &finder);

			if (hFind == INVALID_HANDLE_VALUE)
			{
				return false;
			}

			do
			{
				if (strcmp(finder.cFileName, ".") == 0 || strcmp(finder.cFileName, "..") == 0)
				{
					continue;
				}

				std::string value{ };

				if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
				{
					value.append(path);
					value.append(finder.cFileName);
				}
				else
				{
					value.append(path);
					value.append(TINY_TOOLKIT_PATH_SEP);
					value.append(finder.cFileName);
				}

				if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (subdirectory)
					{
						if (!TraverseFile(value, container, subdirectory))
						{
							return false;
						}
					}
				}
				else
				{
					container.push_back(value);
				}
			}
			while (::FindNextFile(hFind, &finder));

		#else

			DIR * dir = ::opendir(path.c_str());

			if (dir == nullptr)
			{
				return false;
			}

			struct dirent * dirEvent = ::readdir(dir);

			while (dirEvent)
			{
				if (dirEvent->d_name[0] != '.')
				{
					std::string value{ };

					if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
					{
						value.append(path);
						value.append(dirEvent->d_name);
					}
					else
					{
						value.append(path);
						value.append(TINY_TOOLKIT_PATH_SEP);
						value.append(dirEvent->d_name);
					}

					if (IsDirectory(value))
					{
						if (subdirectory)
						{
							if (!TraverseFile(value, container, subdirectory))
							{
								::closedir(dir);

								return false;
							}
						}
					}
					else
					{
						container.push_back(value);
					}
				}

				dirEvent = ::readdir(dir);
			}

			::closedir(dir);

		#endif

			return true;
		}

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
		bool Filesystem::TraverseFile(const std::string & path, std::vector<std::string> & container, const std::regex & rule, bool subdirectory)
		{
			if (!Exists(path))
			{
				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			std::string dir{ };

			if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
			{
				dir.append(path);
				dir.append("*.*");
			}
			else
			{
				dir.append(path);
				dir.append(TINY_TOOLKIT_PATH_SEP);
				dir.append("*.*");
			}

			WIN32_FIND_DATA finder{ };

			HANDLE hFind = ::FindFirstFile(dir.c_str(), &finder);

			if (hFind == INVALID_HANDLE_VALUE)
			{
				return false;
			}

			do
			{
				if (strcmp(finder.cFileName, ".") == 0 || strcmp(finder.cFileName, "..") == 0)
				{
					continue;
				}

				std::string value{ };

				if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
				{
					value.append(path);
					value.append(finder.cFileName);
				}
				else
				{
					value.append(path);
					value.append(TINY_TOOLKIT_PATH_SEP);
					value.append(finder.cFileName);
				}

				if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (subdirectory)
					{
						if (!TraverseFile(value, container, rule, subdirectory))
						{
							return false;
						}
					}
				}
				else
				{
					if (std::regex_match(value, rule))
					{
						container.push_back(value);
					}
				}
			}
			while (::FindNextFile(hFind, &finder));

		#else

			DIR * dir = ::opendir(path.c_str());

			if (dir == nullptr)
			{
				return false;
			}

			struct dirent * dirEvent = ::readdir(dir);

			while (dirEvent)
			{
				if (dirEvent->d_name[0] != '.')
				{
					std::string value{ };

					if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
					{
						value.append(path);
						value.append(dirEvent->d_name);
					}
					else
					{
						value.append(path);
						value.append(TINY_TOOLKIT_PATH_SEP);
						value.append(dirEvent->d_name);
					}

					if (IsDirectory(value))
					{
						if (subdirectory)
						{
							if (!TraverseFile(value, container, rule, subdirectory))
							{
								::closedir(dir);

								return false;
							}
						}
					}
					else
					{
						if (std::regex_match(value, rule))
						{
							container.push_back(value);
						}
					}
				}

				dirEvent = ::readdir(dir);
			}

			::closedir(dir);

		#endif

			return false;
		}

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
		bool Filesystem::TraverseDirectory(const std::string & path, std::vector<std::string> & container, bool subdirectory)
		{
			if (!Exists(path))
			{
				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			std::string dir{ };

			if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
			{
				dir.append(path);
				dir.append("*.*");
			}
			else
			{
				dir.append(path);
				dir.append(TINY_TOOLKIT_PATH_SEP);
				dir.append("*.*");
			}

			WIN32_FIND_DATA finder{ };

			HANDLE hFind = ::FindFirstFile(dir.c_str(), &finder);

			if (hFind == INVALID_HANDLE_VALUE)
			{
				return false;
			}

			do
			{
				if (strcmp(finder.cFileName, ".") == 0 || strcmp(finder.cFileName, "..") == 0)
				{
					continue;
				}

				std::string value{ };

				if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
				{
					value.append(path);
					value.append(finder.cFileName);
				}
				else
				{
					value.append(path);
					value.append(TINY_TOOLKIT_PATH_SEP);
					value.append(finder.cFileName);
				}

				if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					container.push_back(value);

					if (subdirectory)
					{
						if (!TraverseDirectory(value, container, subdirectory))
						{
							return false;
						}
					}
				}
			}
			while (::FindNextFile(hFind, &finder));

		#else

			DIR * dir = ::opendir(path.c_str());

			if (dir == nullptr)
			{
				return false;
			}

			struct dirent * dirEvent = ::readdir(dir);

			while (dirEvent)
			{
				if (dirEvent->d_name[0] != '.')
				{
					std::string value{ };

					if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
					{
						value.append(path);
						value.append(dirEvent->d_name);
					}
					else
					{
						value.append(path);
						value.append(TINY_TOOLKIT_PATH_SEP);
						value.append(dirEvent->d_name);
					}

					if (IsDirectory(value))
					{
						container.push_back(value);

						if (subdirectory)
						{
							if (!TraverseDirectory(value, container, subdirectory))
							{
								::closedir(dir);

								return false;
							}
						}
					}
				}

				dirEvent = ::readdir(dir);
			}

			::closedir(dir);

		#endif

			return true;
		}

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
		bool Filesystem::TraverseDirectory(const std::string & path, std::vector<std::string> & container, const std::regex & rule, bool subdirectory)
		{
			if (!Exists(path))
			{
				return false;
			}

		#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

			std::string dir{ };

			if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
			{
				dir.append(path);
				dir.append("*.*");
			}
			else
			{
				dir.append(path);
				dir.append(TINY_TOOLKIT_PATH_SEP);
				dir.append("*.*");
			}

			WIN32_FIND_DATA finder{ };

			HANDLE hFind = ::FindFirstFile(dir.c_str(), &finder);

			if (hFind == INVALID_HANDLE_VALUE)
			{
				return false;
			}

			do
			{
				if (strcmp(finder.cFileName, ".") == 0 || strcmp(finder.cFileName, "..") == 0)
				{
					continue;
				}

				std::string value{ };

				if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
				{
					value.append(path);
					value.append(finder.cFileName);
				}
				else
				{
					value.append(path);
					value.append(TINY_TOOLKIT_PATH_SEP);
					value.append(finder.cFileName);
				}

				if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (std::regex_match(value, rule))
					{
						container.push_back(value);
					}

					if (subdirectory)
					{
						if (!TraverseDirectory(value, container, rule, subdirectory))
						{
							return false;
						}
					}
				}
			}
			while (::FindNextFile(hFind, &finder));

		#else

			DIR * dir = ::opendir(path.c_str());

			if (dir == nullptr)
			{
				return false;
			}

			struct dirent * dirEvent = ::readdir(dir);

			while (dirEvent)
			{
				if (dirEvent->d_name[0] != '.')
				{
					std::string value{ };

					if (path[path.size() - 1] == TINY_TOOLKIT_PATH_SEP[0])
					{
						value.append(path);
						value.append(dirEvent->d_name);
					}
					else
					{
						value.append(path);
						value.append(TINY_TOOLKIT_PATH_SEP);
						value.append(dirEvent->d_name);
					}

					if (IsDirectory(value))
					{
						if (std::regex_match(value, rule))
						{
							container.push_back(value);
						}

						if (subdirectory)
						{
							if (!TraverseDirectory(value, container, rule, subdirectory))
							{
								::closedir(dir);

								return false;
							}
						}
					}
				}

				dirEvent = ::readdir(dir);
			}

			::closedir(dir);

		#endif

			return true;
		}
	}
}
