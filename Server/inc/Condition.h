#ifndef _CONDITION_H
#define _CONDITION_H

#include "Noncopyable.h"
#include <pthread.h>

class MutexLock; //前向声明
class Condition : private Noncopyable
{
	public:
		Condition(MutexLock & mutex);
		void wait();
		void notify();
		void notifyall();
		~Condition();
	private:
		pthread_cond_t cond_;
		MutexLock & mutex_;
};
#endif
