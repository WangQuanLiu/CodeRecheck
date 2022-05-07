#include "CNNCodeRecheck.h"
bool CNNCodeRecheck::unify_file_function_style(string& token)
{
	unsigned int size = 0;
	string temp = file.get_token();
	vector<string> vec;
	string nextTemp;
	if (token == "struct"||token=="union") {
		nextTemp = file.get_token();
		if (is_struct_name(temp) ) {
			if (is_keyword(nextTemp) == false) {
				unifyedText.push_back(token);
				unifyedText.push_back(UNIFED_STRUCT_NAME);
				unifyedText.push_back(UNIFED_STRUCT_VAR);
				//structVarSet.insert(nextTemp);
				return true;
			}
			else if (nextTemp == "*") {
				unifyedText.push_back(token);
				unifyedText.push_back(UNIFED_STRUCT_NAME);
				unifyedText.push_back("*");
				while ((nextTemp = file.get_token()) == "*") {
					unifyedText.push_back("*");
				}
				if (is_digit(nextTemp)) {
					unifyedText.push_back(UNIFED_DIGIT);
				}
				else {
					unifyedText.push_back(UNIFED_STRUCT_VAR);
					//structVarSet.insert(nextTemp);
				}
				return true;
			}
		}
		structNameSet.insert(temp);
		temp = UNIFED_STRUCT_NAME;
		unifyedText.push_back(token);
		unifyedText.push_back(temp);
		file.roll_back();
		return true;
	}
	else if (token == "enum"&&is_keyword(temp)==false) {
		//enumNameSet.insert(temp);
		temp = UNIFED_ENUM_NAME;
		unifyedText.push_back(token);
		unifyedText.push_back(temp);
		return true;
	}
	else if (is_keyword(token) == false && temp == "(") { //token为函数定义或函数调用
		vec.push_back(temp);
		temp = file.get_token();
		do {
			nextTemp = file.get_token();
			while (nextTemp == "")nextTemp = file.get_token();
			if (nextTemp == EOF)break;
		/*	if (temp == "int" || temp == "double" ||
				temp == "float" || temp == "char" ||
				temp == "unsigned" || temp=="long") {
				vec.push_back(UNIFED_DATA_TYPE);
			}*/
			if (unify_dataType_style(temp) == true) {}
			else if (is_struct_name(temp)&&is_keyword(nextTemp)) {
				vec.push_back(UNIFED_STRUCT_NAME);
			}
			else if (is_char(temp)) {
				vec.push_back(UNIFED_CHAR);
			}
			else if (is_digit(temp)) {
				vec.push_back(UNIFED_DIGIT);
			}
			else if ( is_keyword(temp) == false&&nextTemp!="(")
				vec.push_back(UNIFED_VAR);
			else if (is_keyword(temp)==false&&nextTemp == "(")
				vec.push_back(UNIFED_CALL_FUNC_NAME);
			else 
				vec.push_back(temp);
			temp = nextTemp;
		} while (temp != ";" && temp != "{");
		vec.push_back(temp);
		if (temp == "{") {
			unifyedText.push_back(UNIFED_FUNC_NAME);
		}
		else {
			unifyedText.push_back(UNIFED_CALL_FUNC_NAME);
		}
		unifyedText += vec;
		return true;
	}
	else {
		file.roll_back();
		return false;
	}
}

bool CNNCodeRecheck::is_struct_name(string& token)
{
	if (structNameSet.count(token) >= 1) {
		return true;
	}
	return false;
}

bool CNNCodeRecheck::is_enum_value(string& token)
{
	if (enumNameSet.count(token) >= 1)return true;
	return false;
}

bool CNNCodeRecheck::is_dataType(string& token)
{
	static vector<string>vec{ "float","double","int","char" ,"long","unsigned"};
	for (int i = 0; i < vec.size(); i++)
		if (vec[i] == token)return true;
	return false;
}

void CNNCodeRecheck::unify_file_style()
{
	statistics_struct_name();
	while (file.get_token_size()) {
		string temp = file.get_token();
		if (temp == "")continue;
		else if (temp == "\n") {
			unifyedText.push_back(temp);
		}
		else if (is_keyword(temp)) {
			string nextTemp = file.get_token();
			unifyedText.push_back(temp);
			if (temp == "enum" && is_keyword(nextTemp) == false)
				unifyedText.push_back(UNIFED_ENUM_NAME);
			else file.roll_back();
		}
		else if (is_struct_name(temp) == true) {
			string nextTemp ;
			vector<string>vec;
			while ((nextTemp = file.get_token()) == "*")vec.push_back(nextTemp);
			if (is_keyword(nextTemp) == false) {
				unifyedText.push_back(UNIFED_STRUCT_NAME);
				unifyedText += vec;
				unifyedText.push_back(UNIFED_STRUCT_VAR);
			}
			else file.roll_back();
		}
		else if (is_digit(temp) == true) {
			unifyedText.push_back(UNIFED_DIGIT);
		}
		else if (is_char(temp) == true)
			unifyedText.push_back(UNIFED_CHAR);
		/*else if (is_dataType(temp) == true) {
			string nextTemp;
			do {
				nextTemp = file.get_token();
			} while (is_dataType(nextTemp) && file.get_token_size() >= 1);
			file.roll_back();
			unifyedText.push_back(UNIFED_DATA_TYPE);
		}*/
		/*else if(unify_datType_style(temp))*/
		else  if (unify_dataType_style(temp)==false&& unify_file_function_style(temp) == false &&
			unify_var_style(temp) == false)
			unifyedText.push_back(temp);
		//else unifyedText.push_back(temp);
	}
}

bool CNNCodeRecheck::unify_var_style(string& token)
{
	if (is_keyword(token) == true||
		is_Real_digit_char(token)==true)
		return false;
	else if (is_struct_name(token)) {
		unifyedText.push_back(UNIFED_STRUCT_VAR);
		return true;
	}
	else {
		unifyedText.push_back(UNIFED_VAR);
		return true;
	}
}

bool CNNCodeRecheck::unify_dataType_style(string& token)
{
	if (is_dataType(token) == true) {
	string nextTemp;
	do {
		nextTemp = file.get_token();
	} while (is_dataType(nextTemp) && file.get_token_size() >= 1);
	if(file.get_token_size()>=1)
	file.roll_back();
	unifyedText.push_back(UNIFED_DATA_TYPE);
	return true;
	}
	return false;
}

bool CNNCodeRecheck::read_file()
{
	return file.read_file();
}

bool CNNCodeRecheck::save_CNN_file()
{
	string savefilePath= file.get_fileDir() + "\\" + file.get_filename() + "_cnn" + ".txt";
	return save_CNN_file(savefilePath);
}

bool CNNCodeRecheck::save_CNN_file(const string& filePath)
{
	ofstream outFile(filePath);
	try {
		if (!outFile) {
			throw  FileError(filePath);
		}
	}
	catch (FileError& e) {
		e.showExceptionInform();
		return false;
	}
	for (int i = 0; i < unifyedText.size(); i++) {
		outFile.write(unifyedText[i].c_str(), unifyedText[i].size());
		outFile.put(' ');
	}
	outFile.close();
	return true;
}

inline void CNNCodeRecheck::statistics_struct_name()
{
	vector<string>vec = file.get_vecToken();
	size_t size = vec.size();
	for (int i = 0; i < size; i++) {
		if ((vec[i] == "struct" || vec[i] == "union") && i + 1 < size&&(is_keyword(vec[i+1])==false)) {
			structNameSet.insert(vec[i + 1]);
		}
		else if (vec[i] == "enum" && i + 1 < size&&(is_keyword(vec[i+1])==false))
			enumNameSet.insert(vec[i + 1]);
		else if (vec[i] == "typedef") {
			if (i + 2 < size && structNameSet.count(vec[i + 1]) >= 1) {
				if (is_keyword(vec[i + 2]))enumNameSet.insert(vec[i + 2]);
				i += 2;
			}
			else {
				unsigned int left_count = 0,right_count=0;
				int j = i;
				while (j < size) {
					if (vec[j] == "{")left_count++;
					else if (vec[j] == "}")right_count++;
					else if (left_count == right_count)break;
				}
				if (j + 1 < size&&is_keyword(vec[j+1])==false) {
					structNameSet.insert(vec[j + 1]);
					i += 1;
				}
			}
		}
	}
	
}

bool CNNCodeRecheck::is_digit(string& token)
{
	std::regex regDigit("\\d+"), regReal("\\d+.\\d+");
	bool isDigit = std::regex_match(token, regDigit);
	bool isReal = regex_match(token, regReal);
	if (isDigit || isReal)return true;
	return false;
}

inline bool CNNCodeRecheck::is_char(string& token)
{
	int quote = token.find("\"", 0);
	int sigleQuote = token.find("\'", 0);
	if (quote != -1 || sigleQuote != -1)return true;
	return false;
}

bool CNNCodeRecheck::is_Real_digit_char(string& token)
{
	if (is_digit(token) || is_char(token))return true;
	return false;
}

bool CNNCodeRecheck::is_keyword(string str)
{
	static vector<string>vec{ "for","while","switch","if","else","return","=","*","+","-","/"
							 ,"==","<=",">=","<",">",")","(","goto","do","[","]","+=","-=","*=","/=","struct","union",
							 ",",";","case","break","default","{","}",":","enum","void","|"
							 ,"!","FILE","NULL",".","#define","sizeof","stdin","\n","typedef",
							 "printf","fflush","gets","scanf","fopen","fwrite","fclose","getch",
							  "feof","fread","system","strcmp","&","extern","#ifndef","#endif","itoa"
							  "#if","free","malloc","memset","#else","static","continue","#undef","assert","%",
							  "enum"};
	for (int i = 0; i < vec.size(); i++) {
		if (str == vec[i])return true;
	}
	return false;
}
void CNNCodeRecheck::setFilePath(string filePath)
{
	file.set_file_path(filePath);
}

template<typename T>
inline size_t MVector<T>::size()
{
	return vec.size();
}

template<typename T>
inline T& MVector<T>::operator[](unsigned int index)
{
	return vec[index];
}

template<typename T>
T& MVector<T>::operator[](const unsigned int index) const
{
	return vec[index];
}

template<typename T>
const vector<T>& MVector<T>::operator+=(vector<T>& vec_obj)
{
	for (int i = 0; i < vec_obj.size(); i++) {
		vec.push_back(vec_obj[i]);
	}
	return this->vec;
}


template<typename T>
void MVector<T>::push_back(T element)
{
	vec.push_back(element);
}
