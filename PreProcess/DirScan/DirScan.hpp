/*************************************************************************
	> File Name: DirScan.hpp
	> Author:HOU_JUN
	> Mail: june506@163.com 
	> Created Time: Tue 01 Sep 2015 12:01:35 AM PDT
 ************************************************************************/
#ifndef _DIRSCAN_HPP
#define _DIRSCAN_HPP
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#if 1
#include <iostream>
#endif
class DirScan
{
	public:
		/************************************************
		 * 构造函数
		 * 传入一个数组引用，目的是把值带出来
		 */
		DirScan(std::vector<std::string> &vec)
			:vec_(vec)
		{}
		/*************************************************
		 * 重载函数调用操作符
		 */
		void operator()(const std::string &dirName)
		{
			traverse(dirName);
		}
		/************************************************
		 * 递归遍历目录，获取文本绝对路径
		 */
		void traverse(const std::string &dirName)
		{
			DIR *dp;
			if((dp=opendir(dirName.c_str()))==NULL)
			{
				perror("opendir error !");
				exit(-1);
			}

			chdir(dirName.c_str());
			struct dirent* mdirent;
			struct stat mstat;

			while((mdirent = readdir(dp))!=NULL)
			{
				stat(mdirent->d_name,&mstat);
				if(S_ISDIR(mstat.st_mode))
				{
					if(strcmp(mdirent->d_name,".")==0||strcmp(mdirent->d_name,"..")==0)
					{
						continue;
					}
					else
					{
						traverse(mdirent->d_name);
					}
				}
				else
				{
					std::string filename = "";
					filename = filename + getcwd(NULL,0)+"/"+mdirent->d_name;
					vec_.push_back(filename);

				}
			}
			chdir("..");
			closedir(dp);
		}
		
		/**********************
		 * 测试打印
		 */
#if 1
		void print()
		{
			size_t vs= vec_.size();
			for(int index =0;index<vs;index++)
			{
				std::cout<<vec_[index]<<std::endl;
			}
		}
#endif
	private:
		std::vector<std::string> & vec_;
};
#endif
