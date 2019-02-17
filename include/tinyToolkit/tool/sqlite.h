#ifndef __TINY_TOOLKIT__TOOL__SQLITE__H__
#define __TINY_TOOLKIT__TOOL__SQLITE__H__


/**
 *
 *  作者: hm
 *
 *  说明: sqlite客户端
 *
 */


#ifdef WITH_SQLITE


#include <sqlite3.h>

#include "../debug/exception.h"


namespace tinyToolkit
{
	INHERITANCE_DECLARE(SqliteException, IException)

	struct SqliteBlobValue
	{
		char * data{ nullptr };

		std::size_t size{ 0 };
	};

	class TINY_TOOLKIT_API SqliteBind
	{
	public:
		/**
		 *
		 * 绑定参数
		 *
		 * @return 绑定结果
		 *
		 */
		static int32_t BindParams(sqlite3_stmt *, int32_t);

		/**
		 *
		 * 绑定参数
		 *
		 * @tparam Arg [all built-in types]
		 * @tparam Args [all built-in types]
		 *
		 * @param statement sqlite对象
		 * @param index 索引
		 * @param arg 待绑定值
		 * @param args 待绑定值
		 *
		 * @return 绑定结果
		 *
		 */
		template <typename Arg, typename... Args>
		static int32_t BindParams(sqlite3_stmt * statement, int32_t index, Arg && arg, Args &&... args)
		{
			int32_t code = BindValue(statement, index, arg);

			if (code != SQLITE_OK)
			{
				return code;
			}

			code = BindParams(statement, index + 1, std::forward<Args>(args)...);

			return code;
		}

	protected:
		/**
		 *
		 * 绑定空数据
		 *
		 * @param statement sqlite对象
		 * @param index 索引
		 *
		 * @return 绑定结果
		 *
		 */
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, const std::nullptr_t &);

		/**
		 *
		 * 绑定浮点型数据
		 *
		 * @param statement sqlite对象
		 * @param index 索引
		 * @param value 待绑定的数据
		 *
		 * @return 绑定结果
		 *
		 */
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, std::float_t value);

		/**
		 *
		 * 绑定浮点型数据
		 *
		 * @param statement sqlite对象
		 * @param index 索引
		 * @param value 待绑定的数据
		 *
		 * @return 绑定结果
		 *
		 */
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, std::double_t value);

		/**
		 *
		 * 绑定整形数据
		 *
		 * @param statement sqlite对象
		 * @param index 索引
		 * @param value 待绑定的数据
		 *
		 * @return 绑定结果
		 *
		 */
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, int32_t value);

		/**
		 *
		 * 绑定整形数据
		 *
		 * @param statement sqlite对象
		 * @param index 索引
		 * @param value 待绑定的数据
		 *
		 * @return 绑定结果
		 *
		 */
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, uint32_t value);

		/**
		 *
		 * 绑定整形数据
		 *
		 * @param statement sqlite对象
		 * @param index 索引
		 * @param value 待绑定的数据
		 *
		 * @return 绑定结果
		 *
		 */
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, int64_t value);

		/**
		 *
		 * 绑定整形数据
		 *
		 * @param statement sqlite对象
		 * @param index 索引
		 * @param value 待绑定的数据
		 *
		 * @return 绑定结果
		 *
		 */
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, uint64_t value);

		/**
		 *
		 * 绑定字符串数据
		 *
		 * @param statement sqlite对象
		 * @param index 索引
		 * @param value 待绑定的数据
		 *
		 * @return 绑定结果
		 *
		 */
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, const char * value);

		/**
		 *
		 * 绑定二进制数据
		 *
		 * @param statement sqlite对象
		 * @param index 索引
		 * @param value 待绑定的数据
		 *
		 * @return 绑定结果
		 *
		 */
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, const SqliteBlobValue & value);

		/**
		 *
		 * 绑定字符串数据
		 *
		 * @param statement sqlite对象
		 * @param index 索引
		 * @param value 待绑定的数据
		 *
		 * @return 绑定结果
		 *
		 */
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, const std::string & value);
	};

	class TINY_TOOLKIT_API SqliteQuery
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param list 查询结果列表
		 * @param field 子段对应列表
		 * @param code 执行结果
		 *
		 */
		SqliteQuery(std::list<std::vector<std::string>> & list, std::unordered_map<std::string, std::size_t> & field, int32_t & code);

		/**
		 *
		 * 查询请求
		 *
		 * @param statement sql对象
		 *
		 * @return 是否查询成功
		 *
		 */
		bool Request(sqlite3_stmt * statement);

	protected:
		/**
		 *
		 * 读取表
		 *
		 * @param statement sqlite对象
		 *
		 * @return 是否读取成功
		 *
		 */
		bool ReadTable(sqlite3_stmt * statement);

		/**
		 *
		 * 读取行
		 *
		 * @param statement sqlite对象
		 *
		 * @return 是否读取成功
		 *
		 */
		bool ReadRow(sqlite3_stmt * statement);

	protected:
		int32_t & _code;

		int32_t _column{ 0 };

		std::list<std::vector<std::string>> & _list;

		std::unordered_map<std::string, std::size_t> & _field;
	};

	class TINY_TOOLKIT_API SqliteDataBase
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		SqliteDataBase();

		/**
		 *
		 * 析构函数
		 *
		 */
		~SqliteDataBase();

		/**
		 *
		 * 打开内存数据库
		 *
		 * @return 是否打开成功
		 *
		 */
		bool OpenMemory();

		/**
		 *
		 * 打开文件数据库
		 *
		 * @param path 数据库路径
		 *
		 * @return 是否打开成功
		 *
		 */
		bool OpenFile(const char * path);

		/**
		 *
		 * 打开文件数据库
		 *
		 * @param path 数据库路径
		 *
		 * @return 是否打开成功
		 *
		 */
		bool OpenFile(const std::string & path);

		/**
		 *
		 * 关闭数据库
		 *
		 * @return 是否关闭成功
		 *
		 */
		bool Close();

		/**
		 *
		 * 获取索引对应的值
		 *
		 * @tparam ValueTypeT [all built-in types]
		 *
		 * @param index 索引
		 *
		 * @return 获取的值
		 *
		 */
		template <typename ValueTypeT>
		ValueTypeT GetValue(std::size_t index)
		{
			std::stringstream ss;

			ValueTypeT value = ValueTypeT();

			ss << GetString(index);
			ss >> value;

			return value;
		}

		/**
		 *
		 * 获取索引对应的值
		 *
		 * @tparam ValueTypeT [all built-in types]
		 *
		 * @param field 索引名
		 *
		 * @return 获取的值
		 *
		 */
		template <typename ValueTypeT>
		ValueTypeT GetValue(const std::string & field)
		{
			auto find = _field.find(field);

			if (find == _field.end())
			{
				TINY_TOOLKIT_EXCEPTION_THROW(SqliteException, "Invalid field")
			}

			return GetValue<ValueTypeT>(find->second);
		}

		/**
		 *
		 * 获取索引对应的字符串
		 *
		 * @param index 索引
		 *
		 * @return 获取的值
		 *
		 */
		const std::string & GetString(std::size_t index);

		/**
		 *
		 * 获取索引对应的字符串
		 *
		 * @param field 索引名
		 *
		 * @return 获取的值
		 *
		 */
		const std::string & GetString(const std::string & field);

		/**
		 *
		 * 开启事务
		 *
		 * @return 是否开启成功
		 *
		 */
		bool Begin();

		/**
		 *
		 * 结束事务
		 *
		 * @return 是否结束成功
		 *
		 */
		bool Commit();

		/**
		 *
		 * 回滚事务
		 *
		 * @return 是否回滚成功
		 *
		 */
		bool Rollback();

		/**
		 *
		 * 预处理
		 *
		 * @param sql sql语句
		 *
		 * @return 是否处理成功
		 *
		 */
		bool Prepare(const char * sql);

		/**
		 *
		 * 执行sql语句
		 *
		 * @param sql sql语句
		 *
		 * @return 是否执行成功
		 *
		 */
		bool Execute(const char * sql);

		/**
		 *
		 * 执行sql语句
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param sql sql语句
		 * @param args 参数
		 *
		 * @return 是否执行成功
		 *
		 */
		template <typename... Args>
		bool Execute(const char * sql, Args &&... args)
		{
			if (!Prepare(sql))
			{
				return false;
			}

			if (!BindValue(std::forward<Args>(args)...))
			{
				return false;
			}

			return ExecuteQuery();
		}

		/**
		 *
		 * 绑定sql语句值
		 *
		 * @tparam Args [all built-in types]
		 *
		 * @param args 参数
		 *
		 * @return 是否绑定成功
		 *
		 */
		template <typename... Args>
		bool BindValue(Args &&... args)
		{
			_code = SqliteBind::BindParams(_statement, 1, std::forward<Args>(args)...);

			if (_code != SQLITE_OK)
			{
				return false;
			}

			return Execute();
		}

		/**
		 *
		 * 是否是有效的行
		 *
		 * @return 是否是有效的行
		 *
		 */
		bool IsValidRow();

		/**
		 *
		 * 移动到头一行
		 *
		 */
		void MoveToHeadRow();

		/**
		 *
		 * 移动到上一行
		 *
		 */
		void MoveToPrevRow();

		/**
		 *
		 * 移动到下一行
		 *
		 */
		void MoveToNextRow();

		/**
		 *
		 * 移动到结尾行
		 *
		 */
		void MoveToTailRow();

		/**
		 *
		 * 受影响的行
		 *
		 * @return 受影响的行
		 *
		 */
		int32_t AffectedRows() const;

		/**
		 *
		 * 最后一次错误代码
		 *
		 * @return 错误代码
		 *
		 */
		int32_t LastErrorCode() const;

		/**
		 *
		 * 记录的行数
		 *
		 * @return 记录的行数
		 *
		 */
		std::size_t RecordCount() const;

		/**
		 *
		 * 最后一次错误信息
		 *
		 * @return 错误信息
		 *
		 */
		const char * LastErrorMessage() const;

	protected:
		/**
		 *
		 * 执行
		 *
		 * @return 是否执行成功
		 *
		 */
		bool Execute();

		/**
		 *
		 * 执行查询
		 *
		 * @return 是否查询成功
		 *
		 */
		bool ExecuteQuery();

	protected:
		int32_t _code{ 0 };

		SqliteQuery _query;

		sqlite3 * _db{ nullptr };
		sqlite3_stmt * _statement{ nullptr };

		std::list<std::vector<std::string>> _list{ };
		std::list<std::vector<std::string>>::iterator _iter{ _list.end() };

		std::unordered_map<std::string, std::size_t> _field{ };
	};
}


#endif // WITH_SQLITE


#endif // __TINY_TOOLKIT__TOOL__SQLITE__H__
