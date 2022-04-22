#pragma once
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<vector>
#include<fstream>
#include<map>
#include<set>
#include<regex>
#include"myException.h"
#include"AttributeCountFile.h"
using namespace std;
class AttributeCount
{
public:
	AttributeCount();
	AttributeCount(string filePath);
	void setFilePath(string filePath);
	bool read_file();
	void statistics_file_status();
	void statistics_file_operator(string&token);
	void statistics_file_keyword(string&token);

	void statistics_file_function(string& token);
	static bool is_keyword(string str);
	vector<unsigned int>get_status();
//	bool read_file();
private:
	
	void getText();
	//unsigned int status[STATUS_NUMBER];
	vector<unsigned int >status{ 0 };
	AttributeCountFile file;
	set<string>s;
};

