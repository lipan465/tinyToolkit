/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


class Path
{
public:
	/**
	 *
	 * 构造函数
	 *
	 */
	Path() = default;

	/**
	 *
	 * 构造函数
	 *
	 * @param path 文件路径
	 *
	 */
	explicit Path(std::string path) : _path(std::move(path))
	{
		Normalize();
	}

	/**
	 *
	 * 构造函数
	 *
	 * @param rhs 实例化对象
	 *
	 */
	Path(Path && rhs) noexcept : _path(std::move(rhs._path))
	{

	}

	/**
	 *
	 * 构造函数
	 *
	 * @param lhs 实例化对象
	 *
	 */
	Path(const Path & lhs) = default;

	/**
	 *
	 * 路径
	 *
	 * @return 路径
	 *
	 */
	const std::string & String() const
	{
		return _path;
	}

	/**
	 *
	 * 重载=操作
	 *
	 * @param rhs 实例化对象
	 *
	 */
	Path & operator=(Path && rhs) noexcept
	{
		_path = std::move(rhs._path);

		return *this;
	}

	/**
	 *
	 * 重载=操作
	 *
	 * @param lhs 实例化对象
	 *
	 */
	Path & operator=(const Path & lhs)
	{
		if (this != &lhs)
		{
			_path = lhs._path;
		}

		return *this;
	}

	/**
	 *
	 * 重载=操作
	 *
	 * @param path 文件路径
	 *
	 */
	Path & operator=(const std::string & path)
	{
		_path = path;

		Normalize();

		return *this;
	}

	/**
	 *
	 * 重载=操作
	 *
	 * @param path 文件路径
	 *
	 */
	template <typename ValueTypeT>
	Path & operator=(const ValueTypeT & path)
	{
		_path = path;

		Normalize();

		return *this;
	}

	/**
	 *
	 * 重载<<操作
	 *
	 * @param value 待写入数据
	 *
	 * @return 实例化对象
	 *
	 */
	Path & operator <<(const Path & lhs)
	{
		_path += TINY_TOOLKIT_FOLDER_SEP;
		_path += lhs._path;

		Normalize();

		return *this;
	}

	/**
	 *
	 * 重载<<操作
	 *
	 * @param value 待写入数据
	 *
	 * @return 实例化对象
	 *
	 */
	Path & operator <<(const std::string & value)
	{
		_stream << value;

		return *this;
	}

	/**
	 *
	 * 重载<<操作
	 *
	 * @tparam ValueTypeT [all built-in types]
	 *
	 * @param value 待写入数据
	 *
	 * @return 实例化对象
	 *
	 */
	template <typename ValueTypeT>
	Path & operator <<(const ValueTypeT & value)
	{
		_stream << value;

		return *this;
	}

	/**
	 *
	 * 重载<<操作
	 *
	 * @tparam ValueTypeT [all built-in types]
	 *
	 * @param value 待写入数据
	 *
	 * @return 实例化对象
	 *
	 */
	template <typename ValueTypeT>
	Path & operator <<(const ValueTypeT * value)
	{
		if (value == nullptr)
		{
			_stream << "(null)";
		}
		else
		{
			_stream << value;
		}

		return *this;
	}

protected:
	/**
	 *
	 * 正常化
	 *
	 */
	void Normalize()
	{
		char value[_path.length() + 1];

		memset(value, 0, sizeof(value));

		char * src = value;
		const char * dst = _path.c_str();

		while (*dst != '\0')
		{
			*src = *dst;

			if (IsSeparator(*dst))
			{
				while (IsSeparator(*dst))
				{
					++dst;
				}
			}
			else
			{
				++dst;
			}

			++src;

		}

		*src = '\0';

		_path = value;
	}

	/**
	 *
	 * 是否是分隔符
	 *
	 * @param value 待检测字符
	 *
	 * @return 是否是分隔符
	 *
	 */
	bool IsSeparator(char value)
	{
		return value == TINY_TOOLKIT_FOLDER_CHA;
	}

protected:
	std::string _path;
};

int main(int argc, char * argv[])
{
	std::filesystem::path path1;

	Path path("123///456///789");

	std::cout << path.String() << std::endl;

	std::cout << mkdir("1", 0777) << std::endl;

//	return TINY_TOOLKIT_RUN_ALL_TESTS();

//	ID::Test();
//	Log::Test();
//	Pool::Test();
//	Debug::Test();
//	System::Test();
//	Crypto::Test();
//	Utilities::Test();

//	getchar();

	return 0;
}
