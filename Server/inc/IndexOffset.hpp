
#ifndef _INDEXOFFSET_HPP
#define _INDEXOFFSET_HPP

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <stdlib.h>
using namespace std;

class IndexOffset
{
public:
	IndexOffset(const string path)
	{
		ifstream ifs(path.c_str(),ios::in);
		string line;
		string str;
		pair<int,int> p;
		while(getline(ifs,line),!ifs.eof())
		{
			istringstream iss(line);
			int pos = 0;
			while(iss>>str)
			{
				if(pos == 0)
				{
					p.first = atoi(str.c_str());	
				}
				else if(pos == 1)
				{
					p.second = atoi(str.c_str());
				}
				pos++;
			}
			offset.push_back(p);
		}
		ifs.close();
	}

	pair<int ,int > &operator[](int pos) 
	{
		return offset[pos];
	}

	size_t size() const
	{
		return offset.size();
	}
private:
	vector<pair<int,int> > offset;
};
#endif
