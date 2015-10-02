/*************************************************************************
	> File Name: Thread.h
	> Author: HOUJUN
	> Mail:june506@163.com 
	> Created Time: Wed 26 Aug 2015 11:22:08 AM HKT
 ************************************************************************/

#ifndef _THREAD_H
#define _THREAD_H
#include "Noncopyable.h"
#include <pthread.h>
class Thread : private Noncopyable
{
	public:
		Thread()
			:pthId_(0),
			isRunning_(false)
		{}
		void start();
		void join();
		virtual void run()=0; 
		~Thread();
		static void *runInThread(void *arg);
	private:
		pthread_t pthId_;
		bool isRunning_;
};
#endif
