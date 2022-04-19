#pragma once
#include<iostream>
#include<Windows.h>
#include<string>
#include<exception>
class myException:public std::exception
{
public:
	virtual void showExceptionInform()=0;
};
class FileError:public myException {
public:
	FileError(std::string filePath) { this->filePath = filePath; }
	void showExceptionInform();
private:
	std::string filePath;
};
