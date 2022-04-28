#include "CNNCodeRecheck.h"
bool CNNCodeRecheck::unify_file_function_style(string& token)
{
	unsigned int size = 0;
	string temp = file.get_token();
	vector<string> vec;
	string nextTemp;
	if (token == "struct"||token=="union") {
		nextTemp = file.get_token();
		if (is_struct_value(temp) && is_keyword(nextTemp) == false) {
			unifyedText.push_back(token);
			unifyedText.push_back(UNIFED_STRUCT_NAME);
			unifyedText.push_back(UNIFED_STRUCT_VAR);
			return true;
		}
		structNameSet.insert(temp);
		temp = UNIFED_STRUCT_NAME;
		unifyedText.push_back(token);
		unifyedText.push_back(temp);
		file.roll_back();
		return true;
	}
	else if (token == "enum") {
		enumNameSet.insert(temp);
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
			if (temp == "int" || temp == "double" ||
				temp == "float" || temp == "char") {
				vec.push_back(UNIFED_DATA_TYPE);
			}
			else if (is_struct_value(temp)&&is_keyword(nextTemp)) {
				vec.push_back(UNIFED_STRUCT_NAME);
			}
			else if (is_char(temp)) {
				vec.push_back(UNIFED_CHAR);
			}
			else if ( is_keyword(temp) == false&&nextTemp!="(")
				vec.push_back(UNIFED_VAR);
			else if (is_keyword(temp)==false&&nextTemp == "(")
				vec.push_back(UNIFED_CALL_FUNC_NAME);
			
			else 
				vec.push_back(temp);
			temp = nextTemp;
		} while (temp != ";" && temp != "{"&&temp!="");
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

bool CNNCodeRecheck::is_struct_value(string& token)
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
	static vector<string>vec{ "float","double","int","char" };
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
		else if (is_struct_value(temp) == true) {
			//unifyedText.push_back(UNIFED_STRUCT_VAR);
			string nextTemp = file.get_token();
			if (is_keyword(nextTemp) == false) {
				unifyedText.push_back(UNIFED_STRUCT_NAME);
				unifyedText.push_back(UNIFED_STRUCT_VAR);
			}
			else file.roll_back();
		}
		else if (is_digit(temp) == true) {
			unifyedText.push_back(UNIFED_DIGIT);
		}
		else if (is_char(temp) == true)
			unifyedText.push_back(UNIFED_CHAR);
		else if (is_dataType(temp) == true)
			unifyedText.push_back(UNIFED_DATA_TYPE);
		else  if (unify_file_function_style(temp) == false &&
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
	else if (is_struct_value(token)) {
		unifyedText.push_back(UNIFED_STRUCT_VAR);
		return true;
	}
	else {
		unifyedText.push_back(UNIFED_VAR);
		return true;
	}
}

bool CNNCodeRecheck::read_file()
{
	return file.read_file();
}

bool CNNCodeRecheck::save_CNN_file()
{
	string savefilePath= file.get_fileDir() + "\\" + file.get_filename() + "_cnn" + ".lex";
	ofstream outFile(savefilePath);
	try {
		if (!outFile) {
			throw  FileError(savefilePath);
		}
	}
	catch (FileError& e) {
		e.showExceptionInform();
		return false;
	}
	for (int i = 0; i < unifyedText.size(); i++) {
		/*if (unifyedText[i] == "\n") {
			outFile.write("\n", 2);
		}*/
		outFile.write(unifyedText[i].c_str(), unifyedText[i].size());
		outFile.put(' ');
		//outFile.write(' ', sizeof(" "));
	}

	outFile.close();
	return true;
}

inline void CNNCodeRecheck::statistics_struct_name()
{
	vector<string>vec = file.get_vecToken();
	size_t size = vec.size();
	for (int i = 0; i < size; i++) {
		if ((vec[i] == "struct" || vec[i] == "union") && i + 1 < size)
			structNameSet.insert(vec[i + 1]);
		else if (vec[i] == "enum" && i + 1 < size)
			enumNameSet.insert(vec[i + 1]);
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
	/*std::regex regDigit("\\d+"), regReal("\\d+.\\d+");
		bool isDigit = std::regex_match(token, regDigit);
		bool isReal = regex_match(token, regReal);
		int quote = token.find("\"", 0);
		int sigleQuote = token.find("\'", 0);
		if (isDigit ||isReal || quote != -1 || sigleQuote != -1)return true;*/
	if (is_digit(token) || is_char(token))return true;
	return false;
}

bool CNNCodeRecheck::is_keyword(string str)
{
	static vector<string>vec{ "for","while","switch","if","else","return","=","+","-","*","/"
							 ,"==","<=",">=","<",">",")","(","goto","do","int","float",
							 "double","char","[","]","+=","-=","*=","/=","struct","union",
							 ",",";","case","break","default","{","}",":","enum","void","|"
							 ,"!","FILE","NULL",".","#define","sizeof","stdin"};
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
