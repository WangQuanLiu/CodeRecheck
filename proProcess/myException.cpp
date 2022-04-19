#include "myException.h"

void FileError::showExceptionInform()
{
	std::string temp = filePath + " open error!";
	
	MessageBox(NULL, temp.c_str(), "File Error", MB_OK);
}
