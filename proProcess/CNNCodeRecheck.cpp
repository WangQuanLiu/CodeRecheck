#include "CNNCodeRecheck.h"
bool CNNCodeRecheck::unify_file_function_style(string& token)
{
	unsigned int size = 0;
	string temp = file.get_token();
	vector<string> vec;
	string nextTemp;
	if (token == "struct"||token=="union") {
		temp = "struct_name";
		unifyedText.push_back(token);
		unifyedText.push_back(temp);
		return true;
	}
	else if (is_keyword(token) == false && temp == "(") { //token为函数定义或函数调用
		vec.push_back(temp);
		temp = file.get_token();
		do {
			nextTemp = file.get_token();
			//if (temp == "int" || temp == "double" ||
			//	temp == "float" || temp == "char") {
			//	vec.push_back(temp);
			////	vec.push_back("val");
			////	file.get_token();
			//}
			if (is_Real_digit_char(temp) == false && is_keyword(temp) == false&&nextTemp!="(")
				vec.push_back("var");
			else if (is_keyword(temp)==false&&nextTemp == "(")
				vec.push_back("call_func");
			else 
				vec.push_back(temp);
			temp = nextTemp;
		} while (temp != ";" && temp != "{"&&temp!="");
		if (temp == "{") {
			vec.push_back(temp);
			unifyedText.push_back("func");
			unifyedText += vec;
		}
		else {
			unifyedText.push_back("call_func");
			unifyedText += vec;
		}
		return true;
	}
	else {
		file.roll_back();
		return false;
	}
}

void CNNCodeRecheck::unify_file_style()
{
	while (file.get_token_size()) {
		string temp = file.get_token();
		if (temp == "")continue;
		else if (temp == "\n") {
			unifyedText.push_back(temp);
		}
		else  if (unify_file_function_style(temp) == false&& unify_var_style(temp)==false)
			unifyedText.push_back(temp);
	}
}

bool CNNCodeRecheck::unify_var_style(string& token)
{
	if (is_keyword(token) == true||
		is_Real_digit_char(token)==true)
		return false;
	else {
		unifyedText.push_back("var");
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
		outFile.write(" ", sizeof(" "));
	}

	outFile.close();
	return true;
}

bool CNNCodeRecheck::is_Real_digit_char(string& token)
{
	std::regex regDigit("\\d+"), regReal("\\d+.\\d+");
		bool isDigit = std::regex_match(token, regDigit);
		bool isReal = regex_match(token, regReal);
		int quote = token.find("\"", 0);
		int sigleQuote = token.find("\'", 0);
		if (isDigit ||isReal || quote != -1 || sigleQuote != -1)return true;
	
	return false;
}

bool CNNCodeRecheck::is_keyword(string str)
{
	static vector<string>vec{ "for","while","switch","if","else","return","=","+","-","*","/"
							 ,"==","<=",">=","<",">",")","(","goto","do","int","float",
							 "double","char","[","]","+=","-=","*=","/=","struct","union",
							 ",",";","case","break","default","{","}",":"};
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
