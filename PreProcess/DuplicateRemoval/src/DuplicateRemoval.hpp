/*************************************************************************
	> File Name: DuplicateRemoval.hpp
	> Author:HOU_JUN
	> Mail: june506@163.com 
	> Created Time: Mon 07 Sep 2015 07:21:27 AM PDT
 ************************************************************************/

#ifndef _DUPLICATEREMOVAL_HPP
#define _DUPLICATEREMOVAL_HPP

#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#define LOGGER_LEVEL LL_WARN 
#include "Simhasher.hpp"


using namespace Simhash;
using namespace std::placeholders;

/*传给find_if()算法的函数对象*/
class isDuplicate
{
	public:
		bool operator()(uint64_t lhs,uint64_t rhs,unsigned short n)
		{
			return Simhasher::isEqual(lhs,rhs,n);
		}
};
class DuplicateRemoval
{
public:
	/*构造函数
	 * 从外面传入装文件路径的数组，
	 * 装不重复路径的数组，
	 * 以及一个装simhash的引用
	 */
	DuplicateRemoval(std::vector<std::string> &oldV,
					 std::vector<std::string> &newV,
					 std::set<uint64_t> &hash)
		:oldVec(oldV),
		newVec(newV),
		simhash(hash)
	{
	}
	void handle(Simhasher &simhasher)
	{
		int oldSize = oldVec.size();
		for(int index = 0;index<oldSize;index++)
		{
			std::ifstream ifs(oldVec[index].c_str(),std::ios::in);
			std::string temp,str;
			while(getline(ifs,temp),!ifs.eof())
			{
				str+=temp;
			}
			ifs.close();
			size_t topN = 100;
			uint64_t u64;
			simhasher.make(str,topN,u64);// 提取前100个关键字，得到simhash值
			std::set<uint64_t>::iterator pos;
			/**************************************************
			 *这里寻找set<uint64_t>中simhash值小于或等于3的元素
			 */
			pos = std::find_if(simhash.begin(),simhash.end(),std::bind(isDuplicate(),_1,u64,3));
			//存在重复，下一次循环
			if(pos !=simhash.end())
				continue;
			//不存在重复，进一步处理
			simhash.insert(u64);
#if 1
			std::cout<<u64<<endl;
			std::cout<<oldVec[index]<<endl;
#endif
			newVec.push_back(oldVec[index]);
		}

	}

private:
	std::vector<std::string> & oldVec; 
	std::vector<std::string> & newVec;
	std::set<uint64_t> & simhash;
};
#endif
