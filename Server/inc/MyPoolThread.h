#ifndef _MYPOOLTHREAD_H
#define _MYPOOLTHREAD_H


/***************************************/
//1、MyPoolThread继承自Thread，拥有Thread
//的所有非私有成员及函数。
//
//2、之所以要传入线程池的引用，是因为
//Thread中的run()方法要执行线程池中的
//threadFunc()方法
/***************************************/
#include "Thread.h"
class Threadpool;
class MyPoolThread : public Thread
{
	public:
		MyPoolThread(Threadpool &threadpool);
		void run();
	private:
		Threadpool &threadpool_;
};
#endif
