/*************************************************************************
	> File Name: XMLParser.hpp
	> Author:HOU_JUN
	> Mail: june506@163.com 
	> Created Time: Sun 06 Sep 2015 09:39:29 AM PDT
 ************************************************************************/

#ifndef _XMLPARSER_HPP
#define _XMLPARSER_HPP
#include <stdio.h>
#include <utility>
#include <string>
#include <stdlib.h>
using namespace std;
class XMLParser
{
public:
	XMLParser(string & page)
			:page_(page)
	{}

	string parser(string tag)
	{
		string ltag,rtag;
		ltag = "<"+tag+">";		//拼接左标签
		rtag = "</"+tag+">";	//拼接右标签
		string str;
		//pair<string,string> pr;
		string::size_type bpos;
		string::size_type epos;
		string::size_type len;
		//确定字符串范围
		bpos = page_.find(ltag);//查找作标签下标
		epos = page_.find(rtag);//查找右标签下标
		bpos+=ltag.size();
		len = epos - bpos;		//计算标签包含内容长度
		if(bpos==page_.npos||epos==page_.npos)
		{
			printf("No such tag: %s \n",tag.c_str());
			exit(-1);
		}
		if(bpos==epos)
		{
			printf("标签内容不存在\n");
			exit(-1);
		}

		string content = page_.substr(bpos+1,len-2);//截取标签包含内容
#if 0
		if(content[content.size()-1]=='\n')
			content[content.size()-1]=='\0';
		if(content[content.size()-2]=='\n')
			content[content.size()-2]=='\0';
		if(content[content.size()-3]=='\n')
			content[content.size()-3]=='\0';
#endif
		//pr.first = tag;
		//pr.second = content;
		//return pr;	//最好使用std::move，减少复制
		return content;
	}
private:
	string &page_;	//使用引用，避免大复制
};
#endif
