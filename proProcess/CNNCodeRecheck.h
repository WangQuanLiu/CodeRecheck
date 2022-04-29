#pragma once
#include"file.h"
#include<regex>
#include<set>
#define UNIFED_FUNC_NAME "func"
#define UNIFED_CALL_FUNC_NAME "call_func"
#define UNIFED_DATA_TYPE "data_type"
#define UNIFED_CHAR "words"
#define UNIFED_STRUCT_NAME "struct_name"
#define UNIFED_STRUCT_VAR "struct_var"
#define UNIFED_ENUM_NAME "enum_name"
#define UNIFED_ENUM_VAR "enum_var"
#define UNIFED_VAR "var"
#define UNIFED_DIGIT "nums"
template<typename T>
class MVector {
public:
	MVector(){}
	size_t size();
	T& operator[](unsigned int index) ;
	T& operator[](const unsigned int index)const;
	const vector<T>& operator+=(vector<T>& vec_obj);
	//const MVector& operator+=(MVector& mvec_obj);
	void push_back(T element);
	//void push_back(const T& element);
private:
	vector<T>vec;
};
class CNNCodeRecheck
{
public:
	CNNCodeRecheck() {}
	CNNCodeRecheck(string filePath) :CNNCodeRecheck() {
		setFilePath(filePath);
	}
	void setFilePath(string filePath);
	static bool is_keyword(string str);
	bool unify_file_function_style(string& token);
	bool is_struct_name(string& token);
	bool is_enum_value(string& token);
	bool is_dataType(string& token);
	void unify_file_style();
	bool unify_var_style(string& token);
	bool read_file();
	bool save_CNN_file();
	inline void statistics_struct_name();
	inline bool is_digit(string& token);
	inline bool is_char(string& token);
	inline bool is_Real_digit_char(string& token);
private:
	MVector<string>unifyedText;
	MFile file;
	set<string>structNameSet, enumNameSet,structMemSet;
};


