
#ifndef _PAGEOFFSET_HPP
#define _PAGEOFFSET_HPP

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

class PageOffset
{
public:
	PageOffset(string &path)
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
			offset.insert(p2);
		}
		ifs.close();
	}

	pair<int ,int > &operator[](int docid) 
	{
		return offset[docid];
	}

	size_t size() const
	{
		return offset.size();
	}
private:
	map<int,pair<int,int> > offset;
};
#endif
