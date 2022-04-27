#pragma once
#ifndef _FILE_
#define _CRT_SECURE_NO_WARNINGS
#define _FILE_
#include<iostream>
#include<fstream>
#include<vector>
#include<queue>
#include<cstdio>
#include<cstdlib>
#include<list>
#include<string>
#include<Windows.h>
#include<direct.h>
#include<stack>
#include"myException.h"
#define CHARACTER_MAX 257
#define READ_MAX 10
#define ENTER ""

#define FILE_LINE_MAX_NUMBER 1024
#define SYMBOL_SIZE 15
#define KEYWORD_TABLE 53
	using namespace std;
//	extern enum Gram;
	//using FilePtr = FILE*;
	class MFile //父文件类型
	{
	public:
		unsigned long long get_cur_line();//获取当前列
		static string split_file_dir(string fullPath);//获取文件路径
		static string split_file_dir(char* fullPath);
		static string split_file_name(string fullPath);//获取文件名
		static string split_file_name(char* fullPath);
		static string remove_tabs_and_lineComments(string str);
		static list<list<string>>& remove_blank(list<list<string>>& token);
		 bool save_file(list<string>& vec);
		 bool save_token_file(list<list<string>>& token);
		
		 vector<string>& get_removed_text();
		 MFile(string filePath);
		/*功能：获取符号的大小*/
		unsigned long long get_token_size();//获取符号大小
		MFile() ;
		bool set_file_path(string filePath);//设置文件 名
		void roll_back();//回滚
		void roll_back(int val);
		virtual string get_token();//获取符号
		string get_file();//获取文件名
		bool get_flag() { return flag; }
		bool read_file();
		string& get_filename();
		string& get_fileDir();
		vector<string> get_tokenVec();
	protected:
		bool flag;//检测文件是否成功打开 
		//读取文件
		unsigned long long curLine;//当前存储符号的下标
	//	virtual	bool read_file() = 0;//读取文件
		string rellback;//回滚值
		list<list<string>>token;//存储符号
		vector<string>vec;
		stack<string>rellbackStack;
		std::string filename,fileDir,filePath,savefilePath;//文件名
		bool lineFeed;//换行符
	};


#endif


