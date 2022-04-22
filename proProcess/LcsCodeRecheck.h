#pragma once
#include"file.h"
class LcsCodeRecheck
{
public:
	LcsCodeRecheck() {}
	LcsCodeRecheck(string filePath):LcsCodeRecheck() {
		setFilePath(filePath);
	}
	//void statistics_file_var(string& token);
	static double lcs(vector<string>& vec_one, vector<string>& vec_two);
	void readFile();
	void setFilePath(string filePath);
	vector<string>& get_removed_text();
private:
	
	MFile file;

};

