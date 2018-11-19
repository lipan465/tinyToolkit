#ifndef __EXAMPLE__TEST__POOL__H__
#define __EXAMPLE__TEST__POOL__H__


#include "test.h"


class Pool
{
public:
	static void Test()
	{
		{
			std::cout << "-------------------------------------------------- appPool --------------------------------------------------" << std::endl;

			class A
			{
			public:
				A(int x) : _x(x)
				{
					std::cout << "new int A : " << _x << std::endl;
				}

				~A()
				{
					std::cout << "free int A : " << _x << std::endl;
				}

			protected:
				int _x{ 0 };
			};

			class B
			{
			public:
				B(double x) : _x(x)
				{
					std::cout << "new double B : " << _x << std::endl;
				}

				~B()
				{
					std::cout << "free double B : " << _x << std::endl;
				}

			protected:
				double _x{ 0.0 };
			};

			tinyToolkit::ApplicationPool<A, 4> poolA;
			tinyToolkit::ApplicationPool<B, 8> poolB;

			std::vector<A *> v1;
			std::vector<B *> v2;

			for (std::size_t i = 0; i < poolA.ChunkSize(); ++i)
			{
				v1.push_back(poolA.Create(i));

				std::cout << "create A, used : " << poolA.UsedSize() << ", chunk : " << poolA.ChunkSize() << ", chunkList : " << poolA.ChunkListSize() << std::endl;
			}

			for (std::size_t i = 0; i < poolB.ChunkSize(); ++i)
			{
				v2.push_back(poolB.Create(i / 1.0));

				std::cout << "create B, used : " << poolB.UsedSize() << ", chunk : " << poolB.ChunkSize() << ", chunkList : " << poolB.ChunkListSize() << std::endl;
			}

			for (auto &iter : v1)
			{
				poolA.Recover(iter);

				std::cout << "recover A, used : " << poolA.UsedSize() << ", chunk : " << poolA.ChunkSize() << ", chunkList : " << poolA.ChunkListSize() << std::endl;
			}

			for (auto &iter : v2)
			{
				poolB.Recover(iter);

				std::cout << "recover A, used : " << poolB.UsedSize() << ", chunk : " << poolB.ChunkSize() << ", chunkList : " << poolB.ChunkListSize() << std::endl;
			}
		}

		{
			std::cout << "-------------------------------------------------- threadPool --------------------------------------------------" << std::endl;

			class A
			{
			public:
				static void plus(int a, int b)
				{
					std::cout << std::this_thread::get_id() << ", " << a << " * " << b << " = " << a * b << std::endl;
				}
			};

			tinyToolkit::ThreadPool pool;

			for (int i = 0; i < 10; ++i)
			{
				pool.Commit(&A::plus, i * 11, i * 22);
			}

			pool.Wait();

			pool.Release();
		}

		{
			std::cout << "-------------------------------------------------- callbackPool --------------------------------------------------" << std::endl;

			tinyToolkit::CallBackPool<void, int, int> backPool;

			backPool.Register([](int x, int y){ std::cout << "x:" << x << ", y:" << y << std::endl; });

			class X
			{
			public:
				X(int x, int y)
				{
					std::cout << "X:: x:" << x << ", y:" << y << std::endl;
				}

			public:
				tinyToolkit::CallBackPool<void, int, int> pool;
			};

			class A
			{
			public:
				A()
				{
					_index = tinyToolkit::Singleton<X>::Instance(1, 1).pool.Register(std::bind(&A::Callback, this, std::placeholders::_1, std::placeholders::_2));
				}

				~A()
				{
					tinyToolkit::Singleton<X>::Instance(1, -1).pool.UnRegister(_index);
				}

				void Callback(int a, int b)
				{
					std::cout << "A::" << _index << " a + b = " << a + b << std::endl;
				}

			protected:
				std::size_t _index{ 0 };
			};

			class B
			{
			public:
				B()
				{
					_index = tinyToolkit::Singleton<X>::Instance(2, 2).pool.Register(std::bind(&B::Callback, this, std::placeholders::_1, std::placeholders::_2));
				}

				~B()
				{
					tinyToolkit::Singleton<X>::Instance(2, -2).pool.UnRegister(_index);
				}

				void Callback(int a, int b)
				{
					std::cout << "B::" << _index << " a + b = " << a + b << std::endl;
				}

			protected:
				std::size_t _index{ 0 };
			};

			backPool.Call(11, 22);

			A a1;

			tinyToolkit::Singleton<X>::Instance(0, 1).pool.Call(100, 1);

			{
				B b1;

				tinyToolkit::Singleton<X>::Instance(0, 2).pool.Call(100, 2);
			}

			tinyToolkit::Singleton<X>::Instance(0, 3).pool.Call(100, 3);

			A a2;

			tinyToolkit::Singleton<X>::Instance(0, 4).pool.Call(200, 1);

			{
				B b2;

				tinyToolkit::Singleton<X>::Instance(0, 5).pool.Call(200, 2);
			}

			tinyToolkit::Singleton<X>::Instance(0, 6).pool.Call(200, 3);

			tinyToolkit::Singleton<X>::Instance(0, 7).pool.UnRegister();

			tinyToolkit::Singleton<X>::Instance(0, 8).pool.Call(300, 1);
		}
	}
};


#endif // __EXAMPLE__TEST__POOL__H__
