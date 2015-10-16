#ifndef _TASK_H
#define _TASK_H
#include "../src/Statistics/src/Application.hpp"
#include "InvertIndex.hpp"
#include "IDF.hpp"
#include "InetAddress.h"
#include "Document.h"
#include <string>
#include <queue>
#include <map>
#include <iterator>

using namespace CppJieba;
using namespace std;
//规则
struct Compare
{
	bool operator()(const Document &lhs,const Document &rhs)
	{
		return (rhs.cosValue>lhs.cosValue);
	}
};
class Task
{
	public:
		Task(int servfd,
				string query,
				Application & app,//为了给process()进行切词统计
				InetAddress &clientAddr,
				InvertIndex & index,
				IDF & idf);
		void process();
		int getServfd();
		string &getStr();
		vector<int> getRanged();
		InetAddress  & getAddr();
		~Task();
	private:
		void calculateWeight(map<string,double> & query);
		void statistics(vector<string> &words,map<string,int> &freq);
		map<int,map<string,double> > intersection(map<int,map<string,double> >::iterator *beg1,
												  map<int,map<string,double> >::iterator *last1, 
												  map<int,double>::iterator *beg2,
												  map<int,double>::iterator *last2,
												  string word);
	private:
		int servfd_;
		string queryStr_;
		Application & app_;
		InetAddress & clientAddr_;
		InvertIndex & index_;
		IDF & idf_;
		priority_queue<Document,vector<Document>,Compare> pq_;
};
#endif
