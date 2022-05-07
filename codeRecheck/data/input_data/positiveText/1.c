#include<stdio.h>
#include<stdlib.h>
enum t1{ti_1,ti_2}
/*
13434353532352
*/
struct db {
	int a, b;
	union db2 {
		double temp1;
		int temp2;
	};
};
int a, b; //2243252523
void go_temp(int a,int b) {
	for (int i = 1; i <= 10; i++) {
		while (i) {
			switch (i)
			{
			case 1:
				return 1;
			case 2:
				return 2;
			default:
				break;
			}
		}
		if (i < 2)return 2;
		else if (i < 3)return 3;
		else if (i < 4)return 4;
	}
}
int add_temp(int a, int b) {
	return a + b;
}
int sub_temp (int a, int b) {
	return a - b;
}
int mult_temp(int a, int b) {
	return a * b;
}
int div1_temp(int a, int b) {
	return (int)(a / b);
}
int cal_temp(int oper,int a, int b) {
	switch (oper)
	{
	case 1:
		return add(a, b); break;
	case 2:
		return sub(a, b); break;
	case 3:
		return mult(a, b); break;
	case 4:
		return div1(a, b); break;
	default:
		break;
	}
}
int[] go_temp(int a,int b){
	int p[3]={0};
	return p;
}

int main() {

	db t3;
	int a1, b2;
	int c[2];
	double db3[3];
	char ch2[3];
	char ch32='c';
	a1 = 20, b2 = 20;
	double db1=34.6;
	printf("%d", cal(1, a1/c1, (b1+c1)*a1));




	system("pause");
	return 0;
}