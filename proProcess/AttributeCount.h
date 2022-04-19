#pragma once
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<vector>
#include<fstream>
#include"myException.h"
#include"file.h"
using namespace std;
class AttributeCount
{
public:
	AttributeCount();
	AttributeCount(string filePath);
	void setFilePath(string filePath);
	bool read_file();
private:
	
	void getText();
	void remove_lineComment_and_headFiles();

	AttributeCountFile file;
};

