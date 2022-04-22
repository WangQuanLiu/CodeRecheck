#include "LcsCodeRecheck.h"

double LcsCodeRecheck::lcs(vector<string>& vec_one, vector<string>& vec_two)
{
	int vec_one_size = vec_one.size();
	int vec_two_size = vec_two.size();
	vector<vector<int>> dp;
	dp.resize(vec_one_size);
	for (int i = 0; i < vec_one_size; i++) {
		dp[i].resize(vec_two_size);
		for (int j = 0; j < vec_two_size; j++) {
			dp[i][j] = 0;
		}
	}
	for (int i = 1; i < vec_one_size; i++) {
		for (int j = 1; j < vec_two_size; j++) {
			if (vec_one[i] == vec_two[j])
				dp[i][j] = dp[i - 1][j - 1] + 1;
			else 
				dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);

			//dp[i][j] = vec_one[i] == vec_two[j] ? dp[i - 1][j - 1] + 1 : max(dp[i - 1][j], dp[i][j - 1]);
		}
	}
	vec_one_size -= 1;
	vec_two_size -= 1;
	double min=min(vec_one_size,vec_two_size);
	return (2.0*dp[vec_one_size][vec_two_size])/((double)vec_one_size+ (double)vec_two_size);
}

void LcsCodeRecheck::readFile()
{
	file.read_file();
}

void LcsCodeRecheck::setFilePath(string filePath)
{
	file.set_file_path(filePath);
}
vector<string>& LcsCodeRecheck::get_removed_text()
{
	return file.get_removed_text();
}
//void LcsCodeRecheck::statistics_file_var(string& token)
//{
//	if (token == "double" || token == "int" || token == "char" || token == "float") {
//		string nextToken = file.get_token();
//		int index1 = nextToken.find("["), index2 = nextToken.find("]");
//		if (token == "double" || token == "float") {
//			if (index1 != -1 && index2 != -1) {
//				//status[REAL_ARRAY_NUMBER]++;
//
//			}
//			else status[REAL_VAR_NUMBER]++;
//		}
//		else if (token == "int") {
//			if (index1 != -1 && index2 != -1) {
//				status[INTEGER_ARRAY_NUMBER]++;
//			}
//			else status[INTEGER_VAR_NUMBER]++;
//		}
//		else if (token == "char") {
//			if (index1 != -1 && index2 != -1) {
//				status[CHAR_ARRAY_NUMBER]++;
//			}
//			else status[CHAR_VAR_NUMBER]++;
//		}
//		file.roll_back();
//	}
//	std::regex regDigit("\\d+"), regReal("\\d+.\\d+");
//	bool isDigit = std::regex_match(token, regDigit);
//	bool isReal = regex_match(token, regReal);
//	int leftQuote = token.find("\"", 0);
//	int leftSigleQuote = token.find("\'", 0);
//	if (isDigit) {
//		status[INTEGER_CONSTANT_NUMBER]++;
//	}
//	else if (isReal) {
//		status[REAL_CONSTANT_NUMBER]++;
//	}
//	else if (leftQuote != -1) {
//		int rightQuote = token.find("\"", leftQuote + 1);
//		if (rightQuote != -1)status[CHAR_CONSTANT_NUMBER]++;
//	}
//	else if (leftSigleQuote != -1) {
//		int rightSigleQuote = token.find("\'", leftSigleQuote + 1);
//		if (rightSigleQuote != -1)status[CHAR_CONSTANT_NUMBER]++;
//	}
//
//}
