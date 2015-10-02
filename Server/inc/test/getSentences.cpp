/*************************************************************************
	> File Name: text.cpp
	> Author:HOU_JUN
	> Mail: june506@163.com 
	> Created Time: 2015年09月27日 星期日 15时04分19秒
 ************************************************************************/

#include<iostream>
#include<vector>
#include<string>
using namespace std;
void getSentences(const string &originTxt,vector<string> &sentenceVec);
int main()
{
	string str ="我是侯俊，哈哈哈！吃吃吃？乐乐乐。我今天会泉州了，你妈逼的。惺惺惜惺惺。";
	vector<string> vec;
	getSentences(str,vec);
	int size = vec.size();
	int i;
	for(i=0;i<size;i++)
	{
		cout<<vec[i]<<endl;
	}
	return 0;
}
void getSentences(const string &originTxt,vector<string> &sentenceVec)
{
	int beg=0,end=0,pos=0,pos1=0;
	int txtSize = originTxt.size();
	while(beg<txtSize&&pos!=string::npos)
	{
		if((pos=originTxt.find("。",beg))!=string::npos)
		{
			if((pos1=originTxt.find("？",beg))!=string::npos)
			{
				pos=((pos<pos1)?pos:pos1);
				if((pos1=originTxt.find("！",beg))!=string::npos)
				{
					pos=((pos<pos1)?pos:pos1);
				}

			}
			else if((pos1=originTxt.find("！",beg))!=string::npos)
			{
				pos=((pos<pos1)?pos:pos1);
			}
		}
		else if((pos=originTxt.find("？",beg))!=string::npos)
		{
			if((pos1=originTxt.find("！",beg))!=string::npos)
			{
				pos=((pos<pos1)?pos:pos1);
			}

		}
		else if((pos1=originTxt.find("！",beg))!=string::npos)
		{
			pos = pos1;
		}
		else
		{
			break;
		}
		if(pos!=-1)
		{
			int len = pos-beg;
			string sentence(originTxt.substr(beg,len));
			sentenceVec.push_back(sentence);
			beg = pos+3;
		}
	}
}
