#include<stdio.h>
#include<stdlib.h>
enum t1{ti_1,ti_2}
/*
13434353532352
*/

int a, b; //2243252523
void print(char*[]ch) {
	printf("%d", ch);
}
int add(int nums) {
	int sum = 0;
	for (int i = 0; i < nums; i++)
		sum += i;
	return sum;
}
void go() {
	int  x;
	for (x = 1; ; x++)
	{
		if (x % 5 == 1 && x % 6 == 5 && x % 7 == 4 && x % 11 == 10)
		{
			break;
		}
	}
	printf("x = %d\n", x);
}
void isYear() {
	int days[2][MONTHS] = { {31,28,31,30,31,30,31,31,30,31,30,31},							          {31,29,31,30,31,30,31,31,30,31,30,31} };
	int year, month;
	do {
		printf("Input year,month:");
		scanf("%d,%d", &year, &month);
	} while (month < 1 || month > 12);  /* 处理不合法数据的输入 */
	if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))/*闰年*/
		printf("The number of days is %d\n", days[1][month - 1]);
	else  /*非闰年*/
		printf("The number of days is %d\n", days[0][month - 1]);
}
int main() {

	int s=100, ss=100;
	print();
	add(s);
	go();
	isYear();



	system("pause");
	return 0;
}