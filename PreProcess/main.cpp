/*************************************************************************
	> File Name: test.cpp
	> Author:HOU_JUN
	> Mail: june506@163.com 
	> Created Time: Tue 08 Sep 2015 02:17:48 AM PDT
 ************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <time.h>
#include <stdio.h>
#include "./DirScan/DirScan.hpp"
#include "./DuplicateRemoval/src/DuplicateRemoval.hpp"
#include "./FileHandle/FileHandle.hpp"

int main()
{

	/*加载字典，创建Simhasher对象*/
	Simhasher simhasher("DuplicateRemoval/dict/jieba.dict.utf8",
						"DuplicateRemoval/dict/hmm_model.utf8", 
						"DuplicateRemoval/dict/idf.utf8", 
						"DuplicateRemoval/dict/stop_words.utf8");

	/*扫描文件夹，获取所有文件的路径*/
	std::vector<std::string> oldVec;
	DirScan ds(oldVec);
	ds.traverse("material");//遍历语料库
	 

	/*网页去重，得到一个不重复网页的路径
	 *
	 * 计算时间
	 * */
	std::vector<std::string> newVec;
	std::set<uint64_t> hashcode;//这里可以使用hashset进行优化。
	DuplicateRemoval dr(oldVec,newVec,hashcode);
	time_t begin,end,last;
	(void) time(&begin);
	dr.handle(simhasher);//把Simhasher的对象传给 去重工具
	(void) time(&end);
	last = end-begin;

#if 1
	/*print info*/
	printf("using %ld seconds\n",last);
	cout<<"oldVec size = "<<oldVec.size()<<endl;
	cout<<"newVec size = "<<newVec.size()<<endl;
#endif

	
	std::string titlename = "【 标  题 】";
	const std::string filename("./Library/pagesLibrary.lib");
	const std::string offset_file("./Library/pagesOffset.txt");
	FileHandle *handler = new FileHandle(newVec,titlename);
	handler->handle(filename,offset_file);

	return 0;
}
