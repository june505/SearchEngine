/*************************************************************************
	> File Name: Threadpool.cpp
	> Author: HOUJUN
	> Mail:june506@163.com 
	> Created Time: Tue 25 Aug 2015 11:59:03 PM HKT
 ************************************************************************/

#include "../inc/Threadpool.h"
#include "../inc/XMLParser.hpp"
#include "../inc/Thread.h"
#include "../inc/MyPoolThread.h"
#include "../inc/Task.h"
#include "../inc/SocketIO.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <json/json.h>	//	加载json头文件
//#define LOG4CPP
#include "../inc/Mylog.h"
Threadpool::Threadpool(int bufsize,
					   int threadNum,
					   string & pageoffsetPath,
					   string & pagelibPath,
					   Application & app)
	:isExit_(false),
	size_(bufsize),				//任务队列大小
	threadNum_(threadNum),		//线程池线程数量
	pagelibPath_(pagelibPath),	//网页库路径
	pageoffsetPath_(pageoffsetPath),	//初始化文档库偏移文件
	buffer_(size_),				//任务缓存的初始化
	vecThreads_(threadNum),
	app_(app),
	summary_(app_)			//摘要取10句
{
	pageOffset_ = new PageOffset(pageoffsetPath_);
	if(pageOffset_==NULL)
		LOG_ERROR("PageOffset Loading fail!");
	conn = redisConnect("127.0.0.1",6379);
	if(conn!=NULL&&conn->err)
	{
		LOG_ERROR("Redis connecting fail!");
	}else
	{
		LOG_INFO("Redis connecting success!");
	}
}

Threadpool::~Threadpool()
{
	stop();
	if(conn!=NULL)
	{
		redisReply *reply;
		reply = (redisReply*)redisCommand(conn,"SAVE");
		freeReplyObject(reply);
		redisFree(conn);
	}
}


void Threadpool::start()
{
	LOG_INFO("Threadpool starting ……");
	for(int idx = 0;idx!=threadNum_;idx++)
	{
		Thread *pthread = new MyPoolThread(*this);
		vecThreads_.push_back(pthread);
		pthread->start();
	}
}


void Threadpool::stop()
{
	if(isExit_)
	{
		isExit_=true;
		std::vector<Thread*>::iterator iter;
		for(iter = vecThreads_.begin();iter!=vecThreads_.end();iter++)
		{
			(*iter)->join();
			delete *iter;		//释放iter所指向的空间
		}
		vecThreads_.clear();
	}
	LOG_INFO("Threadpool stop ……");
}

void Threadpool::addTask(Task* task)
{
	buffer_.push(task);
}

Task* Threadpool::getTask()
{
	return buffer_.pop();
}


/*************************************/
/* 这是一个在线程中执行的回调函数
 *************************************/
void Threadpool::threadFunc()
{
	while(!isExit_)
	{
		Task *task = getTask();
		if(task != NULL)
		{
			/*此处代码表示查询缓存*/
			string res;
			redisReply *reply;
			string s= task->getStr();
			/*
			int len = s1.size();
			string s = s1.substr(0,len-1); 
			*/
			reply = (redisReply*)redisCommand(conn,"get %s",s.c_str());
			if(reply->str!=NULL)
			{
				s = s+" is found in cache ……";
				LOG_INFO(s.c_str());
				res=reply->str;
				freeReplyObject(reply);
			}
			else
			{
				task->process();						//执行task中的process()函数
				vector<int> ranged = task->getRanged();	//从Task对象中返回排序结果
				res = createJsonString(ranged);	//返回jsonString
				/*将查询结果加入缓存·*/
				if(!res.empty())
				{
					redisReply *reply1;
					reply1 = (redisReply*)redisCommand(conn,"SET %s %s",s.c_str(),res.c_str());
					s=s+"\'s result add into cache ……";
					LOG_INFO(s.c_str());
					freeReplyObject(reply1);
				}
				else
				{
					s=s+" not found ……";
					LOG_INFO(s.c_str());
				}

			}
			SocketIO sockio(task->getServfd());		//返回socket文件描述符
			InetAddress addr = task->getAddr();		//获取Task对象中的客户端地址
			addr.port();							//客户端端口号
			//发送给客户端
			int resSize = res.size();
			char ch[resSize+1];
			strcpy(ch,res.c_str());
			sockio.writen(ch,strlen(ch),addr);
		}
		delete task;
		task=NULL;
	}
}



/*************************************/
/* Json序列化函数
 *************************************/
string Threadpool::createJsonString(vector<int> &vec)//传入根据权重排序后的文档的id
{

	ifstream ifs(pagelibPath_.c_str(),ios::in);
	int i = 0;
	int size = vec.size();
	char *buf = new char[1024*1024];
	Json::Value *root = new Json::Value();
	Json::Value *arrayObject = new Json::Value();
	while(i<size)
	{
		Json::Value item;
		pair<int,int> page =(*pageOffset_)[vec[i]];		//从偏移文件中提取文档相应的offset与size
		ifs.seekg(page.first,ios::beg);					//定位文件起始位置
		memset(buf,0,1024*1024);
		ifs.read(buf,page.second);						//读取一篇文档
		string str(buf);								//C风格的字符串转化为C++风格的字符串
		XMLParser xmlparser(str);					//将文章加入解析器
		string title = xmlparser.parser("doctitle");		//解析标题
		string url = xmlparser.parser("docurl");			//解析url
		string content = xmlparser.parser("doccontent");	//解析内容
		string summarization = summary_.summarizer(content,5);//自动提取摘要  问题出现在这里,已解决！
		// Json字符串序列化
		item["id"]=i+1;
		item["title"]=title;
		item["url"]=url;
		item["summary"]=summarization;
		arrayObject->append(item);
		i++;
	}
	delete buf;		//释放堆内存
	ifs.close();
	string ret;
	(*root)["pages"]=*arrayObject;
	if(size!=0)
		return root->toStyledString();		//将Json对象转为字符串
	else
	{
	//	cout<<"No intersection!"<<endl;
		return ret;
	}
}


