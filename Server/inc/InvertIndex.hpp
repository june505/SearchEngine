
#ifndef _INVERTINDEX_HPP
#define _INVERTINDEX_HPP

#include <fstream>
#include <sstream>
#include <cstring>
#include <map>
#include <utility>
#include <stdlib.h>
#include "XMLParser.hpp"
#include "IndexOffset.hpp"
using namespace std;
class InvertIndex
{
	public:
		InvertIndex(const string & offsetfile,const string & indexfile)
		{
			ifstream ifs(indexfile.c_str());//加载索引文件
			IndexOffset offset(offsetfile);//加载索引偏移文件
			int len = offset.size();
			int idx = 0;
			char *buf = new char[1024*1024];
			while(idx<len)	
			{
				memset(buf,0,1024*1024);
				pair<string,map<int,double> > pr;
				ifs.seekg(offset[idx].first,ios::beg);
				ifs.read(buf,offset[idx].second);
				string str(buf);	//转为C++风格的字符串
				XMLParser xmlparser(str);	//此处可以优化
				string word = xmlparser.parser("word");
				pr.first = word;
				string invertList = xmlparser.parser("docweight");
				istringstream iss1(invertList);
				string line;
				while(getline(iss1,line))
				{
					istringstream iss2(line);
					pair<int,double> p;
					string temp;
					int pos = 0;
					while(iss2>>temp)
					{
						if(pos==0)
						{
							p.first = atoi(temp.c_str());
						}
						else if(pos ==1)
						{
							p.second = atof(temp.c_str());
						}
						pos++;
					}
					pr.second.insert(p);
				}
				index.insert(pr);
				idx++;
			}
			ifs.close();
		}

		map<int,double> search(string query)
		{
			/*
			map<string,map<int,double> >::iterator found;
			found = index.find(query);
			if(found!=index.end())
				return (found->second);
			*/
			return index[query];
		}

	private:
		map<string,map<int,double> > index;
};
#endif
