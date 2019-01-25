#ifndef __TINY_TOOLKIT__UTILITIES__FILE__H__
#define __TINY_TOOLKIT__UTILITIES__FILE__H__


/**
 *
 *  作者: hm
 *
 *  说明: 文件操作
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	class TINY_TOOLKIT_API LockFile
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		LockFile() = default;

		/**
		 *
		 * 构造函数
		 *
		 * @param path 待打开文件路径
		 * @param truncate 是否覆盖文件
		 *
		 */
		explicit LockFile(std::string path, bool truncate = false);

		/**
		 *
		 * 析构函数
		 *
		 */
		~LockFile();

		/**
		 *
		 * 打开文件
		 *
		 * @param path 待打开文件路径
		 * @param truncate 是否覆盖文件
		 *
		 * @return 是否打开成功
		 *
		 */
		bool Open(std::string path, bool truncate = false);

		/**
		 *
		 * 关闭文件
		 *
		 * @return 是否关闭成功
		 *
		 */
		bool Close();

		/**
		 *
		 * 是否已经打开文件
		 *
		 * @return 是否已经打开文件
		 *
		 */
		bool IsOpen();

		/**
		 *
		 * 写入文件
		 *
		 * @param value 待写入数据
		 *
		 * @return 是否写入成功
		 *
		 */
		bool Write(const char * value);

		/**
		 *
		 * 写入文件
		 *
		 * @param value 待写入数据
		 *
		 * @return 是否写入成功
		 *
		 */
		bool Write(const std::string & value);

		/**
		 *
		 * 写入文件
		 *
		 * @param value 待写入数据
		 * @param size 待写入数据长度
		 *
		 * @return 是否写入成功
		 *
		 */
		bool Write(const char * value, std::size_t size);

		/**
		 *
		 * 写入文件
		 *
		 * @param value 待写入数据
		 * @param size 待写入数据长度
		 *
		 * @return 是否写入成功
		 *
		 */
		bool Write(const std::string & value, std::size_t size);

		/**
		 *
		 * 函数重载
		 *
		 * @param value 待写入数据
		 *
		 * @return 实例化对象
		 *
		 */
		LockFile & operator << (const char * value);

		/**
		 *
		 * 函数重载
		 *
		 * @param data 待写入数据
		 *
		 * @return 实例化对象
		 *
		 */
		LockFile & operator << (const std::string & value);

		/**
		 *
		 * 文件大小
		 *
		 * @return 文件大小
		 *
		 */
		std::size_t Size() const;

		/**
		 *
		 * 文件路径
		 *
		 * @return 文件路径
		 *
		 */
		const std::string & Path() const;

	protected:
		/**
		 *
		 * 锁定
		 *
		 * @return 是否锁定成功
		 *
		 */
		bool Lock();

		/**
		 *
		 * 创建文件
		 *
		 * @return 是否创建成功
		 *
		 */
		bool Create();

		/**
		 *
		 * 设置描诉符
		 *
		 * @param truncate 是否清空文件
		 *
		 * @return 是否设置成功
		 *
		 */
		bool Execl(bool truncate);

	protected:
		int32_t _fd{ -1 };

		std::size_t _len{ 0 };

		std::string _path{ };
	};
}


#endif // __TINY_TOOLKIT__UTILITIES__FILE__H__
