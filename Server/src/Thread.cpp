/*************************************************************************
	> File Name: Thread.cpp
	> Author: HOUJUN
	> Mail:june506@163.com 
	> Created Time: Wed 26 Aug 2015 11:26:43 AM HKT
 ************************************************************************/

#include "../inc/Thread.h"

/************************************/
//调用start()创建线程,调用phtread_create()
//设置运行标志位true
/************************************/
void Thread::start()
{
	pthread_create(&pthId_,NULL,runInThread,this);
	isRunning_=true;
}

/************************************/
//调用join()回收线程
//设置运行标志位为false
//
//注意：创建者调用join()函数回收子线程，
//当子线程没有运行结束，则创建者会阻塞
//等待子线程结束。
/************************************/
void Thread::join()
{
	pthread_join(pthId_,NULL);
	isRunning_=false;
}


/*************************************/
//析构函数
//
//但是我们并不希望主线程吊死在一棵树上，
//它还有其它任务，所以：
//	pthread_detach(pthId),将线程状态设置
//为detached状态，当线程运行结束时候自动
//释放资源。（非阻塞，可立即返回）
/*************************************/
Thread::~Thread()
{
	if(isRunning_)
	{
		pthread_detach(pthId_);
		isRunning_=false;
	}
}


/********************************************/
//arg是在创建线程时候传入的this,再此将void类型
//的arg强转成Thread类型。
/********************************************/
void *Thread::runInThread(void *arg)
{
	Thread *pThread = static_cast<Thread*>(arg);
	pThread->run();
	return NULL;
}
