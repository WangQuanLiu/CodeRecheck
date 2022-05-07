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
