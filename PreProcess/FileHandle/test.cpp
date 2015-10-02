/*************************************************************************
	> File Name: test.cpp
	> Author:HOU_JUN
	> Mail: june506@163.com 
	> Created Time: Wed 02 Sep 2015 12:45:31 AM PDT
 ************************************************************************/

#include "FileHandle.hpp"
#include "../DirScan/DirScan.hpp"
#include <stdio.h>
#include <string>
#include <vector>
int main(int argc,char *argv[])
{
#if 1
	if(argc!=2)
	{
		perror("args error");
		return -1;
	}
#endif

	std::string titlename = "【 标  题 】";
	//std::string titlename = "标  题";
	//std::string path(argv[1]);
	std::vector<std::string> vec;
	DirScan ds(vec);
	ds.traverse(argv[1]);
	//ds.traverse("/home/june505/yuliao");
	//ds.print();
	const std::string filename("webbase.txt");
	const std::string offset_file("offset.txt");
	FileHandle *handler = new FileHandle(vec,titlename);
	handler->handle(filename,offset_file);
	return 0;

}
