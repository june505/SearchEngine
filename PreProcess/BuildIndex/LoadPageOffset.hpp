/*************************************************************************
	> File Name: loadPageOffset.hpp
	> Author:HOU_JUN
	> Mail: june506@163.com 
	> Created Time: Sun 06 Sep 2015 06:55:00 AM PDT
 ************************************************************************/

#ifndef _LOADPAGEOFFSET_HPP
#define _LOADPAGEOFFSET_HPP

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <stdlib.h>
using namespace std;

class LoadPageOffset
{
public:
	LoadPageOffset(const string path)
	{
		ifstream ifs(path.c_str(),ios::in);
		string line;
		string str;
		pair<int,int> p1;
		pair<int ,pair<int ,int > > p2;
		while(getline(ifs,line),!ifs.eof())
		{
			istringstream iss(line);
			int pos = 0;
			while(iss>>str)
			{
				if(pos == 0)
				{
					p2.first = atoi(str.c_str());	
				}
				else if(pos == 1)
				{
					p1.first = atoi(str.c_str());
				}
				else if(pos == 2)
				{
					p1.second = atoi(str.c_str());
					p2.second = p1;
				}
				pos++;
			}
			offset.push_back(p2);
		}
		ifs.close();
	}

	pair<int ,pair<int ,int > > &operator[](int pos) 
	{
		return offset[pos];
	}

	size_t size() const
	{
		return offset.size();
	}
private:
	vector<pair<int,pair<int,int> > > offset;
};
#endif
