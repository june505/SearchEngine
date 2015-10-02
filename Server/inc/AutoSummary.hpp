#ifndef _AUTOSUMMERY_HPP
#define _AUTOSUMMERY_HPP
#include"../src/Statistics/src/Application.hpp"
#include <string>
#include<set>
#include <utility>
#include<vector>
#include<functional>
using namespace std;
using namespace CppJieba;
class AutoSummary
{
	public:
		AutoSummary(Application &app,int maxSentenceNum=10)//初始化一个自动摘要对象
			:maxSentenceNum_(maxSentenceNum),
			app_(app)
		{}

		//自动提取摘要
		string summarizer(string & originTxt,int KEYNUM=50)
		{
			vector<pair<string,double> > keywords;
			app_.extract(originTxt,keywords,KEYNUM);	//取文章的前50个关键词，按权重排序
			vector<string> sentences;					//装载句子的数组
			getSentences(originTxt,sentences);			//把文章拆分成句子
			int sentencesNum = sentences.size();		//句子的数量
			vector<string> summaryRet;					//装包含关键字的句子
			set<int> summarySet;						//句子去重
			set<int>::iterator it;
			for(int i = 0;i<KEYNUM;i++)
			{
				for(int j = 0;j<sentencesNum;j++)
				{
					int pos = sentences[j].find(keywords[i].first,0);
					if(pos!=string::npos)
					{
						it = summarySet.find(pos);
						if(it==summarySet.end())
						{
							summaryRet.push_back(sentences[j]);//向数组添加句子
							summarySet.insert(j);			
							break;	//跳出循环，找下一个关键字
						}
					}
				}

				if(summaryRet.size()>maxSentenceNum_)	//如果句子的数目超过预订数目，跳出循环
					break;
			}

			string summaryStr;
			int i = 0;
			int num  = summaryRet.size();
			while(i<num)
			{
				summaryStr = summaryStr + sentences[i]+"……";
				i++;
			}

			return summaryStr;//后期改为使用std::move避免复制，提高效率
		}

	private:
		//私有成员函数，在summarizer()中调用
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
	private:
		Application & app_;
		int maxSentenceNum_;
};
#endif

