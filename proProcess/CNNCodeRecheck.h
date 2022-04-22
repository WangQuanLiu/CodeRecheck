#pragma once
#include"file.h"
#include<regex>
template<typename T>
class MVector {
public:
	MVector(){}
	size_t size();
	T& operator[](unsigned int index) ;
	T& operator[](const unsigned int index)const;
	const vector<T>& operator+=(vector<T>& vec_obj);
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
	void unify_file_style();
	bool unify_var_style(string& token);
	bool read_file();
	bool save_CNN_file();
	inline bool is_Real_digit_char(string& token);
private:
	MVector<string>unifyedText;
	MFile file;
};


