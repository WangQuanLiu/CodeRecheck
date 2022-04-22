#pragma once
#include"file.h"
#define STATUS_NUMBER 19
#define TOTAL_LINE 0
#define INTEGER_VAR_NUMBER 1
#define REAL_VAR_NUMBER 2
#define INTEGER_ARRAY_NUMBER 3
#define REAL_ARRAY_NUMBER 4
#define CHAR_VAR_NUMBER 5
#define CHAR_ARRAY_NUMBER 6
#define INTEGER_CONSTANT_NUMBER 7
#define REAL_CONSTANT_NUMBER 8
#define CHAR_CONSTANT_NUMBER 9
#define FUNCTION_NUMBER 10
#define FOR_NUMBER 11
#define WHILE_NUMBER 12
#define STRUCT_NUMBER 13
#define MULTIPLE_BRANCH_NUMBER 14
#define VAR_NUMBER 15
#define FUNCTION_PARAMER_NUMBER 16
//#define VAR_PARAMER_NUMBER 17
#define DIFF_OPERATOR_NUMBER 17
#define OPERATOR_TOTAL_NUMBER 18
class AttributeCountFile :public MFile { //语法分析文件类型

public:
	
	 bool save_status_file(vector<unsigned int>& vec);
	
public:
	AttributeCountFile() {}
	AttributeCountFile(string filePath) :MFile(filePath) {  }

};

