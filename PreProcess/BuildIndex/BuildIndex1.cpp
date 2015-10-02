/*************************************************************************
	> File Name: BuildIndex.cpp
	> Author:HOU_JUN
	> Mail: june506@163.com 
	> Created Time: Thu 10 Sep 2015 07:33:10 AM PDT
 ************************************************************************/


#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <cmath>
#include "Statistics/src/Application.hpp"
#include "LoadPageOffset.hpp"
#include "XMLParser.hpp"
using namespace std;
int main()
{
	 CppJieba::Application app("Statistics/dict/jieba.dict.utf8",
								"Statistics/dict/hmm_model.utf8",
								"Statistics/dict/user.dict.utf8",
								"Statistics/dict/idf.utf8",
								"Statistics/dict/stop_words.utf8");

	ifstream ifs("../Library/pagesLibrary.lib");//加载网页库
	LoadPageOffset load("../Library/pagesOffset.txt");//加载偏移文件
	int len = load.size();
	int index = 0;
	char *buf = new char[1024*1024];

	map<string,map<int,double> > *invertIndex = new map<string,map<int,double> >;
	int N =0;
	while(index<len)//根据偏移文件对网页库的遍历
	{
		N++;		//统计网页库中网页的数量
		int docid = index+1;
		cout<<"docid :"<<docid<<" is processing ……"<<endl;
		memset(buf,0,1024*1024);
		ifs.seekg(load[index].second.first,ios::beg);
		ifs.read(buf,load[index].second.second);
		string str(buf);
		XMLParser xmlparser(str);
		string result = xmlparser.parser("doccontent");//得到一篇网页的内容
		map<string,int> tf;
		app.statistics(result,tf);//得到一篇网页的单词+词频 map<string,int>

		map<string,int>::iterator iter= tf.begin();
		map<string,map<int,double> >::iterator found;

		while(iter!=tf.end())//遍历该map<string,int>建立网页库索引
		{
			found = invertIndex->find(iter->first);//在索引中找单词
			if(found!=invertIndex->end())//找到
			{
				pair<int,double> id_tf;
				id_tf.first = docid;		//文档id
				id_tf.second = iter->second;//词频
				(found->second).insert(id_tf);
			}
			else//没找到
			{
				pair<string,map<int,double> > indexItem;//用于插入到倒排索引的map中
				indexItem.first = iter->first;//单词
				pair<int,double> id_tf;
				id_tf.first = docid;		//文档id
				id_tf.second = iter->second;//词频
				indexItem.second.insert(id_tf);
				invertIndex->insert(indexItem);
			}
			iter++;
		}
		index++;

	}
	delete []buf;
	//整理完毕
	


	//计算权重,并将你文档频率写到文件
	cout<<"计算权重，idf写入文件 "<<endl;	
	map<string,map<int,double> >::iterator pos1 = (*invertIndex).begin();//外层循环迭代器
	ofstream out3("../../Server/lib/idf.lib",ios::out);//打开输出流
	double idf;
	while(pos1!=(*invertIndex).end())
	{
		idf = log(N/(pos1->second).size());
		out3<<pos1->first<<" "<<idf<<endl;
		map<int,double>::iterator iter1 = (pos1->second).begin();//内层循环迭代器
		while(iter1!=(pos1->second).end())
		{

			iter1->second = (iter1->second)*idf;
			iter1++;
		}
		pos1++;
	}
	out3.close();//关闭输出流
	cout<<"转为正排索引"<<endl;
	//转为正排
	// docid     WEIGHT      word  weight
	map<int,pair<double,map<string,double> > > *Index = new map<int,pair<double,map<string,double> > >;
	map<int,pair<double,map<string,double> > >::iterator found1;//查找docid的迭代器

	pos1 = (*invertIndex).begin();//倒排索引的迭代器
	while(pos1!=(*invertIndex).end())//循环处理倒排索引，pos1指向一个记录
	{
		map<int,double>::iterator iter2 = (pos1->second).begin();//docid weight的迭代器
		while(iter2!=(pos1->second).end())//循环处理每一个记录中的map
		{
			found1 = (*Index).find(iter2->first);
			if(found1!=(*Index).end())//找到
			{
				pair<string,double> p;
				p.first = pos1->first;
				p.second = iter2->second;

				(found1->second).first+=(iter2->second)*(iter2->second);//计算总权重
				(found1->second).second.insert(p);//插入“单词/权重”
			}
			else//找不到
			{
				pair<int,pair<double,map<string,double> > > p1;
				p1.first = iter2->first;
				p1.second.first = (iter2->second)*(iter2->second);
				pair<string,double> p;
				p.first = pos1->first;
				p.second = iter2->second;
				p1.second.second.insert(p);
				(*Index).insert(p1);

			}
			iter2++;
		}
		pos1++;
	}

	//归一化
	cout<<"权重归一化"<<endl;
	map<int,pair<double,map<string,double> > >::iterator it = (*Index).begin();
	while(it!=(*Index).end())
	{
		map<string,double>::iterator it1 = (it->second).second.begin();
		while(it1!=(it->second).second.end())
		{
			it1->second = (it1->second)/sqrt((it->second).first);
			it1++;
		}
		it++;
	}

	//转倒排
	cout<<"转为倒排索引"<<endl;
	map<int,pair<double,map<string,double> > >::iterator it3 = (*Index).begin();
	map<string,map<int,double> >::iterator found2;
	while(it3!=(*Index).end())
	{
		map<string,double>::iterator it4 = (it3->second).second.begin();
		while(it4!=(it3->second).second.end())
		{
			(*invertIndex)[it4->first][it3->first]=it4->second;
			it4++;
		}
		it3++;
	}
	delete Index;//释放堆空间

	//将倒排索引及其偏移文件写入文件
	cout<<"将倒排索引与偏移文件写入文件"<<endl;
	ofstream out1("../../Server/lib/invertIndex.lib");
	ofstream out2("../../Server/lib/indexOffset.lib");
	map<string,map<int,double> >::iterator pos = (*invertIndex).begin();
	char str1[16];
	char str2[16];
	while(pos!=(*invertIndex).end())
	{
		map<int,double>::iterator it5 = (pos->second).begin();
		string word;
		word=word+" <item> \n <word> "+(pos->first)+" </word> \n <docweight> \n";
		size_t offset = out1.tellp();
		while(it5!=(pos->second).end())
		{
			memset(str1,'\0',16);
			memset(str2,'\0',16);
			sprintf(str1,"%d",it5->first);
			sprintf(str2,"%lf",it5->second);
			word=word+str1+" "+str2+"\n";
			it5++;
		}
		word = word+" </docweight> "+"\n"+" </item> "+"\n";
		size_t size = word.size();
		out1<<word;
		out2<<offset<<" "<<size<<endl;
		pos++;
	}
	delete invertIndex;//释放堆空间
	out1.close();
	out2.close();
	cout<<"结束"<<endl;
return 0;
}
