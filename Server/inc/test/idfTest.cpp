/*************************************************************************
	> File Name: idf.cpp
	> Author:HOU_JUN
	> Mail: june506@163.com 
	> Created Time: Fri 18 Sep 2015 06:47:24 AM PDT
 ************************************************************************/

#include<iostream>
#include "LoadIDF.hpp"
using namespace std;
int main()
{
	LoadIDF idf("/home/june505/Documents/idf.lib");
	cout<<idf.idf("性冲动")<<endl;
	return 0;
}
