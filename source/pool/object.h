#ifndef __TINY_TOOLKIT__POOL__OBJECT__H__
#define __TINY_TOOLKIT__POOL__OBJECT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 对象池
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"


#if TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_WINDOWS
#
#  include <utility>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_APPLE
#
#  include <utility>
#
#elif TINY_TOOLKIT_PLATFORM == TINY_TOOLKIT_PLATFORM_LINUX
#
#  include <utility>
#
#endif


namespace tinyToolkit
{
	namespace pool
	{
		template <typename ObjectTypeT, std::size_t CHUNK_SIZE = 4>
		class TINY_TOOLKIT_API ObjectPool
		{
			struct ChunkList;

			struct Chunk
			{
				char buffer[sizeof(ObjectTypeT)]{ 0 };

				bool isUsed{ false };

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
			 * 析构函数
			 *
			 */
			~ObjectPool()
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
					DeleteChunkList(_listHead);
				}

				_usedCount = 0;
			}

			/**
			 *
			 * 获取对象
			 *
			 * @param args 参数
			 *
			 * @return 对象指针
			 *
			 */
			template <typename... Args>
			ObjectTypeT * Borrow(Args &&... args)
			{
				Chunk * chunk = BorrowChunk();

				if (chunk == nullptr)
				{
					return nullptr;
				}

				return new(chunk->buffer) ObjectTypeT(std::forward<Args>(args)...);
			}

			/**
			 *
			 * 归还对象
			 *
			 * @param object 对象指针
			 *
			 */
			void Return(ObjectTypeT * object)
			{
				if (object == nullptr)
				{
					return;
				}

				object->~ObjectTypeT();

				ReturnChunk(reinterpret_cast<Chunk *>(object));
			}

			/**
			 *
			 * 活跃对象个数
			 *
			 * @return 活跃对象个数
			 *
			 */
			std::size_t ActiveCount()
			{
				return _usedCount;
			}

		private:
			/**
			 *
			 * 插入数据块
			 *
			 * @param chunk 数据块
			 *
			 */
			void InsertChunk(Chunk * chunk)
			{
				if (chunk == nullptr)
				{
					return;
				}

				chunk->next = _chunkHead;

				if (_chunkHead)
				{
					_chunkHead->prev = chunk;
				}

				_chunkHead = chunk;
			}

			/**
			 *
			 * 断开数据块
			 *
			 * @param chunk 数据块
			 *
			 */
			void BrokenChunk(Chunk * chunk)
			{
				if (chunk == nullptr)
				{
					return;
				}

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

			/**
			 *
			 * 归还数据块
			 *
			 * @param chunk 数据块
			 *
			 */
			void ReturnChunk(Chunk * chunk)
			{
				if (chunk == nullptr || !chunk->isUsed)
				{
					return;
				}

				chunk->isUsed = false;

				--_usedCount;

				--chunk->parent->used;

				if (chunk->parent->used == 0 && _chunkListCount > 1)
				{
					DeleteChunkList(chunk->parent);
				}
				else
				{
					InsertChunk(chunk);
				}
			}

			/**
			 *
			 * 获取数据块
			 *
			 * @return 数据块
			 *
			 */
			Chunk * BorrowChunk()
			{
				if (_chunkHead == nullptr)
				{
					InsertChunkList(new ChunkList);
				}

				auto chunk = _chunkHead;

				if (chunk->isUsed)
				{
					return nullptr;
				}

				BrokenChunk(_chunkHead);

				chunk->isUsed = true;

				++_usedCount;

				++chunk->parent->used;

				return chunk;
			}

			/**
			 *
			 * 插入数据块链表
			 *
			 * @param list 数据块链表
			 *
			 */
			void InsertChunkList(ChunkList * list)
			{
				if (list == nullptr)
				{
					return;
				}

				for (std::size_t i = 0; i < CHUNK_SIZE; ++i)
				{
					list->chunks[i].prev   = nullptr;
					list->chunks[i].next   = nullptr;
					list->chunks[i].isUsed = false;
					list->chunks[i].parent = list;

					InsertChunk(&(list->chunks[i]));
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

			/**
			 *
			 * 删除数据块链表
			 *
			 * @param list 数据块链表
			 *
			 */
			void DeleteChunkList(ChunkList * list)
			{
				if (list == nullptr)
				{
					return;
				}

				for (std::size_t i = 0; i < CHUNK_SIZE; ++i)
				{
					BrokenChunk(&(list->chunks[i]));
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

		private:
			Chunk * _chunkHead{ nullptr };

			ChunkList * _listHead{ nullptr };

			std::size_t _usedCount{ 0 };
			std::size_t _chunkCount{ 0 };
			std::size_t _chunkListCount{ 0 };
		};
	}
}


#endif // __TINY_TOOLKIT__POOL__OBJECT__H__
