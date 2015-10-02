/*************************************************************************
	> File Name: MyPoolThread.cpp
	> Author: HOUJUN
	> Mail:june506@163.com 
	> Created Time: Wed 26 Aug 2015 03:51:05 PM HKT
 ************************************************************************/

#include "../inc/MyPoolThread.h"
#include "../inc/Threadpool.h"   //在.h文件中进行前向声明
MyPoolThread::MyPoolThread(Threadpool & threadpool)
	:threadpool_(threadpool)
{}

void MyPoolThread::run()
{
	threadpool_.threadFunc();
}

