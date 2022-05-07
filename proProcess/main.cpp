#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<vector>
#include<cmath>
#include<climits>
#include<io.h>
#include<algorithm>
#include"AttributeCount.h"
#include"LcsCodeRecheck.h"
#include"CNNCodeRecheck.h"
using namespace std;
void getFileNames(string path, vector<string>& files) {
	//文件句柄
	//注意：我发现有些文章代码此处是long类型，实测运行中会报错访问异常
	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,递归查找
			//如果不是,把文件绝对路径存入vector中
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFileNames(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
bool isDirExist(const string& dirPath) {
	if (_access(dirPath.c_str(), 0) == 0)return true;
	return false;
}

void train_cnn(const string& filePath) {
	CNNCodeRecheck cnn(filePath);
	cnn.read_file();
	cnn.unify_file_style();
	cnn.save_CNN_file(filePath);
}
void dataUnorderWrite(string datasetDir, string writeDataDir) {
	vector<string>datasetVec, writeVec;
	getFileNames(datasetDir, datasetVec);
	getFileNames(writeDataDir, writeVec);

	int index = 0;
	for (string v : writeVec) {
		fstream file(datasetVec[index++], ios::in);
		fstream file2(v, ios::out);
		try {
			if (!file||!file2)throw FileError(v);
		}
		catch (FileError& error) {
			error.showExceptionInform();
			return ;
		}
		char tempCharacter[1024];
		vector<string>data;
		while (!file.eof()) {	//读出文件数据到saveList
			file.getline(tempCharacter, FILE_LINE_MAX_NUMBER);
			string temp = tempCharacter;
			data.push_back(temp);
		}
		for (string temp : data) {
			temp += "\n";
			file2.write(temp.c_str(), temp.size());
		}
		file.close();
		file2.close();
	}

}
int main() {

	//train_cnn("C:\\Users\\lwq\\Desktop\\trainData\\source\\7\\L7-1-1.c");
	vector<string>fileNames;
	string path("C:\\Users\\lwq\\Desktop\\trainData");
	getFileNames(path, fileNames);
	string fl = "positive";
	string f2 = "source";
	string f3 = ".c";
	string f4 = ".txt";
	for (string ph : fileNames) {
		//cout << ph << "\n";
		//string p = ph.replace(ph.find(f2),f2.size() , fl);
		//string pp = p.replace(p.find(f3), f3.size(), f4);
		//cout << pp << endl;
		CNNCodeRecheck cnn(ph);
		cnn.read_file();
		cnn.unify_file_style();
		cnn.save_CNN_file();
	}
	//string file1 = "C:\\Users\\lwq\\Desktop\\dataset";
	//string file2 = "C:\\Users\\lwq\\Desktop\\trainData\\negative";
	//dataUnorderWrite(file1, file2);
	system("pause");
	return 0;
}


