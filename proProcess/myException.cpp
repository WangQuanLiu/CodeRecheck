#include "myException.h"

void FileError::showExceptionInform()
{
	std::string temp = filePath + " open error!";
	
	MessageBox(NULL,LPCWSTR(temp.c_str()), LPCWSTR("File Error"), MB_OK);
}
