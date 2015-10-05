#include "../inc/Task.h"
#include <utility>
#include <unistd.h>
#include <math.h>	//sqrt()
#include <cstring>
#include <iostream>
Task::Task(int servfd,string queryStr,Application & app,InetAddress &clientAddr,InvertIndex & index,IDF & idf)
	:servfd_(servfd),
	queryStr_(queryStr),
	app_(app),
	clientAddr_(clientAddr),
	index_(index),
	idf_(idf)
{}


/*任务类要执行的任务
 */
void Task::process()
{
	map<string,double> query;						//存储查询词权重
	calculateWeight(query);							//计算查询词权重
	map<string,double>::iterator it = query.begin();
	cout<<it->first<<endl;
	map<int,double> ret = index_.search(it->first);	//调用InvertIndex中的search()函数，返回对应单词的倒排列表
	map<int,double>::iterator pos = ret.begin();	//用于遍历ret的迭代器
	map<int,map<string,double> > common;			//存储交集容器
	while(pos!=ret.end())							//遍历map<int,double> ret 生成第一个交集
	{
		pair<int,map<string,double> > pr1;	//element of common
		pr1.first = pos->first;				//docid
		pair<string,double> pr2;			//element of pr1.second
		pr2.first = it->first;				//string
		pr2.second = pos->second;			//weight
		pr1.second.insert(pr2);	
		common.insert(pr1);
		++pos;
	}
	it++;
	//取交集值
	while(it!=query.end())
	{
		cout<<it->first<<endl;
		ret.clear();
		string word(it->first);
		ret = index_.search(word);//查找下一个查询词
		map<int,map<string,double> >::iterator beg1 = common.begin();
		map<int,map<string,double> >::iterator last1 = common.end();
		map<int,double>::iterator beg2 = ret.begin();
		map<int,double>::iterator last2 = ret.end();
		common = intersection(&beg1,&last1,&beg2,&last2,word);
		it++;
	}
	//求余弦值
	map<int,map<string,double> >::iterator iter = common.begin();
	map<string,double>::iterator iter1;
	Document doc;
	while(iter!=common.end())
	{
		double fenzi=0,fenmu=0,part1=0,part2=0;
		double cos;
		while(iter1!=query.end())
		{
			fenzi = fenzi +(iter1->second)*(query[iter1->first]);
			part1 = part1 +(iter1->second)*(iter1->second);
			part2 = part2 +(query[iter1->first])*(query[iter1->first]);
			++iter1;
		}
		fenmu = sqrt(part1)*sqrt(part2);
		cos = fenzi/fenmu;
		memset(&doc,0,sizeof(doc));
		doc.docid = iter->first;
		doc.cosValue = cos;
		pq_.push(doc);
		++iter;
	}
}

InetAddress &Task::getAddr()
{
	return clientAddr_;
}
int Task::getServfd()
{
	return this->servfd_;
}


//返回排序结果
vector<int> Task::getRanged()
{
	vector<int> vec;
	int size = pq_.size();
	for(int i=0;i<size;i++)
	{
		vec.push_back(pq_.top().docid);
		pq_.pop();
	}
	return vec;//此处可以使用移动语义优化
}
Task::~Task()
{
}

void Task::calculateWeight(map<string,double> & query)
{
	map<string,int> freq;
	vector<string> words;//切词结果
	//app_.statistics(queryStr_,freq);//不能这样
	app_.cut(queryStr_,words);//切词
	statistics(words,freq);//词频统计
	map<string,int>::iterator iter = freq.begin();
	while(iter!=freq.end())
	{
		cout<<"在calculateWeight中word:"<<iter->first<<endl;
		query[iter->first] =(iter->second)*(idf_.idf(iter->first));// weiget = tf*idf
		cout<<query[iter->first]<<endl;
		iter++;
	}
}

//词频统计
void Task::statistics(vector<string> &words,map<string,int> &freq)
{
	vector<string>::iterator iter = words.begin();
	map<string,int>::iterator pos;
	while(iter!=words.end())
	{
		pos = freq.find(*iter);
		if(pos!=freq.end())
			freq[*iter]++;
		else
			freq.insert(pair<string,int>(*iter,1));
	}
}
#if 1
map<int,map<string,double> > Task::intersection(map<int,map<string,double> >::iterator *beg1,
												map<int,map<string,double> >::iterator *last1,
												map<int,double>::iterator *beg2,
												map<int,double>::iterator *last2,
												string word)
{
	map<int,map<string,double> > result;
	while(*beg1!=*last1 && *beg2!=*last2)
	{
		if((*beg1)->first<(*beg2)->first)
			++*beg1;
		else if((*beg2)->first<(*beg1)->first)
			++*beg2;
		else
		{
			pair<int,map<string,double> > p1;
			p1 = **beg1;
			pair<string,double> p2;
			p2.first = word;
			p2.second =(*beg2)->second;
			p1.second.insert(p2);
			result.insert(p1);
			++*beg1,++*beg2;
		}
	}
}
#endif
