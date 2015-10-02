/*************************************************************************
	> File Name: FileHandle.hpp
	> Author:HOU_JUN
	> Mail: june506@163.com 
	> Created Time: Tue 01 Sep 2015 03:46:57 AM PDT
 ************************************************************************/

#ifndef _FILEHANDLE_HPP
#define _FILEHANDLE_HPP
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <stdlib.h>
#include <string.h>
#include <iostream>

class FileHandle
{
	public:
		FileHandle(std::vector<std::string> &vec,std::string &str)
			:vec_(vec)
		{
			m_title_=str;
		}
		
		/**************************************
		 * 重载函数操作运算符
		 */
		void operator()(const std::string &file_name,
									const std::string &offset_file)
		{
			handle(file_name,offset_file);
		}

		/**************************************
		 * 处理文档函数
		 * ************
		 *		对外部提供接口，传入网页库文件和偏移
		 *		文件的路径，在函数内部只写打开文件流，
		 *		依次从数组中取文档路径并打开，处理并
		 *		写入网页库文件和偏移文件。
		 */
		void handle(const std::string &file_name,
					const std::string &offset_file)
		{
			FILE* fp_base = fopen(file_name.c_str(),"w");
			FILE* fp_offset = fopen(offset_file.c_str(),"w");
			if(fp_base==NULL||fp_offset==NULL)
			{
				std::cout<<"file open"<<std::endl;
				exit(0);
			}
			//为文档及其信息开辟内存
			char *mytxt = new char[1024*1024];
			int mydocid;
			char myurl[256]="";
			char *mytitle = new char[1024];
			char *mycontent = new char[1024*1024];

			int vs = vec_.size();
			for(int index = 0;index<vs;index++)
			{
				//初始化内存
				memset(mytxt,0,1024*1024);
				memset(myurl,0,256);
				memset(mycontent,0,1024*1024);
				memset(mytitle,0,1024);

				FILE *fp_file = fopen(vec_[index].c_str(),"r");
				readFile(fp_file,mycontent,mytitle);
				fclose(fp_file);
				//读完一篇
				mydocid = index+1;
				strncpy(myurl,vec_[index].c_str(),vec_[index].size());

				//开始格式化,即字符串拼接
				sprintf(mytxt,"<doc>\n <docid> %d </docid>\n <docurl> %s </docurl>\n <doctitle> %s </doctitle>\n <doccontent>\n %s \n</doccontent>\n </doc>\n ",mydocid,myurl,mytitle,mycontent);
		
				//获取当前指针的位置
				//获取字符串的长度
				//写到网页看偏移文件
				int myoffset = ftell(fp_base);
				int mysize = strlen(mytxt);
				/**********************
				 * 把每一个文档在网页库
				 * 中的偏移信息添加到
				 * 容器中
				 */
				std::pair<int ,int > p1;
				p1.first = myoffset;
				p1.second = mysize;
				std::pair<int ,std::pair<int,int> > p2;
				p2.first = mydocid;
				p2.second = p1;
				m_offset_.insert(p2);
				/******************/
				fprintf(fp_offset,"%d\t%d\t%d\n",mydocid,myoffset,mysize);

				writeFile(fp_base,mytxt);
			}
			fclose(fp_base);
			fclose(fp_offset);
		}
		/***************************************
		 * 读文档函数
		 * **********
		 * 其功能是：
		 *		1、获取文章的标题
		 *		2、获取文章的内容
		 */
	private:
		void readFile(FILE *fp,char *mycontent,char *mytitle)
		{
			int ret;
			const int size = 1024*1024;
			char line[1024] = {0};
			char pos = 0;

			memset(line,0,sizeof(line));
			//装载文章内容（整篇）
			while(1)
			{
				int ret = fread(mycontent+pos,1,size-pos,fp);
				if(ret == 0)
					break;
				else
					pos+=ret;//记住文件指针的偏移量
			}

			//装载标题
			rewind(fp);
			int count =0,flag = 0;
			/* 根据文章的具体结构，这里是扫描文档前11行，寻找包含“标题”的行
			 * 若存在，则将该行赋值给mytitle
			 *
			 * 若文章行数大于11行，且标题不存在，则用第12行当做标题
			 *
			 * 若文章小于11行，则使用第一行作为标题
			 */
			while(count<=10&&fgets(line,1024,fp)!=NULL)
			{
				std::string str(line);
				std::string::size_type pos = str.find(m_title_.c_str(),0);
				if(pos != std::string::npos)
				{
					pos += strlen(mytitle);//跳过“标题”二字
					//substr()返回指定位置后定长的字符串。
					std::string title_content = str.substr(pos);
					strncpy(mytitle,title_content.c_str(),title_content.size());
					int len = strlen(mytitle);
					if(mytitle[len-1]=='\n')
							mytitle[len-1]=='\0';
					flag = 1;//找到
					break;//跳出while循环
				}
				count ++;
			}

			//没找到，且文章行数小于11
			if(count<11&&flag==0)
			{
				rewind(fp);
				fgets(mytitle,1024,fp);
				int len = strlen(mytitle);
				if(mytitle[len-1]=='\n')
					mytitle[len-1]=='\0';
			}
			//没找到，其文章行数大于11
			else if(count == 11&&flag == 0)
			{
				fgets(mytitle,1024,fp);
				int len = strlen(mytitle);
				if(mytitle[len-1]=='\n')
					mytitle[len-1]=='\0';
			}
		}


		/***********************************
		 * 将格式化后的网页写到网页库文件中
		 ***********************************
		 */
		void writeFile(FILE *fp,char *mytxt)
		{
			int ret,pos = 0;
			int len = strlen(mytxt);
			while(1)
			{
				ret =fwrite(mytxt+pos,1,len - pos,fp);
				len = len -ret;
				if(len == 0)
					break;
			}
		}
	private:
		std::vector<std::string> &vec_;
		std::string m_title_;
		std::map<int ,std::pair<int,int> > m_offset_;
};
#endif
