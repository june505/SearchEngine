/*************************************************************************
	> File Name: test.cpp
	> Author:HOU_JUN
	> Mail: june506@163.com 
	> Created Time: Thu 17 Sep 2015 06:59:54 AM PDT
 ************************************************************************/

#include<iostream>
#include <map>
#include <string>
#include <utility>
#include "InvertIndex.hpp"
using namespace std;
map<int,map<string,double> > intersection(map<int,map<string,double> >::iterator beg1,
											map<int,map<string,double> >::iterator last1,
											map<int,double>::iterator beg2,
											map<int,double>::iterator last2,
											string word);

int main()
{
	InvertIndex index("/home/june505/Documents/indexOffset.lib",
							"/home/june505/Documents/invertIndex.lib");
	cout<<"Load index ………… "<<endl;
	map<string,double> query;
	query["地球"] = 0.235500;
	query["经济"] = 0.235500;
	map<string,double>::iterator it = query.begin();//词频容器
	map<int,double> ret;			//用于存储查询所有返回值
	ret = index.find(it->first);	//调用InvertIndex中的find()函数
	map<int,double>::iterator pos = ret.begin();
	map<int,map<string,double> > commonpart;//存储交集容器
	while(pos!=ret.end())//遍历map<int,double> ret 生成第一个交集
	{
		pair<int,map<string,double> > pr1;
		pr1.first = pos->first;
		pair<string,double> pr2;
		pr2.first = it->first;
		pr2.second = pos->second;
		pr1.second.insert(pr2);
		commonpart.insert(pr1);
		++pos;
	}
	++it;
	//取交集
	while(it!=query.end())
	{
		ret  = index.find(it->first);
		string word = it->first;
		commonpart = intersection(commonpart.begin(),commonpart.end(),ret.begin(),ret.end(),word);
		it++;
	}
	map<int,map<string,double> >::iterator iter1 = commonpart.begin();
	cout<<commonpart.size()<<endl;
	return 0;
}


map<int,map<string,double> >  intersection(map<int,map<string,double> >::iterator beg1,
										   map<int,map<string,double> >::iterator last1,
										   map<int,double>::iterator beg2,
										   map<int,double>::iterator last2,
										   string word)
{
	map<int,map<string,double> > result;
	while(beg1!=last1 && beg2!=last2)
	{
		if((*beg1).first<(*beg2).first)
			++beg1;
		else if((*beg2).first<(*beg1).first)
			++beg2;
		else
		{
			pair<int,map<string,double> > p1;
			p1 = *beg1;
			pair<string,double> p2;
			p2.first = word;
			p2.second = (*beg2).second;
			p1.second.insert(p2);
			result.insert(p1);
			++beg1,++beg2;
		}
	}
	return result;
}
