/*************************************************************************
	> File Name: offsetTest.cpp
	> Author:HOU_JUN
	> Mail: june506@163.com 
	> Created Time: Wed 09 Sep 2015 11:52:02 PM PDT
 ************************************************************************/

#include<iostream>
#include<fstream>
#include <stdio.h>
#include"LoadPageOffset.hpp"
#include "XMLParser.hpp"
using namespace std;
int main()
{
	ifstream ifs("/home/june505/Documents/webbase.txt");
	LoadPageOffset load("/home/june505/Documents/offset.txt");


	char *buf = new char[1024*1024];

	ifs.seekg(load[200].second.first,ios::beg);
	ifs.read(buf,load[200].second.second);
	//printf("%s\n",buf);
	string str(buf);
	XMLParser xmlparser(str);
	cout<<xmlparser.parser("doccontent")<<endl;

	delete []buf;
#if 0
	for(int i = 0;i<50;i++)
	{
		cout<<load[i].first<<":"<<load[i].second.first<<":"<<load[i].second.second<<endl;
	}
#endif
	return 0;
}
