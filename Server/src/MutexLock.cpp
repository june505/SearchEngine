/*************************************************************************
	> File Name: MutexLock.cpp
	> Author: HOUJUN
	> Mail:june506@163.com 
	> Created Time: Tue 25 Aug 2015 11:11:36 PM HKT
 ************************************************************************/

#include "../inc/MutexLock.h"
MutexLock::MutexLock()
{
	pthread_mutex_init(&mutex_,NULL);
}

void MutexLock::lock()
{
	pthread_mutex_lock(&mutex_);
}

void MutexLock::unlock()
{
	pthread_mutex_unlock(&mutex_);
}

pthread_mutex_t * MutexLock::getMutexPtr()
{
	return &mutex_;
}

MutexLock::~MutexLock()
{
	pthread_mutex_destroy(&mutex_);
}
