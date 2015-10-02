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
#include "./DirScan/DirScan.hpp"
#include "./DuplicateRemoval/DuplicateRemoval.hpp"

using namespace Simhash;
int main()
{
	std::vector<std::string> oldVec;
	DirScan ds(oldVec);
	ds.traverse("/home/june505/yuliao");
	ds.print();

	std::vector<std::string> newVec;
	std::set<std::string> hashcode;

	DuplicateRemoval dr(oldVec,newVec,hashcode);
	dr.handle();
	return 0;
}
