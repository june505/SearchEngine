#ifndef _THREADPOOL_H
#define _THREADPOOL_H
#include "../src/Statistics/src/Application.hpp"
#include "Buffer.h"
#include "AutoSummary.hpp"	//自动摘要
#include "PageOffset.hpp"
#include <vector>
#include <string>
#include <hiredis/hiredis.h>
class Task;
class Thread;

using namespace CppJieba;
using namespace std;
class Threadpool
{
public:
	Threadpool(int bufsize,
				 int threadNum,
				 string & pageoffsetPath,
				 string & pagelibPath,
				 Application & app);
	~Threadpool();
	void start();
	void stop();
	void addTask(Task* task);				//添加任务
	Task* getTask();						//获取任务
	void threadFunc();						//线程回调函数
private:
	string createJsonString(vector<int> &vec);	//Json序列化


private:
	bool isExit_;							//线程池状态
	int size_;								//任务缓存容量
	int threadNum_;							//线程数量
	string pagelibPath_;					//文档库路径
	string pageoffsetPath_;					//文档库偏移文件路径
	PageOffset *pageOffset_;				//文档库偏移文件
	Buffer buffer_;							//任务缓存器
	std::vector<Thread *> vecThreads_;		//工作线程池
	Application & app_;						//分词库
	AutoSummary summary_;					//自动摘要
	redisContext *conn;
/*************************************/
};
#endif
