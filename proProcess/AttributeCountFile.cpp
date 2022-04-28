#include "AttributeCountFile.h"
bool MFile::save_token_file(list<list<string>>& token)
{
	string fielPath = this->fileDir + "\\" + this->filename + "_token" + ".lex";
	ofstream outFile(fielPath);
	try {
		if (!outFile) {
			throw  FileError(fielPath);
		}
	}
	catch (FileError& e) {
		e.showExceptionInform();
		return false;
		//flag = false;
	}
	//vec.push_back("");
	for (auto iter : token) {
		string temp = "";
		for (auto iter2 : iter) {
			outFile.write((iter2).c_str(), (iter2).length());
			outFile.write(" ", 2);
			temp += (iter2).c_str() + string(" ");
		}
	//	outFile.write("\n", 2);
		//outFile << endl;
		vec.push_back(temp);
	}
	outFile.close();
	return true;
}

bool MFile::is_operator_symbol(char ch)
{
		static vector<char>vec{ ',','+','-','*','/','<','>','=',')','(',' ',';',
								'{','}',':','[',']','|','!','.' ,'&'};
		for (auto c : vec)
			if (c == ch)return true;
	return false;
}

vector<string>& MFile::get_removed_text()
{
	return vec;
}

bool AttributeCountFile::save_status_file(vector<unsigned int>& vec)
{
	string filePath = this->fileDir + "\\" + this->filename + "_status" + ".lex";
	FILE* pFile;
	pFile = fopen(filePath.c_str(), "w");
	try {
		if (pFile == NULL) {
			throw  FileError(filePath);
		}
	}
	catch (FileError& e) {
		e.showExceptionInform();
		return false;
		//flag = false;
	}
	fprintf(pFile, "TOTAL_LINE: %d\n", vec[TOTAL_LINE]);
	fprintf(pFile, "INTEGER_VAR_NUMBER: %d\n", vec[INTEGER_VAR_NUMBER]);
	fprintf(pFile, "REAL_VAR_NUMBER: %d\n", vec[REAL_VAR_NUMBER]);
	fprintf(pFile, "INTEGER_ARRAY_NUMBER: %d\n", vec[INTEGER_ARRAY_NUMBER]);
	fprintf(pFile, "REAL_ARRAY_NUMBER: %d\n", vec[REAL_ARRAY_NUMBER]);
	fprintf(pFile, "CHAR_VAR_NUMBER: %d\n", vec[CHAR_VAR_NUMBER]);
	fprintf(pFile, "CHAR_ARRAY_NUMBER: %d\n", vec[CHAR_ARRAY_NUMBER]);
	fprintf(pFile, "INTEGER_CONSTANT_NUMBER: %d\n", vec[INTEGER_CONSTANT_NUMBER]);
	fprintf(pFile, "REAL_CONSTANT_NUMBER: %d\n", vec[REAL_CONSTANT_NUMBER]);
	fprintf(pFile, "CHAR_CONSTANT_NUMBER: %d\n", vec[CHAR_CONSTANT_NUMBER]);
	fprintf(pFile, "FUNCTION_NUMBER: %d\n", vec[FUNCTION_NUMBER]);
	fprintf(pFile, "FOR_NUMBER: %d\n", vec[FOR_NUMBER]);
	fprintf(pFile, "WHILE_NUMBER: %d\n", vec[WHILE_NUMBER]);
	fprintf(pFile, "STRUCT_NUMBER: %d\n", vec[STRUCT_NUMBER]);
	fprintf(pFile, "MULTIPLE_BRANCH_NUMBER: %d\n", vec[MULTIPLE_BRANCH_NUMBER]);
	fprintf(pFile, "VAR_NUMBER: %d\n", vec[VAR_NUMBER]);
	fprintf(pFile, "FUNCTION_PARAMER_NUMBER: %d\n", vec[FUNCTION_PARAMER_NUMBER]);
	//	fprintf(pFile, "VAR_PARAMER_NUMBER: %d\n", vec[17]);
	fprintf(pFile, "DIFF_OPERATOR_NUMBER: %d\n", vec[DIFF_OPERATOR_NUMBER]);
	fprintf(pFile, "OPERATOR_TOTAL_NUMBER: %d\n", vec[OPERATOR_TOTAL_NUMBER]);
	fclose(pFile);
	return true;
}
