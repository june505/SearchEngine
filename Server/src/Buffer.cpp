/*************************************************************************
	> File Name: Buffer.cpp
	> Author: HOUJUN
	> Mail:june506@163.com 
	> Created Time: Tue 25 Aug 2015 10:14:41 PM HKT
 ************************************************************************/

#include "../inc/Buffer.h"
#include "../inc/Task.h"


/************************************************/
//构造函数
//由于Buffer 与MutexLock,Condition是组合关系，因此
//Buffer负责他们的初始化工作
//mutex_()为对象初始化
/************************************************/
Buffer::Buffer(int size)
	:mutex_(),
	notfull_(mutex_),
	notempty_(mutex_),
	size_(size)
{}


//任务缓存的判空
bool Buffer::empty()
{
	return (que_.size()==0);
}

//任务缓存的判满
bool Buffer::full()
{
	return (que_.size()==size_);
}


/********************************************************/
//1、先加锁
//2、如果满，睡眠等待不满
//3、当满足不满（即有空位）的条件，唤醒阻塞等待添加任务的线程
//4、向队列添加任务
//5、通知等到非空（即有任务）的条件，唤醒等待取任务的线程
/*********************************************************/
void Buffer::push(Task* task)
{
	MutexLockGuard guard(mutex_);
	while(full())
		notfull_.wait();
	que_.push(task);
	notempty_.notify();
}


/********************************************************/
//1、先加锁
//2、如果空，睡眠等待非空
//3、通知等到非空（即有任务可执行）的条件，唤醒等待取任务的线程
//4、从队列取任务,返回任务的地址
//5、当满足不满（即有空位）的条件，唤醒阻塞等待添加任务的线程
/*********************************************************/
Task* Buffer::pop()
{
	MutexLockGuard guard(mutex_);
	while(empty())
		notempty_.wait();
	Task* task = que_.front();
	que_.pop();
	notfull_.notify();
	return task;
}
