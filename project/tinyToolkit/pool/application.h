#ifndef __TINY_TOOLKIT__POOL__APPLICATION__H__
#define __TINY_TOOLKIT__POOL__APPLICATION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 应用池
 *
 */


#include "../common/common.h"


namespace tinyToolkit
{
	template <typename AppTypeT, std::size_t CHUNK_SIZE = 32>
	class TINY_TOOLKIT_API ApplicationPool
	{
		struct ChunkList;

		struct Chunk
		{
			char buffer[sizeof(AppTypeT)]{ 0 };

			bool isUsed{ false };

			std::size_t size{ 0 };
			std::size_t index{ 0 };

			Chunk * prev{ nullptr };
			Chunk * next{ nullptr };

			ChunkList * parent{ nullptr };
		};

		struct ChunkList
		{
			std::size_t used{ 0 };

			ChunkList * prev{ nullptr };
			ChunkList * next{ nullptr };

			Chunk chunks[CHUNK_SIZE];
		};

	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		ApplicationPool()
		{
			AddChunkList(new ChunkList);
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~ApplicationPool()
		{
			Release();
		}

		/**
		 *
		 * 释放
		 *
		 */
		void Release()
		{
			while (_listHead)
			{
				DelChunkList(_listHead);
			}
		}

		/**
		 *
		 * 创建实例对象
		 *
		 * @tparam Args [all types]
		 *
		 * @param args 生成实例对象参数
		 *
		 * @return 实例对象
		 *
		 */
		template <typename... Args>
		AppTypeT * Create(Args &&... args)
		{
			Chunk * chunk = CreateChunk();

			if (chunk)
			{
				return new(chunk->buffer) AppTypeT(std::forward<Args>(args)...);
			}
			else
			{
				return nullptr;
			}
		}

		/**
		 *
		 * 回收实例对象
		 *
		 * @param app 待回收实例对象
		 *
		 */
		void Recover(AppTypeT * app)
		{
			if (app)
			{
				app->~AppTypeT();

				RecoverChunk((Chunk *)app);
			}
		}

		/**
		 *
		 * 使用的对象块个数
		 *
		 * @return 使用的对象块个数
		 *
		 */
		std::size_t UsedSize() const
		{
			return _usedCount;
		}

		/**
		 *
		 * 应用池对象块个数
		 *
		 * @return 应用池对象块个数
		 *
		 */
		std::size_t ChunkSize() const
		{
			return _chunkCount;
		}

		/**
		 *
		 * 应用池对象链表个数
		 *
		 * @return 应用池对象链表个数
		 *
		 */
		std::size_t ChunkListSize() const
		{
			return _chunkListCount;
		}

	protected:
		/**
		 *
		 * 获取内存块
		 *
		 * @return 内存块
		 *
		 */
		Chunk * CreateChunk()
		{
			if (_chunkHead == nullptr)
			{
				AddChunkList(new ChunkList);
			}

			auto ret = _chunkHead;

			CutChunk(_chunkHead);

			if (ret->isUsed)
			{
				return nullptr;
			}

			ret->isUsed = true;

			++_usedCount;

			++ret->parent->used;

			return ret;
		}

		/**
		 *
		 * 回收内存块
		 *
		 * @param chunk 待回收内存块
		 *
		 */
		void RecoverChunk(Chunk * chunk)
		{
			if (chunk && chunk->isUsed)
			{
				chunk->isUsed = false;

				--_usedCount;

				--chunk->parent->used;

				if (chunk->parent->used == 0 && _chunkListCount > 1)
				{
					DelChunkList(chunk->parent);
				}
				else
				{
					AddChunk(chunk);
				}
			}
		}

		/**
		 *
		 * 添加内存块表
		 *
		 * @param list 待添加内存块表
		 *
		 */
		void AddChunkList(ChunkList * list)
		{
			if (list)
			{
				for (size_t i = 0; i < CHUNK_SIZE; ++i)
				{
					list->chunks[i].size   = sizeof(Chunk);
					list->chunks[i].prev   = nullptr;
					list->chunks[i].next   = nullptr;
					list->chunks[i].index  = i;
					list->chunks[i].isUsed = false;
					list->chunks[i].parent = list;

					AddChunk(&(list->chunks[i]));
				}

				list->used = 0;
				list->prev = nullptr;
				list->next = _listHead;

				if (_listHead)
				{
					_listHead->prev = list;
				}

				_listHead = list;

				++_chunkListCount;

				_chunkCount += CHUNK_SIZE;
			}
		}

		/**
		 *
		 * 删除内存块表
		 *
		 * @param list 待删除的内存块表
		 *
		 */
		void DelChunkList(ChunkList * list)
		{
			if (list)
			{
				for (size_t i = 0; i < CHUNK_SIZE; ++i)
				{
					CutChunk(&(list->chunks[i]));
				}

				if (list->next)
				{
					list->next->prev = list->prev;
				}

				if (list->prev)
				{
					list->prev->next = list->next;
				}

				if (list == _listHead)
				{
					_listHead = list->next;
				}

				delete list;

				--_chunkListCount;

				_chunkCount -= CHUNK_SIZE;
			}
		}

		/**
		 *
		 * 插入内存块
		 *
		 * @param chunk 待插入的内存块
		 *
		 */
		void AddChunk(Chunk * chunk)
		{
			if (chunk)
			{
				chunk->next = _chunkHead;

				if (_chunkHead)
				{
					_chunkHead->prev = chunk;
				}

				_chunkHead = chunk;
			}
		}

		/**
		 *
		 * 断开内存块
		 *
		 * @param chunk 待断开的内存块
		 *
		 */
		void CutChunk(Chunk * chunk)
		{
			if (chunk)
			{
				if (chunk->next)
				{
					chunk->next->prev = chunk->prev;
				}

				if (chunk->prev)
				{
					chunk->prev->next = chunk->next;
				}

				if (chunk == _chunkHead)
				{
					_chunkHead = chunk->next;
				}

				chunk->next = nullptr;
				chunk->prev = nullptr;
			}
		}

	protected:
		Chunk * _chunkHead{ nullptr };

		ChunkList * _listHead{ nullptr };

		std::size_t _usedCount{ 0 };
		std::size_t _chunkCount{ 0 };
		std::size_t _chunkListCount{ 0 };
	};
}


#endif // __TINY_TOOLKIT__POOL__APPLICATION__H__
