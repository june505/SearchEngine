/*************************************************************************
	> File Name: Test.cpp
	> Author:HOU_JUN
	> Mail: june506@163.com 
	> Created Time: Tue 01 Sep 2015 12:29:01 AM PDT
 ************************************************************************/

#include "DirScan.hpp"
#include <stdio.h>
int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("please input path");
		return -1;
	}
	std::vector<std::string> vec;
	DirScan ds(vec);
	ds.traverse(argv[1]);
	ds.print();

	int size = vec.size();
	printf("vec size = %d\n",size);
	return 0;
}

