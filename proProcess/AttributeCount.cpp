#include "AttributeCount.h"

AttributeCount::AttributeCount()
{
}

AttributeCount::AttributeCount(string filePath)
{
	file.set_file_path(filePath);
}

void AttributeCount::setFilePath(string filePath)
{
	file.set_file_path(filePath);
}

bool AttributeCount::read_file() 
{
	this->file.read_file();
	return false;
}

void AttributeCount::remove_lineComment_and_headFiles()
{

}
