#include "AttributeCount.h"

AttributeCount::AttributeCount()
{
	status.resize(20);
}

AttributeCount::AttributeCount(string filePath):AttributeCount()
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
void AttributeCount::statistics_file_status()
{
	status[TOTAL_LINE] = file.get_token_size();
	string last = "",lastType="";
	while (file.get_token_size()) {
		
		string temp = file.get_token();
		if (temp == "")continue;
		statistics_file_keyword(temp);
		statistics_file_operator(temp);
		//statistics_file_var(temp);
		status[VAR_NUMBER] = status[INTEGER_VAR_NUMBER] + status[REAL_VAR_NUMBER] + status[CHAR_VAR_NUMBER];
		statistics_file_function(temp);
	}
	file.save_status_file(status);
}

void AttributeCount::statistics_file_operator(string& token)
{
	static vector<string>vec = { "=" ,"+" ,"-" ,"*" , "/" ,"<" ,"<=",">",">=","|" ,"||","&&"};
	bool flag = false;
	for (int i = 0; i < vec.size(); i++)
		if (token == vec[i])flag = true;
	if (flag) {
		if (s.count(token) == 0) {
			status[DIFF_OPERATOR_NUMBER]++;
			s.insert(token);
		}
		status[OPERATOR_TOTAL_NUMBER]++;
	}
}

void AttributeCount::statistics_file_keyword(string& token)
{
	static vector<string> keyword = { "struct","union","while","for","switch" };
	int type = -1;
	for (int i = 0; i < keyword.size(); i++) {
		if (keyword[i] == token)type = i;
	}
	switch (type)
	{
	case 0:
	case 1:
		status[STRUCT_NUMBER]++; break;
	case 2:
		status[WHILE_NUMBER]++; break;
	case 3:
		status[FOR_NUMBER]++; break;
	case 4:
		status[MULTIPLE_BRANCH_NUMBER]++; break;
	default:
		break;
	}
}


void AttributeCount::statistics_file_function(string& token)
{
	unsigned int size= 0;
	string temp = file.get_token();
	
	if (is_keyword(token)==false && temp == "(") {
		do {
			temp = file.get_token();
			if (temp == "int" || temp == "double" ||
				temp == "real" || temp == "char") {
				size++;
			}
			else if (temp.find("\"")!=-1 || temp.find("\'")!=-1)
				size++;
			
		} while (temp != ";"&&temp!="{");
	//	temp = file.get_token();
		 if (temp == "{") {
			status[FUNCTION_PARAMER_NUMBER] += size;
			status[FUNCTION_NUMBER]++;
		 }
		/* else if (temp == ";")
			 status[VAR_PARAMER_NUMBER] += size;*/
	}
	else 
		file.roll_back();
}


bool AttributeCount::is_keyword(string str)
{
	static vector<string>vec{ "for","while","switch","if","else","return","=","+","-","*","/"
							 ,"==","<=",">=","<",">",")","("};
	for (int i = 0; i < vec.size(); i++) {
		if (str == vec[i])return true;
	}
	return false;
}

vector<unsigned int> AttributeCount::get_status()
{
	return status;
}