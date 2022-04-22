#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<vector>
#include<cmath>
#include<climits>
#include"AttributeCount.h"
#include"LcsCodeRecheck.h"
using namespace std;
double compare_one(double diff_value, double mean) {
	double two = 1.0 + ((diff_value) / 79.0);
	return 1.0 / two;
}
double repeat_rate(vector<unsigned int>code_one, vector<unsigned int>code_two) {
	double mean_one = 0,mean_two=0;
	/*double max_one = 0, min_one = INT_MAX;
	double max_two = 0, min_two = INT_MAX;
	for (int i = 1; i < code_one.size(); i++) {
		mean_one += code_one[i];
		mean_two += code_two[i];
		if (max_one < code_one[i])max_one = code_one[i];
		if (min_one > code_one[i])min_one = code_one[i];
		if (max_two < code_two[i])max_two = code_two[i];
		if (min_two > code_two[i])min_two = code_two[i];
	}
	double diff_value_one = max_one - min_one;
	double diff_value_two = max_two - min_two;
	mean_one /= code_one.size();
	mean_two /= code_two.size();
	if (diff_value_one > 2 * mean_one) {
		for (int i = 1; i < code_one.size(); i++) {
			if (code_one[i] > mean_one * 2)code_one[i] = mean_one * 2;
			
		}
	}
	if(diff_value_two>2*mean_one){
		for (int i = 1; i < code_two.size(); i++) {
			if (code_two[i] > mean_two * 2)code_two[i] = mean_two * 2;

		}
	}*/
	double sum_one = 0;
	for (int i = 1; i < code_one.size(); i++) {
		sum_one += (code_one[i]) * (code_two[i]);
	}
	double sum_two = 0,sum_three=0;
	for (int i = 1; i < code_one.size(); i++) {
		sum_two += code_one[i] * code_one[i];
	}
	for (int i = 1; i < code_two.size(); i++)
		sum_three += code_two[i] * code_two[i];
	sum_two = sqrt(sum_two);
	sum_three = sqrt(sum_three);
	return sum_one/(sum_two*sum_three);
}
int main() {
	
	/*AttributeCount attributeCount("C:\\Users\\lwq\\Desktop\\data\\1\\1.c");
	attributeCount.read_file();
	attributeCount.statistics_file_status();
	AttributeCount attributeCount2("C:\\Users\\lwq\\Desktop\\data\\2\\2.c");
	attributeCount2.read_file();
	attributeCount2.statistics_file_status();
	std::cout << repeat_rate(attributeCount.get_status(), attributeCount2.get_status());

	std::cout << compare_one(10,39);*/
	LcsCodeRecheck lcs("C:\\Users\\lwq\\Desktop\\data\\1\\1.c");
	lcs.readFile();
	LcsCodeRecheck lcs2("C:\\Users\\lwq\\Desktop\\data\\3\\3.c");
	lcs2.readFile();
	cout << LcsCodeRecheck::lcs(lcs.get_removed_text(), lcs2.get_removed_text());
	system("pause");
	return 0;
}


