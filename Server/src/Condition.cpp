/*************************************************************************
	> File Name: Condition.cpp
	> Author: HOUJUN
	> Mail:june506@163.com 
	> Created Time: Tue 25 Aug 2015 11:24:58 PM HKT
 ************************************************************************/

#include "../inc/MutexLock.h"
#include "../inc/Condition.h"

/***************************************/
//由于Condition中的wait()函数要使用到
//MutexLock对象，因此在此要传入一个MutexLock
//对象，对成员函数进行初始化
//
//以下函数体中的的系统调用，要求我们熟悉
//pthread_cond_t的API使用方法
//
/***************************************/
Condition::Condition(MutexLock & mutex) //Condition依赖MutexLock
	:mutex_(mutex)
{
	pthread_cond_init(&cond_,NULL);
}

Condition::~Condition()
{
	pthread_cond_destroy(&cond_);
}

void Condition::wait()
{
	pthread_cond_wait(&cond_,mutex_.getMutexPtr());
}

void Condition::notify()
{
	pthread_cond_signal(&cond_);
}

void Condition::notifyall()
{
	pthread_cond_broadcast(&cond_);
}

