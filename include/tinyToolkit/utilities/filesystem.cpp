/**
 *
 *  作者: hm
 *
 *  说明: 文件处理
 *
 */


#include "filesystem.h"


namespace tinyToolkit
{
	/**
	 *
	 * 文件是否存在
	 *
	 * @param path 文件路径
	 *
	 * @return 文件是否存在
	 *
	 */
	bool Filesystem::Exists(const std::string & path)
	{
	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		return _access(path.c_str(), S_OK) == 0;

	#else

		return access(path.c_str(), F_OK) == 0;

	#endif // TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
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
	bool Filesystem::Remove(const std::string & path)
	{
		return std::remove(path.c_str()) == 0;
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
	bool Filesystem::Rename(const std::string & src, const std::string & dst)
	{
		if (std::rename(src.c_str(), dst.c_str()) == 0)
		{
			return true;
		}
		else
		{
			throw std::runtime_error("Failed Rename [" + src + "] to [" + dst + "]");
		}
	}

	/**
	 *
	 * 是否为目录
	 *
	 * @param path 待检测文件路径
	 *
	 * @return 是否为目录
	 *
	 */
	bool Filesystem::IsDirectory(const std::string & path)
	{
	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		return GetFileAttributesA(path.c_str()) & FILE_ATTRIBUTE_DIRECTORY;

	#else

		struct stat status{ };

		if (::stat(path.c_str(), &status) == -1)
		{
			throw std::runtime_error("Failed stat : [" + path + "]");
		}

		return S_ISDIR(status.st_mode);

	#endif // TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
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

		if (ifs.is_open())
		{
			std::size_t size = static_cast<std::size_t>(ifs.tellg());

			ifs.close();

			return size;
		}
		else
		{
			throw std::runtime_error("Failed open file : [" + path + "]");
		}
	}

	/**
	 *
	 * 文件内容
	 *
	 * @param path 待读取文件路径
	 *
	 * @return 文件内容
	 *
	 */
	std::string Filesystem::Content(const std::string & path)
	{
		std::ifstream ifs(path, std::ios::binary);

		if (ifs.is_open())
		{
			std::string str((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

			ifs.close();

			return str;
		}
		else
		{
			throw std::runtime_error("Failed open file : [" + path + "]");
		}
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
	std::size_t Filesystem::ReadFile(const std::string & path, std::vector<std::string> & container, bool keepEmpty)
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
		else
		{
			throw std::runtime_error("Failed open file : [" + path + "]");
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
	std::vector<std::string> Filesystem::ReadFile(const std::string & path, bool keepEmpty)
	{
		std::vector<std::string> container;

		ReadFile(path, container, keepEmpty);

		return container;
	}

	/**
	 * 创建文件
	 *
	 * @param path 待创建文件路径
	 *
	 * @return 创建结果
	 *
	 */
	bool Filesystem::CreateFile(const std::string & path)
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
	 * 创建文件夹
	 *
	 * @param path 待创建文件路径
	 *
	 * @return 创建结果
	 *
	 */
	bool Filesystem::CreateDirectory(const std::string & path)
	{
	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		if (_mkdir(path.c_str()) == -1)
		{
			if (!Exists(path))
			{
				throw std::runtime_error("Failed create directory : [" + path + "]");
			}
		}

		return true;

	#else

		if (mkdir(path.c_str(), 0777) == -1)
		{
			if (!Exists(path))
			{
				throw std::runtime_error("Failed create directory : [" + path + "]");
			}
		}

		return true;

	#endif // TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
	}

	/**
	 *
	 * 创建层级目录
	 *
	 * @param path 待创建目录路径
	 *
	 * @return 创建结果
	 *
	 */
	bool Filesystem::CreateDirectories(const std::string & path)
	{
		std::size_t size = path.size();

		for (std::size_t i = 0; i < size; ++i)
		{
			if (i == size - 1)
			{
				if (!CreateDirectory(path))
				{
					return false;
				}
			}
			else if (path[i] == TINY_TOOLKIT_FOLDER_SEP[0])
			{
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
	 * 当前目录
	 *
	 * @return 当前目录
	 *
	 */
	std::string Filesystem::CurrentDirectory()
	{
		char directory[TINY_TOOLKIT_PATH_MAX + 1] = { 0 };

#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		return _getcwd(directory, TINY_TOOLKIT_PATH_MAX) ? directory : TINY_TOOLKIT_FOLDER_EOL;

#else

		return getcwd(directory, TINY_TOOLKIT_PATH_MAX) ? directory : TINY_TOOLKIT_FOLDER_EOL;

#endif // TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
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
	std::string Filesystem::Name(const std::string & path)
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
	std::string Filesystem::Steam(const std::string & path)
	{
		std::size_t srcPos = path.rfind(TINY_TOOLKIT_FOLDER_SEP[0]);
		std::size_t dstPos = path.rfind('.');

		if (srcPos == std::string::npos)
		{
			return dstPos == std::string::npos ? path : path.substr(0, dstPos);
		}
		else
		{
			return (dstPos == std::string::npos || dstPos < srcPos) ? path.substr(srcPos + 1) : path.substr(srcPos + 1, dstPos - srcPos - 1);
		}
	}

	/**
	 *
	 * 绝对路径
	 *
	 * @param path 待处理路径
	 *
	 * @return 绝对路径
	 *
	 */
	std::string Filesystem::Canonical(const std::string & path)
	{
		char directory[TINY_TOOLKIT_PATH_MAX + 1] = { 0 };

	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		return _fullpath(directory, path.c_str(), TINY_TOOLKIT_PATH_MAX) ? directory : TINY_TOOLKIT_FOLDER_EOL;

	#else

		return realpath(path.c_str(), directory) ? directory : TINY_TOOLKIT_FOLDER_EOL;

	#endif // TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
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
	std::string Filesystem::Extension(const std::string & path)
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
	std::string Filesystem::ParentDirectory(const std::string & path)
	{
		std::size_t pos = path.rfind(TINY_TOOLKIT_FOLDER_SEP[0]);

		if (pos == std::string::npos)
		{
			return CurrentDirectory();
		}
		else
		{
			return path.substr(0, pos);
		}
	}

	/**
	 *
	 * 遍历文件
	 *
	 * @param path 待遍历根目录路径
	 * @param container 文件结果容器
	 * @param subdirectory 是否遍历子目录
	 *
	 */
	void Filesystem::TraverseFile(const std::string & path, std::vector<std::string> & container, bool subdirectory)
	{
	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		std::string dir{ };

		if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
		{
			dir = path + "*.*";
		}
		else
		{
			dir = path + TINY_TOOLKIT_FOLDER_SEP + "*.*";
		}

		WIN32_FIND_DATA finder{ };

		HANDLE hFind = FindFirstFile(dir.c_str(), &finder);

		if (hFind == INVALID_HANDLE_VALUE)
		{
			throw std::runtime_error("No such file or directory : [" + path + "]");
		}

		do
		{
			if (strcmp(finder.cFileName, ".") == 0 || strcmp(finder.cFileName, "..") == 0)
			{
				continue;
			}

			std::string value{ };

			if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
			{
				value = path + finder.cFileName;
			}
			else
			{
				value = path + TINY_TOOLKIT_FOLDER_SEP + finder.cFileName;
			}

			if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (subdirectory)
				{
					TraverseFile(value, container, subdirectory);
				}
			}
			else
			{
				container.push_back(value);
			}
		}
		while (FindNextFile(hFind, &finder));

	#else

		DIR * dir = opendir(path.c_str());

		if (dir == nullptr)
		{
			throw std::runtime_error("No such file or directory : [" + path + "]");
		}

		struct dirent * dirEvent = readdir(dir);

		while (dirEvent)
		{
			if (dirEvent->d_name[0] != '.')
			{
				std::string value{ };

				if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
				{
					value = path + dirEvent->d_name;
				}
				else
				{
					value = path + TINY_TOOLKIT_FOLDER_SEP + dirEvent->d_name;
				}

				if (IsDirectory(value))
				{
					if (subdirectory)
					{
						TraverseFile(value, container, subdirectory);
					}
				}
				else
				{
					container.push_back(value);
				}
			}

			dirEvent = readdir(dir);
		}

	#endif // TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
	}

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
	void Filesystem::TraverseFile(const std::string & path, std::vector<std::string> & container, const std::regex & rule, bool subdirectory)
	{
	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		std::string dir{ };

		if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
		{
			dir = path + "*.*";
		}
		else
		{
			dir = path + TINY_TOOLKIT_FOLDER_SEP + "*.*";
		}

		WIN32_FIND_DATA finder{ };

		HANDLE hFind = FindFirstFile(dir.c_str(), &finder);

		if (hFind == INVALID_HANDLE_VALUE)
		{
			throw std::runtime_error("No such file or directory : [" + path + "]");
		}

		do
		{
			if (strcmp(finder.cFileName, ".") == 0 || strcmp(finder.cFileName, "..") == 0)
			{
				continue;
			}

			std::string value{ };

			if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
			{
				value = path + finder.cFileName;
			}
			else
			{
				value = path + TINY_TOOLKIT_FOLDER_SEP + finder.cFileName;
			}

			if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (subdirectory)
				{
					TraverseFile(value, container, rule, subdirectory);
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
		while (FindNextFile(hFind, &finder));

	#else

		DIR * dir = opendir(path.c_str());

		if (dir == nullptr)
		{
			throw std::runtime_error("No such file or directory : [" + path + "]");
		}

		struct dirent * dirEvent = readdir(dir);

		while (dirEvent)
		{
			if (dirEvent->d_name[0] != '.')
			{
				std::string value{ };

				if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
				{
					value = path + dirEvent->d_name;
				}
				else
				{
					value = path + TINY_TOOLKIT_FOLDER_SEP + dirEvent->d_name;
				}

				if (IsDirectory(value))
				{
					if (subdirectory)
					{
						TraverseFile(value, container, rule, subdirectory);
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

			dirEvent = readdir(dir);
		}

	#endif // TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
	}

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
	std::vector<std::string> Filesystem::TraverseFile(const std::string & path, bool subdirectory)
	{
		std::vector<std::string> container;

		TraverseFile(path, container, subdirectory);

		return container;
	}

	/**
	 *
	 * 遍历文件
	 *
	 * @param path 待遍历根目录路径
	 * @param subdirectory 是否遍历子目录
	 * @param rule 遍历规则
	 *
	 * @return 文件结果容器
	 *
	 */
	std::vector<std::string> Filesystem::TraverseFile(const std::string & path, const std::regex & rule, bool subdirectory)
	{
		std::vector<std::string> container;

		TraverseFile(path, container, rule, subdirectory);

		return container;
	}

	/**
	 *
	 * 遍历目录
	 *
	 * @param path 待遍历根目录路径
	 * @param container 目录结果容器
	 * @param subdirectory 是否遍历子目录
	 *
	 */
	void Filesystem::TraverseDirectory(const std::string & path, std::vector<std::string> & container, bool subdirectory)
	{
	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		std::string dir{ };

		if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
		{
			dir = path + "*.*";
		}
		else
		{
			dir = path + TINY_TOOLKIT_FOLDER_SEP + "*.*";
		}

		WIN32_FIND_DATA finder{ };

		HANDLE hFind = FindFirstFile(dir.c_str(), &finder);

		if (hFind == INVALID_HANDLE_VALUE)
		{
			throw std::runtime_error("No such file or directory : [" + path + "]");
		}

		do
		{
			if (strcmp(finder.cFileName, ".") == 0 || strcmp(finder.cFileName, "..") == 0)
			{
				continue;
			}

			std::string value{ };

			if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
			{
				value = path + finder.cFileName;
			}
			else
			{
				value = path + TINY_TOOLKIT_FOLDER_SEP + finder.cFileName;
			}

			if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				container.push_back(value);

				if (subdirectory)
				{
					TraverseDirectory(value, container, subdirectory);
				}
			}
		}
		while (FindNextFile(hFind, &finder));

	#else

		DIR * dir = opendir(path.c_str());

		if (dir == nullptr)
		{
			throw std::runtime_error("No such file or directory : [" + path + "]");
		}

		struct dirent * dirEvent = readdir(dir);

		while (dirEvent)
		{
			if (dirEvent->d_name[0] != '.')
			{
				std::string value{ };

				if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
				{
					value = path + dirEvent->d_name;
				}
				else
				{
					value = path + TINY_TOOLKIT_FOLDER_SEP + dirEvent->d_name;
				}

				if (IsDirectory(value))
				{
					container.push_back(value);

					if (subdirectory)
					{
						TraverseDirectory(value, container, subdirectory);
					}
				}
			}

			dirEvent = readdir(dir);
		}

	#endif // TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
	}

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
	void Filesystem::TraverseDirectory(const std::string & path, std::vector<std::string> & container, const std::regex & rule, bool subdirectory)
	{
	#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS

		std::string dir{ };

		if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
		{
			dir = path + "*.*";
		}
		else
		{
			dir = path + TINY_TOOLKIT_FOLDER_SEP + "*.*";
		}

		WIN32_FIND_DATA finder{ };

		HANDLE hFind = FindFirstFile(dir.c_str(), &finder);

		if (hFind == INVALID_HANDLE_VALUE)
		{
			throw std::runtime_error("No such file or directory : [" + path + "]");
		}

		do
		{
			if (strcmp(finder.cFileName, ".") == 0 || strcmp(finder.cFileName, "..") == 0)
			{
				continue;
			}

			std::string value{ };

			if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
			{
				value = path + finder.cFileName;
			}
			else
			{
				value = path + TINY_TOOLKIT_FOLDER_SEP + finder.cFileName;
			}

			if (finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (std::regex_match(value, rule))
				{
					container.push_back(value);
				}

				if (subdirectory)
				{
					TraverseDirectory(value, container, rule, subdirectory);
				}
			}
		}
		while (FindNextFile(hFind, &finder));

	#else

		DIR * dir = opendir(path.c_str());

		if (dir == nullptr)
		{
			throw std::runtime_error("No such file or directory : [" + path + "]");
		}

		struct dirent * dirEvent = readdir(dir);

		while (dirEvent)
		{
			if (dirEvent->d_name[0] != '.')
			{
				std::string value{ };

				if (path[path.size() - 1] == TINY_TOOLKIT_FOLDER_SEP[0])
				{
					value = path + dirEvent->d_name;
				}
				else
				{
					value = path + TINY_TOOLKIT_FOLDER_SEP + dirEvent->d_name;
				}

				if (IsDirectory(value))
				{
					if (std::regex_match(value, rule))
					{
						container.push_back(value);
					}

					if (subdirectory)
					{
						TraverseDirectory(value, container, rule, subdirectory);
					}
				}
			}

			dirEvent = readdir(dir);
		}

	#endif // TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
	}

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
	std::vector<std::string> Filesystem::TraverseDirectory(const std::string & path, bool subdirectory)
	{
		std::vector<std::string> container;

		TraverseDirectory(path, container, subdirectory);

		return container;
	}

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
	std::vector<std::string> Filesystem::TraverseDirectory(const std::string & path, const std::regex & rule, bool subdirectory)
	{
		std::vector<std::string> container;

		TraverseDirectory(path, container, rule, subdirectory);

		return container;
	}
}
