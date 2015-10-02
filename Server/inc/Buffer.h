#ifndef __BUFFER_H
#define __BUFFER_H

#include "MutexLock.h"
#include "Condition.h"
#include <queue>

class Task; //前向声明
class Buffer
{
	public:
		Buffer(int size);
		void push(Task* task);
		Task* pop();
		bool empty();
		bool full();

	private:
		MutexLock mutex_;
		Condition notfull_;
		Condition notempty_;
		int size_;
		std::queue<Task*> que_;
};
#endif
