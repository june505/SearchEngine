
#include <iostream>
#include <fstream>

#define LOGGER_LEVEL LL_WARN //this define can avoid some logs which you dont care about.
#include "Simhasher.hpp"
using namespace Simhash;

int main(int argc, char** argv)
{
    Simhasher simhasher("../dict/jieba.dict.utf8", "../dict/hmm_model.utf8", "../dict/idf.utf8", "../dict/stop_words.utf8");
    //string s("我是蓝翔技工拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上总经理，出任CEO，走上人生巅峰。");
	ifstream ifs1("./pages/doc.txt",ios::in);
	ifstream ifs2("./pages/doc1.txt",ios::in);
	string str,s1,s2;
	while(getline(ifs1,str),!ifs1.eof())
	{
		s1+=str;
	}
	while(getline(ifs2,str),!ifs2.eof())
	{
		s2+=str;
	}
	ifs1.close();
	ifs2.close();
    size_t topN = 50;
    uint64_t u64_1,u64_2;
    vector<pair<string ,double> > res_1,res_2;
    simhasher.extract(s1, res_1, topN);
    simhasher.make(s1, topN, u64_1);
    simhasher.extract(s2, res_2, topN);
    simhasher.make(s2, topN, u64_2);
#if 0
    cout<< "文本：\"" << s << "\"" << endl;
    cout << "关键词序列是: " << res << endl;
    cout<< "simhash值是: " << u64<<endl;
#endif
	string bin1,bin2;
	Simhasher::toBinaryString(u64_1,bin1);
	Simhasher::toBinaryString(u64_2,bin2);
	cout<<bin1<<endl;
	cout<<bin2<<endl;
	cout<<"Hamming distance :"<<Simhasher::isEqual(u64_1,u64_2,9)<<endl;
#if 0
    const char * bin3 = "100010110110";
    const char * bin4 = "110001110011";
    uint64_t u1, u2;
    u1 = Simhasher::binaryStringToUint64(bin3); 
    u2 = Simhasher::binaryStringToUint64(bin4); 
    
    cout<< bin3 << "和" << bin4 << " simhash值的相等判断如下："<<endl;
    cout<< "海明距离阈值默认设置为3，则isEqual结果为：" << (Simhasher::isEqual(u1, u2)) << endl; 
    cout<< "海明距离阈值默认设置为5，则isEqual结果为：" << (Simhasher::isEqual(u1, u2, 5)) << endl; 
#endif
    return EXIT_SUCCESS;
}
