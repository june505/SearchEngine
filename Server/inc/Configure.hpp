/*************************************************************************
	> File Name: Configure.hpp
	> Author: HOU_JUN
	> Mail: june506@163.com 
	> Created Time: Wed 19 Aug 2015 08:29:14 PM PDT
 ************************************************************************/

#ifndef _CONFIGURE_HPP
#define _CONFIGURE_HPP

#include <fstream>
#include <sstream>
#include <utility>
#include <map>
#include <string>

using namespace std;
class Configure
{
public:
	Configure(string confPath)
	{
		ifstream ifs(confPath.c_str(),ios::in);
		string line;
		string str;
		int pos;
		string first,second;
		while(getline(ifs,line),!ifs.eof())
		{
			pos = 1;
			istringstream iss(line);
			while(iss>>str)
			{
				if(pos%2==1)
				{
					first = str;
					pos++;
					continue;
				}
				second = str;
			}
			conf_.insert(pair<string,string>(first,second));
		}

	}
	string getConf(string str)
	{
		return conf_[str];
	}
private:
		map<string,string> conf_;
};
#endif
