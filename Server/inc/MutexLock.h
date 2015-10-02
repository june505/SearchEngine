#ifndef __MUTEXLOCK_H
#define __MUTEXLOCK_H

#include "Noncopyable.h"	//条件变量为系统资源类，禁止复制
#include <pthread.h>		//互斥量定义在线程头文件中

/******************************************/
//互斥锁无非就是进行加锁与解锁，因此除了提供
//一个构造函数与一个析构函数外，还有一个加锁
//和一个解锁函数。同时，还有一个用于获取当前
//互斥量的一个指针。
/******************************************/
class MutexLock : private Noncopyable
{
	public:
		MutexLock();
		~MutexLock();
		void lock();
		void unlock();
		pthread_mutex_t * getMutexPtr();
	private:
		pthread_mutex_t mutex_;
};


/********************************************/
//基于MutexLock类的一个操作类，私有成员是mutex_
//在创建MutexGuard对象的时候进行加锁，因此我们
//可以不去直接操作MutexLock的lock()函数。
//在析构对象的时候解锁_
/********************************************/
class MutexLockGuard
{
	public:
		MutexLockGuard(MutexLock &mutex)
			:mutex_(mutex)
		{
			mutex_.lock();
		}

		~MutexLockGuard()
		{
			mutex_.unlock();
		}
	private:
		MutexLock & mutex_;
};
#endif
