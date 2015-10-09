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
	map<string,double> query;		//存储查询词权重
	calculateWeight(query);			//计算查询词权重
	if(query.size()==0)
	{
		cout<<"查询词为空"<<endl;
		return;
	}
	map<string,double>::iterator it = query.begin();
	map<int,double> ret;
	ret = index_.search(it->first);	//调用InvertIndex中的search()函数，返回对应单词的倒排列表
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
		string word(it->first);
		ret = index_.search(word);//查找下一个查询词
		map<int,double>::iterator invalid = ret.begin();
		if(-1==(invalid->first))
		{

			query.erase(it);
			it++;
			continue;
		}
		if(0==(invalid->first))
			ret.erase(invalid);
		map<int,map<string,double> >::iterator beg1 = common.begin();
		map<int,map<string,double> >::iterator last1 = common.end();
		map<int,double>::iterator beg2 = ret.begin();
		map<int,double>::iterator last2 = ret.end();
		common = intersection(&beg1,&last1,&beg2,&last2,word);//交集
		it++;
	}
	//求余弦值
	if(common.size()==0)
	{
		cout<<"交集为空"<<endl;
		return;
	}
	map<int,map<string,double> >::iterator iter = common.begin();
	map<string,double>::iterator iter1 = query.begin();
	Document doc;
	while(iter!=common.end())
	{
		double fenzi=0,fenmu=0,part1=0,part2=0;
		double cos;
		while(iter1!=query.end())
		{
			fenzi = fenzi +(iter1->second)*(query[iter1->first]);//分子 = 分子 +
			part1 = part1 +(iter1->second)*(iter1->second);
			part2 = part2 +(query[iter1->first])*(query[iter1->first]);
			++iter1;
		}
		fenmu = sqrt(part1)*sqrt(part2);
		cos = fenzi/fenmu;
		memset(&doc,0,sizeof(doc));
		if(iter->first==0)
		{
			++iter;
			continue;
		}
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
	int limit = (size<10)?size:10;
	for(int i=0;i<limit;i++)
	{
		vec.push_back(pq_.top().docid);
		pq_.pop();
	}
	return vec;//此处可以使用移动语义优化
}
Task::~Task()
{
}


//计算权重
void Task::calculateWeight(map<string,double> & query)
{
	map<string,int> freq;
	//queryStr_ = "核武器，原子弹外星生物科技";
	app_.statistics(queryStr_,freq);
	map<string,int>::iterator iter = freq.begin();
	while(iter!=freq.end())
	{
		double idfVal = idf_.idf(iter->first);
		if(idfVal==0)//找不到该单词的idf值
		{
			freq.erase(iter);//若在idf中找不到该单词，则将其删除
			iter++;
			continue;
		}
		query[iter->first] =(iter->second)*idfVal;// weiget = tf*idf
		iter++;
	}
}

//取交集函数
map<int,map<string,double> > Task::intersection(
		map<int,map<string,double> >::iterator *beg1,
		map<int,map<string,double> >::iterator *last1,
		map<int,double>::iterator *beg2,
		map<int,double>::iterator *last2,
		string word)
{
	map<int,map<string,double> > result;
	++*beg2;//去掉返回的莫名其妙的0:0
	while(*beg1!=*last1 && *beg2!=*last2)
	{
		if((*beg1)->first<(*beg2)->first)
			++*beg1;
		else if((*beg2)->first<(*beg1)->first)
			++*beg2;
		else
		{
			pair<int,map<string,double> > p1;	//common中docid与ret中docid相同的部分
			p1 = **beg1;						//把common中迭代器指向的位置赋给p1	
			pair<string,double> p2;
			p2.first = word;
			p2.second =(*beg2)->second;
			p1.second.insert(p2);
			result.insert(p1);
			++*beg1,++*beg2;
		}
	}
	return result;
}
