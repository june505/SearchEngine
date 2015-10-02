#include "Statistics/src/Application.hpp"
#include "../inc/InvertIndex.hpp"
#include "../inc/IndexOffset.hpp"
#include "../inc/PageOffset.hpp"
#include "../inc/Configure.hpp"
#include "../inc/IDF.hpp"

#include "../inc/InetAddress.h"
#include "../inc/Threadpool.h"
#include "../inc/SocketIO.h"
#include "../inc/Socket.h"
#include "../inc/Task.h"
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#if 1
#include <iostream>
#endif

using namespace CppJieba;
int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		perror("args error!");
		exit(1);
	}
	cout<<"加载Jieba分词库……";
	 CppJieba::Application *app = new CppJieba::Application
						   ("Statistics/dict/jieba.dict.utf8",
                            "Statistics/dict/hmm_model.utf8",
                            "Statistics/dict/user.dict.utf8",
                            "Statistics/dict/idf.utf8",
                            "Statistics/dict/stop_words.utf8");
	cout<<" 完成"<<endl;
	/*****************************************/
	//从命令行参数读取配置文件
	/*****************************************/
	cout<<"加载搜索引擎配置……";
	Configure conf(argv[1]);	//加载配置文件
	cout<<" 完成"<<endl;
	//Configure conf("../conf/configure.txt");	//加载配置文件
	cout<<"加载倒排索引文件……";
	InvertIndex *index = new InvertIndex(conf.getConf("indexOffsetPath"),conf.getConf("invertIndexPath"));
	cout<<" 完成"<<endl;
	cout<<"加载逆文档频率文件……";
	IDF *idf = new IDF(conf.getConf("idfPath"));	//加载逆文档频率文件
	cout<<" 完成"<<endl;
	string pageoffsetPath = conf.getConf("pagesOffsetPath");
	string pagelibPath = conf.getConf("pagesLibPath");

	/*****************************************/
	//初始化线程池
	/*****************************************/
	int bufSiz = atoi(conf.getConf("bufSiz").c_str());
	int threadNum = atoi(conf.getConf("threadNum").c_str());
	 cout<<"启动线程池Threadpool……";
	//Threadpool threadpool(bufSiz,threadNum,pageoffsetPath,pagelibPath,*app);
	Threadpool *threadpool = new Threadpool(bufSiz,threadNum,pageoffsetPath,pagelibPath,*app);
	(*threadpool).start();
	cout<<" 完成"<<endl;

	/*****************************************/
	//初始化UDP服务器
	/*****************************************/
	cout<<"打开UDP服务器……";
	const string IP =conf.getConf("ip");
	uint16_t PORT = atoi(conf.getConf("port").c_str());
	InetAddress serverAddr(IP,PORT);
	InetAddress clientAddr;
	Socket socket;
	int servfd = socket.fd();
	socket.ready(serverAddr);
	char buf[32];
	cout<<" 完成"<<endl;


	/*****************************************/
	//循环接收请求，并封装成任务，并加入线程池
	/*****************************************/
	Task *ptask ;
	cout<<"等待客户端连接……"<<endl;
	while(1)
	{
		SocketIO sockIO(servfd);
		memset(buf,'\0',32);
		sockIO.readn(buf,32,clientAddr);
		std::string str(buf);
		/******************************
		 *log
		 */
		cout<<"IP:"<<clientAddr.ip()<<endl;
		cout<<"PORT:"<<clientAddr.port()<<endl;
		cout<<"Query:"<<str<<endl;
		cout<<"------------------"<<endl;

		ptask = new Task(servfd,str,*app,clientAddr,*index,*idf);//封装成任务
		threadpool->addTask(ptask);		//将任务添加到线程池中
		cout<<"ok"<<endl;
	}
	return 0;
}
