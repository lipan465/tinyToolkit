#ifndef __TINY_TOOLKIT__3RD__SQLITE_CLIENT__H__
#define __TINY_TOOLKIT__3RD__SQLITE_CLIENT__H__


#ifdef TINY_TOOLKIT_SQLITE_SUPPORT


/**
 *
 *  作者: hm
 *
 *  说明: sqlite客户端
 *
 */


#include <sqlite3.h>

#include "../debug/exception.h"


namespace tinyToolkit
{
	INHERITANCE_DECLARE(SqliteException, IException)

	struct SqliteBlobValue
	{
		char * data;

		std::size_t size;
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
		static int32_t BindParams(sqlite3_stmt *, int32_t)
		{
			return SQLITE_OK;
		}

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
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, const std::nullptr_t &)
		{
			return sqlite3_bind_null(statement, index);
		}

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
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, std::float_t value)
		{
			return sqlite3_bind_double(statement, index, value);
		}

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
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, std::double_t value)
		{
			return sqlite3_bind_double(statement, index, value);
		}

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
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, int32_t value)
		{
			return sqlite3_bind_int(statement, index, value);
		}

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
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, uint32_t value)
		{
			return sqlite3_bind_int(statement, index, value);
		}

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
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, int64_t value)
		{
			return sqlite3_bind_int64(statement, index, value);
		}

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
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, uint64_t value)
		{
			return sqlite3_bind_int64(statement, index, static_cast<sqlite3_int64>(value));
		}

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
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, const char * value)
		{
			return sqlite3_bind_text(statement, index, value, static_cast<int32_t>(strlen(value) + 1), SQLITE_TRANSIENT);
		}

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
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, const SqliteBlobValue & value)
		{
			return sqlite3_bind_blob(statement, index, value.data, static_cast<int32_t>(value.size), SQLITE_TRANSIENT);
		}

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
		static int32_t BindValue(sqlite3_stmt * statement, int32_t index, const std::string & value)
		{
			return sqlite3_bind_text(statement, index, value.c_str(), static_cast<int32_t>(value.size()), SQLITE_TRANSIENT);
		}
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
		SqliteQuery(std::list<std::vector<std::string>> & list, std::unordered_map<std::string, std::size_t> & field, int32_t & code) : _code(code),
																																		_list(list),
																																		_field(field)
		{

		}

		/**
		 *
		 * 查询请求
		 *
		 * @param statement sql对象
		 *
		 * @return 是否查询成功
		 *
		 */
		bool Request(sqlite3_stmt * statement)
		{
			_list.clear();
			_field.clear();

			_column = sqlite3_column_count(statement);

			if (_column == 0)
			{
				return true;
			}

			return ReadTable(statement);
		}

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
		bool ReadTable(sqlite3_stmt * statement)
		{
			while (true)
			{
				_code = sqlite3_step(statement);

				if (_code == SQLITE_DONE)
				{
					break;
				}

				if (!ReadRow(statement))
				{
					_list.clear();

					sqlite3_reset(statement);

					return false;
				}
			}

			sqlite3_reset(statement);

			return true;
		}

		/**
		 *
		 * 读取行
		 *
		 * @param statement sqlite对象
		 *
		 * @return 是否读取成功
		 *
		 */
		bool ReadRow(sqlite3_stmt * statement)
		{
			std::vector<std::string> rowVector;

			for (int32_t i = 0; i < _column; ++i)
			{
				_field.insert(std::make_pair(sqlite3_column_name(statement, i), i));

				switch (sqlite3_column_type(statement, i))
				{
					case SQLITE_NULL:
					{
						rowVector.emplace_back("");

						break;
					}

					case SQLITE_TEXT:
					{
						rowVector.emplace_back(std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, i))));

						break;
					}

					case SQLITE_BLOB:
					{
						rowVector.emplace_back(std::string(reinterpret_cast<const char *>(sqlite3_column_blob(statement, i)),
														   static_cast<std::size_t>(sqlite3_column_bytes(statement, i))));

						break;
					}

					case SQLITE_FLOAT:
					{
						rowVector.emplace_back(std::to_string(sqlite3_column_double(statement, i)));

						break;
					}

					case SQLITE_INTEGER:
					{
						rowVector.emplace_back(std::to_string(sqlite3_column_int64(statement, i)));

						break;
					}

					default:
					{
						return false;
					}
				}
			}

			_list.emplace_back(std::move(rowVector));

			return true;
		}

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
		SqliteDataBase() : _query(_list, _field, _code)
		{

		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~SqliteDataBase()
		{
			Close();
		}

		/**
		 *
		 * 打开内存数据库
		 *
		 * @return 是否打开成功
		 *
		 */
		bool OpenMemory()
		{
			return OpenFile(":memory:");
		}

		/**
		 *
		 * 打开文件数据库
		 *
		 * @param path 数据库路径
		 *
		 * @return 是否打开成功
		 *
		 */
		bool OpenFile(const char * path)
		{
			_code = sqlite3_open(path, &_db);

			return _code == SQLITE_OK;
		}

		/**
		 *
		 * 关闭数据库
		 *
		 * @return 是否关闭成功
		 *
		 */
		bool Close()
		{
			if (_db)
			{
				_code = sqlite3_finalize(_statement);
				_code = sqlite3_close(_db);

				while (_code == SQLITE_BUSY)
				{
					sqlite3_stmt * stmt = sqlite3_next_stmt(_db, nullptr);

					if (stmt == nullptr)
					{
						break;
					}

					_code = sqlite3_finalize(stmt);

					if (_code == SQLITE_OK)
					{
						_code = sqlite3_close(_db);
					}
				}

				_db = nullptr;
				_statement = nullptr;

				return _code == SQLITE_OK;
			}

			return true;
		}

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
		ValueTypeT GetValue(const std::size_t index)
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
		const std::string & GetString(const std::size_t index)
		{
			return _iter->at(index);
		}

		/**
		 *
		 * 获取索引对应的字符串
		 *
		 * @param field 索引名
		 *
		 * @return 获取的值
		 *
		 */
		const std::string & GetString(const std::string & field)
		{
			auto find = _field.find(field);

			if (find == _field.end())
			{
				TINY_TOOLKIT_EXCEPTION_THROW(SqliteException, "Invalid field")
			}

			return GetString(find->second);
		}

		/**
		 *
		 * 开启事务
		 *
		 * @return 是否开启成功
		 *
		 */
		bool Begin()
		{
			_code = sqlite3_exec(_db, "begin", nullptr, nullptr, nullptr);

			return _code == SQLITE_OK;
		}

		/**
		 *
		 * 结束事务
		 *
		 * @return 是否结束成功
		 *
		 */
		bool Commit()
		{
			_code = sqlite3_exec(_db, "commit", nullptr, nullptr, nullptr);

			return _code == SQLITE_OK;
		}

		/**
		 *
		 * 回滚事务
		 *
		 * @return 是否回滚成功
		 *
		 */
		bool Rollback()
		{
			_code = sqlite3_exec(_db, "rollback", nullptr, nullptr, nullptr);

			return _code == SQLITE_OK;
		}

		/**
		 *
		 * 预处理
		 *
		 * @param sql sql语句
		 *
		 * @return 是否处理成功
		 *
		 */
		bool Prepare(const char * sql)
		{
			_code = sqlite3_prepare_v2(_db, sql, -1, &_statement, nullptr);

			return _code == SQLITE_OK;
		}

		/**
		 *
		 * 执行sql语句
		 *
		 * @param sql sql语句
		 *
		 * @return 是否执行成功
		 *
		 */
		bool Execute(const char * sql)
		{
			if (!Prepare(sql))
			{
				return false;
			}

			if (!Execute())
			{
				return false;
			}

			return ExecuteQuery();
		}

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
		bool IsValidRow()
		{
			return _iter != _list.end();
		}

		/**
		 *
		 * 移动到头一行
		 *
		 */
		void MoveToHeadRow()
		{
			_iter = _list.begin();
		}

		/**
		 *
		 * 移动到上一行
		 *
		 */
		void MoveToPrevRow()
		{
			if (IsValidRow())
			{
				--_iter;
			}
		}

		/**
		 *
		 * 移动到下一行
		 *
		 */
		void MoveToNextRow()
		{
			if (IsValidRow())
			{
				++_iter;
			}
		}

		/**
		 *
		 * 移动到结尾行
		 *
		 */
		void MoveToTailRow()
		{
			_iter = std::prev(_list.end());
		}

		/**
		 *
		 * 受影响的行
		 *
		 * @return 受影响的行
		 *
		 */
		int32_t AffectedRows() const
		{
			return sqlite3_changes(_db);
		}

		/**
		 *
		 * 最后一次错误代码
		 *
		 * @return 错误代码
		 *
		 */
		int32_t LastErrorCode() const
		{
			return _code;
		}

		/**
		 *
		 * 记录的行数
		 *
		 * @return 记录的行数
		 *
		 */
		std::size_t RecordCount() const
		{
			return _list.size();
		}

		/**
		 *
		 * 最后一次错误信息
		 *
		 * @return 错误信息
		 *
		 */
		const char * LastErrorMessage() const
		{
			return sqlite3_errmsg(_db);
		}

	protected:
		/**
		 *
		 * 执行
		 *
		 * @return 是否执行成功
		 *
		 */
		bool Execute()
		{
			_code = sqlite3_step(_statement);

			sqlite3_reset(_statement);

			/**
			 *
			 * 当执行select操作时, 成功返回SQLITE_ROW.
			 *
			 * 当执行insert, update, drop等操作时成功返回SQLITE_DONE.
			 *
			 */
			return (_code == SQLITE_DONE || _code == SQLITE_ROW || _code == SQLITE_OK);
		}

		/**
		 *
		 * 执行查询
		 *
		 * @return 是否查询成功
		 *
		 */
		bool ExecuteQuery()
		{
			if (_query.Request(_statement))
			{
				MoveToHeadRow();

				return true;
			}

			return false;
		}

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


#endif // TINY_TOOLKIT_SQLITE_SUPPORT


#endif // __TINY_TOOLKIT__3RD__SQLITE_CLIENT__H__
