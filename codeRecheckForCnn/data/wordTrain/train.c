#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define NUM 5

/* 【自学去】网站收集 http://www.zixue7.com */

struct item{
	char brand[20];
	char id[10];
	float in_price;
	float out_price;
	int   storage;
};
struct item_node{
	struct item wanted;
	int    amount;
	struct item_node *next;
};
struct item goods[NUM];
struct item_node *cart;
void* main()
{
	printf("***********************************\n");
	printf("     欢迎进入超市管理系统\n");
	printf("***********************************\n");
	while(1)
	{
		switch(menu())
		{
		case 1:
			establish();break;
		case 2:
			dis_all();break;
		case 3:
			shop_cart();break;
		case 4:
			calculate();break;
		case 5:
			printf("感谢使用，再见!\n");
			exit(0);
		}
	}
}

int menu()
{
	char str[5];
	int  select;
	printf("\n\n请选择数字进行操作\n");
	printf("1.建立库存信息\n");
	printf("2.显示所有信息\n");
	printf("3.购物车\n");
	printf("4.结算\n");
	printf("5.退出\n");
	printf("请选择对应数字1--5");
	while(1)
	{
		fflush(stdin);
		gets(str);
		select=atoi(str);
		if(select<1||select>5)
			printf("输入错误，请重新输入:");
		else
			break;
		
	}
	return select;
	
}

void dis_all()
{
	int i;
	FILE  *fp;
	fp=fopen("goods","r");
	for(i=0;(fread(goods+i,sizeof(struct item),1,fp))!=0;i++)
	{
		printf("---------------------------------\n");
		printf("货品 品名  单价       库存量\n");
		printf("%s%7s%7.2f%8d\n",goods[i].id,goods[i].brand,goods[i].out_price,goods[i].storage);
		
	}
	fclose(fp);
}


void shop_cart()
{
	while(1)
	{
		switch(cart_menu())
		{
		case 1:
			display();break;
		case 2:
			add();break;
		case 3:
			return;
		}
	}
}
int  cart_menu()
{
	char  str[5];
	int   select;
	printf("\n请选择操作\n");
	printf("-----------------------\n");
	printf("1.显示当前购物列表\n");
	printf("2.添加商品\n");
	printf("3.退出\n");
	printf("-----------------------\n\n");
	while(1)
	{
		fflush(stdin);
		gets(str);
		select=atoi(str);
		if(select<1||select>3)
			printf("输入错误，请重新输入:");
		else
			break;
	}
	return select;
}

void display()
{
	struct item_node *p=cart;
	if(p==NULL){
		printf("购物车为空\n");
		return ;
	}
	while(p!=NULL){
		printf("----------------------------------\n");
		printf("货号                品名  单价   数量\n");
		printf("%10s%20s%7.2f%8d\n",p->wanted.id,p->wanted.brand,p->wanted.out_price,p->amount);
		p=p->next;
	}
}

void add()
{
	FILE *fp;
	int   i,n;
	char  str[20];
	char  choice1,choice2;
	struct item_node *p,*p1;
	do
	{
		printf("输入所需物品的名称或货号: ");
		fflush(stdin);
		gets(str);
		if((fp=fopen("goods","r"))==NULL){
			printf("打开文件失败\n");
			continue;
		}
		for(i=0;fread(goods+i,sizeof(struct item),1,fp)!=0;i++){
			if((strcmp(goods[i].brand,str)==0||strcmp(goods[i].id,str)==0)&&goods[i].storage!=0){
				printf("已经找到所需物品:  \n");
				printf("---------------------\n");
				printf("货号  品名  单价  库存量\n");
				printf("%s%6s%3.2f%4d\n",goods[i].id,goods[i].brand,goods[i].out_price,goods[i].storage);
				printf("请输入所需数量: ");
				scanf("%d",&n);
				if(n>goods[i].storage){
					printf("库存不足\n");
					break;
				}
				printf("\n是否购买?(Y/N)");
				fflush(stdin);
				choice1=getchar();
				if(choice1=='Y'||choice1=='y'){
					p1=(struct item_node*)malloc(sizeof(struct item_node));
					if(p1==NULL){
						printf("内存申请失败!\n");
						exit(1);
					}
					p1->amount=n;
					p1->wanted=goods[i];
					p1->next=NULL;
					p=cart;
					if(cart==NULL)
						cart=p1;
					else{
						while(p->next!=NULL)
							p=p->next;
						p1->next=p->next;
						p->next=p1;
					}
				}
				break;
			}
		}
		if(i==NUM)
			printf("未找到所需物品\n");
		fclose(fp);
		printf("是否继续购物?(Y/N)");
		fflush(stdin);
		choice2=getchar();
	}while(choice2=='Y'||choice2=='y');
}


void  establish(){
	FILE *fp;
	int   i;
	printf("请依次输入货物信息:\n");
	printf("----------------------------\n");
	for(i=0;i<NUM;i++)
	{
		printf("品名: ");
		fflush(stdin);
		gets(goods[i].brand);
		printf("货号: ");
		fflush(stdin);
		gets(goods[i].id);
		printf("进价: ");
		fflush(stdin);
		scanf("%f",&goods[i].in_price);
		printf("哨价: ");
		fflush(stdin);
		scanf("%f",&goods[i].out_price);
		printf("数量: ");
		fflush(stdin);
		scanf("%d",&goods[i].storage);
		printf("\n");
	}
	if((fp=fopen("goods","w"))==NULL){
		printf("创建文件失败.\n");
		return;
	}
	fwrite(goods,sizeof(struct item),NUM,fp);
	fclose(fp);
}

void calculate()
{
	float total=0,pay;
	struct item_node *p;
	int   i;
	FILE   *fp;
	printf("以下是购物清单:  \n");
	display();
	if((fp=fopen("goods","r"))==NULL){
		printf("打开文件失败:  \n");
		return;
	}
	for(i=0;(fread(goods+i,sizeof(struct item),1,fp))!=0;i++);
	fclose(fp);
	p=cart;
	while(p!=NULL){
		total+=p->wanted.out_price*p->amount;
		for(i=0;strcmp(goods[i].id,p->wanted.id)!=0;i++);
		goods[i].storage-=p->amount;
		p=p->next;
	}
	printf("总计 %7.2f",total);
	printf("\n输入实付金额: ");
	scanf("%f",&pay);
	printf("实付:         %7.2f    找零:          %7.2f",pay,pay-total);
	if((fp=fopen("goods","w"))==NULL){
		printf("打开文件失败.\n");
		return;
	}
	fwrite(goods,sizeof(struct item),NUM,fp);
	fclose(fp);
}
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<dos.h>
#include<string.h>
#define LEN sizeof(struct student)
#define FORMAT "%-8d%-15s%-12.1lf%-12.1lf%-12.1lf%-12.1lf\n"
#define DATA stu[i].num,stu[i].name,stu[i].elec,stu[i].expe,stu[i].requ,stu[i].sum

/* 【自学去】网站收集 http://www.zixue7.com */

struct student/*定义学生成绩结构体*/
{ int num;/*学号*/
  char name[15];/*姓名*/
  double elec;/*选修课*/
  double expe;/*实验课*/
  double requ;/*必修课*/
  double sum;/*总分*/
};
struct student stu[50];/*定义结构体数组*/
void in();/*录入学生成绩信息*/
void show();/*显示学生信息*/
void order();/*按总分排序*/
void del();/*删除学生成绩信息*/
void modify();/*修改学生成绩信息*/
void menu();/*主菜单*/
void insert();/*插入学生信息*/
void total();/*计算总人数*/
void search();/*查找学生信息*/
void main()/*主函数*/
{ int n;
  menu();
  scanf("%d",&n);/*输入选择功能的编号*/
  while(n)
  { switch(n)
     { case 1: in();break;
       case 2: search();break;
       case 3: del();break;
       case 4: modify();break;
       case 5: insert();break;
       case 6: order();break;
       case 7: total();break;
       default:break;
     }
    getch();
    menu();/*执行完功能再次显示菜单界面*/
    scanf("%d",&n);
  }
}

void in()/*录入学生信息*/
{ int i,m=0;/*m是记录的条数*/
  char ch[2];
  FILE *fp;/*定义文件指针*/
  if((fp=fopen("data.txt","a+"))==NULL)/*打开指定文件*/
     { printf("can not open\n");return;}
  while(!feof(fp)) { 
	  if(fread(&stu[m] ,LEN,1,fp)==1)
		  m++;/*统计当前记录条数*/
  }
  fclose(fp);
  if(m==0) 
	  printf("No record!\n");
  else 
  {
	  system("cls");
          show();/*调用show函数，显示原有信息*/
  }
  if((fp=fopen("data.txt","wb"))==NULL)
     { printf("can not open\n");return;}
  for(i=0;i<m;i++) fwrite(&stu[i] ,LEN,1,fp);/*向指定的磁盘文件写入信息*/
  printf("please input(y/n):");
  scanf("%s",ch);
while(strcmp(ch,"Y")==0||strcmp(ch,"y")==0)/*判断是否要录入新信息*/
    {
    printf("number:");scanf("%d",&stu[m].num);/*输入学生学号*/
    for(i=0;i<m;i++)
	    if(stu[i].num==stu[m].num)
	    {
	    printf("the number is existing,press any to continue!");
	    getch();
	    fclose(fp);
	    return;
	    }
     printf("name:");scanf("%s",stu[m].name);/*输入学生姓名*/
     printf("elective:");scanf("%lf",&stu[m].elec);/*输入选修课成绩*/
     printf("experiment:");scanf("%lf",&stu[m].expe);/*输入实验课成绩*/
     printf("required course:");scanf("%lf",&stu[m].requ);/*输入必修课成绩*/
     stu[m].sum=stu[m].elec+stu[m].expe+stu[m].requ;/*计算出总成绩*/
     if(fwrite(&stu[m],LEN,1,fp)!=1)/*将新录入的信息写入指定的磁盘文件*/
       { printf("can not save!"); getch(); }
     else { printf("%s saved!\n",stu[m].name);m++;}
     printf("continue?(y/n):");/*询问是否继续*/
     scanf("%s",ch);
  }
  fclose(fp);
  printf("OK!\n");
 }

void show()
 { FILE *fp;
   int i,m=0;
   fp=fopen("data.txt","rb");
   while(!feof(fp))
   {
   if(fread(&stu[m] ,LEN,1,fp)==1) 
   m++;
   }  
   fclose(fp);
   printf("number  name           elective    experiment  required    sum\t\n");
   for(i=0;i<m;i++)
       { 
	   printf(FORMAT,DATA);/*将信息按指定格式打印*/
       }
     }
 
void menu()/*自定义函数实现菜单功能*/
{
  system("cls");
  printf("\n\n\n\n\n");
  printf("\t\t|---------------------STUDENT-------------------|\n");
  printf("\t\t|\t 0. exit                                |\n");
  printf("\t\t|\t 1. input record                        |\n");
  printf("\t\t|\t 2. search record                       |\n");
  printf("\t\t|\t 3. delete record                       |\n");
  printf("\t\t|\t 4. modify record                       |\n");
  printf("\t\t|\t 5. insert record                       |\n");
  printf("\t\t|\t 6. order                               |\n");
  printf("\t\t|\t 7. number                              |\n");
  printf("\t\t|-----------------------------------------------|\n\n");
  printf("\t\t\tchoose(0-7):");
}

void order()/*自定义排序函数*/
{ FILE *fp;
  struct student t;
  int i=0,j=0,m=0;
  if((fp=fopen("data.txt","r+"))==NULL)
     { 
	printf("can not open!\n");
        return;
  }
  while(!feof(fp)) 
  if(fread(&stu[m] ,LEN,1,fp)==1) 
	  m++;
  fclose(fp);
  if(m==0) 
  {
	  printf("no record!\n");
	  return;
  }
  if((fp=fopen("data.txt","wb"))==NULL)
     {
	  printf("can not open\n");
	  return;}
  for(i=0;i<m-1;i++)
      for(j=i+1;j<m;j++)/*双重循环实现成绩比较并交换*/
          if(stu[i].sum<stu[j].sum)
          { t=stu[i];stu[i]=stu[j];stu[j]=t;}
	  if((fp=fopen("data.txt","wb"))==NULL)
     { printf("can not open\n");return;}
  for(i=0;i<m;i++)/*将重新排好序的内容重新写入指定的磁盘文件中*/
      if(fwrite(&stu[i] ,LEN,1,fp)!=1)
       { 
        printf("%s can not save!\n"); 
        getch();
      }
  fclose(fp);
  printf("save successfully\n");
}
void del()/*自定义删除函数*/
{FILE *fp;
  int snum,i,j,m=0;
  char ch[2];
  if((fp=fopen("data.txt","r+"))==NULL)
     { printf("can not open\n");return;}
  while(!feof(fp))  if(fread(&stu[m],LEN,1,fp)==1) m++;
  fclose(fp);
  if(m==0) 
  {
	  printf("no record!\n");
	  return;
  }
  printf("please input the number:");
  scanf("%d",&snum);
    for(i=0;i<m;i++)
     if(snum==stu[i].num)
	     break;
     printf("find the student,delete?(y/n)");
     scanf("%s",ch);
      if(strcmp(ch,"Y")==0||strcmp(ch,"y")==0)/*判断是否要进行删除*/
      for(j=i;j<m;j++)
	      stu[j]=stu[j+1];/*将后一个记录移到前一个记录的位置*/
      m--;/*记录的总个数减1*/
      if((fp=fopen("data.txt","wb"))==NULL)
     { printf("can not open\n");return;}
  for(j=0;j<m;j++)/*将更改后的记录重新写入指定的磁盘文件中*/
      if(fwrite(&stu[j] ,LEN,1,fp)!=1)
       { printf("can not save!\n");
      getch();}
  fclose(fp);
  printf("delete successfully!\n");
}

void search()/*自定义查找函数*/
{ FILE *fp;
  int snum,i,m=0;
  char ch[2];
  if((fp=fopen("data.txt","rb"))==NULL)
     { printf("can not open\n");return;}
  while(!feof(fp))  if(fread(&stu[m],LEN,1,fp)==1) m++;
  fclose(fp);
  if(m==0) {printf("no record!\n");return;}
  printf("please input the number:");
  scanf("%d",&snum);
  for(i=0;i<m;i++)
     if(snum==stu[i].num)/*查找输入的学号是否在记录中*/
     { printf("find the student,show?(y/n)");
     scanf("%s",ch);
      if(strcmp(ch,"Y")==0||strcmp(ch,"y")==0) 
        {
          printf("number  name           elective    experiment  required    sum\t\n");
          printf(FORMAT,DATA);/*将查找出的结果按指定格式输出*/
	  break;
     }
     }   
  if(i==m) printf("can not find the student!\n");/*未找到要查找的信息*/
}

void modify()/*自定义修改函数*/
{ FILE *fp;
  int i,j,m=0,snum;
  if((fp=fopen("data.txt","r+"))==NULL)
     { printf("can not open\n");return;}
  while(!feof(fp))  
 if(fread(&stu[m],LEN,1,fp)==1) m++;
  if(m==0) {printf("no record!\n");
  fclose(fp);
  return;
  }
  printf("please input the number of the student which do you want to modify!\n");
  scanf("%d",&snum);
  for(i=0;i<m;i++)
	  if(snum==stu[i].num)/*检索记录中是否有要修改的信息*/
		  break;
	  printf("find the student!you can modify!\n");
	  printf("name:\n");
	  scanf("%s",stu[i].name);/*输入名字*/
          printf("\nelective:");
	  scanf("%lf",&stu[i].elec);/*输入选修课成绩*/
          printf("\nexperiment:");
	  scanf("%lf",&stu[i].expe);/*输入实验课成绩*/
          printf("\nrequired course:");
	  scanf("%lf",&stu[i].requ);/*输入必修课成绩*/
	  stu[i].sum=stu[i].elec+stu[i].expe+stu[i].requ;
	  if((fp=fopen("data.txt","wb"))==NULL)
     { printf("can not open\n");return;}
	  for(j=0;j<m;j++)/*将新修改的信息写入指定的磁盘文件中*/
	  if(fwrite(&stu[j] ,LEN,1,fp)!=1)
       { printf("can not save!"); getch(); }
  fclose(fp);
 }

void insert()/*自定义插入函数*/
{ FILE *fp;
  int i,j,k,m=0,snum;
  if((fp=fopen("data.txt","r+"))==NULL)
     { printf("can not open\n");return;}
  while(!feof(fp))  
 if(fread(&stu[m],LEN,1,fp)==1) m++;
  if(m==0) {printf("no record!\n");
  fclose(fp);
  return;
  }
  printf("please input position where do you want to insert!(input the number)\n");
  scanf("%d",&snum);/*输入要插入的位置*/
  for(i=0;i<m;i++)
	  if(snum==stu[i].num)
		  break;
	  for(j=m-1;j>i;j--)
           stu[j+1]=stu[j];/*从最后一条记录开始均向后移一位*/
	  printf("now please input the new information.\n");
          printf("number:");
	  scanf("%d",&stu[i+1].num);
	  for(k=0;k<m;k++)
	    if(stu[k].num==stu[i+1].num)
	    {
	    printf("the number is existing,press any to continue!");
	    getch();
	    fclose(fp);
	    return;
	    }
	  printf("name:\n");
	  scanf("%s",stu[i+1].name);
          printf("\nelective:");
	  scanf("%lf",&stu[i+1].elec);
          printf("\nexperiment:");
	  scanf("%lf",&stu[i+1].expe);
          printf("\nrequired course:");
	  scanf("%lf",&stu[i+1].requ);
	  stu[i+1].sum=stu[i+1].elec+stu[i+1].expe+stu[i+1].requ;
	  if((fp=fopen("data.txt","wb"))==NULL)
     { printf("can not open\n");return;}
	  for(k=0;k<=m;k++)
	  if(fwrite(&stu[k] ,LEN,1,fp)!=1)/*将修改后的记录写入磁盘文件中*/
       { printf("can not save!"); getch(); }
  fclose(fp);
 }

void total()
{ FILE *fp;
  int m=0;
  if((fp=fopen("data.txt","r+"))==NULL)
     { printf("can not open\n");return;}
  while(!feof(fp))  
	  if(fread(&stu[m],LEN,1,fp)==1) 
		  m++;/*统计记录个数即学生个数*/
  if(m==0) {printf("no record!\n");fclose(fp);return;}
  printf("the class are %d students!\n",m);/*将统计的个数输出*/
  fclose(fp);
 }
/*文本编辑器editor源代码*/

/* 【自学去】网站收集 http://www.zixue7.com */

#include <stdio.h>
#include <conio.h>
#include <bios.h>
#include <math.h>
#define LEFT 0x4b00   /*←：光标左移*/
#define RIGHT 0x4d00  /*→：光标右移*/
#define DOWN 0x5000   /*↓键：光标下移*/
#define UP 0x4800     /*↑键：光标上移*/
#define ESC 0x011b    /*ESC键：取消菜单打开操作*/
#define ENTER 0x1c0d  /*回车键：换行*/
#define DEL 21248     /*DEL键：删除当前字符*/
#define BACK 3592     /*BackSpace键：删除当前光标位置前一个字符*/
#define CL 29440      /*ctrl+←键：从右至左，选定文本*/
#define CR 29696      /*ctrl+→键：从左到右，选定文本*/
#define Cc 11779      /*ctrl+c键：将选定文本，复制一份到剪贴板中*/
#define Cv 12054      /*ctrl+v键：将剪贴板中的内容复制到当前位置*/
#define Cx 11544      /*ctrl+x键：对选定文本，执行剪切操作*/
#define F1 15104      /*F1键：打开文件菜单*/
#define F2 15360      /*F2键：打开编辑菜单*/
#define F3 15616      /*F3键：打开帮助菜单*/
#define F10 17408     /*F10键:进入文本快速预览模式*/
int value,backup,NUM;
/*value保存有值数组元素的最大下标值，backup保存value的副本，NUM保存当前行中的用户输入的字符个数*/
typedef struct record
 {
       char ch;  /*保存一字符*/
       int col, line;  /*x轴和y轴坐标*/
 }record;
record r[500]; /*定义一个有500个元素的结构体数组，保存选定的文本字符的属性*/

typedef struct node /*定义保存行中的单个字符的结构*/
 {
        char ch; /*数据域：保存一字符*/
        struct node *next; /*指针域：指向下一个结点的指针*/
 }node;/*由此类型节点构成的单链表，命名为：列单链表*/


  typedef struct Hnode /*定义保存所有列单链表首节点的指针的结构*/
 {
         node *next; /*指向列单链表的首节点的地址*/
         struct Hnode *nextl; /*指向下一个节点的指针*/
 }Hnode;/*由此类型节点构成的单链表，命名为：行单链表*/

void drawmain() /*画主窗口函数*/
 {
          int i,j;
          gotoxy(1,1);       /*在文本窗口中设置光标至(1,1)处*/
          textbackground(7); /*选择新的文本背景颜色,7为LIGHTGRAY淡灰色*/
          textcolor(0);      /*在文本模式中选择新的字符颜色0为BLACK黑*/
          insline();         /*在文本窗口的(1,1)位置处中插入一个空行*/
          for(i=1;i<=24;i++)
         {
              gotoxy(1,1+i);     /*（x,y)中x不变，y++*/
              cprintf("%c",196); /*在窗口左边输出-,即画出主窗口的左边界 */
              gotoxy(80,1+i);
              cprintf("%c",196); /*在窗口右边，输出-，即画出主窗口的右边界*/
         }
          for(i=1;i<=79;i++)
         {
               gotoxy(1+i,2);     /*在第2行，第2列开始*/
               cprintf("%c",196); /*在窗口顶端，输出-*/
               gotoxy(1+i,25);    /*在第25行，第2列开始*/
               cprintf("%c",196); /*在窗口底端，输出-*/
         }
          gotoxy(1,1);   cprintf("%c",196); /*在窗口左上角，输出-*/
          gotoxy(1,24);  cprintf("%c",196); /*在窗口左下角，输出-*/
          gotoxy(80,1);  cprintf("%c",196); /*在窗口右上角，输出-*/
          gotoxy(80,24); cprintf("%c",196); /*在窗口右下角，输出-*/
          gotoxy(7,1); cprintf("%c  %c File %c  %c",179,17,16,179);  /* | < > |*/
          gotoxy(27,1); cprintf("%c  %c Edit %c  %c",179,17,16,179); /* | < > |*/
          gotoxy(47,1); cprintf("%c  %c Help %c  %c",179,17,16,179); /* | < > |*/
          gotoxy(5,25); /*跳至窗口底端*/
          textcolor(1);
          cprintf(" Row:1         Col:1");
          gotoxy(68,25);
          cprintf("Version 2.0");
 }

void qview(Hnode *q) /*快速预览文本：开头：#，回车：* */
 {
       void view(Hnode *q); /*view()函数声明*/
       node *p;
       int i;
       window(1,1,80,25); /*定义文本窗口大小*/
       clrscr(); /*清屏*/
       /*循环读取两个单链表中的值：q是一个指向行单链表首节点的指针，
       此单链表数据域的值为实际保存各行字符的列单链表p中的首节点地址*/
       do{
              p=q->next;    /*p指向保存行数据的列单链表的首节点的地址*/
              cprintf("#"); /*每行开头，打印此字符，不管前面是否有回车符*/
              while(p!=NULL) /*循环读取单链表p中的值*/
             {
                    if(p->ch==13)  putch('*'); /*若为回车键，打印出*号*/
                    else
                    putch(p->ch);  /*输出各行中的字符到预览窗口*/
                    p=p->next; /*指向下一个节点*/
             }
             q=q->nextl; /*指向下一个节点*/
             printf("\n");/*输出一个回车*/
         }while(q!=NULL);

       getch();
       clrscr();
       drawmain();/*按任意键后，回到主窗口界面*/
       window(2,2,79,23);
       textbackground(9);
       for(i=0;i<24;i++)
            insline(); /*插入24个空行*/
       window(3,3,78,23);
       textcolor(10);
  }

void view(Hnode *q)  /*按行显示保存在单链表中的文本字符,q为指向行单链表中第一个节点的指针*/
 {
       node *p; /*p为保存列单链表节点元素地址的指针*/
       clrscr(); /*清屏*/
       /*双重循环，读取并显示保存在单链表中字符*/
       do{
              p=q->next;
              while(p!=NULL&&p->ch>=32&&p->ch<127&&p->ch!=13&&p->ch!=-1)  /*指针p不能为空，且数据域必须为常规字符*/
             {
                    putch(p->ch);/*在文本窗口中输出该字符*/
                    p=p->next;  /*指向下一个节点*/
             }
             q=q->nextl; /*指向下一个节点*/
             if((p->ch==13||p->ch==-1)&&q!=NULL)  gotoxy(1,wherey()+1); /*若ch为回车或EOF标记，光标跳至下行的开始处*/
         }while(q!=NULL); /*逐行逐列显示文本字符*/
 }
 
int check(Hnode *Hhead,int m,int n) /*check():在单链表中检查第m行第n列位置的字符，若为常规字符，则返回该字符*/
 {
        int i;
        Hnode *q;
        node *p;
        q=Hhead;
        for(i=1;i<m;i++) /*定位至行单链表中的第m个元素*/
            q=q->nextl;
        p=q->next;/*获取第m个节点的数据域*/
        for(i=1;i<n;i++) /*定位至列单链表中的第n个元素*/
            p=p->next;
        if(p->ch==13)  return -1; /*若第m行，第n列的字符为回车键，则返回-1*/
        if(p->ch>=32&&p->ch<127)  return p->ch; /*若第m行，第n列的字符为常规字符,则返回该字符*/
        else return 0; /*若第m行，第n列的字符既非回车符又非常规字符，则返回0*/
  }



int judge(Hnode *Hhead,int m) /*judge():返回第m行中的常规字符总的个数，不包括回车符*/
  {
        Hnode *q;
        node *p;
        int i,num=0;
        q=Hhead;
        for(i=1;i<m;i++) /*定位至行单链表中的第m个元素*/
            q=q->nextl;

        if(q==NULL)  return -1; /*返回-1,表示第m行不存在*/
        p=q->next;
        while(p->next!=NULL)
       {
                p=p->next;
                num++;       /*统计第m行的字符个数*/
       }
        /*行尾字符还没有判断，接下来判断行尾字符*/
        if(p->ch==13&&num==0)  return 0;    /*返回0,表示当前行只有一个回车字符*/
        if(p->ch>=32&&p->ch<127)  return num+1; /*返回num+1,表示当前行的最后一个字符为常规字符*/
        if(p->ch==13&&num!=0)  return num; /*返回num,表示当前行的最后一个字符为回车符，不计算在内*/
        else return 1;/*返回num,表示当前行中只有一个字符，且没有回车符*/
  }






  int del(Hnode *Hhead,int m,int n)  /*del():删除第m行，第n列位置的字符*/
 {
        Hnode *q,*q1;
        node *p1,*p2,*tail;
        int i,num=0,j,flag=0;
        q=Hhead;
        if(n==0&&m==1)  return; /*第1行，第0列不存在*/
        if(n==0&&m>1) /*若为第0列字符，但行必须大于1,执行向上行移处理*/
         {
               n=76;
               m=m-1;
               gotoxy(n,m);/*移至第m-1行，第76列*/
               flag=1; /*移位的标志置1*/
         }

        for(i=1;i<m;i++) /*定位至行单链表中的第m个元素*/
            q=q->nextl;
        p1=q->next;

        for(i=1;i<n-1;i++) /*定位至列单链表中的第n-1个元素*/
            p1=p1->next;
        p2=p1->next; /*p2指向列单链表中的第n个元素*/

             if(n==1) /*若是删除第m行第1列的字符*/
               {
                    q->next=p1->next;
                    free(p1);
               }
                else
               {
                    p1->next=p2->next; /*在单链表中删除第m行第n列的元素*/
                    free(p2);
               }

                /*删除掉第m行第n列的元素后，处理行单链表中第m个节点后的数据向前移的任务*/
                 while((num=judge(Hhead,m++))>0) /*执行一次judge(Head,m)后，m才加1.这里必须满足行常规字符数不为0的条件*/
                {
                         p1=q->next; q1=q;
                         if(p1!=NULL) /*若当前行非空*/
                        {
                                while(p1->next!=NULL)
                                    p1=p1->next;
                                tail=p1;/*tail保存列单链表最后一个元素的地址*/
                                q=q->nextl; /*指向下一行的元素的地址*/
                                p1=p2=q->next;
                                tail->next=p1; /*tail的指针域指向下一行的第一个元素的地址*/
                         }
                          else  /*若当前行的字符个数为0,即删除该字符后，只剩下回车符，则将下一个行单链表中节点的数据域移至前一下节点的数据域*/
                         {
                                 q=q->nextl;   p1=p2=q->next;
                                 q1->next=p1;/*q1->next指向下一行的第一个元素的地址*/
                          }

                         for(i=0;i<76-num;i++)
                         /*当前行还有76-num个空位没有字符，在下一行的单链表中读取字符，直至遇到回车符为止*/
                        {
                                p1=p2; /*p1指向p2的前一个节点,p2指向行单链表中下一个节点*/
                                p2=p2->next;
                                if(p2->ch==13)  break; /*若为回车，跳出循环*/
                         }
                          q->next=p2;   /*在列单链表中去掉移至上行的元素*/
                          p1->next=NULL;/*下行移至上行的最后一个元素，指针置空*/
                }
           return flag; /*返回0：表示没有换位，返回1：表示有换位*/
  }

 /*执行insert()后，检验第n行及后面的数据，使其满足规则*/
 int test(Hnode *Hhead,int n)
 {
       int i=0,num1=1;
       node *p1,*p2,*tail,*temp1,*temp2;
       Hnode *q;
       q=Hhead;
       for(i=1;i<n;i++) /*定位至行单链表中的第n个元素*/
            q=q->nextl;
       tail=p1=q->next;
       if(p1==NULL) return; /*若此行没有任何字符，则返回*/
       while(tail->next!=NULL) /*定位至列单链表中的最后一个元素*/
            tail=tail->next;

       /*若此单链表中没有回车符且有超过76个节点时，则p1会指向此列单链表中的第76个节点*/
       for(i=0;i<75;i++)
      {
             if(p1->ch==13||p1->next==NULL)  break;
             p1=p1->next;
      }

       p2=p1->next;
       p1->next=NULL; /*在此行的最后一个字符的前一个字符处断行,因为插入在此行插入了一个新的字符*/
       if(tail->ch!=13) /*若此行行尾不是回车键*/
      {
             if(p1->ch==13&&q->nextl==NULL)/*若p1的数据域为回车符且行单链表中只有n个节点*/
            {
                       q->nextl=(Hnode *)malloc(sizeof(Hnode)); /*新建一个行单链表节点，相当于添加一个新行*/
                       q->nextl->nextl=NULL;
                       tail->next=(node *)malloc(sizeof(node));/*在tail所指节点位置开始继续准备添加字符*/
                       tail->next->ch=13;   tail->next->next=NULL;
                       q->nextl->next=p2; /*新行单链表节点保存此行多出的字符*/
             }
             else /*若此行行尾和行中都没有回车键,或者q->nextl不为空*/
            {
                   q=q->nextl;/*q->nextl有可能为空*/
                   tail->next=q->next;/*将多出的字符与下一行的字符相连*/
                   q->next=p2;/**/
                   if(q!=NULL)  test(Hhead,++n); /*若行单链表第n个节点后还有节点，继续test()的相同处理*/
            }

      }
       else  /*若此列单链表最后一个元素为回车符*/
      {
            temp2=p2; /*p2指向第77个字符，或者为空(为空表示此行插入一个字符后，没有超出范围*/
            while(q!=NULL&&p2!=NULL) /*q指向行列表中的第n个节点.条件:行单链表中第n个节点存中且有第77个字符*/
             {  /*条件：在行单链表中只有n个节点,且字符超过了一行规定的76个，且num1标志为1*/
                 if((q->nextl==NULL)&&(p1!=tail||p2!=NULL)&&(num1==1))
                {
                       num1++;
                       q->nextl=(Hnode *)malloc(sizeof(Hnode)); /*新建一个行单链表节点,准备存储此行中多出的字符*/
                       q->nextl->nextl=NULL;   q->nextl->next=NULL; /*初始化值*/

                }
                /*行单链表中第n+1个节点已经存在，下面为在行单链表中插入一个新的节点*/
                 q=q->nextl; /*q指向行列表中的第n+1个节点*/
                 temp1=q->next;
                 q->next=temp2; /*q的数据域为此行中多出的字符所在的列单链表中的节点地址*/
                 temp2=temp1;
           }
      }
 }

void insert(Hnode *Hhead,int m,int n, char a) /*第m行，第n列的位置之前一个位置，插入单字符*/
 {
        int i;
        Hnode *q;
        node *p,*p1,*p2;
        q=Hhead;
        for(i=1;i<m;i++) /*定位至行单链表中的第m个元素*/
            q=q->nextl;
        p1=q->next;
        for(i=1;i<n-1;i++) /*定位至列单链表中的第n-1个元素*/
            p1=p1->next;
        p=(node *)malloc(sizeof(node)); /*创建一个新的列单链表节点*/
        p->ch=a; /*给此节点的数据域赋值*/
        if(n==1) /*插入之前，若只有一个字符在行中，则插在此节点之前*/
       {
               p->next=q->next;
               q->next=p;
       }
        else
       {
                 p->next=p1->next; /*在第m行，第n列的字符前，插入一字符*/
                 p1->next=p;
       }
    test(Hhead,m); /*在插入新元素后，检验并处理单链表中第m行开始的元素，使其满足规则*/

 }

/*对控制键进行响应，A：按键的整数值，Hhead:行单链表的首地址*/
void control(int A, Hnode *Hhead)
   {
           void colorview(Hnode *,int,int); /*函数声明*/
           int x,y,flag=0;
           x=wherex();   y=wherey(); /*得到当前光标的坐标值*/
           if((A==CL)&&(x!=1)) /*ctrl+←，当前光标不是在行首,光标移动*/
                gotoxy(wherex()-1,wherey());

           if((A==CL)&&(x==1)) /*ctrl+←，在行首*/
                gotoxy(abs(judge(Hhead,wherey()-1)),wherey()-1); /*judge(Hhead,wherey()-1)上一行的字符个数作为x值，光标移动*/

           if((A==CR)&&check(Hhead,wherey(),wherex())>0) /*ctrl+→,当前光标的右边有字符，光标移动*/
          {    flag=1;  gotoxy(wherex()+1,wherey()); }

           if((A==CR)&&check(Hhead,wherey()+1,1)>0&&check(Hhead,y,x)==0) /*ctrl+→,当前光标处没有字符但下一行的第一列有字符，光标移动*/
          {    flag=1;  gotoxy(1,wherey()+1);  }

           if((A==CR)&&x==76) /*ctrl+→，当前光标在当前行的行尾，光标移动*/
          {    flag=1;      gotoxy(1,wherey()+1);  }

           if(A==CR&&flag==1) /*ctrl+→，光标已经跳至新处，将当前光标所在位置的字符的坐标和值保存在r数组中*/
          {
             r[abs(value)].col=wherex();
             r[abs(value)].line=wherey();
             r[abs(value)].ch=check(Hhead,r[abs(value)].line,r[abs(value)].col);
             if(r[abs(value)].ch==-1)   r[abs(value)].ch=13; /*若第line行，第col列的字符为回车键，则返回-1*/
             value--;

          }

           if(A==CL&&(x!=1||y!=1))   /*ctrl+←,当前光标并不在窗口左上角,将当前光标所在位置的字符的坐标和值保存在r数组中*/
          {

             r[abs(value)].col=wherex();
             r[abs(value)].line=wherey();
             r[abs(value)].ch=check(Hhead,r[abs(value)].line,r[abs(value)].col);
             value++;
          }

           colorview(Hhead,wherex(),wherey());

  }

/*用不同的前背景色显示选择的字符*/
void colorview(Hnode *Hhead,int x,int y)
  {

           int i;
           view(Hhead);/*重新显示所有文本字符*/
           for(i=0;i<abs(value);i++)   /*value为数组下标*/
          {
                    gotoxy(r[i].col,r[i].line);
                    textbackground(7);
                    textcolor(0);
                    if(r[i].ch!=13&&r[i].ch!=-1)
                        cprintf("%c",r[i].ch);
                    if(r[i].ch==13||r[i].ch==-1)
                        cprintf(" ");
          }

           gotoxy(x,y);

  }

void drawmenu(int m,int n) /*画菜单,m:第几项菜单，n：第m项的第n个子菜单*/
 {
       int i;
      if(m%3==0) /*画File菜单项*/
     {
            window(8,2,19,9);
            textcolor(0);
            textbackground(7);
            for(i=0;i<7;i++) /*在上面定义的文本窗口中先输出7个空行*/
           {
                gotoxy(1,1+i);
                insline();
           }
            window(1,1,80,25);
            gotoxy(7,1);
            for(i=1;i<=7;i++)
           {
                gotoxy(8,1+i);
                cprintf("%c",179); /*窗口内文本的输出函数，在窗口左边输出 | */
                gotoxy(19,1+i);
                cprintf("%c",179); /*窗口内文本的输出函数，在窗口右边输出 | */
            }
            for(i=1;i<=11;i++)
           {
                  gotoxy(8+i,2);
                  cprintf("%c",196);  /*窗口内文本的输出函数，在窗口上边输出 - */
                  gotoxy(8+i,9);
                  cprintf("%c",196);  /*窗口内文本的输出函数，在窗口下边输出 - */
            }
            textbackground(0);
            gotoxy(10,10); cprintf("            "); /*输出下边的阴影效果*/
            for(i=0;i<9;i++)
           {
                  gotoxy(20,2+i);
                  cprintf("   "); /*输出右边的阴影效果*/
           }
            /*以上为显示菜单项的外观*/
            textbackground(7);
            gotoxy(8,2);  cprintf("%c",218); /*输出四个边角表格符*/
            gotoxy(8,9);  cprintf("%c",192);
            gotoxy(19,2); cprintf("%c",191);
            gotoxy(19,9); cprintf("%c",217);
            gotoxy(9,3);  cprintf(" New    ");
            gotoxy(9,4);  cprintf(" Open   ");
            gotoxy(9,5);  cprintf(" Save   ");
            gotoxy(9,6);  cprintf(" Save as");
            for(i=1;i<=10;i++)
           {
                  gotoxy(8+i,7);
                  cprintf("%c",196); /*在Save as下输出一行分隔符*/
           }
            gotoxy(9,8);  cprintf(" Exit");
            textcolor(15);  textbackground(0);
            gotoxy(7,1);
            cprintf("%c  %c File %c  %c",179,17,16,179);
            switch(n%5)
           {
                case 0:gotoxy(9,3);  cprintf(" New      "); break;
                case 1:gotoxy(9,4);  cprintf(" Open     "); break;
                case 2:gotoxy(9,5);  cprintf(" Save     "); break;
                case 3:gotoxy(9,6);  cprintf(" Save as  "); break;
                case 4:gotoxy(9,8);  cprintf(" Exit     "); break;
           }
      }

     /********************************************************/
     if(m%3==1) /*画Edit菜单项*/
    {
            window(28,2,38,7);
            textcolor(0);
            textbackground(7);
            for(i=0;i<5;i++)
           {
                gotoxy(1,1+i);
                insline();
            }
            window(1,1,80,25);
            gotoxy(27,1);
            for(i=1;i<=5;i++)
           {
                gotoxy(28,1+i);
                cprintf("%c",179);
                gotoxy(39,1+i);
                cprintf("%c",179);
            }
            for(i=1;i<=11;i++)
           {
                  gotoxy(28+i,2);
                  cprintf("%c",196);
                  gotoxy(28+i,7);
                  cprintf("%c",196);
            }

            textbackground(0);
            gotoxy(30,8); cprintf("            ");
            for(i=0;i<7;i++)
           {
                  gotoxy(40,2+i);
                  cprintf("   ");
           }
            textbackground(7);
            gotoxy(28,2);  cprintf("%c",218);
            gotoxy(28,7);  cprintf("%c",192);
            gotoxy(39,2);  cprintf("%c",191);
            gotoxy(39,7);  cprintf("%c",217);
            gotoxy(29,3);  cprintf("  Cut     ");
            gotoxy(29,4);  cprintf("  Copy    ");
            gotoxy(29,5);  cprintf("  Paste   ");
            gotoxy(29,6);  cprintf("  Clear   ");
            textcolor(15);  textbackground(0);
            gotoxy(27,1);
            cprintf("%c  %c Edit %c  %c",179,17,16,179);
            switch(n%4)
            {
                    case 0:gotoxy(29,3);  cprintf("  Cut     "); break;
                    case 1:gotoxy(29,4);  cprintf("  Copy    "); break;
                    case 2:gotoxy(29,5);  cprintf("  Paste   "); break;
                    case 3:gotoxy(29,6);  cprintf("  Clear   "); break;
            }
      }

    /*********************************************************/
     if(m%3==2) /*画Help菜单项3*/
    {
            window(48,2,48,6);
            textcolor(0);
            textbackground(7);
            for(i=0;i<3;i++)
            {
                    gotoxy(1,1+i);
                    insline();
            }
            window(1,1,80,25);
            gotoxy(47,1);
            for(i=1;i<=5;i++)
            {
                    gotoxy(48,1+i);
                    cprintf("%c",179);
                    gotoxy(59,1+i);
                    cprintf("%c",179);
            }
            for(i=1;i<=11;i++)
            {
                    gotoxy(48+i,2);
                    cprintf("%c",196);
                    gotoxy(48+i,6);
                    cprintf("%c",196);
            }

            textbackground(0);
            gotoxy(50,7); cprintf("            ");
            for(i=0;i<6;i++)
           {
                  gotoxy(60,2+i);
                  cprintf("   ");
           }
            textbackground(7);
            gotoxy(48,2);   cprintf("%c",218);
            gotoxy(48,6);   cprintf("%c",192);
            gotoxy(59,2);   cprintf("%c",191);
            gotoxy(59,6);   cprintf("%c",217);
            gotoxy(49,3);   cprintf("Help...   ");
            gotoxy(49,5);   cprintf("About...  ");
            for(i=1;i<=10;i++)
            {
                    gotoxy(48+i,4);
                    cprintf("%c",196);
            }
            textcolor(15);  textbackground(0);
            gotoxy(47,1);
            cprintf("%c  %c Help %c  %c",179,17,16,179);
            switch(n%2)
            {
                    case 0:gotoxy(49,3);  cprintf("Help...   "); break;
                    case 1:gotoxy(49,5);  cprintf("About...  "); break;
            }
    }
 }

int menuctrl(Hnode *Hhead,int A) /*菜单控制*/
  {
        int x,y,i,B,value,flag=100,a,b;
        x=wherex();  y=wherey();
        if(A==F1) {  drawmenu(0,flag);   value=300;  } /*显示File及其子菜单,并将光带显示在第一个子菜单上*/
        if(A==F2) {  drawmenu(1,flag);   value=301;  } /*显示Edit及其子菜单,并将光带显示在第一个子菜单上*/
        if(A==F3) {  drawmenu(2,flag);   value=302;  } /*显示Help及其子菜单,并将光带显示在第一个子菜单上*/

        if(A==F1||A==F2||A==F3)
       {
                while((B=bioskey(0))!=ESC) /*选择用户按键*/
               {
                      if(flag==0)   flag=100;
                      if(value==0)  value=300;  /*此value为局部变量*/

                      if(B==UP)     drawmenu(value,--flag); /*循环上下移*/
                      if(B==DOWN)   drawmenu(value,++flag); /*循环上下移*/

                      if(B==LEFT) /*菜单项之间循环选择（左移）*/
                     {
                            flag=100;
                            drawmain();
                            window(2,2,79,23);
                            textbackground(9);
                            for(i=0;i<24;i++)
                                insline();
                            window(3,3,78,23);
                            textcolor(10);
                            view(Hhead);
                            drawmenu(--value,flag);

                      }
                       if(B==RIGHT)/*菜单项之间循环选择（右移）*/
                     {
                            flag=100;
                            drawmain();
                            window(2,2,79,23);
                            textbackground(9);
                            for(i=0;i<24;i++)
                                insline();
                            window(3,3,78,23);
                            textcolor(10);
                            view(Hhead);
                            drawmenu(++value,flag);

                     }
                       if(B==ENTER) /*选中某主菜单项的子菜单项（选中某项）*/
                      {
                            if(value%3==0)  b=5; /*File下有5个子菜单项*/
                            if(value%3==1)  b=4; /*Edit下有4个子菜单项*/
                            if(value%3==2)  b=2; /*Help下有2个子菜单项*/
                            a=(value%3)*10+flag%b;/*a表示选择子菜单的编号*/
                            drawmain();
                            window(2,2,79,23);
                            textbackground(9);
                            for(i=0;i<24;i++)
                                   insline();
                            window(3,3,78,23);
                            textcolor(10);
                            view(Hhead);
                            gotoxy(x,y);
                            if(a==0)   return 100; /*New*/
                            if(a==1)   return 101; /*Open*/
                            if(a==2)   return 102; /*Save*/
                            if(a==3)   return 103; /*Save As*/
                            if(a==4)   exit(0);    /*Exit*/

                            if(a==10)  return Cx; /*Cut*/
                            if(a==11)  return Cc; /*Copy*/
                            if(a==12)  return Cv; /*Paste*/
                            if(a==13)  return DEL;/*Clear*/

                            if(a==20)  return 120; /*Help... */
                            if(a==21)  return 121; /*About...*/
                      }

                       gotoxy(x+2,y+2);

              }
              /*若按键非F1、F2、F3*/
                drawmain();
                window(2,2,79,23);
                textbackground(9);
                for(i=0;i<24;i++)
                     insline();
                window(3,3,78,23);
                textcolor(10);
                view(Hhead);
                gotoxy(x,y);



       }
        return A;


  }


/*将head所指的行单链表中所指的各个列单链表中的数据域的值写入文件，文件路径和文件名由用户指定*/
void save(Hnode *head)
{
FILE* fp;
Hnode *q;
node *p;
int count=0,x,y;
char filename[10]; /*保存文件名*/
q=head;
clrscr();/*清屏*/
printf("Enter infile name,for example [c:\\wb.txt]:");/*输入文件名格式*/
scanf("%s",filename); /*输入文件名*/
fp=fopen(filename,"w");
if(fp==NULL) /*打开文件失败*/
{
  printf("\n=====>open file error!\n");
  getchar();
  return ;
}
       do{
           p=q->next; /*指向node类型的数据*/
           while(p!=NULL)
             {      if((int)p->ch==13)
                    {
                      fputc('\n',fp);p=p->next; count++;
                     }
                    else
                    {fputc(p->ch, fp);
                     p=p->next;
                     count++;}
             }
           q=q->nextl;
         }while(q!=NULL);

fclose(fp); /*关闭此文件*/
return ;
}

/*文件另存为:将head所指的行单链表中所指的各个列单链表中的数据域的值写入文件，文件路径和文件名由用户指定*/
void saveas(Hnode *head)
{
FILE* fp;
Hnode *q;
node *p;
int count=0,x,y;
char filename[10]; /*保存文件名*/
q=head;
clrscr();/*清屏*/
printf("Enter infile name,for example [c:\\wb.txt]:");/*输入文件名格式*/
scanf("%s",filename); /*输入文件名*/
fp=fopen(filename,"w");
if(fp==NULL) /*打开文件失败*/
{
  printf("\n=====>open file error!\n");
  getchar();
  return ;
}
       do{
           p=q->next; /*指向node类型的数据*/
           while(p!=NULL)
             {       if((int)p->ch==13)
                    {
                      fputc('\n',fp);p=p->next; count++;
                     }
                    else
                    {fputc(p->ch, fp);
                     p=p->next;
                     count++;}

             }
           q=q->nextl;
         }while(q!=NULL);

fclose(fp); /*关闭此文件*/
return ;
}

/*从任意文本文件中读取文件内容，保存至行单链表和列单链表形式的数据结构中*/
void opens(Hnode *Hp)
{
FILE* fp;
Hnode *q11,*q22;
node *p11,*p22,*hp;
char temp;
int count=0,flags=1;
char filename[10]; /*保存文件名*/
clrscr();/*清屏*/
printf("Enter infile name,for example [c:\\wb.txt]:");/*输入文件名格式*/
scanf("%s",filename); /*输入文件名*/
fp=fopen(filename,"r");/*以只读方式打开文件，filename必须要存在*/
if(fp==NULL)/*打开文件失败*/
{  textbackground(2);
   textcolor(13);
   cprintf("open file error!");
   getchar();
   exit(0) ;
}
q11=Hp;
while(!feof(fp))
{   count=0;flags=1;
    q22=(Hnode *)malloc(sizeof(Hnode));/*新建一个行单链表中的节点*/
    p11=(node *)malloc(sizeof(node));  /*新建一个列单链表中的节点*/
    while((temp=fgetc(fp))!=10&&count<=76&&!feof(fp)) /*循环结束，表示在单链表中一行处理完毕，开始新行*/
    {  p22=(node *)malloc(sizeof(node));/*新建一个列单链表中的节点*/
       if(flags==1) {hp=p22;flags=0;} /*hp保存列单链表中的首节点的地址*/
       p22->ch=temp;  p22->next=NULL;
       p11->next=p22; p11=p22;
       count++;
    }
    if(temp==10){ /*若为换行符，将其转换为回车符，因为在程序中，是按回车符处理的*/
       p22=(node *)malloc(sizeof(node));p22->ch=13; p22->next=NULL;
       p11->next=p22; p11=p22;
    }
    if(!feof(fp))/*若没此条件，文件最后一行会处理两次.*/
    {q22->next=hp;q22->nextl=NULL; /*将存储了字符的新列单链表与行单链表中的新节点建立关联*/
     q11->nextl=q22;q11=q22;}
}
fclose(fp);
Hp=Hp->nextl;/*因为Hp的所在节点的数据域为空，所以Hp=Hp->nextl*/
return ;
}


void main()
 {
          char a;
          int i,A,x,y,flag=0,b;
          Hnode *Hhead,*q;
          node *p1,*p2;
          Hhead=(Hnode *)malloc(sizeof(Hnode)); /*为行单链表中首节点分配内存空间*/
          q=Hhead;      Hhead->nextl=NULL;
          p1=p2=q->next=(node *)malloc(sizeof(node)); /*为列单链表中首节点分配内存空间*/
          p1->ch=13; p1->next=NULL;
          drawmain();     /*显示主窗口*/
          window(2,2,79,23);
          textbackground(9);
          for(i=0;i<24;i++)
                insline();
          window(3,3,78,23);
          textcolor(10);

          while(1)
         {
                while(bioskey(1)==0) continue; /*等待用户按键*/
                a=A=bioskey(0); /*返回输入的字符的键值*/
                if(a>=32&&a<127) /*若输入为常规字符或回车键*/
               {

                     if(check(Hhead,wherey(),wherex())<=0)/*当前位置没有字符且输入是常规字符，则执行添加字符操作*/
                    {
                            NUM++;
                            p2->ch=a;
                            putch(a);
                            if(NUM==76) /*连续输入满行，分别生成一个新的行单链表和列单链表节点*/
                           {
                                p2->next=NULL;
                                q->nextl=(Hnode *)malloc(sizeof(Hnode));
                                q=q->nextl;   q->nextl=NULL;  q->next=NULL;
                                p1=p2=q->next=(node *)malloc(sizeof(node));
                                p1->ch=13;  p1->next=NULL;
                                NUM=0;
                           }
                            else /*连续输入未满一行,生成一个新的列单链表节点*/
                           {
                                 p2->next=(node *)malloc(sizeof(node));
                                 p2=p2->next;
                                 p2->ch=13;
                                 p2->next=NULL;

                           }
                     }

                      else /*当前位置有字符且输入是常规字符,则执行插入字符操作*/
                     {
                             x=wherex();  y=wherey();
                             insert(Hhead,wherey(),wherex(),a);
                             NUM++;
                             view(Hhead);
                             gotoxy(x,y);
                     }



                }

                /*若输入为回车键*/
                if(a==13)
               {
                       gotoxy(1,wherey()+1);
                       q->nextl=(Hnode *)malloc(sizeof(Hnode));
                       q=q->nextl;    q->nextl=NULL;   q->next=NULL;
                       p1=p2=q->next=(node *)malloc(sizeof(node));
                       p1->ch=13; p1->next=NULL;
                       NUM=0;
               }

                 x=wherex();   y=wherey();
                /*文本窗口中左移,当前光标不在窗口的第1列*/
                if((A==LEFT)&&(x!=1))  gotoxy(wherex()-1,wherey());
                /*文本窗口中左移,当前光标在窗口的第1列*/
                if((A==LEFT)&&(x==1))  gotoxy(abs(judge(Hhead,wherey()-1)),wherey()-1);
                /*文本窗口中右移,若当前光标的右边一位有字符*/
                if((A==RIGHT)&&check(Hhead,wherey(),wherex())>0)  gotoxy(wherex()+1,wherey());
                /*文本窗口中右移至下行的第1列,若当前光标位置没有字符且下行的第1列有字符*/
                if((A==RIGHT)&&check(Hhead,wherey()+1,1)!=0&&check(Hhead,y,x)<=0)  gotoxy(1,wherey()+1);
                 /*右移*/
                if((A==RIGHT)&&x==76)    gotoxy(1,wherey()+1);
                 /*上移*/
                if((A==UP)&&check(Hhead,wherey()-1,wherex())!=0)  gotoxy(wherex(),wherey()-1);
                 /*上移*/
                if((A==UP)&&check(Hhead,wherey()-1,wherex())<=0)
               {
                    if(judge(Hhead,wherey()-1)==0)
                        gotoxy(-judge(Hhead,wherey()-1)+1,wherey()-1);
                    else
                        gotoxy(-judge(Hhead,wherey()-1),wherey()-1);
               }

                /*下移*/
                if((A==DOWN)&&check(Hhead,wherey()+1,wherex())!=0)
                    gotoxy(wherex(),wherey()+1);

                /*处理BackSpace键*/
                if(A==BACK) /*处理BackSpace键*/
               {
                       flag=del(Hhead,wherey(),wherex()-1);
                       x=wherex()-1;    y=wherey();
                       view(Hhead);
                       if(flag==0)
                      {
                            if(x!=0)  gotoxy(x,y);
                            else  gotoxy(x+1,y);
                      }
                       if(flag==1)
                      {
                           gotoxy(x+1,y);
                           flag=0;
                      }
               }
               /*处理菜单按键F1 F2 F3*/
               if((A==F1)||(A==F2)||(A==F3)||(a<32||a>127))
               {   A=menuctrl(Hhead,A);
                  if(A==100){main();} /*新建文件*/

                  if(A==101){ /*打开文件*/
                    Hhead=(Hnode *)malloc(sizeof(Hnode));
                    opens(Hhead);
                    getchar();clrscr();gotoxy(3,3);view(Hhead);
                    }
                /*保存文件*/
                 if(A==102){save(Hhead);clrscr();cprintf("save successfully!");getch();gotoxy(3,3);view(Hhead);}
                /*文件另存为*/
                 if(A==103){saveas(Hhead);clrscr();cprintf("save as successfully!");getch();gotoxy(3,3);view(Hhead);}
                /*帮助*/
                 if(A==120){clrscr();cprintf("<Help> F1:File  F2:Edit F3:Help ");
                           getch();gotoxy(3,3);view(Hhead);}
                 if(A==121){clrscr();cprintf("Abort:Version 2.0 Tel:XXXXXXXXXX");getch();gotoxy(3,3);view(Hhead);}
               }

                /*处理DEL键,删除当前位置的单个字符*/
               if(A==DEL)
               {
                      x=wherex();    y=wherey();
                      del(Hhead,wherey(),wherex());
                      view(Hhead);
                      gotoxy(x,y);
               }
               /*处理已经选定文本字符后，按DEL键的情况*/
               if(A==DEL&&value!=0)
                {
                      if(value>0)
                            x=wherex(),    y=wherey();
                      else
                            x=r[0].col, y=r[0].line;
                      for(i=0;i<abs(value);i++)
                     {
                           if(value>0)
                              del(Hhead,r[i].line,r[i].col);
                           if(value<0)
                              del(Hhead,r[abs(value)-1-i].line,r[abs(value)-1-i].col);
                     }
                      value=0; /*此value为全局变量*/
                      view(Hhead);
                      gotoxy(x,y);
               }
              /*处理Ctrl+x按键*/
              if(A==Cx&&value!=0)
               {
                      if(value>0)
                            x=wherex(),    y=wherey();
                      else
                            x=r[0].col, y=r[i].line;
                      for(i=0;i<abs(value);i++)
                     {
                           if(value>0)
                              del(Hhead,r[i].line,r[i].col);
                           if(value<0)
                              del(Hhead,r[abs(value)-1-i].line,r[abs(value)-1-i].col);
                     }
                           backup=value; /*保存r数组的有值元素的最大下标值*/
                           value=0; /*此value为全局变量*/
                           view(Hhead);
                           gotoxy(x,y);
               }

               /*处理Ctrl+c按键*/
                if(A==Cc&&value!=0)
               {
                     x=wherex();      y=wherey();
                     backup=value;    value=0; /*此value为全局变量*/
                     view(Hhead);
                     gotoxy(x,y);
               }

               /*处理Ctrl+v按键*/
                if(A==Cv&&backup!=0)
               {
                     x=wherex();    y=wherey();
                     if(backup<0) /*Ctrl+右移键选定的文本，贴切此当前位置*/
                            for(i=0;i<abs(backup);i++)
                                insert(Hhead,y,x+i,r[i].ch);/*逐个插入*/


                     if(backup>0) /*Ctrl+左移键选定的文本，贴切此当前位置*/
                            for(i=0;i<backup;i++)
                                insert(Hhead,y,x+i,r[backup-1-i].ch);

                     view(Hhead);
                     gotoxy(x,y);
               }
               /*快速预览*/
                if(A==F10)
               {
                        qview(Hhead);
                        view(Hhead);
                        gotoxy(x,y);
               }

                /*处理Ctrl+左移键或右移键*/
                if(A==CL||A==CR)    control(A,Hhead);
                /*显示当前行列号*/
                x=wherex();   y=wherey();
                window(1,1,80,25);
                textcolor(0);
                textbackground(7);
                gotoxy(10,25); /*第25行，第10列,输出当前行号wherey()*/
                cprintf("%-3d",y);
                gotoxy(24,25); /*第25行，第24列*/
                cprintf("%-3d",x);
                window(3,3,78,23);
                textcolor(10);
                gotoxy(x,y);
                textcolor(10);
                textbackground(1);
        }
  }
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

/* 【自学去】网站收集 http://www.zixue7.com */

#define HEADER1 " -------------------------------BOOK TICKET----------------------------------\n"
#define HEADER2 " |  number  |start city|reach city|takeofftime|receivetime|price|ticketnumber|\n"
#define HEADER3 " |----------|----------|----------|-----------|-----------|-----|------------|\n"
#define FORMAT  " |%-10s|%-10s|%-10s|%-10s |%-10s |%5d|  %5d     |\n"
#define DATA p->data.num,p->data.startcity,p->data.reachcity,p->data.takeofftime,p->data.receivetime,p->data.price,p->data.ticketnum
int saveflag=0 ;
/*定义存储火车信息的结构体*/
struct train 
{
    char num[10];/*列车号*/
    char startcity[10];/*出发城市*/
    char reachcity[10];/*目的城市*/
    char takeofftime[10];/*发车时间*/
    char receivetime[10];/*到达时间*/
    int  price;/*票价*/
    int  ticketnum ;/*票数*/
};
/*订票人的信息*/
struct man 
{
    char num[10];/*ID*/
    char name[10];/*姓名*/
    int  bookNum ;/*订的票数*/
};
/*定义火车信息链表的结点结构*/
typedef struct node 
{
    struct train data ;
    struct node * next ;
}Node,*Link ;
/*定义订票人链表的结点结构*/
typedef struct Man 
{
    struct man data ;
    struct Man *next ;
}book,*bookLink ;
/* 初始界面*/
void menu()
{
    puts("\n\n");
    puts("\t\t|------------------------------------------------------|");
    puts("\t\t|                   Booking Tickets                    |");
    puts("\t\t|------------------------------------------------------|");
    puts("\t\t|       0:quit the system                              |");
    puts("\t\t|       1:Insert a train information                   |");
    puts("\t\t|       2:Search a train information                   |");
    puts("\t\t|       3:Book a train ticket                          |");
    puts("\t\t|       4:Modify the train information                 |");
    puts("\t\t|       5:Show the train information                   |");
    puts("\t\t|       6:save information to file                     |");
    puts("\t\t|------------------------------------------------------|");
}
/*添加一个火车信息*/
void Traininfo(Link linkhead)
{
    struct node *p,*r,*s ;
    char num[10];
    r = linkhead ;
    s = linkhead->next ;
    while(r->next!=NULL)
    r=r->next ;
    while(1)
    {
        printf("please input the number of the train(0-return)");
        scanf("%s",num);
        if(strcmp(num,"0")==0)
          break ;
        /*判断是否已经存在*/
        while(s)
        {
            if(strcmp(s->data.num,num)==0)
            {
                printf("the train '%s'is existing!\n",num);
                return ;
            }
            s = s->next ;
        }
        p = (struct node*)malloc(sizeof(struct node));
        strcpy(p->data.num,num);/*输入车号*/
	 printf("Input the city where the train will start:");
        scanf("%s",p->data.startcity);/*输入出发城市*/
        printf("Input the city where the train will reach:");
        scanf("%s",p->data.reachcity);/*输入到站城市*/
        printf("Input the time which the train take off:");
	scanf("%s",p->data.takeofftime);/*输入出发时间*/
        printf("Input the time which the train receive:");
	scanf("%s",&p->data.receivetime);/*输入到站时间*/
        printf("Input the price of ticket:");
        scanf("%d",&p->data.price);/*输入火车票价*/
        printf("Input the number of booked tickets:");
	scanf("%d",&p->data.ticketnum);/*输入预定票数*/
        p->next=NULL ;
        r->next=p ;/*插入到链表中*/
        r=p ;
       saveflag = 1 ;
    }
}
/*打印火车票信息*/
void printheader() /*格式化输出表头*/
{
printf(HEADER1);
printf(HEADER2);
printf(HEADER3);
}
void printdata(Node *q) /*格式化输出表中数据*/
{
Node* p;
p=q;
printf(FORMAT,DATA);
}

/*查询火车信息*/
void searchtrain(Link l)

{
    Node *s[10],*r;
    int sel,k,i=0 ;
    char str1[5],str2[10];
    if(!l->next)
    {
        printf("There is not any record !");
        return ;
    }
    printf("Choose the way:\n1:according to the number of train;\n2:according to the city:\n");
    scanf("%d",&sel);/*输入选择的序号*/
    if(sel==1)
    {
        printf("Input the the number of train:");
        scanf("%s",str1);
        r=l->next;
	while(r!=NULL)
        if(strcmp(r->data.num,str1)==0)/*检索是否有与输入的车号相匹配的*/
        {
            s[i]=r;
	    i++;
	    break;
        }
        else 
            r=r->next;
    }
    else if(sel==2)
    {
        printf("Input the city  you want to go:");
        scanf("%s",str2);
        r=l->next;
	while(r!=NULL)
        if(strcmp(r->data.reachcity,str2)==0)/*检索是否有与输入的城市相匹配的火车*/
        {
            s[i]=r;
	    i++;
	    r=r->next;
        }
        else 
            r=r->next;
    }
    	if(i==0)
		printf("can not find!");
	else
	{
		printheader();
    for(k=0;k<i;k++)
printdata(s[k]);
	}
}

/*订票子模块*/
void Bookticket(Link l,bookLink k)
{
    Node *r[10],*p ;
    char ch[2],tnum[10],str[10],str1[10],str2[10];
    book *q,*h ;
    int i=0,t=0,flag=0,dnum;
    q=k ;
    while(q->next!=NULL)
    q=q->next ;
    printf("Input the city you want to go: ");
    scanf("%s",&str);/*输入要到达的城市*/
    p=l->next ;
    while(p!=NULL)
    {
        if(strcmp(p->data.reachcity,str)==0)
        {
            r[i]=p ;/*将满足条件的记录存到数组r中*/
            i++;
        }
        p=p->next ;
    }
    printf("\n\nthe number of record have %d\n",i);
       printheader();
    for(t=0;t<i;t++)
	    printdata(r[t]);
    if(i==0)
    printf("\nSorry!Can't find the train for you!\n");
    else
    {
        printf("\ndo you want to book it?<y/n>\n");
        scanf("%s",ch);
	if(strcmp(ch,"Y")==0||strcmp(ch,"y")==0)/*判断是否订票*/
        {
	    h=(book*)malloc(sizeof(book));
            printf("Input your name: ");
            scanf("%s",&str1);
            strcpy(h->data.name,str1);
            printf("Input your id: ");
            scanf("%s",&str2);
            strcpy(h->data.num,str2);
	    printf("please input the number of the train:");
	    scanf("%s",tnum);
	    for(t=0;t<i;t++)
	    if(strcmp(r[t]->data.num,tnum)==0)
	    {
	       if(r[t]->data.ticketnum<1)/*判断剩余的供订票的票数是否为0*/
	       {
		       printf("sorry,no ticket!");
		       sleep(2);
		       return;
	       }
	      printf("remain %d tickets\n",r[t]->data.ticketnum);
               flag=1;
	       break;
	    }
	    if(flag==0)
	    {
		    printf("input error");
		   sleep(2);
                    return;
	    }
	    printf("Input your bookNum: ");
            scanf("%d",&dnum);
            r[t]->data.ticketnum=r[t]->data.ticketnum-dnum;/*定票成功则可供订的票数相应减少*/
	    h->data.bookNum=dnum ;
            h->next=NULL ;
	    q->next=h ;
	    q=h ;
            printf("\nLucky!you have booked a ticket!");
            getch();
            saveflag=1 ;
        }
    }
}
/*修改火车信息*/
void Modify(Link l)
{
    Node *p ;
    char tnum[10],ch ;
    p=l->next;
    if(!p)
    {
        printf("\nthere isn't record for you to modify!\n");
        return ;
    }
    else
    {
	    printf("\nDo you want to modify it?(y/n)\n");
            getchar();
            scanf("%c",&ch);
            if(ch=='y'||ch=='Y')
            {
                printf("\nInput the number of the train:");
		scanf("%s",tnum);
	while(p!=NULL)
	if(strcmp(p->data.num,tnum)==0)/*查找与输入的车号相匹配的记录*/
	    break;
        else
            p=p->next;
                if(p)
                {
                    printf("Input new number of train:");
                    scanf("%s",&p->data.num);
		    printf("Input new city the train will start:");
                    scanf("%s",&p->data.startcity);
                    printf("Input new city the train will reach:");
                    scanf("%s",&p->data.reachcity);
                    printf("Input new time the train take off");
		    scanf("%s",&p->data.takeofftime);
                    printf("Input new time the train reach:");
		    scanf("%s",&p->data.receivetime);
                    printf("Input new price of the ticket::");
                    scanf("%d",&p->data.price);
                    printf("Input new number of people who have booked ticket:");
                    scanf("%d",&p->data.ticketnum);
                    printf("\nmodifying record is sucessful!\n");
                    saveflag=1 ;
                }
                else
                printf("\tcan't find the record!");
            }
    }
}
void showtrain(Link l)/*自定义函数显示列车信息*/
{
Node *p;
p=l->next;
printheader();
if(l->next==NULL)
printf("no records!");
else
 while(p!=NULL)
{
	printdata(p);
	p=p->next;
}
}
/*保存火车信息*/
void SaveTrainInfo(Link l)
{
    FILE*fp ;
    Node*p ;
    int count=0,flag=1 ;
    fp=fopen("f:\\train.txt","wb");
    if(fp==NULL)
    {
        printf("the file can't be opened!");
        return ;
    }
    p=l->next ;
    while(p)
    {
        if(fwrite(p,sizeof(Node),1,fp)==1)
        {
            p=p->next ;
            count++;
        }
        else
        {
            flag=0 ;
            break ;
        }
    }
    if(flag)
    {
        printf(" saved %d train records\n",count);
        saveflag=0 ;
    }
    fclose(fp);
}
/*保存订票人的信息*/
void SaveBookInfo(bookLink k)
{
    FILE*fp ;
    book *p ;
    int count=0,flag=1 ;
    fp=fopen("f:\\man.txt","wb");
    if(fp==NULL)
    {
        printf("the file can't be opened!");
        return ;
    }
    p=k->next ;
    while(p)
    {
	if(fwrite(p,sizeof(book),1,fp)==1)
        {
            p=p->next ;
            count++;
        }
        else
        {
            flag=0 ;
            break ;
        }
    }
    if(flag)
    {
        printf(" saved %d booking records\n",count);
        saveflag=0 ;
    }
    fclose(fp);
}

main()
{
    FILE*fp1,*fp2 ;
    Node *p,*r ;
    char ch1,ch2 ;
    Link l ;
    bookLink k ;
    book *t,*h ;
    int sel ;
    l=(Node*)malloc(sizeof(Node));
    l->next=NULL ;
    r=l ;
    k=(book*)malloc(sizeof(book));
    k->next=NULL ;
    h=k ;
    fp1=fopen("f:\\train.txt","ab+");/*打开存储车票信息的文件*/
    if((fp1==NULL))
    {
        printf("can't open the file!");
        return 0 ;
    }
    while(!feof(fp1))
    {
        p=(Node*)malloc(sizeof(Node));
        if(fread(p,sizeof(Node),1,fp1)==1)/*从指定磁盘文件读取记录*/
        {
            p->next=NULL ;
            r->next=p ;/*构造链表*/
            r=p ;
       
        }
    }
    fclose(fp1);
    fp2=fopen("f:\\man.txt","ab+");
    if((fp2==NULL))
    {
        printf("can't open the file!");
        return 0 ;
    }
    
    while(!feof(fp2))
    {
        t=(book*)malloc(sizeof(book));
        if(fread(t,sizeof(book),1,fp2)==1)
        {
            t->next=NULL ;
            h->next=t ;
            h=t ;
           
        }
    }
    fclose(fp2);
    while(1)
    {
        clrscr();
        menu();
        printf("\tplease choose (0~6):  ");
        scanf("%d",&sel);
        clrscr();
        if(sel==0)
        {
	    if(saveflag==1)/*当退出时判断信息是否保存*/
            {
                getchar();
                printf("\nthe file have been changed!do you want to save it(y/n)?\n");
                scanf("%c",&ch1);
                if(ch1=='y'||ch1=='Y')
                {
		    SaveBookInfo(k);
                    SaveTrainInfo(l);
                }
            }
            printf("\nThank you!!You are welcome too\n");
            break ;

        }
        switch(sel)/*根据输入的sel值不同选择相应操作*/
        {
            case 1 :
              Traininfo(l);break ;
            case 2 :
              searchtrain(l);break ;
            case 3 :
              Bookticket(l,k);break ;
            case 4 :
              Modify(l);break ;
	    case 5:
		    showtrain(l);break;
            case 6 :
	      SaveTrainInfo(l);SaveBookInfo(k);break ;
            case 0:
            return 0;
        }
        printf("\nplease press any key to continue.......");
        getch();
    }
}

#include "stdio.h"   /*标准输入输出函数库*/
#include "stdlib.h"  /*标准函数库*/
#include "string.h"  /*字符串函数库*/
#include "conio.h"    /*屏幕操作函数库*/
#define HEADER1 "      ----------------------------STUDENT----------------------------------  \n"
#define HEADER2 "     |    number     |      name     |Comp|Math|Eng |   sum  |  ave  |mici | \n"
#define HEADER3 "     |---------------|---------------|----|----|----|--------|-------|-----| "
#define FORMAT  "        |    %-10s |%-15s|%4d|%4d|%4d| %4d   | %.2f |%4d |\n"
#define DATA  p->data.num,p->data.name,p->data.egrade,p->data.mgrade,p->data.cgrade,p->data.total,p->data.ave,p->data.mingci
#define END     "       --------------------------------------------------------------------- \n"

/* 【自学去】网站收集 http://www.zixue7.com */

int saveflag=0;  /*是否需要存盘的标志变量*/
/*定义与学生有关的数据结构*/
typedef struct student      /*标记为student*/
{
char num[10];   /*学号*/
char name[15];  /*姓名*/
int cgrade;     /*C语言成绩*/
int mgrade;     /*数学成绩*/
int egrade;     /*英语成绩*/
int total;      /*总分*/
float ave;      /*平均分*/
int mingci;     /*名次*/
};

/*定义每条记录或结点的数据结构，标记为：node*/
typedef struct node
{
struct student data;  /*数据域*/
struct node *next;    /*指针域*/
}Node,*Link;   /*Node为node类型的结构变量，*Link为node类型的指针变量*/


void menu()  /*主菜单*/
{
system("cls");   /*调用DOS命令，清屏.与clrscr()功能相同*/
textcolor(10);   /*在文本模式中选择新的字符颜色*/
gotoxy(10,5);     /*在文本窗口中设置光标*/
cprintf("                 The Students' Grade Management System \n");
gotoxy(10,8);
cprintf("     *************************Menu********************************\n");
gotoxy(10,9);
cprintf("     *  1 input   record             2 delete record             *\n");
gotoxy(10,10);
cprintf("     *  3 search  record             4 modify record             *\n");
gotoxy(10,11);
cprintf("     *  5 insert  record             6 count  record             *\n");
gotoxy(10,12);
cprintf("     *  7 sort    reord              8 save   record             *\n");
gotoxy(10,13);
cprintf("     *  9 display record             0 quit   system             *\n");
gotoxy(10,14);
cprintf("     *************************************************************\n");
/*cprintf()送格式化输出至文本窗口屏幕中*/
}
void printheader() /*格式化输出表头*/
{
  printf(HEADER1);
  printf(HEADER2);
  printf(HEADER3);
}
void printdata(Node *pp) /*格式化输出表中数据*/
{
 Node* p;
 p=pp;
 printf(FORMAT,DATA);

}

void Wrong()  /*输出按键错误信息*/
{
printf("\n\n\n\n\n***********Error:input has wrong! press any key to continue**********\n");
getchar();
}

void Nofind()  /*输出未查找此学生的信息*/
{
printf("\n=====>Not find this student!\n");
}

void Disp(Link l)  /*显示单链表l中存储的学生记录，内容为student结构中定义的内容*/
{
Node *p;
p=l->next; /*l存储的是单链表中头结点的指针，该头结点没有存储学生信息，指针域指向的后继结点才有学生信息*/

if(!p)  /*p==NULL,NUll在stdlib中定义为0*/
{
  printf("\n=====>Not student record!\n");
  getchar();
  return;
}

printf("\n\n");
printheader(); /*输出表格头部*/

while(p)    /*逐条输出链表中存储的学生信息*/
{
  printdata(p);
  p=p->next;  /*移动至下一个结点*/
  printf(HEADER3);
}
getchar();
}

/*************************************************************
作用：用于定位链表中符合要求的节点，并返回指向该节点的指针
参数：findmess[]保存要查找的具体内容; nameornum[]保存按什么查找;
      在单链表l中查找;
**************************************************************/
Node* Locate(Link l,char findmess[],char nameornum[])
{
Node *r;
if(strcmp(nameornum,"num")==0) /*按学号查询*/
{
  r=l->next;
  while(r)
  {
   if(strcmp(r->data.num,findmess)==0) /*若找到findmess值的学号*/
    return r;
   r=r->next;
  }
}
else if(strcmp(nameornum,"name")==0)  /*按姓名查询*/
{
  r=l->next;
  while(r)
  {
   if(strcmp(r->data.name,findmess)==0)    /*若找到findmess值的学生姓名*/
    return r;
   r=r->next;
  }
}
return 0; /*若未找到，返回一个空指针*/
}


/*输入字符串，并进行长度验证(长度<lens)*/
void stringinput(char *t,int lens,char *notice)
{
   char n[255];
   do{
      printf(notice);  /*显示提示信息*/
      scanf("%s",n);  /*输入字符串*/
      if(strlen(n)>lens)printf("\n exceed the required length! \n"); /*进行长度校验，超过lens值重新输入*/
     }while(strlen(n)>lens);
   strcpy(t,n); /*将输入的字符串拷贝到字符串t中*/
}

/*输入分数，0<＝分数<＝100)*/
int numberinput(char *notice)
{
  int t=0;
   do{
      printf(notice);  /*显示提示信息*/
      scanf("%d",&t);  /*输入分数*/
      if(t>100 || t<0) printf("\n score must in [0,100]! \n"); /*进行分数校验*/
   }while(t>100 || t<0);
   return t;
}


 /*增加学生记录*/
void Add(Link l)
{
Node *p,*r,*s;  /*实现添加操作的临时的结构体指针变量*/
char ch,flag=0,num[10];
r=l;
s=l->next;
system("cls");
Disp(l); /*先打印出已有的学生信息*/
while(r->next!=NULL)
  r=r->next; /*将指针移至于链表最末尾，准备添加记录*/
while(1) /*一次可输入多条记录，直至输入学号为0的记录结点添加操作*/
{
 while(1) /*输入学号，保证该学号没有被使用，若输入学号为0，则退出添加记录操作*/
{

  stringinput(num,10,"input number(press '0'return menu):"); /*格式化输入学号并检验*/
  flag=0;

  if(strcmp(num,"0")==0) /*输入为0，则退出添加操作，返回主界面*/
      {return;}
  s=l->next;
    while(s) /*查询该学号是否已经存在，若存在则要求重新输入一个未被占用的学号*/
    {
      if(strcmp(s->data.num,num)==0)
      {
       flag=1;
       break;
       }
     s=s->next;
    }

  if(flag==1) /*提示用户是否重新输入*/

     {  getchar();
        printf("=====>The number %s is not existing,try again?(y/n):",num);
        scanf("%c",&ch);
        if(ch=='y'||ch=='Y')
         continue;
        else
          return;
      }
     else
      {break;}
  }
  
  p=(Node *)malloc(sizeof(Node)); /*申请内存空间*/
  if(!p)
   {
      printf("\n allocate memory failure "); /*如没有申请到，打印提示信息*/
      return ;             /*返回主界面*/
   }
  strcpy(p->data.num,num); /*将字符串num拷贝到p->data.num中*/
  stringinput(p->data.name,15,"Name:");
  p->data.cgrade=numberinput("C language Score[0-100]:"); /*输入并检验分数，分数必须在0－100之间*/
  p->data.mgrade=numberinput("Math Score[0-100]:");   /*输入并检验分数，分数必须在0－100之间*/
  p->data.egrade=numberinput("English Score[0-100]:"); /*输入并检验分数，分数必须在0－100之间*/
  p->data.total=p->data.egrade+p->data.cgrade+p->data.mgrade; /*计算总分*/
  p->data.ave=(float)(p->data.total/3);  /*计算平均分*/
  p->data.mingci=0;
  p->next=NULL; /*表明这是链表的尾部结点*/
  r->next=p;  /*将新建的结点加入链表尾部中*/
  r=p;
  saveflag=1;

  }
     return ;
}

void Qur(Link l) /*按学号或姓名，查询学生记录*/
{
int select; /*1:按学号查，2：按姓名查，其他：返回主界面（菜单）*/
char searchinput[20]; /*保存用户输入的查询内容*/
Node *p;
if(!l->next) /*若链表为空*/
{
  system("cls");
  printf("\n=====>No student record!\n");
  getchar();
  return;

}
system("cls");
printf("\n     =====>1 Search by number  =====>2 Search by name\n");
printf("      please choice[1,2]:");
scanf("%d",&select);
if(select==1)   /*按学号查询*/
 {

  stringinput(searchinput,10,"input the existing student number:");
  p=Locate(l,searchinput,"num");/*在l中查找学号为searchinput值的节点，并返回节点的指针*/
  if(p) /*若p!=NULL*/
  {
   printheader();
   printdata(p);
   printf(END);
   printf("press any key to return");
   getchar();
  }
  else
   Nofind();
   getchar();
}
else if(select==2) /*按姓名查询*/
{
  stringinput(searchinput,15,"input the existing student name:");
  p=Locate(l,searchinput,"name");
  if(p)
  {
   printheader();
   printdata(p);
   printf(END);
   printf("press any key to return");
   getchar();
  }
  else
   Nofind();
   getchar();
}
else
  Wrong();
  getchar();
  
}

/*删除学生记录：先找到保存该学生记录的节点，然后删除该节点*/
void Del(Link l)
{
int sel;
Node *p,*r;
char findmess[20];
if(!l->next)
{ system("cls");
  printf("\n=====>No student record!\n");
  getchar();
  return;
}
system("cls");
Disp(l);
printf("\n        =====>1 Delete by number       =====>2 Delete by name\n");
printf("       please choice[1,2]:");
scanf("%d",&sel);
if(sel==1)
{

  stringinput(findmess,10,"input the existing student number:");
  p=Locate(l,findmess,"num");
  if(p)  /*p!=NULL*/
  {
   r=l;
   while(r->next!=p)
    r=r->next;
   r->next=p->next;/*将p所指节点从链表中去除*/
   free(p); /*释放内存空间*/
   printf("\n=====>delete success!\n");
   getchar();
   saveflag=1;
  }
  else
   Nofind();
   getchar();
}
else if(sel==2) /*先按姓名查询到该记录所在的节点*/
{
  stringinput(findmess,15,"input the existing student name");
  p=Locate(l,findmess,"name");
  if(p)
  {
   r=l;
   while(r->next!=p)
    r=r->next;
   r->next=p->next;
   free(p);
   printf("\n=====>delete success!\n");
   getchar();
   saveflag=1;
  }
  else
   Nofind();
   getchar();
}
else
  Wrong();
  getchar();
}

/*修改学生记录。先按输入的学号查询到该记录，然后提示用户修改学号之外的值，学号不能修改*/
void Modify(Link l)
{
Node *p;
char findmess[20];
if(!l->next)
{ system("cls");
  printf("\n=====>No student record!\n");
  getchar();
  return;
}
system("cls");
printf("modify student recorder");
Disp(l);


stringinput(findmess,10,"input the existing student number:"); /*输入并检验该学号*/
p=Locate(l,findmess,"num"); /*查询到该节点*/
if(p) /*若p!=NULL,表明已经找到该节点*/
{
  printf("Number:%s,\n",p->data.num);
  printf("Name:%s,",p->data.name);
  stringinput(p->data.name,15,"input new name:");

  printf("C language score:%d,",p->data.cgrade);
  p->data.cgrade=numberinput("C language Score[0-100]:");

  printf("Math score:%d,",p->data.mgrade);
  p->data.mgrade=numberinput("Math Score[0-100]:");


  printf("English score:%d,",p->data.egrade);
   p->data.egrade=numberinput("English Score[0-100]:");

  p->data.total=p->data.egrade+p->data.cgrade+p->data.mgrade;
  p->data.ave=(float)(p->data.total/3);
  p->data.mingci=0;
  printf("\n=====>modify success!\n");
  Disp(l);
  saveflag=1;
}
else
  Nofind();
  getchar();

}



/*插入记录:按学号查询到要插入的节点的位置，然后在该学号之后插入一个新节点。*/
void Insert(Link l)
{
   Link p,v,newinfo; /*p指向插入位置，newinfo指新插入记录*/
   char ch,num[10],s[10];  /*s[]保存插入点位置之前的学号,num[]保存输入的新记录的学号*/
   int flag=0;
   v=l->next;
   system("cls");
   Disp(l);
   while(1)
   { stringinput(s,10,"please input insert location  after the Number:");
     flag=0;v=l->next;
     while(v) /*查询该学号是否存在，flag=1表示该学号存在*/
     {
      if(strcmp(v->data.num,s)==0)  {flag=1;break;}
          v=v->next;
     }
      if(flag==1)
        break; /*若学号存在，则进行插入之前的新记录的输入操作*/
     else
     {  getchar();
        printf("\n=====>The number %s is not existing,try again?(y/n):",s);
        scanf("%c",&ch);
        if(ch=='y'||ch=='Y')
         {continue;}
        else
          {return;}
      }
   }
  /*以下新记录的输入操作与Add()相同*/
  stringinput(num,10,"input new student Number:");
  v=l->next;
  while(v)
  {
   if(strcmp(v->data.num,num)==0)
   {
    printf("=====>Sorry,the new number:'%s' is existing !\n",num);
    printheader();
    printdata(v);
    printf("\n");
    getchar();
    return;
   }
   v=v->next;
  }
  

  newinfo=(Node *)malloc(sizeof(Node));
  if(!newinfo)
   {
      printf("\n allocate memory failure "); /*如没有申请到，打印提示信息*/
      return ;             /*返回主界面*/
   }
  strcpy(newinfo->data.num,num);
  stringinput(newinfo->data.name,15,"Name:");
  newinfo->data.cgrade=numberinput("C language Score[0-100]:");
  newinfo->data.mgrade=numberinput("Math Score[0-100]:");
  newinfo->data.egrade=numberinput("English Score[0-100]:");
  newinfo->data.total=newinfo->data.egrade+newinfo->data.cgrade+newinfo->data.mgrade;
  newinfo->data.ave=(float)(newinfo->data.total/3);
  newinfo->data.mingci=0;
  newinfo->next=NULL;
  saveflag=1; /*在main()有对该全局变量的判断，若为1,则进行存盘操作*/
  /*将指针赋值给p,因为l中的头节点的下一个节点才实际保存着学生的记录*/
  p=l->next;
  while(1)
   {
     if(strcmp(p->data.num,s)==0) /*在链表中插入一个节点*/
      {
        newinfo->next=p->next;
        p->next=newinfo;
        break;
       }
      p=p->next;
    }

   Disp(l);
   printf("\n\n");
   getchar();

}

/*统计该班的总分第一名和单科第一,和各科不及格人数*/
void Tongji(Link l)
{
Node *pm,*pe,*pc,*pt; /*用于指向分数最高的节点*/
Node *r=l->next;
int countc=0,countm=0,counte=0; /*保存三门成绩中不及格的人数*/
if(!r)
{ system("cls");
  printf("\n=====>Not student record!\n");
  getchar();
  return ;
}
system("cls");
Disp(l);
pm=pe=pc=pt=r;
while(r)
{
  if(r->data.cgrade<60) countc++;
  if(r->data.mgrade<60) countm++;
  if(r->data.egrade<60) counte++;

  if(r->data.cgrade>=pc->data.cgrade)    pc=r;
  if(r->data.mgrade>=pm->data.mgrade)    pm=r;
  if(r->data.egrade>=pe->data.egrade)    pe=r;
  if(r->data.total>=pt->data.total)      pt=r;
  r=r->next;
}
printf("\n------------------------------the TongJi result--------------------------------\n");
printf("C Language<60:%d (ren)\n",countc);
printf("Math      <60:%d (ren)\n",countm);
printf("English   <60:%d (ren)\n",counte);
printf("-------------------------------------------------------------------------------\n");
printf("The highest student by total   scroe   name:%s totoal score:%d\n",pt->data.name,pt->data.total);
printf("The highest student by English score   name:%s totoal score:%d\n",pe->data.name,pe->data.egrade);
printf("The highest student by Math    score   name:%s totoal score:%d\n",pm->data.name,pm->data.mgrade);
printf("The highest student by C       score   name:%s totoal score:%d\n",pc->data.name,pc->data.cgrade);
printf("\n\npress any key to return");
getchar();
}
/*利用插入排序法实现单链表的按总分字段的降序排序，从高到低*/
void Sort(Link l)
{
Link ll;
Node *p,*rr,*s;
int i=0;
if(l->next==NULL)
{ system("cls");
  printf("\n=====>Not student record!\n");
  getchar();
  return ;
}

ll=(Node*)malloc(sizeof(Node)); /*用于创建新的节点*/
if(!ll)
   {
      printf("\n allocate memory failure "); /*如没有申请到，打印提示信息*/
      return ;             /*返回主界面*/
   }
ll->next=NULL;
system("cls");
Disp(l);  /*显示排序前的所有学生记录*/
p=l->next;
while(p) /*p!=NULL*/
{
  s=(Node*)malloc(sizeof(Node)); /*新建节点用于保存从原链表中取出的节点信息*/
  if(!s) /*s==NULL*/
   {
      printf("\n allocate memory failure "); /*如没有申请到，打印提示信息*/
      return ;             /*返回主界面*/
   }
  s->data=p->data; /*填数据域*/
  s->next=NULL;    /*指针域为空*/
  rr=ll;
  /*rr链表于存储插入单个节点后保持排序的链表，ll是这个链表的头指针,每次从头开始查找插入位置*/

  while(rr->next!=NULL && rr->next->data.total>=p->data.total)
   {rr=rr->next;} /*指针移至总分比p所指的节点的总分小的节点位置*/
  if(rr->next==NULL)/*若新链表ll中的所有节点的总分值都比p->data.total大时，就将p所指节点加入链表尾部*/
     rr->next=s;
  else /*否则将该节点插入至第一个总分字段比它小的节点的前面*/
  {
   s->next=rr->next;
   rr->next=s;
  }
   p=p->next; /*原链表中的指针下移一个节点*/
 }

   l->next=ll->next; /*ll中存储是的已排序的链表的头指针*/
   p=l->next;           /*已排好序的头指针赋给p，准备填写名次*/
   while(p!=NULL)  /*当p不为空时，进行下列操作*/
   {
      i++;       /*结点序号*/
      p->data.mingci=i;   /*将名次赋值*/
      p=p->next;   /*指针后移*/

   }
Disp(l);
saveflag=1;
printf("\n    =====>sort complete!\n");

}

/*数据存盘,若用户没有专门进行此操作且对数据有修改，在退出系统时， 会提示用户存盘*/
void Save(Link l)
{
FILE* fp;
Node *p;
int count=0;
fp=fopen("c:\\student","wb");/*以只写方式打开二进制文件*/
if(fp==NULL) /*打开文件失败*/
{
  printf("\n=====>open file error!\n");
  getchar();
  return ;
}
p=l->next;

while(p)
{
  if(fwrite(p,sizeof(Node),1,fp)==1)/*每次写一条记录或一个节点信息至文件*/
  { 
   p=p->next;
   count++;
  }
  else
  {
   break;
  }
}
if(count>0)
{
  getchar();
  printf("\n\n\n\n\n=====>save file complete,total saved's record number is:%d\n",count);
  getchar();
  saveflag=0;
}
else
{system("cls");
 printf("the current link is empty,no student record is saved!\n");
 getchar();
 }
fclose(fp); /*关闭此文件*/
}

void main()
{

  Link l;      /*定义链表*/
  FILE *fp;    /*文件指针*/
  int select;     /*保存选择结果变量*/
  char ch;     /*保存(y,Y,n,N)*/
  int count=0; /*保存文件中的记录条数（或结点个数）*/
  Node *p,*r;   /*定义记录指针变量*/


  l=(Node*)malloc(sizeof(Node));
  if(!l)
   {
      printf("\n allocate memory failure "); /*如没有申请到，打印提示信息*/
      return ;             /*返回主界面*/
   }
  l->next=NULL;
  r=l;
  fp=fopen("C:\\student","ab+"); /*以追加方式打开一个二进制文件，可读可写，若此文件不存在，会创建此文件*/
  if(fp==NULL)
  {
    printf("\n=====>can not open file!\n");
    exit(0);
  }

while(!feof(fp))
{
  p=(Node*)malloc(sizeof(Node));
  if(!p)
   {
      printf(" memory malloc failure!\n");    /*没有申请成功*/
      exit(0);       /*退出*/
   }

  if(fread(p,sizeof(Node),1,fp)==1) /*一次从文件中读取一条学生成绩记录*/
  {
   p->next=NULL;
   r->next=p;
   r=p;                            /*r指针向后移一个位置*/
   count++;
   }
}

fclose(fp); /*关闭文件*/
printf("\n=====>open file sucess,the total records number is : %d.\n",count);
menu();
while(1)
{
   system("cls");
   menu();
   p=r;
   printf("\n              Please Enter your choice(0~9):");    /*显示提示信息*/
   scanf("%d",&select);

  if(select==0)
  {
   if(saveflag==1) /*若对链表的数据有修改且未进行存盘操作，则此标志为1*/
   { getchar();
     printf("\n=====>Whether save the modified record to file?(y/n):");
     scanf("%c",&ch);
     if(ch=='y'||ch=='Y')
       Save(l);
   }
   printf("=====>thank you for useness!");
   getchar();
   break;
  }

  switch(select)
  {
  case 1:Add(l);break;            /*增加学生记录*/
  case 2:Del(l);break;           /*删除学生记录*/
  case 3:Qur(l);break;           /*查询学生记录*/
  case 4:Modify(l);break;        /*修改学生记录*/
  case 5:Insert(l);break;        /*插入学生记录*/
  case 6:Tongji(l);break;       /*统计学生记录*/
  case 7:Sort(l);break;        /*排序学生记录*/
  case 8:Save(l);break;        /*保存学生记录*/
  case 9:system("cls");Disp(l);break;         /*显示学生记录*/
  default: Wrong();getchar();break;        /*按键有误，必须为数值0-9*/
  }
}
}
#include"stdlib.h"
#include"stdio.h"
#include"graphics.h"
#include"string.h"
#include"conio.h"
#define file "c:\\bookdata.txt"
#define ufile "c:\\userdata.txt"
#define bfile "c:\\borrowdata.txt"
int bookcount;
int usercount;
int borrowcount;
int booksum;
int myborrow;

int flg=0;
int esc=0;
int flag=0;

/* 【自学去】网站收集 http://www.zixue7.com */

/************定义输出字符的函数*************/

void pchar(int x,int y,char c)
{
gotoxy(x,y);
putch(c);
}

/**********************定义画边框的函数*************/
void box(int x,int y,int w,int height)
{

int i;
pchar(x,y,218);/*left up*/
pchar(x+w-1,y,191); /*  right up*/
for(i=0;i<w-2;i++) /*witgh*/
{
	pchar(x+1+i,y,196);
	pchar(x+1+i,y+height-1,196);
}
pchar(x,y+height-1,192);
pchar(x+w-1,y+height-1,217);
for(i=0;i<height-2;i++)
{
	pchar(x,y+1+i,179);
	pchar(x+w-1,y+i+1,179);
}
}
/********************定义画横线的函数*************/
void h_line(int x,int y,int w)
{
int i;
for(i=0;i<w;i++)
	pchar(x+i,y,196);
}
void h_line_close(int x,int y,int w)
{

int i;
pchar(x,y,195);
h_line(x+1,y,w-2);
pchar(x+w-1,y,180);
}
/***********************定义画填充矩形的函数*************/
void bar_(int x,int y,int w,int height,int color)
{
int i,j;
textcolor(color);
textbackground(color);
for(i=0;i<height;i++)
{
	gotoxy(x,y+i);
	for(j=0;j<w;j++)
		putch(' ');
}
}
/*************************定义画竖线的函数*************/
void v_line(int x,int y,int height)
{
int i;
for(i=0;i<height;i++)
    pchar(x,y+i,179);
}
/****************模拟菜单条******************/
void menu()
{
 bar_(1,25,80,1,7);
}


typedef struct book/*图书结构体*/
{
char num[10];  /*书号*/
char name[10];  /*书名*/
char auth[10]; /*作者*/
int count;
int sum;
}Book;
/*定义节点*/
typedef struct node
{
Book data;
struct node *next;
}Bnode,*blink;
/*******保存图书*******/
/*******保存图书*******/
void save(blink l) 
{ 
 FILE* fp; 
 Bnode *p; 
 int flag=1,count=0; 
 fp=fopen(file,"wb"); 
 if(fp==NULL) 
 { 
      printf("\n=====>open error!\n"); 
      exit(1); 
 } 
 p=l->next; 
 while(p) 
 { 
      if(fwrite(p,sizeof(Bnode),1,fp)==1)
      { 
          p=p->next; 
          count++; 
      } 
      else 
      { 
          flag=0; 
          break; 
      } 
} 
if(flag) 
{ 
     /* printf("save success.(saved%d.)\n",count); */
}
fclose(fp); 
} 

 /*******图书定位******/
Bnode* locate(blink l,char findmess[],char nameornum[]) 
{ 
Bnode *r; 
if(strcmp(nameornum,"num")==0) 
{ 
  r=l->next; 
  while(r!=NULL) 
  { 
      if(strcmp(r->data.num,findmess)==0) 
      return r; 
      r=r->next; 
  } 
} 
else if(strcmp(nameornum,"name")==0) 
{ 
  r=l->next; 
  while(r!=NULL) 
  { 
     if(strcmp(r->data.name,findmess)==0) 
     return r; 
     r=r->next; 
  } 
} 
return 0; 
} 
 /*******图书的添加******/
void add(blink l)
{ 
Bnode *p,*r,*s; 
char num[10]; 
r=l; 
s=l->next; 
while(r->next!=NULL) 
	r=r->next; 
textcolor(RED);
gotoxy(25,4);
cputs("INPUT THE MESSAGE ABOUT BOOK");
gotoxy(31,10);
textcolor(YELLOW);
cputs("Book ID:");
scanf("%s",num); 
p=(Bnode *)malloc(sizeof(Bnode));  
while(s) 
{ 
	if(strcmp(s->data.num,num)==0) 
	{ 
		textcolor(WHITE);
		gotoxy(25,15);
		cputs("This ID:");
		printf("'%s'",num); 
		cputs("is exist!");    
		gotoxy(25,22);
		cputs("please Press any key to continue...");  
		gotoxy(255,252);
		getch();                    
		return; 
	} 
	s=s->next; 
} 
strcpy(p->data.num,num); 
gotoxy(31,12);
textcolor(YELLOW);
cputs("Input Book name:"); 
scanf("%s",p->data.name); 
gotoxy(31,14);
cputs("input your Book auth:"); 
scanf("%s",p->data.auth); 
gotoxy(31,16);
cputs("input your Book count:"); 
scanf("%d",&p->data.count); 
bookcount=p->data.count+bookcount;
p->data.sum=0;
p->next=NULL; 
r->next=p; 
r=p; 
gotoxy(30,22);
textcolor(RED);
cputs("Add Book Success !!!");
getch();
textcolor(YELLOW);
} 

/*******管理员查找图书*******/
void qur(blink l)
{ 
int sel; 
char findmess[20]; 
Bnode *p; 

if(!l->next) 
{ 
gotoxy(30,4);
textcolor(WHITE);
     cputs("Not Find Bookdata!!!"); 
	 getch();
    return; 
} 
textcolor(RED);
gotoxy(25,4);
cputs("Please Select Search  Type !");
gotoxy(10,8);
textcolor(WHITE);
cputs("1.Search by ID");
gotoxy(10,10);
cputs("2.Search by Name");
gotoxy(10,12);
cputs("Please Select 1 or 2:");
scanf("%d",&sel); 
if(sel==1) 
{
	gotoxy(36,8);
	textcolor(YELLOW);
	cputs("Input the search ID:"); 
	scanf("%s",findmess); 
	p=locate(l,findmess,"num"); 
	if(p) 
	{ 
		gotoxy(36,12);
		textcolor(WHITE);
		cputs("Book ID:");
		printf("%s",p->data.num);
		gotoxy(36,14);
		textcolor(WHITE);
		cputs("Book Name:");
		printf("%s",p->data.name);
		gotoxy(36,16);
		textcolor(WHITE);
		cputs("Book author:");
		printf("%s",p->data.auth);
		gotoxy(36,18);
		textcolor(WHITE);
		cputs("Book count:");
		printf("%d",p->data.count);
		getch();
		textcolor(YELLOW);
		gotoxy(30,21);
		cputs("Search Success !!");
	}
	else 
	{
		gotoxy(30,22);
		textcolor(RED);
		cputs("Not finde !!!"); 
		getch();
	}
}	
else if(sel==2) /* 姓名 */
{ 
	gotoxy(36,8);
	textcolor(YELLOW);
	cputs("Input the search name:");
	scanf("%s",findmess); 
	p=locate(l,findmess,"name"); 		
	if(p) 
	{ 
		gotoxy(36,12);
		textcolor(WHITE);
		cputs("Book ID:");
		printf("%s",p->data.num);
		gotoxy(36,14);
		textcolor(WHITE);
		cputs("Book Name:");
		printf("%s",p->data.name);
		gotoxy(36,16);
		textcolor(WHITE);
		cputs("Book author:");
		printf("%s",p->data.auth);
		gotoxy(36,18);
		textcolor(WHITE);
		cputs("Book count:");
		printf("%d",p->data.count);
		getch();
		textcolor(YELLOW);
	} 
	else 
	{
		textcolor(RED);
		gotoxy(30,22);
		cputs("Not finde !!!"); 
	}
} 
else 
{
	textcolor(RED);
	gotoxy(30,22); 
	cputs("Error !!"); 
	getch();
}
} 

/*******用户查找图书*******/
void usersearch()
{ 
int sel; 
char findmess[20]; 
Bnode *p; 
blink l;/* 连表 */ 
FILE *fp; /* 文件指针 */ 
int count=0; 
Bnode *P,*r;
l=(Bnode*)malloc(sizeof(Bnode)); 
l->next=NULL; 
r=l; 
fp=fopen(file,"rb"); 
if(fp==NULL) 
{ 
	fp=fopen(file,"wb"); 
 } 
while(!feof(fp)) 
{ 
    P=(Bnode*)malloc(sizeof(Bnode)); 
    if(fread(P,sizeof(Bnode),1,fp)) /* 将文件的内容放入接点中 */ 
    { 
        P->next=NULL; 
		bookcount=bookcount+P->data.count;
		booksum=booksum+P->data.sum;
        r->next=P; 
        r=P; /* 将该接点挂入连中 */ 
        count++; 
    }
} 
fclose(fp); /* 关闭文件 */ 


if(!l->next) 
{ 
gotoxy(30,4);
textcolor(WHITE);
     cputs("Not Find Bookdata!!!");
	 getch();
    return; 
} 
textcolor(RED);
gotoxy(25,4);
cputs("Please Select Delete  Type !");
gotoxy(10,8);
textcolor(WHITE);
cputs("1.Search by ID");
gotoxy(10,10);
cputs("2.Search by Name");
gotoxy(10,12);
cputs("Please Select 1 or 2:");
scanf("%d",&sel); 
if(sel==1) 
{
	gotoxy(36,8);
	textcolor(YELLOW);
	cputs("Input the search ID:"); 
	scanf("%s",findmess); 
	p=locate(l,findmess,"num"); 
	if(p) 
	{ 
		gotoxy(36,12);
		textcolor(WHITE);
		cputs("Book ID:");
		printf("%s",p->data.num);
		gotoxy(36,14);
		textcolor(WHITE);
		cputs("Book Name:");
		printf("%s",p->data.name);
		gotoxy(36,16);
		textcolor(WHITE);
		cputs("Book author:");
		printf("%s",p->data.auth);
		gotoxy(36,18);
		textcolor(WHITE);
		cputs("Book count:");
		printf("%d",p->data.count-p->data.sum);
		getch();
		textcolor(YELLOW);
	}
	else 
	{
		gotoxy(30,22);
		textcolor(RED);
		cputs("Not finde !!!"); 
		getch();
	}
}	
else if(sel==2)
{ 
	gotoxy(36,8);
	textcolor(YELLOW);
	cputs("Input the search name:");
	scanf("%s",findmess); 
	p=locate(l,findmess,"name"); 		
	if(p) 
	{ 
		gotoxy(36,12);
		textcolor(WHITE);
		cputs("Book ID:");
		printf("%s",p->data.num);
		gotoxy(36,14);
		textcolor(WHITE);
		cputs("Book Name:");
		printf("%s",p->data.name);
		gotoxy(36,16);
		textcolor(WHITE);
		cputs("Book author:");
		printf("%s",p->data.auth);
		gotoxy(36,18);
		textcolor(WHITE);
		cputs("Book count:");
		printf("%d",(p->data.count-p->data.sum));
		getch();
		textcolor(YELLOW);
	} 
	else 
	{
		textcolor(RED);
		gotoxy(30,22);
		cputs("Not finde !!!"); 
	}
} 
else 
{
	textcolor(RED);
	gotoxy(30,22); 
	cputs("Error !!"); 
	getch();
}
} 

/*******图书查找*******/
int cmpbook(blink l,char id[],char na[])
{ 
char findm[20];
Bnode *p; 
if(!l->next) 
{ 
	gotoxy(25,4);
	textcolor(RED);
     cputs("Not Find Book!!!"); 
	 getch();
    return 0; 
} 
strcpy(findm,id);
	p=locate(l,findm,"num"); 
	if(p) 
	{ 
		strcpy(findm,na);
		p=locate(l,findm,"name"); 		
		if(p) 
		{ 
			return 1;	
		} 
		else 
		{
			textcolor(RED);
			gotoxy(30,22);
			cputs("Book name is NULL !!!"); 
			getch();
			return 0;
		}
	}
	else 
	{
		gotoxy(30,22);
		textcolor(RED);
		cputs("Book id is NULL !!!"); 
		getch();
		return 0;
	}
	


	
	

} 

/*** 删除图书*** */ 
void del(blink l) 
{ 
	int sel; 
	Bnode *p,*r; 
	char findmess[20]; 
	if(!l->next) 
	{ 
		gotoxy(25,4);
		textcolor(RED);
		cputs("=====>not thing could delete!\n"); 
		getch();
		return; 
	} 
	textcolor(RED);
	gotoxy(25,4);
	puts("Please Select Delete  Type !");
	gotoxy(10,8);
	
	textcolor(WHITE);
	cputs("1.Delete by Book ID");
	gotoxy(10,10);
	cputs("2.Delete by Book Name");
	gotoxy(10,12);
	cputs("Please Select 1 or 2:");
	scanf("%d",&sel); 
	if(sel==1)     
	{ 
		gotoxy(36,8);
		textcolor(YELLOW);
		cputs("Input the delete ID:"); 
		scanf("%s",findmess); 
		p=locate(l,findmess,"num"); 
		if(p) 
		{ 
			bookcount=bookcount-p->data.count;
			r=l; 
			while(r->next!=p) 
				r=r->next; 
			r->next=p->next; 
			free(p); 
			gotoxy(30,22);
			textcolor(RED);
			cputs("Delete success!\n");     
			textcolor(YELLOW);  
		} 
		else 
		{
	textcolor(RED);
	gotoxy(30,22); 
	cputs("Error !!"); 
		}
	} 
	else if(sel==2) 
	{ 
		gotoxy(36,8);
		textcolor(YELLOW);
		cputs("Input the delete name:"); 
		scanf("%s",findmess); 
		p=locate(l,findmess,"name"); 
		if(p) 
		{ 
			r=l; 
			while(r->next!=p) 
				r=r->next; 
			r->next=p->next; 
			free(p); 
			gotoxy(30,22);
			textcolor(RED);
			cputs("Delete success!\n"); 
			bookcount--;
			textcolor(YELLOW);  
		} 
		else
		{
			gotoxy(25,18);
			cputs("Not find!!"); 
		}
	} 
	else
	{
	textcolor(RED);
	gotoxy(30,22); 
	cputs("Error !!");  
	}
	getch();
	textcolor(YELLOW);
} 
/**************************************************************/
 /* 本函数用于修改图书资料 */
void mod(blink l)
{ 
	int tmp;
	Bnode *p; 
	char findmess[20]; 
	if(!l->next) 
	{
		gotoxy(30,4);
		textcolor(RED);
		cputs("not thing could modify!"); 
		getch();
		return; 
	} 
	gotoxy(30,4);
	textcolor(RED);
	cputs("Modify Book Message");
	gotoxy(25,8);
	textcolor(YELLOW);
	cputs("input the Book Id:");
	scanf("%s",findmess); 
	p=locate(l,findmess,"num"); 
	if(p) 
	{ 
		gotoxy(25,10);
		printf("Inpute The New ID(old:%s):",p->data.num); 
		scanf("%s",p->data.num); 
		gotoxy(25,12);
		printf("Input The New Book Name(old:%s):",p->data.name); 
		scanf("%s",p->data.name); 
		gotoxy(25,14);
		printf("Input The New Book author(old:%s):",p->data.auth); 
		scanf("%s",p->data.auth); 
		gotoxy(25,16);
		printf("Input The New count(old:%d):",p->data.count); 
		scanf("%d",&tmp); 
		bookcount=bookcount-p->data.count;
		bookcount=bookcount+tmp;
		p->data.count=tmp;
		gotoxy(30,18);
		textcolor(RED);
		cputs("Modify Success !!!"); 
		getch();
		textcolor(YELLOW);
	} 
	else 
	{	
		gotoxy(30,16);
		textcolor(RED);
		cputs("Not Finde !!!"); 
		getch();
		textcolor(YELLOW);
	}
} 






struct user 
{ 
char num[10];/* 学号 */ 
char pass[15];
char name[15]; 
char sex[10]; 

}; 

typedef struct unode 
{ 
struct user data; 
struct unode *next; 
}Unode,*Ulink; 
Unode *os;
/*******借书结构定义*******/
typedef struct borrow
{
char id[20];
char user[20];
char book[20];
char bookid[20];
struct borrow *next;
}BBnode,*BBlink;

void Save(Ulink l) /*******用户信息保存*******/
{ 
 FILE* fp; 
 Unode *p; 
 int flag=1,count=0; 
 fp=fopen(ufile,"wb"); 
 if(fp==NULL) 
 { 
gotoxy(35,12);
	textcolor(RED);
      cputs("open error!"); 
      exit(1); 
 } 
 p=l->next; 
 while(p) 
 { 
      if(fwrite(p,sizeof(Unode),1,fp)==1) 
      { 
          p=p->next; 
          count++; 
      } 
      else 
      { 
          flag=0; 
          break; 
      } 
} 
if(flag) 
{ 
textcolor(RED);
gotoxy(30,24);
  printf("save success.(saved%d.)",count);
}
fclose(fp); 
} 

/*******借书信息保存*******/
void Save_borrow(BBlink l) 
{ 
 FILE* fp; 
 BBnode *p; 
 int flag=1,count=0; 
 fp=fopen(bfile,"wb"); 
 if(fp==NULL) 
 { 
gotoxy(35,12);
	textcolor(RED);
      cputs("open error!"); 
      exit(1); 
 } 
 p=l->next; 
 while(p) 
 { 
      if(fwrite(p,sizeof(BBnode),1,fp)==1) 
      { 
          p=p->next; 
          count++; 
      } 
      else 
      { 
          flag=0; 
          break; 
      } 
} 
if(flag) 
{ 
textcolor(RED);
gotoxy(30,24);
 /*** printf("save success.(saved%d.)",count);**调试的时候用的*/
}
fclose(fp); 
} 

/*******用户定位*******/
Unode* Locate(Ulink l,char findmess[],char nameornum[]) 
{ 
Unode *r; 
if(strcmp(nameornum,"num")==0) 
{ 
  r=l->next; 
  while(r!=NULL) 
  { 
      if(strcmp(r->data.num,findmess)==0) 
      return r; 
      r=r->next; 
  } 
} 
else if(strcmp(nameornum,"name")==0) 
{ 
  r=l->next; 
  while(r!=NULL) 
  { 
     if(strcmp(r->data.name,findmess)==0) 
     return r; 
     r=r->next; 
  } 
} 

else if(strcmp(nameornum,"pass")==0) 
{ 
  r=l->next; 
  while(r!=NULL) 
  { 
     if(strcmp(r->data.pass,findmess)==0) 
     return r; 
     r=r->next; 
  } 
} 
return 0; 
} 
/*******图书借书查找模块*******/
BBnode* Locate_borrow(BBlink l,char findmess[],char nameornum[]) 
{ 
BBnode *r; 
if(strcmp(nameornum,"num")==0) 
{ 
  r=l->next; 
  while(r!=NULL) 
  { 
      if(strcmp(r->id,findmess)==0) 
      return r; 
      r=r->next; 
  } 
} 
else if(strcmp(nameornum,"user")==0) 
{ 
  r=l->next; 
  while(r!=NULL) 
  { 
     if(strcmp(r->user,findmess)==0) 
     return r; 
     r=r->next; 
  } 
} 

else if(strcmp(nameornum,"book")==0) 
{ 
  r=l->next; 
  while(r!=NULL) 
  { 
     if(strcmp(r->book,findmess)==0) 
     return r; 
     r=r->next; 
  } 
} 
return 0; 
} 
 /*******管理员添加用户*******/
void Add(Ulink l)
{ 
Unode *p,*r,*s; 
char num[10]; 
r=l; 
s=l->next; 
while(r->next!=NULL) 
	r=r->next; 
textcolor(RED);
gotoxy(25,4);
cputs("INPUT THE MESSAGE ABOUT BOOK");
gotoxy(31,10);
textcolor(YELLOW);
cputs("User ID:");
scanf("%s",num); 
p=(Unode *)malloc(sizeof(Unode));  
while(s) 
{ 
	if(strcmp(s->data.num,num)==0) 
	{ 
		gotoxy(25,15);
		cputs("This ID:");
		printf("'%s'",num); 
		cputs("is exist!");    
		gotoxy(25,22);
		textcolor(RED);
		cputs("please Press any key to continue...");  
		gotoxy(255,252);
		getch();                    
		return; 
	} 
	s=s->next; 
} 
strcpy(p->data.num,num); 
gotoxy(31,12);
textcolor(YELLOW);
cputs("Input Password:"); 
scanf("%s",p->data.pass); 
gotoxy(31,14);
cputs("input your name:"); 
scanf("%s",p->data.name); 
gotoxy(31,16);
cputs("input the sex:"); 
scanf("%s",p->data.sex); 
p->next=NULL; 
r->next=p; 
r=p; 
gotoxy(30,22);
cputs("Add User Success !!!");
usercount++;
getch();
textcolor(YELLOW);
} 


/******查询用户信息*******/
void Qur(Ulink l)
{ 
int sel; 
char findmess[20]; 
Unode *p; 

if(!l->next) 
{
gotoxy(25,4);
textcolor(RED);
     cputs("Error!!!"); 
    return; 
} 
textcolor(RED);
gotoxy(25,4);
cputs("Please Select Search  Type !");
gotoxy(10,8);
textcolor(WHITE);
cputs("1.Search by ID");
gotoxy(10,10);
cputs("2.Search by Name");
gotoxy(10,12);
cputs("Please Select 1 or 2:");
scanf("%d",&sel); 
if(sel==1)/*学号 */ 
{
	gotoxy(36,8);
	textcolor(YELLOW);
	cputs("Input the User ID:"); 
	scanf("%s",findmess); 
	p=Locate(l,findmess,"num"); 
	if(p) 
	{ 
		gotoxy(36,12);
		textcolor(WHITE);
		cputs("User ID:");
		printf("%s",p->data.num);
		gotoxy(36,14);
		textcolor(WHITE);
		cputs("User Name:");
		printf("%s",p->data.name);
		gotoxy(36,16);
		textcolor(WHITE);
		cputs("User Sex:");
		printf("%s",p->data.sex);
		textcolor(WHITE);
		gotoxy(36,18);
		cputs("User password:");
		printf("%s",p->data.pass);
		getch();
		textcolor(YELLOW);
	}
	else 
	{
		gotoxy(30,22);
		textcolor(RED);
		cputs("Not finde !!!"); 
		getch();
	}
}	
else if(sel==2) /* 姓名 */
{ 
	gotoxy(36,8);
	textcolor(YELLOW);
	cputs("Input the User name:"); 
	scanf("%s",findmess); 
	p=Locate(l,findmess,"name"); 		
	if(p) 
	{ 
		gotoxy(36,10);
		printf("Search Resault\n");
		gotoxy(36,12);
		printf("User ID:%s",p->data.num);
		gotoxy(36,14);
		printf("User Name:%s",p->data.name);
		gotoxy(36,16);
		printf("User Sex:%s",p->data.sex);
		textcolor(WHITE);
		gotoxy(36,18);
		cputs("User passwordx:");
		printf("%s",p->data.pass);
		getch();
		textcolor(YELLOW);
	} 
	else 
	{
		textcolor(RED);
		gotoxy(30,22);
		cputs("Not finde !!!"); 
	}
} 
else 
{
	textcolor(RED);
	gotoxy(30,22); 
	cputs("Error !!"); 
	getch();
}
} 

int cmpuser(Ulink l,char id[])/*******用于登录模块查找用户账号*******/
{ 
Unode *p; 
if(!l->next) 
{
gotoxy(30,4);
textcolor(RED);
     cputs("Error!!!"); 
    return 0; 
} 
	p=Locate(l,id,"num"); 
	if(p) 
	{ 
		return 1;
	}
	else 
	{
		gotoxy(30,22);
		textcolor(RED);
		cputs(" User id Not finde !!!"); 
		getch();
		return 0;
	}

} 

/******查询用户借书*******/
void Qur_borrow(BBlink l)
{ 
int sel; 
char findmess[20]; 
BBnode *p; 

if(!l->next) 
{
gotoxy(25,4);
textcolor(RED);
     cputs("Error!!!"); 
    return; 
} 
textcolor(RED);
gotoxy(25,4);
cputs("Please Select Search  Type !");
gotoxy(10,8);
textcolor(WHITE);
cputs("1.Search by borrow ID");
gotoxy(10,10);
cputs("2.Search by book Name");
gotoxy(10,12);
cputs("Please Select 1 or 2:");
scanf("%d",&sel); 
if(sel==1)/*学号 */ 
{
	gotoxy(36,8);
	textcolor(YELLOW);
	cputs("Input the Borrow ID:"); 
	scanf("%s",findmess); 
	p=Locate_borrow(l,findmess,"num"); 
	if(p) 
	{ 
		gotoxy(36,12);
		textcolor(WHITE);
		cputs("Borrow ID:");
		printf("%s",p->id);
		gotoxy(36,14);
		textcolor(WHITE);
		cputs("Book Name:");
		printf("%s",p->book);
		gotoxy(36,16);
		textcolor(WHITE);
		cputs("User name:");
		printf("%s",p->user);
		getch();
		textcolor(YELLOW);
	}
	else 
	{
		gotoxy(30,22);
		textcolor(RED);
		cputs("Not finde !!!"); 
		getch();
	}
}	
else if(sel==2) /* 姓名 */
{ 
	gotoxy(36,8);
	textcolor(YELLOW);
	cputs("Input the Book name:"); 
	scanf("%s",findmess); 
	p=Locate_borrow(l,findmess,"book");
	if(p) 
	{ 
		gotoxy(36,12);
		printf("Borrow ID:%s",p->id);
		gotoxy(36,14);
		printf("Book Name:%s",p->book);
		gotoxy(36,16);
		printf("User name:%s",p->user);
		getch();
		textcolor(YELLOW);
	} 
	else 
	{
		textcolor(RED);
		gotoxy(30,22);
		cputs("Not finde !!!"); 
		getch();
	}
} 
else 
{
	textcolor(RED);
	gotoxy(30,22); 
	cputs("Error !!"); 
	getch();
}
} 
void Del(Ulink l) /* 删除用户 */ 
{ 
	int sel; 
	Unode *p,*r; 
	char findmess[20]; 
	if(!l->next) 
	{ 
		gotoxy(30,4);
		textcolor(RED);
		cputs("not thing could delete!\n"); 
		getch();
		return; 
	} 
	textcolor(RED);
	gotoxy(25,4);
	puts("Please Select Delete  Type !");
	gotoxy(10,8);
	
	textcolor(WHITE);
	cputs("1.Delete by User ID");
	gotoxy(10,10);
	cputs("2.Delete by name");
	gotoxy(10,12);
	cputs("Please Select 1 or 2:");
	scanf("%d",&sel); 
	if(sel==1)     
	{ 
		gotoxy(36,8);
		textcolor(YELLOW);
		cputs("Input the User ID:"); 
		scanf("%s",findmess); 
		p=Locate(l,findmess,"num"); 
		if(p) 
		{ 
			r=l; 
			while(r->next!=p) 
				r=r->next; 
			r->next=p->next; 
			free(p); 
			gotoxy(30,22);
			textcolor(RED);
			usercount--;
			cputs("Delete success!\n");
			getch();
			textcolor(YELLOW);  
		} 
		else 
		{
			gotoxy(30,22);
			textcolor(RED);
			cputs("Not find !!"); 
			getch();
			
		}
	} 
	else if(sel==2) 
	{ 
		gotoxy(36,8);
		textcolor(YELLOW);
		cputs("Input the User name:"); 
		scanf("%s",findmess); 
		p=Locate(l,findmess,"name"); 
		if(p) 
		{ 
			r=l; 
			while(r->next!=p) 
				r=r->next; 
			r->next=p->next; 
			free(p); 
			gotoxy(30,22);
			textcolor(RED);
			usercount--;
			cputs("Delete success!\n"); 
			getch();
			textcolor(YELLOW);  
		} 
		else
		{
			gotoxy(30,22);
			textcolor(RED);
			cputs("Not find!!"); 
			getch();
		}
	} 
	else
	{
		gotoxy(30,22);
		textcolor(RED);
		cputs("Not find !!"); 
		getch();
	}
	textcolor(YELLOW);
} 
/* 还书的操作 */ 
void Del_borrow(BBlink l) 
{ 
		int sel; 
	BBnode *p,*r; 
	Bnode *L;
	char findmess[20];
	FILE *fp; /* 文件指针 */
Bnode *P,*R,*Q;
L=(Bnode*)malloc(sizeof(Bnode)); 
L->next=NULL; 
R=L; 
fp=fopen(file,"rb"); 
if(fp==NULL) 
{ 
	fp=fopen(file,"wb");
 }
while(!feof(fp))
{
    P=(Bnode*)malloc(sizeof(Bnode));
    if(fread(P,sizeof(Bnode),1,fp)) /* 将文件的内容放入接点中 */
    {
        P->next=NULL;
        R->next=P;
        R=P; /* 将该接点挂入连中 */
    }
}
fclose(fp); /* 关闭文件 */

	if(!l->next)
	{
		gotoxy(30,4);
		textcolor(RED);
		cputs("not Book could Return!\n");
		getch();
		return;
	}
	textcolor(RED);
	gotoxy(25,4);
	puts("Please Select Return  Type !");
	gotoxy(10,8);

	textcolor(WHITE);
	cputs("1.Return by Borrow ID");
	gotoxy(10,10);
	cputs("2.Return by book name");
	gotoxy(10,12);
	cputs("Please Select 1 or 2:");
	scanf("%d",&sel);
	if(sel==1)
	{
		gotoxy(36,8);
		textcolor(YELLOW);
		cputs("Input the Borrow ID:");
		scanf("%s",findmess);
		p=Locate_borrow(l,findmess,"num");
		if(p)
		{

				Q=locate(L,findmess,"num");
				if(Q) 
				{ 
					Q->data.sum=Q->data.sum-1;
						save(L);
				} 
			r=l; 
			while(r->next!=p) 
				r=r->next; 
			r->next=p->next; 
			free(p); 
			gotoxy(30,22);
			textcolor(RED);
			cputs("Return success!\n"); 
			borrowcount--;
			getch();
			textcolor(YELLOW);  
		} 
		else 
		{
			gotoxy(30,22);
			textcolor(RED);
			cputs("Note find !!"); 
			getch();
			
		}
	} 
	else if(sel==2) 
	{ 
		gotoxy(36,8);
		textcolor(YELLOW);
		cputs("Input the Book name:"); 
		scanf("%s",findmess); 
		p=Locate_borrow(l,findmess,"book");
		if(p) 
		{ 
			
				Q=locate(L,findmess,"name"); 
				if(Q) 
				{ 
					Q->data.sum=Q->data.sum-1;
						save(L);
				} 
			
			r=l; 
			while(r->next!=p) 
				r=r->next; 
			r->next=p->next; 
			free(p); 
			gotoxy(30,22);
			textcolor(RED);
			cputs("Borrow success!\n"); 
			borrowcount--;
			getch();
			textcolor(YELLOW);  
		} 
		else
		{
			gotoxy(30,18);
			textcolor(RED);
			cputs("Not find!!"); 
			getch();
		}
	} 
		
	else
	{
		gotoxy(30,22);
		textcolor(RED);
		cputs("Not finde !!"); 
		getch();
	}
	textcolor(YELLOW);
} 
/* ****用于管理员修改用户资料 ***/
void Modify(Ulink l) 
{ 
	Unode *p; 
	char findmess[20]; 
	if(!l->next) 
	{
		gotoxy(30,4);
		textcolor(RED);
		cputs("not thing could modify!"); 
		getch();
		return; 
	} 
	gotoxy(30,4);
	textcolor(RED);
	cputs("Modify User Message");
	gotoxy(25,8);
	textcolor(YELLOW);
	cputs("input the User Id:");
	scanf("%s",findmess); 
	p=Locate(l,findmess,"num"); 
	if(p) 
	{ 
		textcolor(YELLOW);
		gotoxy(25,10);
		printf("Inpute The New ID(old:%s):",p->data.num); 
		scanf("%s",p->data.num); 
		gotoxy(25,12);
		printf("Input The New Password(old:%s):",p->data.pass); 
		scanf("%s",p->data.pass); 
		gotoxy(25,14);
		printf("Input The New Name(old:%s):",p->data.name); 
		scanf("%s",p->data.name); 
		gotoxy(25,16);
		printf("Input The New Sex(old:%s):",p->data.sex); 
		scanf("%s",p->data.sex); 
		gotoxy(30,20);
		textcolor(RED);
		cputs("Modify Success !!!"); 
		getch();
		textcolor(YELLOW);
	} 
	else 
	{	
		gotoxy(30,16);
		textcolor(RED);
		cputs("Not Finde !!!"); 
		getch();
	}
} 







 /****供用户修改用户自己资料 */
void Modify_user(Ulink l)
{ 
	Unode *p; 
	char findmess[20]; 
	if(!l->next) 
	{
		gotoxy(30,4);
		textcolor(RED);
		cputs("not thing could modify!"); 
		getch();
		return; 
	} 
	gotoxy(30,4);
	textcolor(RED);
	cputs("Modify User Message");
	gotoxy(33,8);
	textcolor(YELLOW);
	strcpy(findmess,os->data.num);
	printf("your id:%s",findmess);
	p=Locate(l,findmess,"num"); 
	if(p) 
	{ 
		textcolor(YELLOW);
		gotoxy(24,10);
		printf("Input The New Password(old:%s):",p->data.pass); 
		scanf("%s",p->data.pass); 
		gotoxy(24,12);
		printf("Input The New Name(old:%s):",p->data.name); 
		scanf("%s",p->data.name); 
		gotoxy(24,14);
		printf("Input The New Sex(old:%s):",p->data.sex); 
		scanf("%s",p->data.sex); 
		gotoxy(31,18);
		textcolor(RED);
		cputs("Modify Success !!!"); 
		getch();
		textcolor(YELLOW);
	} 
	else 
	{	
		gotoxy(30,16);
		textcolor(RED);
		cputs("Not Finde !!!"); 
		getch();
	}
} 







/*******用户账号密码匹配*******/
int cmps(Ulink l,char id[20],char passw[20])
{ 

Unode *p; 
if(!l->next) 
{ 
gotoxy(34,18);
textcolor(RED);
textbackground(179);

     cputs("Missing Datas !!!"); 
getch();
    return 0; 
}
p=Locate(l,id,"num"); 
if(p) 
{ 
	if(strcmp(p->data.pass,passw)==0)
	{
			os=p;
			return 1;

	}
	else
	{
		textcolor(RED);
		gotoxy(34,18);
		textbackground(179);
		cputs("Password Error !!!"); 
		getch();
		return 0;
	}

 	
}
else 
{
	gotoxy(34,18);
	textcolor(RED);
	textbackground(179);
	cputs("User NotFind !!!"); 
	getch();
	return 0;
}
}

 /* 用于修改图书资料 */
int changeb(blink l,char findmess[20])
{ 
int tmp;
Bnode *p; 
if(!l->next) 
{
	gotoxy(30,4);
	textcolor(RED);
	cputs("not thing could modify!"); 
	getch();
	return 0; 
} 	
p=locate(l,findmess,"num"); 
if(p) 
{ 
	if(p->data.sum<p->data.count)
	{				
		booksum=booksum+1;
		p->data.sum=p->data.sum+1;
		save(l);
		return 1;
	}
	else
	{
		textcolor(WHITE);
		gotoxy(25,20);
		cputs("Book  have not return !!"); 
		getch();
		return 0;

	}
} 
else 
{	
	gotoxy(20,16);
	textcolor(RED);
	cputs("Not Finde !!!"); 
	getch();
	textcolor(YELLOW);
	return 0;
}
} 






/*******添加借书*******/
void Add_borrow(BBlink l) 
{ 
Ulink H;/* 连表 */ 
FILE *Fp; /* 文件指针 */ 
Unode *Q,*T;
blink L;/* 连表 */ 
FILE *FP; /* 文件指针 */ 
int ttl;

Bnode *P,*R;
char bookid[20];
char bookname[20];
char userid[20];
BBnode *p,*r,*s; 
char num[10]; 
r=l; 
s=l->next; 
while(r->next!=NULL) 
	r=r->next; 
L=(Bnode*)malloc(sizeof(Bnode)); 
L->next=NULL; 
R=L; 
FP=fopen(file,"rb"); 
if(FP==NULL) 
{ 
	FP=fopen(file,"wb"); 
 } 
while(!feof(FP)) 
{ 
    P=(Bnode*)malloc(sizeof(Bnode)); 
    if(fread(P,sizeof(Bnode),1,FP)) /* 将文件的内容放入接点中 */ 
    { 
        P->next=NULL; 
		bookcount=bookcount+P->data.count;
        R->next=P; 
        R=P; /* 将该接点挂入连中 */ 

    } 
} 
fclose(FP); /* 关闭文件 */ 


H=(Unode*)malloc(sizeof(Unode)); 
H->next=NULL; 
T=H;
Fp=fopen(ufile,"rb"); 
if(Fp==NULL) 
{ 
	Fp=fopen(ufile,"wb"); 
 } 
while(!feof(Fp)) 
{ 
    Q=(Unode*)malloc(sizeof(Unode));
    if(fread(Q,sizeof(Unode),1,Fp)) /* 将文件的内容放入接点中 */ 
    { 
        Q->next=NULL; 
        T->next=Q; 
        T=Q; /* 将该接点挂入连中 */ 
    } 
} 
fclose(Fp); 
textcolor(RED);
gotoxy(25,4);
cputs("Please input thease message");
gotoxy(30,10);
textcolor(YELLOW);
cputs("Input Borrow ID:");
scanf("%d",&ttl);
itoa(ttl,num,10) ;
p=(BBnode *)malloc(sizeof(BBnode));
while(s) 
{ 
	if(strcmp(s->id,num)==0) 
	{ 
		gotoxy(30,15);
		cputs("Borrow ID:");
		printf("'%s'",num); 
		cputs("is exist!");    
		gotoxy(26,22);
		textcolor(RED);
		cputs("please Press any key to continue...");  
		gotoxy(255,252);
		getch();                    
		return; 
	} 
	s=s->next; 
} 
strcpy(p->id,num);
gotoxy(31,12);
textcolor(YELLOW);
cputs("Input book id:"); 
scanf("%s",bookid);
gotoxy(31,14);
textcolor(YELLOW);
cputs("Input book name:"); 
scanf("%s",bookname);/***************************************图书判断在否***************************************/ 
if(cmpbook(L,bookid,bookname))
{
	strcpy(p->bookid,bookid);
	strcpy(p->book,bookname);
	gotoxy(31,16);
cputs("input your ID:"); 
scanf("%s",userid); /**************************************用户判断在否********************************/
if(cmpuser(H,userid))
{
	strcpy(p->user,userid);
	p->next=NULL; 
r->next=p; 
r=p; 
if(changeb(L,bookid))
{
gotoxy(30,22);
cputs("Borrow Success !!!");
Save_borrow(l);
borrowcount++;

getch();
}
}

}

textcolor(YELLOW);
} 


/*******我借的书*******/
void Myborrow()
{
int i;

BBlink l,p,r;/* 连表 */ 
FILE *fp; /* 文件指针 */ 
int count=0; 
l=(BBnode*)malloc(sizeof(BBnode)); 
l->next=NULL; 
r=l; 
fp=fopen(bfile,"rb"); 
if(fp==NULL) 
{ 
	fp=fopen(bfile,"wb"); 
} 
i=6;
while(!feof(fp)) 
{


	
	p=(BBnode*)malloc(sizeof(BBnode)); 
	if(fread(p,sizeof(BBnode),1,fp)) /* 将文件的内容放入接点中 */ 
	{ 
		textcolor(WHITE);
		gotoxy(30,4);
		cputs("Your borrow book");
		
		if(strcmp(p->user,os->data.num)==0)
		{
			textcolor(YELLOW);

			gotoxy(20,i);
			printf("Borrow ID:%s\tBook id:%s\tBook name:%s",p->id,p->bookid,p->book);
			i++;

			p->next=NULL; 
			r->next=p; 
			r=p; /* 将该接点挂入连中 */ 
			count++; 
		}
    } 
	myborrow=count;
	if(myborrow==0)
	{
		textcolor(YELLOW);
		gotoxy(30,13);
		cputs("You  no borrow !!");


	}



}
fclose(fp);
textcolor(YELLOW);
	gotoxy(65,2);
	printf("(borrow=%d)",myborrow);
		
	getch();

}
/******用户登录界面*******/
int login(Ulink l)
{
 

/*****************登陆窗口初始化****************************/
while(!flag)
{
char User_id[10],password[10],tmp;
int i=0;
User_id[0]='\0';
password[0]='\0';
textbackground(179);
clrscr();
gotoxy(33,23);
textcolor(YELLOW);
cputs("ESC = EXIT");
textcolor(WHITE);
textbackground(179);
box(1,1,80,24);
h_line(2,3,78);
gotoxy(15,2);
cputs("Login now please input the User__ID  and Password");
bar_(30,10,23,5,5);
/******************账号过滤*********************/
while(User_id[0]=='\0')
{
gotoxy(31,11);
textcolor(YELLOW);
textbackground(5);
cputs("User__ID:");
gotoxy(31,13);
cputs("Password:");
textbackground(179);
textcolor(WHITE);
gotoxy(28,7);
cputs("please input the User__ID!");
textbackground(179);
textcolor(YELLOW);
gotoxy(27,11);
putch(272);
gotoxy(27,13);
putch(' ');
gotoxy(40,11);
tmp=getch();
if(tmp==27)
{
flg=27;
return 0;
}
while(tmp!='\r'&&i<=9&&(tmp>=64&&tmp<=90)||(tmp>=97&&tmp<=122||(tmp>=48&&tmp<=57)))
{
textcolor(YELLOW);
textbackground(5);
	putch(tmp);
	User_id[i]=tmp;
	i++;
	tmp=getch();
	if(tmp==27)
	{
flg=27;
return 0;
}
}
User_id[i]='\0';
}
/**********************密码过滤****************************/
while(password[0]=='\0')
{
gotoxy(28,7);
textbackground(179);
textcolor(WHITE);
cputs("  input the password now  ");
textbackground(179);
textcolor(YELLOW);
gotoxy(27,13);
putch(272);
gotoxy(27,11);
putch(' ');
gotoxy(40,13);
i=0;
tmp=getch();
if(tmp==27)
{
flg=27;
return 0;
}
while(tmp!='\r'&&i<=10&&(tmp>=64&&tmp<=90)||(tmp>=97&&tmp<=122||(tmp>=48&&tmp<=57)))
{
	textbackground(5);
	putch('*');

	password[i]=tmp;
	i++;
	tmp=getch();
	if(tmp==27)
	{
flg=27;
return 0;
}
}
password[i]='\0';
}
/*************把账号和密码进行对比验证**************/
if(!strcmp(User_id,"admin")&&!strcmp(password,"admin"))
	{
		return 2;
	}
if(cmps(l,User_id,password))
{	 
	return 1;
}
}
}



/*******图书管理*******/
void bookss()
{
bookcount=0;
booksum=0;
while(1)
{
int flag;
blink l;/* 连表 */ 
FILE *fp; /* 文件指针 */ 
int count=0; 
Bnode *p,*r;
l=(Bnode*)malloc(sizeof(Bnode)); 
l->next=NULL; 
r=l; 
fp=fopen(file,"rb"); 
if(fp==NULL) 
{ 
	fp=fopen(file,"wb"); 
 } 
while(!feof(fp)) 
{ 
    p=(Bnode*)malloc(sizeof(Bnode)); 
    if(fread(p,sizeof(Bnode),1,fp)) /* 将文件的内容放入接点中 */ 
    { 
        p->next=NULL; 
		bookcount=bookcount+p->data.count;
		booksum=booksum+p->data.sum;
        r->next=p; 
        r=p; /* 将该接点挂入连中 */ 
        count++; 
    } 
} 
fclose(fp); /* 关闭文件 */ 
gotoxy(30,4);
printf("Ok loading %ds.\n",count); 

while(1) 
{	
	textbackground(179);
	clrscr();
	textcolor(YELLOW);
	box(1,1,80,24);
	h_line(2,3,78);
	gotoxy(3,2);
	textcolor(RED);
	cputs("A");
	textcolor(BLUE);
	cputs("dd Book  ");
	textcolor(RED);
	cputs("D");
	textcolor(BLUE);
	cputs("el Book  ");
	textcolor(RED);
	cputs("S");
	textcolor(BLUE);
	cputs("earch Book  ");
	textcolor(RED);
	cputs("M");
	textcolor(BLUE);
	cputs("odify Book  ");
	textcolor(YELLOW);
	printf("(Book=%d/%d)",bookcount-booksum,bookcount);
	printf("%d",booksum);
	gotoxy(50,50);
	flag=getch(); 
    switch(flag)		
	{ 		
	case 'a':
		add(l);
		save(l);
		break; /*************添加图书*********/
	case 'd':
		del(l);
		save(l); 
		break;/*************删除图书**********/
	case 's':
		qur(l);
		break;
	case 'm':
		mod(l);
		save(l); /*************修改图书**********/
		break;
	case 27:
		return;
		
	} 

} 	

}

}

/*******用户管理*******/
void users()
{
while(1)
{
int flag;
Ulink l;/* 连表 */ 
FILE *fp; /* 文件指针 */ 
int count=0; 
Unode *p,*r;
l=(Unode*)malloc(sizeof(Unode)); 
l->next=NULL; 
r=l;
fp=fopen(ufile,"rb"); 
if(fp==NULL) 
{ 
	fp=fopen(ufile,"wb"); 
 } 
while(!feof(fp)) 
{ 
    p=(Unode*)malloc(sizeof(Unode)); 
    if(fread(p,sizeof(Unode),1,fp)) /* 将文件的内容放入接点中 */ 
    { 
        p->next=NULL; 
        r->next=p; 
        r=p; /* 将该接点挂入连中 */ 
        count++; 
    } 
	usercount=count;
} 
fclose(fp); /* 关闭文件 */ 
gotoxy(30,4);
printf("Ok loading %ds.\n",count); 

while(1) 
{	
	textbackground(179);
	clrscr();
	textcolor(YELLOW);
	box(1,1,80,24);
	h_line(2,3,78);
	gotoxy(3,2);
	textcolor(RED);
	cputs("A");
	textcolor(BLUE);
	cputs("dd user  ");
	textcolor(RED);
	cputs("D");
	textcolor(BLUE);
	cputs("el user  ");
	textcolor(RED);
	cputs("S");
	textcolor(BLUE);
	cputs("earch user  ");
	textcolor(RED);
	cputs("M");
	textcolor(BLUE);
	cputs("odify user  ");
	textcolor(YELLOW);
	printf("count: (user=%d)",usercount);
	gotoxy(50,50);
	flag=getch(); 
    switch(flag)		
	{ 		
	case 'a':
		Add(l);
		Save(l);
		break; /*************添加用户**********/
	case 'd':
		Del(l);
		Save(l); 
		break;/*************删除用户**********/
	case 's':
		Qur(l);
		break;
	case 'm':
		Modify(l);
		Save(l); /*************修改用户**********/
		break;
	case 27:
		return;
		
	} 

} 	

}

}

/*******借书管理*******/
void borrow()
{
while(1)
{
int flag;
BBlink l,p,r;/* 连表 */ 
FILE *fp; /* 文件指针 */ 
int count=0; 
l=(BBnode*)malloc(sizeof(BBnode)); 
l->next=NULL; 
r=l; 
fp=fopen(bfile,"rb"); 
if(fp==NULL) 
{ 
	fp=fopen(bfile,"wb"); 
} 
while(!feof(fp)) 
{
	p=(BBnode*)malloc(sizeof(BBnode)); 
	if(fread(p,sizeof(BBnode),1,fp)) /* 将文件的内容放入接点中 */ 
	{ 
		p->next=NULL; 
        r->next=p; 
        r=p; /* 将该接点挂入连中 */ 
        count++; 
    } 
	borrowcount=count;
} 
while(1) 
{	
	textbackground(179);
	clrscr();
	textcolor(YELLOW);
	box(1,1,80,24);
	h_line(2,3,78);
	gotoxy(3,2);
	textcolor(RED);
	cputs("B");
	textcolor(BLUE);
	cputs("orrow book  ");
	textcolor(RED);
	cputs("R");
	textcolor(BLUE);
	cputs("eturn book  ");
	textcolor(RED);
	cputs("S");
	textcolor(BLUE);
	cputs("earch borrow  ");
	textcolor(YELLOW);
	printf("count: (borrow=%d)",borrowcount);
	textcolor(RED);
	gotoxy(50,50);
	flag=getch(); 
    switch(flag)		
	{ 		
	case 'b':
		Add_borrow(l);
		
		break; /*************添加用户**********/
	case 'r':
		Del_borrow(l);
		Save_borrow(l);
		break;
	case 's':
		Qur_borrow(l);
		break;
	case 27:
		return;
		
	} 

} 	

}


}



/*******管理员选择界面*******/
void choose()
{
while(1)
{

textbackground(179);
clrscr();
gotoxy(33,2);
textcolor(WHITE);
cputs("Administrastor");
textcolor(YELLOW);
box(1,1,80,24);
h_line(2,3,78);
gotoxy(3,6);
cputs(">>>-------------------------1.User Management----------------------------<<<");
gotoxy(3,10);
cputs(">>>-------------------------2.Book Management----------------------------<<<");
	gotoxy(3,14);
cputs(">>>-------------------------3.Borrow Books-------------------------------<<<");
gotoxy(30,22);
textcolor(RED);
cputs("Please Select!!!");
flg=getch();
if(flg=='2')		
	bookss();									
if(flg=='1')
	users();
if(flg=='3')
	borrow();
if(flg==27)
{
	flg=-1;
	return;
}	
}		
}
void admin()
{
while(1)
{
choose();	
if(flg=='1')	
bookss();
if(flg=='2')
users();
if(flg=='3')
borrow();
if(flg==27)
{
return;
}
}
}
/******用户查询自己的信息*******/
void show(Ulink l)
{
if(l) 
	{ 
		gotoxy(30,8);
		textcolor(WHITE);
		cputs("User ID:");
		printf("%s",l->data.num);
		gotoxy(30,10);
		textcolor(WHITE);
		cputs("User Name:");
		printf("%s",l->data.name);
		gotoxy(30,12);
		textcolor(WHITE);
		cputs("User Sex:");
		printf("%s",l->data.sex);
		textcolor(WHITE);
		gotoxy(30,14);
		textcolor(WHITE);
		cputs("User password:");
		printf("%s",l->data.pass);
		getch();
		textcolor(YELLOW);
	}
	else 
	{
		gotoxy(30,22);
		textcolor(RED);
		cputs("Error !!!"); 
		getch();
	}


}
/*******用户操作界面*******/
void user(Ulink h)
{
int flag;
BBlink l,p,r;/* 连表 */ 
FILE *fp; /* 文件指针 */ 
int count=0; 
l=(BBnode*)malloc(sizeof(BBnode)); 
l->next=NULL; 
r=l; 
fp=fopen(bfile,"rb"); 
if(fp==NULL) 
{ 
	fp=fopen(bfile,"wb"); 
} 
while(!feof(fp)) 
{
	p=(BBnode*)malloc(sizeof(BBnode)); 
	if(fread(p,sizeof(BBnode),1,fp)) /* 将文件的内容放入接点中 */ 
	{ 
		p->next=NULL; 
        r->next=p; 
        r=p; /* 将该接点挂入连中 */ 
        count++; 
    } 
} 
	while(1) 
{	
	textbackground(179);
	clrscr();
	textcolor(YELLOW);
	box(1,1,80,24);
	h_line(2,3,78);
	gotoxy(3,2);
	textcolor(RED);
	cputs("A.");
	textcolor(BLUE);
	cputs("my message  ");
	textcolor(RED);
	cputs("B.");
	textcolor(BLUE);
	cputs("modyfy my message  ");
	textcolor(RED);
	cputs("C.");
	textcolor(BLUE);
	cputs("my borrow  ");
	textcolor(RED);
	cputs("D.");
	textcolor(BLUE);
	cputs("search book");
	textcolor(YELLOW);
	gotoxy(50,50);
	flag=getch(); 
    switch(flag)		
	{ 		
	case 'a':
		show(os);
		
		break; /*************添加用户**********/
	case 'b':
		Modify_user(h);
		Save(h);
		
		break;/*************删除用户**********/
	case 'c':
		Myborrow();
		break;
	case 'd':
		usersearch();

		
		break;
	case 27:
		return;
		
	} 

} 

}

main()
{

Ulink h,os;/* 连表 */ 
FILE *fp; /* 文件指针 */ 
Unode *p,*r;
h=(Unode*)malloc(sizeof(Unode)); 
h->next=NULL; 
r=h; 
fp=fopen(ufile,"rb"); 
if(fp==NULL) 
{ 
	fp=fopen(ufile,"wb"); 
 } 
while(!feof(fp)) 
{ 
    p=(Unode*)malloc(sizeof(Unode)); 
    if(fread(p,sizeof(Unode),1,fp)) /* 将文件的内容放入接点中 */
    {
        p->next=NULL;
        r->next=p;
        r=p; /* 将该接点挂入连中 */

    }
}
fclose(fp);
system("wellcome");
	if(flg==27)
	{
		flg=-1;
		return;
	}
		while(1)
		{
			flag=login(h);
			if(flg==27)
				{
					flg=-1;
					break;
				}

				if(flag==2)
				{
					choose();
					flag=0;
				}
				if(flag==1)
				{
					user(h);
					flag=0;
				}

		}

}

/* 【自学去】网站收集 http://www.zixue7.com */

#include<stdio.h>  //头文件
#include<string.h> //头文件
#include<stdlib.h> //头文件
#define M 100      //货物种类
#define N 100      //顾客数目
struct goods       //单个货物信息格式
{
	int  number;   //产品编号
	char name[20]; //产品名称
	int  price1;   //进价（或利润）
	int  price2;   //售价
	int  amount;   //数量
};
struct orderlist           //订单格式
{
	struct goods L[M];
};
struct customer            //客户信息内容和格式
{
	char name[20];         //姓名
	char password[6];      //服务密码
	int count;             //订单号
};
struct customer khl[N];    //所有客户信息
struct goods base[M];      //库存信息
struct orderlist odl[10*N];//订单表
struct goods lackl[M];     //缺货表
struct goods suml[M+1];    //销售报表
int PP=0;                  //订单编号
int QQ=0;				   //客户编号	
int goodssum=0;              //货物总数


int main()                    //主函数开始!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
{
	int SysUseWay();        //系统使用说明函数声明
	int chushihua();       //系统初始化函数声明
	int welcome();        //欢迎界面函数声明
	int kh();            //客户函数声明
	int buyer();        //采购部函数声明
	int manager();     //公司经理函数声明
	int goodbye();    //欢送函数声明
	int n,m=9;       //变量声明
	SysUseWay();    //系统使用说明书
	chushihua();   //系统初始化
	while(m==9)   //循环三岔口
	{
    welcome();  //欢迎界面
	scanf("%d",&n);
	switch(n) //各奔东西
	{
	case 0 : ;break;
	case 1 : kh(); break;       //客户之家
	case 2 : buyer(); break;    //采购之家
	case 3 : manager(); break;  //经理之家
	default : printf("输入错误!请退出重从新输入");
	}
    if(n==0)                   
		break;
	printf("              继续进入主界面请按 9  退出请按0\n");
	scanf("%d",&m);
	}
	goodbye();                  //欢送界面
	return 0;
}                               //主函数结束!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                                       

int SysUseWay()                 //系统使用说明书函数定义
{
	FILE *fp;
	fp=fopen("D://商品销售系统使用说明书.txt","w");
    fprintf(fp,"                                                  商品销售系统使用说明书\n");
    fprintf(fp,"    作者  卢孟江    学号  13091012   西安电子科技大学软件学院130911班   2010年10月6日\n");
	fprintf(fp,"    本销售系统仅是一个销售部门管理系统的的简单模型，功能少，不太稳定，不太安全。\n");
	fprintf(fp,"    主要分为三个模块。客户--销售部门--公司经理。\n");
	fprintf(fp,"    1--客户。客户主要是通过该系统进行订货。进入系统后，需要注册，登陆。系统会自主分配\n");
	fprintf(fp,"一个服务编号给客户，由客户自己设定密码。订购之后，可以登录查询自己的订货情况.不足之处\n");
	fprintf(fp,"在于该系统是一次性的---关闭系统之后，系统会丢失一切数据。这涉及数据库的知识，恕我现在\n");
	fprintf(fp,"无能为力。还有，每一个服务编号只能对应一个订货单，即客户每定一次货就得重新注册一次,否\n");
	fprintf(fp,"则就会覆盖以前的数据。\n");
	fprintf(fp,"    2--采购部门。主管仓库，查看处理缺货，进货。使用该系统前，首先必须由采购部门建立仓\n");
	fprintf(fp,"库。填写货物名称，进价，售价，库存数量。货物编号由系统自主按序分配。接下来，采购部门需\n");
    fprintf(fp,"要经常查看是否缺货。及时进货。\n");
	fprintf(fp,"    3--经理只需要抽时间查看一下销售报表。了解每一种货物的销售情况，以及总的销售利润。\n");
	fprintf(fp,"\n");
	fprintf(fp,"                             谨以此纪念我碌碌无为的大一。\n");
    fclose(fp);
    printf("................................................................................\n");
    printf("本系统使用说明书详见于您的计算机D盘“商品销售系统使用说明书.txt”文件。欢迎查阅 \n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	return 0;
}



int welcome()                   //欢迎界面函数定义
{
	
	printf("................................................................................\n");
	printf("\n");
	printf("                      ~~~~~~~~欢迎使用商品销售系统~~~~~~~~\n");
	printf("\n");
	printf("..............................................................................\n");
	printf("                              公司客户请按       1\n");
	printf("\n");
	printf("..............................................................................\n");
	printf("                              采购人员请按       2\n");
	printf("\n");
	printf("...............................................................................\n");
	printf("                              公司经理请按       3\n");
	printf("\n");
	printf("................................................................................");
	printf("                              退出主界面请按     0\n");
	printf("\n");
	printf("\n");
	printf("................................................................................\n");
    return 0;
}


int chushihua()                  //初始化函数
{
    int i=0,j=0;
	for(i=0;i<M;i++)
	{
     suml[i].number=lackl[i].number=base[i].number=-1;
     base[i].amount=0;
	 base[i].price1=base[i].price2=0;
	 lackl[i].amount=0;
	 suml[i].price1=0;
	 suml[i].amount=0;
	}
	suml[M].price1=0;
	for(i=0;i<N;i++)
	khl[i].count=-1;
	for(i=0;i<10*N;i++)
	{
        for(j=0;j<M;j++)
		{
		odl[i].L[j].number=j;
		odl[i].L[j].amount=0;
		odl[i].L[j].price1=base[j].price1;
		odl[i].L[j].price2=base[j].price2;
		}
	}
    
	return 0;
}



int goodbye()                 //欢送界面函数定义
{
	printf("                 ~~~~感谢您使用商品销售系统~~~~\n");
	printf(".........................................................................\n");
	printf("作者    卢孟江     学号    13091012    软件学院 130911班\n");
	printf(".........................................................................\n");
	printf("\n");
	printf(".........................................................................\n");
	printf("由于时间仓促，能力有限，系统不是很完美，如有问题敬请原谅。欢迎垂询15109281415\n");
	printf(".........................................................................\n");
	return 0;
}
int buyer()                 //采购函数
{
	int welcomebuyer();     //欢迎函数
	int buildbaser();       //仓库建设
	int solvelack();        //查询缺货情况
	int chackbase();        //查询仓库情况
    int n;
    int m=9;
	while(m==9)             //循环三岔口
	{
    welcomebuyer();             //欢迎欢迎
    scanf("%d",&n);             //各奔东西
	switch(n)
	{
	case 0 : ;break;            //退出
	case 1: solvelack();break;  //查缺货
	case 2: buildbaser();break; //建设仓库
	case 3: chackbase();break;  //检查仓库
	default :printf("输入错误!\n");
	}
    if(n==0)
		break;
	printf("              继续采购请按 9  否则请按0\n");
	scanf("%d",&m);
	}
	return 0;
}
	

int welcomebuyer()
{
    printf("................................................................................\n");
	printf("\n");
	printf("                      ~~~~~~~~欢迎来到采购之家~~~~~~~~\n");
	printf("\n");
	printf("..............................................................................\n");
	printf("                              处理缺货                请按   1\n");
	printf("\n");
	printf("..............................................................................\n");
	printf("                              进货                    请按   2\n");
	printf("\n");
	printf("...............................................................................\n");
	printf("                              查询当前存货情况        请按   3\n");
	printf("\n");
	printf("................................................................................");
	printf("                              退出采购                请按   0\n");
	printf("\n");
	printf("\n");
	printf("................................................................................\n");	
return 0;
}


int buildbaser()                //采购函数--进货函数
{
    int i=0;
	printf("			若想停止进货,请在货物数量上输入 0\n\n");
	for(i=0;i<M;i++)
	{
		printf("\n货物名称:");
		scanf("%s",base[i].name);
		base[i].number=i;
        printf("\n货物数量:");
		scanf("%d",&base[i].amount);
		if(base[i].amount<=0){
            base[i].amount=0;
			break;
		}
		goodssum+=base[i].amount;
		printf("\n货物进价:");
		scanf("%d",&base[i].price1);
		printf("\n货物售价:");
        scanf("%d",&base[i].price2);
		}
	return 0;
}


int chackbase()                //采购函数---查询当前存货函数
{
    int i=0;
	printf("................................................................................\n");
	if(goodssum==0)
	{
		printf("           仓库没货!!!         请采购部门及时进货!!!\n");
		return 0;
	}
	for(i=0;i<M;i++)
	{
		if(base[i].amount>0){
              printf("................................................................................\n");
			  printf("货物编号  货物名称             货物数量          进价       售价\n");
	          printf(" %3d      %-20s %6d          %6d     %6d \n",base[i].number,base[i].name,base[i].amount,base[i].price1,base[i].price2);
			  printf("................................................................................\n");
		}
		else
			continue;
	}
    printf("................................................................................\n");
	return 0;
}



int solvelack()                //采购函数--处理缺货函数
{
    int i=0,k=0;
	int printlack();           //采购函数--处理缺货函数--查看函数声明
	int supplement();          //采购函数--处理缺货函数--添货函数声明
	printlack();               //查看是否缺货
    printf("           如果需要进货，请按1  ； 否则，请按0   \n");
	scanf("%d",&k);
	switch(k)
	{
	case 1:supplement();break;
	case 0:;break;
	default :printf("输入错误!\n");
	}
	return 0;
}
int printlack()                //采购函数--处理缺货函数--查看函数定义
{
	int i=0,count=0;
	if(goodssum==0){
          printf("                  !!!仓库没货!!!请采购人员及时进货!!!\n");
		  return 0;
		  }
	printf("................................................................................\n");
	for(i=0;i<M;i++)
	{
		if(lackl[i].amount==0){
		    count++;
			continue;
		 }
	    else{
		 printf("................................................................................\n");
         printf("货物编号  货物名称             缺货数量\n");
	     printf(" %3d      %-20s %6d          \n",lackl[i].number,base[i].name,lackl[i].amount);
		}
	}
	printf("................................................................................\n");
    if(count==M)
		printf("              ~~~不缺~~~货物充足~~~\n");
	else 
		printf("                    缺!   缺!   缺!\n");
return 0;
}
int supplement()              //采购函数--处理缺货函数--添货函数定义
{
    int i=0,j=1;
	int printlack();
	printlack();
	printf("     若想停止进货，请在货物数量上输入0\n");
	while(j>0)
	{
		printf("产品编号\n");
        scanf("%d",&i);
	    while(i>=M){
	    printf("此货物不存在，请重新输入\n");
	    scanf("%d",&i);}
		base[i].number=i;
        printf("产品名称:\n");
        scanf("%s",base[i].name);
		printf("进货数量:\n");
		scanf("%d",&j);
        if(j>0){
           base[i].amount+=j;
		   goodssum+=j;
		}
		else 
			break;
        printf("\n货物进价:");
		scanf("%d",&base[i].price1);
		printf("\n货物售价:");
        scanf("%d",&base[i].price2);
	}
	return 0;
}



int kh()                          //客户函数
{

int login();                      //客户函数--登陆函数声明
int registe();                    //客户函数--注册函数声明
int welcomekh();                  //客户函数--欢迎函数声明
int searchkh(int falg);           //客户函数--查询订货情况的函数声明
int order(int falg);              //客户函数--订货函数声明
int i,k=0,flag=0;
int n=9,m=9;
welcomekh();                     //欢迎欢迎
while(m==9)                      //登陆？注册循环二岔路
{
printf("        登陆请按1，          注册请按2,          否则请按 0\n");                    
scanf("%d",&i);                  
switch(i)
{
case 0 : ;break;
case 1 : flag=login(); break;
case 2 : registe(); break;
default :printf("输入错误!\n");
}
if(i==0)
		break;
printf("           登陆或注册请按 9  否则请按0\n");
scanf("%d",&m);
}

while(n==9)                      //订购？查询循环二岔路
{
printf("         查询请按 1,   订购请按 2,      否则请按 0\n");
scanf("%d",&i);
switch(i)
{
case 0 : ;break;
case 1 : searchkh(flag); break;
case 2 : order(flag); break;
default :printf("输入错误!\n");
}
if(i==0)
	break;
printf("           继续查询或订购请按 9  否则请按0\n");
scanf("%d",&n);
}
return 0;
}


int welcomekh()                    //客户函数---欢迎函数定义
{ 
  
    printf("................................................................................\n");
	printf("\n");
	printf("                      ~~~~~~~~欢迎来到客户之家~~~~~~~~\n");
	printf("\n");
	printf("..............................................................................\n");
    return 0;
}


int login()                       //客户函数---登陆函数定义
{
int n,count=0;
char password[6];
printf("-------------------请输入服务编号-----------------------\n");
scanf("%d",&n);
printf("-------------------请输入六位密码-----------------------\n");
scanf("%s",password);
while(strcmp(password,khl[n].password))
{
    printf("~~~~~~~~~~~~~~~请重新输入六位密码~~~~~~~~~~~~~~~~~~~~\n");
	printf("\n");
	printf("               退出登陆,请输入123456\n");
    scanf("%s",password);
	if(!strcmp(password,"123456"))
		break;
}
if(!strcmp(password,"123456"))
return 0;
else 
return n;
}


int registe()                      //客户函数---注册函数定义
{
printf("~~~~~~~~~~~~~~~~~~~~~~~~~您的服务编号是: %d ,请你一定要记牢您! ~~~~~~~~~~~~~~~~~~~~~~~~~\n",QQ);
printf("\n");
printf("~~~~~~~~~~~~~~~~~~~~~~~~~请输入您的用户名:~~~~~~~~~~~~~~~~~~~~~~~~\n");
printf("\n");
scanf("%s",khl[QQ].name);
printf("~~~~~~~~~~~~~~~~~~~~~~~~~请输入六位密码:~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
printf("\n");
scanf("%s",khl[QQ].password);
QQ++;
return 0;
}


int searchkh(int flag)             //客户函数---查询函数定义
{
int n,i=0;
n=flag;
printf("................................................................................\n");
printf("               Hello   %s   您的服务编号是: %d\n",khl[n].name,n);
printf("................................................................................\n");
if(khl[n].count!=-1){
	for(i=0;i<M;i++){
		if(odl[khl[n].count].L[i].amount==0)
			i++;
		else{
             printf("................................................................................\n");
             printf("货物编号  货物名称             订货数量          单价\n");
	         printf(" %3d      %-20s %6d          %6d\n",base[i].number,base[i].name,odl[khl[n].count].L[i].amount,base[i].price2);
             printf("................................................................................\n");
		}
           
	}
}
else{
	  printf("\n");
      printf("................................................................................\n");
      printf("                                您当前没有订货。                                \n");
	  printf("................................................................................\n");
}
printf("................................................................................\n");
return 0;
}



int order(int flag)              //客户函数---订购函数
{
int i=0,k=0,m=0,n=0,count=0;
printf("\n");
n=flag;
printf("货物编号  货物名称               售价\n");
printf("\n");
for(i=0;i<M;i++){
	if(base[i].number==-1){
		count++;
		continue;}
	else
      printf(" %3d      %-20s %6d\n",base[i].number,base[i].name,base[i].price2);
}
if(count==M)
{
	printf("仓库现在没货，请稍后再来。麻烦您通知采购部门及时补货。。。谢谢。。。\n");
	return 0;
}
printf("请输入产品编号和数量,若想结束,在订购数量上输入 0 \n");
printf("产品编号\n");
scanf("%d",&k);
while((k>=M)||(base[k].number==-1)){
	printf("此货物不存在，请重新输入\n");
	scanf("%d",&k);}
printf("数量\n");
scanf("%d",&m);
while(m>0)
{
	if(base[k].amount<m)
	{
	odl[PP].L[k].amount=base[k].amount;
    base[k].amount=0;
	printf("存货不足,缺 %d 个,先卖给您%d个,剩下的请您稍后重新注册服务编号再预定!同时麻烦您通知采购部门及时补货。。。谢谢。。。\n",odl[PP].L[k].amount-base[k].amount,base[k].amount);
	lackl[k].amount+=(m-base[k].amount);
	}
	else{
		odl[PP].L[k].amount=m;
		base[k].amount-=m;
	}
	goodssum-=odl[PP].L[k].amount;
	suml[k].price1+=(odl[PP].L[k].amount*(base[k].price2-base[k].price1));
	suml[M].price1+=suml[k].price1;
	suml[k].amount+=odl[PP].L[k].amount;
    printf("产品编号\n");
    scanf("%d",&k);
	while((k>=M)||(base[k].number==-1)){
	printf("此货物不存在，请重新输入\n");
	scanf("%d",&k);}
    printf("订购数量\n");
    scanf("%d",&m);
}
khl[n].count=PP;
PP++;
return 0;
}

int manager()                    //经理函数定义
{

    int i=0;
    for(i=0;i<M;i++)
	{
     if(suml[i].amount==0)
		 i++;
	 else{
              printf("................................................................................\n");
			  printf("货物编号  货物名称           该货物利润\n");
	          printf(" %3d      %-20s %6d          \n",suml[i].number,base[i].name,suml[i].price1);
			  printf("................................................................................\n");

	 }
	   
	}
    printf("\n--------------------总货量为  %d  个-----------------------------\n",goodssum);
	printf("\n--------------------总利润为  %d  美元-----------------------------\n",suml[M].price1);
	return 0;
}
/*导入库文件*/
#pragma comment( lib, "ws2_32.lib" )
/*加载头文件*/
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* 【自学去】网站收集 http://www.zixue7.com */
/*定义常量*/
/*表示要记录路由*/
#define IP_RECORD_ROUTE  0x7
/*默认数据报大小*/
#define DEF_PACKET_SIZE  32 
/*最大的ICMP数据报大小*/
#define MAX_PACKET       1024    
/*最大IP头长度*/
#define MAX_IP_HDR_SIZE  60 
/*ICMP报文类型，回显请求*/      
#define ICMP_ECHO        8
/*ICMP报文类型，回显应答*/ 
#define ICMP_ECHOREPLY   0
/*最小的ICMP数据报大小*/
#define ICMP_MIN         8

/*自定义函数原型*/
void InitPing();
void UserHelp();

void GetArgments(int argc, char** argv); 
USHORT CheckSum(USHORT *buffer, int size);
void FillICMPData(char *icmp_data, int datasize);
void FreeRes();

void DecodeIPOptions(char *buf, int bytes);
void DecodeICMPHeader(char *buf, int bytes, SOCKADDR_IN* from);

void PingTest(int timeout);



/*IP报头字段数据结构*/
typedef struct _iphdr 
{
    unsigned int   h_len:4;        /*IP报头长度*/
    unsigned int   version:4;      /*IP的版本号*/
    unsigned char  tos;            /*服务的类型*/
    unsigned short total_len;      /*数据报总长度*/
    unsigned short ident;          /*惟一的标识符*/
    unsigned short frag_flags;     /*分段标志*/
    unsigned char  ttl;            /*生存期*/
    unsigned char  proto;          /*协议类型(TCP、UDP等)*/
    unsigned short checksum;       /*校验和*/
    unsigned int   sourceIP;       /*源IP地址*/
    unsigned int   destIP;         /*目的IP地址*/
} IpHeader;

/*ICMP报头字段数据结构*/
typedef struct _icmphdr 
{
    BYTE   i_type;				   /*ICMP报文类型*/
    BYTE   i_code;                 /*该类型中的代码号*/
    USHORT i_cksum;                /*校验和*/ 
    USHORT i_id;                   /*惟一的标识符*/
    USHORT i_seq;                  /*序列号*/
    ULONG  timestamp;              /*时间戳*/  
} IcmpHeader;

/*IP选项头字段数据结构*/
typedef struct _ipoptionhdr
{
   
	unsigned char  code;           /*选项类型*/
    unsigned char  len;            /*选项头长度*/
    unsigned char  ptr;            /*地址偏移长度*/
	unsigned long  addr[9];        /*记录的IP地址列表*/
} IpOptionHeader;

/*定义全局变量*/
SOCKET m_socket;
IpOptionHeader IpOption;
SOCKADDR_IN DestAddr;
SOCKADDR_IN SourceAddr;
char *icmp_data;
char *recvbuf;
USHORT seq_no ;
char *lpdest;
int datasize;
BOOL RecordFlag;
double PacketNum;
BOOL SucessFlag;


/*初始化变量函数*/   
void InitPing()
{
  WSADATA wsaData;
  icmp_data = NULL;
  seq_no = 0;
  recvbuf = NULL;
  RecordFlag = FALSE;
  lpdest = NULL;
  datasize = DEF_PACKET_SIZE;
  PacketNum = 5;
  SucessFlag = FALSE;

  /*Winsock初始化*/
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        /*如果初始化不成功则报错，GetLastError()返回发生的错误信息*/
		printf("WSAStartup() failed: %d\n", GetLastError());
        return ;
    }
  m_socket = INVALID_SOCKET;
}

/*显示信息函数*/
void UserHelp()
{
	printf("UserHelp: ping -r <host> [data size]\n");
    printf("          -r           record route\n");
	printf("          -n           record amount\n");
    printf("          host         remote machine to ping\n");
    printf("          datasize     can be up to 1KB\n");
		
    ExitProcess(-1); 
}


/*获取ping选项函数*/
void GetArgments(int argc,char** argv)
{
   int i;
   int j;
   int exp;
   int len;
   int m;
   /*如果没有指定目的地地址和任何选项*/
   if(argc == 1)
	{
		printf("\nPlease specify the destination IP address and the ping option as follow!\n");
	    UserHelp();
	}

	for(i = 1; i < argc; i++)
    {
        len = strlen(argv[i]);
		if (argv[i][0] == '-')
        {
            /*选项指示要获取记录的条数*/
			if(isdigit(argv[i][1]))
			{
				PacketNum = 0;
				for(j=len-1,exp=0;j>=1;j--,exp++)
					/*根据argv[i][j]中的ASCII值计算要获取的记录条数(十进制数)*/
					PacketNum += ((double)(argv[i][j]-48))*pow(10,exp);
			}
			else
			{
				switch (tolower(argv[i][1]))
				{
					/*选项指示要获取路由信息*/
					case 'r':        
						RecordFlag = TRUE;
						break;
					/*没有按要求提供选项*/
					default:
						UserHelp();
						break;
				}
			}
        }
		/*参数是数据报大小或者IP地址*/
        else if (isdigit(argv[i][0]))
		{
			for(m=1;m<len;m++)
			{
				if(!(isdigit(argv[i][m])))
				{
					/*是IP地址*/
					lpdest = argv[i];
					break;
				}
				/*是数据报大小*/
				else if(m==len-1)
					datasize = atoi(argv[i]);				 
			}
		}
		/*参数是主机名*/
        else
            lpdest = argv[i];
    }
}

/*求校验和函数*/
USHORT CheckSum(USHORT *buffer, int size)
{
	unsigned long cksum=0;
    while (size > 1) 
    {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if (size) 
    {
        cksum += *(UCHAR*)buffer;
    }
	/*对每个16bit进行二进制反码求和*/
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >>16);
    return (USHORT)(~cksum);
}


/*填充ICMP数据报字段函数*/
void FillICMPData(char *icmp_data, int datasize)
{
    IcmpHeader *icmp_hdr = NULL;
    char      *datapart = NULL;

    icmp_hdr = (IcmpHeader*)icmp_data;
	/*ICMP报文类型设置为回显请求*/
    icmp_hdr->i_type = ICMP_ECHO;        
    icmp_hdr->i_code = 0;
	/*获取当前进程IP作为标识符*/
    icmp_hdr->i_id = (USHORT)GetCurrentProcessId();
    icmp_hdr->i_cksum = 0;
    icmp_hdr->i_seq = 0;
	datapart = icmp_data + sizeof(IcmpHeader);
	/*以数字0填充剩余空间*/
	memset(datapart,'0',datasize-sizeof(IcmpHeader));
}

/*释放资源函数*/
void FreeRes()
{
	/*关闭创建的套接字*/
	if (m_socket != INVALID_SOCKET) 
        closesocket(m_socket);
	/*释放分配的内存*/
    HeapFree(GetProcessHeap(), 0, recvbuf);
    HeapFree(GetProcessHeap(), 0, icmp_data);
	/*注销WSAStartup()调用*/
    WSACleanup();
    return ;
}


/*解读IP选项头函数*/
void DecodeIPOptions(char *buf, int bytes)
{
	IpOptionHeader *ipopt = NULL;
    IN_ADDR inaddr;
    int i;
    HOSTENT *host = NULL;
	/*获取路由信息的地址入口*/
    ipopt = (IpOptionHeader *)(buf + 20);

    printf("RR:   ");
    for(i = 0; i < (ipopt->ptr / 4) - 1; i++)
    {
        inaddr.S_un.S_addr = ipopt->addr[i];
        if (i != 0)
            printf("      ");
		/*根据IP地址获取主机名*/
        host = gethostbyaddr((char *)&inaddr.S_un.S_addr,sizeof(inaddr.S_un.S_addr), AF_INET);
        /*如果获取到了主机名，则输出主机名*/
		if (host)
            printf("(%-15s) %s\n", inet_ntoa(inaddr), host->h_name);
        /*否则输出IP地址*/
		else
            printf("(%-15s)\n", inet_ntoa(inaddr));
    }
    return;
}

/*解读ICMP报头函数*/
void DecodeICMPHeader(char *buf, int bytes, SOCKADDR_IN *from)
{
	IpHeader *iphdr = NULL;
    IcmpHeader *icmphdr = NULL;
    unsigned short iphdrlen;
    DWORD tick;
    static int icmpcount = 0;

    iphdr = (IpHeader *)buf;
    /*计算IP报头的长度*/
    iphdrlen = iphdr->h_len * 4;
    tick = GetTickCount();
	
	/*如果IP报头的长度为最大长度(基本长度是20字节)，则认为有IP选项，需要解读IP选项*/
    if ((iphdrlen == MAX_IP_HDR_SIZE) && (!icmpcount))
		/*解读IP选项，即路由信息*/
        DecodeIPOptions(buf, bytes);

	/*如果读取的数据太小*/
    if (bytes < iphdrlen + ICMP_MIN) 
    {
        printf("Too few bytes from %s\n", 
            inet_ntoa(from->sin_addr));
    }
    icmphdr = (IcmpHeader*)(buf + iphdrlen);

	/*如果收到的不是回显应答报文则报错*/
    if (icmphdr->i_type != ICMP_ECHOREPLY) 
    {
        printf("nonecho type %d recvd\n", icmphdr->i_type);
        return;
    }
    /*核实收到的ID号和发送的是否一致*/
    if (icmphdr->i_id != (USHORT)GetCurrentProcessId()) 
    {
        printf("someone else's packet!\n");
        return ;
    }
	SucessFlag = TRUE;
	/*输出记录信息*/
    printf("%d bytes from %s:", bytes, inet_ntoa(from->sin_addr));
    printf(" icmp_seq = %d. ", icmphdr->i_seq);
    printf(" time: %d ms", tick - icmphdr->timestamp);
    printf("\n");

    icmpcount++;
    return;
}

/*ping函数*/
void PingTest(int timeout)
{   
	int ret;
	int readNum;
	int fromlen;
	struct hostent *hp = NULL;
	/*创建原始套接字，该套接字用于ICMP协议*/
	m_socket = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0,WSA_FLAG_OVERLAPPED);
	/*如果套接字创建不成功*/
    if (m_socket == INVALID_SOCKET) 
    {
        printf("WSASocket() failed: %d\n", WSAGetLastError());
        return ;
    }

	/*若要求记录路由选项*/
    if (RecordFlag)
    {
        /*IP选项每个字段用0初始化*/
        ZeroMemory(&IpOption, sizeof(IpOption));
		/*为每个ICMP包设置路由选项*/
        IpOption.code = IP_RECORD_ROUTE; 
        IpOption.ptr  = 4;               
        IpOption.len  = 39;              
  
        ret = setsockopt(m_socket, IPPROTO_IP, IP_OPTIONS,(char *)&IpOption, sizeof(IpOption));
        if (ret == SOCKET_ERROR)
        {
            printf("setsockopt(IP_OPTIONS) failed: %d\n",WSAGetLastError());
        }
    }

    /*设置接收的超时值*/
    readNum = setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO,(char*)&timeout, sizeof(timeout));
    if(readNum == SOCKET_ERROR) 
    {
        printf("setsockopt(SO_RCVTIMEO) failed: %d\n",WSAGetLastError());
        return ;
    }
	/*设置发送的超时值*/
    timeout = 1000;
    readNum = setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO,(char*)&timeout, sizeof(timeout));
    if (readNum == SOCKET_ERROR) 
    {
        printf("setsockopt(SO_SNDTIMEO) failed: %d\n",WSAGetLastError());
        return ;
    }

	/*用0初始化目的地地址*/
    memset(&DestAddr, 0, sizeof(DestAddr));
	/*设置地址族，这里表示使用IP地址族*/
	DestAddr.sin_family = AF_INET;
    if ((DestAddr.sin_addr.s_addr = inet_addr(lpdest)) == INADDR_NONE)
    {   
        	
        /*名字解析，根据主机名获取IP地址*/
		if ((hp = gethostbyname(lpdest)) != NULL)
        {
            /*将获取到的IP值赋给目的地地址中的相应字段*/
			memcpy(&(DestAddr.sin_addr), hp->h_addr, hp->h_length);
			/*将获取到的地址族值赋给目的地地址中的相应字段*/
            DestAddr.sin_family = hp->h_addrtype;
            printf("DestAddr.sin_addr = %s\n", inet_ntoa(DestAddr.sin_addr));
        }
        /*获取不成功*/
		else
        {
            printf("gethostbyname() failed: %d\n",WSAGetLastError());
            return ;
        }
    }        

	/*数据报文大小需要包含ICMP报头*/
    datasize += sizeof(IcmpHeader);  
	/*根据默认堆句柄，从堆中分配MAX_PACKET内存块，新分配内存的内容将被初始化为0*/
    icmp_data =(char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,MAX_PACKET);
    recvbuf =(char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,MAX_PACKET);

	/*如果分配内存不成功*/
    if (!icmp_data) 
    {
        printf("HeapAlloc() failed: %d\n", GetLastError());
        return ;
    }
	/* 创建ICMP报文*/
    memset(icmp_data,0,MAX_PACKET);
    FillICMPData(icmp_data,datasize);

    
    while(1) 
    {
        static int nCount = 0;
        int writeNum;
        /*超过指定的记录条数则退出*/        
        if (nCount++ == PacketNum) 
            break;

         /*计算校验和前要把校验和字段设置为0*/       
        ((IcmpHeader*)icmp_data)->i_cksum = 0;
		/*获取操作系统启动到现在所经过的毫秒数，设置时间戳*/
        ((IcmpHeader*)icmp_data)->timestamp = GetTickCount();
		/*设置序列号*/
        ((IcmpHeader*)icmp_data)->i_seq = seq_no++;
		/*计算校验和*/
        ((IcmpHeader*)icmp_data)->i_cksum = CheckSum((USHORT*)icmp_data, datasize);
		/*开始发送ICMP请求 */
        writeNum = sendto(m_socket, icmp_data, datasize, 0,(struct sockaddr*)&DestAddr, sizeof(DestAddr));
        
		/*如果发送不成功*/
		if (writeNum == SOCKET_ERROR)
        {
            /*如果是由于超时不成功*/
			if (WSAGetLastError() == WSAETIMEDOUT) 
            {
                printf("timed out\n");
				continue;
            }
			/*其他发送不成功原因*/
            printf("sendto() failed: %d\n", WSAGetLastError());
            return ;
        }

		/*开始接收ICMP应答 */
        fromlen = sizeof(SourceAddr);
        readNum = recvfrom(m_socket, recvbuf, MAX_PACKET, 0,(struct sockaddr*)&SourceAddr, &fromlen);
        /*如果接收不成功*/
		if (readNum == SOCKET_ERROR)
        {
            /*如果是由于超时不成功*/
			if (WSAGetLastError() == WSAETIMEDOUT) 
            {
                printf("timed out\n");
				continue;
            }
			/*其他接收不成功原因*/
            printf("recvfrom() failed: %d\n", WSAGetLastError());
            return ;
        }
		/*解读接收到的ICMP数据报*/
        DecodeICMPHeader(recvbuf, readNum, &SourceAddr);
    }
}


int main(int argc, char* argv[])
{   
    InitPing();  
	GetArgments(argc, argv); 
		
    PingTest(1000);
	
	/*延迟1秒*/
    Sleep(1000);

	if(SucessFlag)
		printf("\nPing end, you have got %.0f records!\n",PacketNum);
	 
	else
		printf("Ping end, no record!");	
    FreeRes();
    getchar();
    getch();
}

#include<conio.h>
#include<windows.h>
#include<stdio.h>
#include<dos.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

/* 【自学去】网站收集 http://www.zixue7.com */

struct player
{
	char name[10];
	int power,speed,wisdom;
	int att,def;
	int HPmax,HP;
	float burst,dodge;
	int EXP,EXPmax,LV,spot;
}player1;
struct monster
{
	char name[10];
	int HP;
	int att,def;
	int	EXP;
	float burst,dodge;
}monster1;
main()
{
	int i=1,j=0,m=28,n=12;
	char c1;
	void play1();
	int bug1(int sli);
	void help();
	void HideCursor();
	void gotoxy(int x,int y);
	void start();
	void wait();
    void initp1();
	void initm1();
	void boundary();
	void play();
	void addspot();
	void sure();
	void p1order();
	void p1die();
S:	m=28,n=12;
	start();
	system("color 2B");
	HideCursor();
	gotoxy(m,n);
    while(1)//{gotoxy(28,12);}
	{
		
		c1=getch();
		switch(c1)
		{
		case'w':
		case'W':
			printf("  ");
			n-=3;
			if(n<12)n=18;
			gotoxy(m,n);
			printf("★");
			gotoxy(m-1,n);
			break;
		case's':
		case'S':
   			printf("  ");
			n+=3;
			if(n>18)n=12;
			gotoxy(m,n);
			printf("★");
			gotoxy(m-1,n);
			break;
		case'j':
		case'J':
			if(18==n)
				exit(0);
			if(15==n)
			{
				system("cls");
			    help();
				while(getch()){system("cls");goto S;}
			}
			if(12==n)
			{	
				system("cls");
			    wait();
				initp1();
				initm1();
				while(1)
				{
					printf("请输入你的名字:(请少于10个字符)\n");
					sprintf(player1.name,"%s","\0");
		            gets(player1.name);
		            if(strlen(player1.name)<=9)break;
				}
				system("cls");
                boundary();
				while(1){
				initm1();
				play();
				Sleep(1000);
				addspot();
				sure();
             	Sleep(1000);
				p1order();}

			}
			break;
		}
	}
			


    



}
void gotoxy(int x,int y)       //将光标移动到坐标为(x,y)的地方
{
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;                            
    HANDLE hConsoleOut;
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);
    csbiInfo.dwCursorPosition.X = x;                                    
    csbiInfo.dwCursorPosition.Y = y;                                    
    SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);   
}

void HideCursor()//隐藏光标
{
CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

int bug1(int sli)
{
    if(sli<0)sli=0;
	return sli;
}

void play1()
{
	int t=0,i=1;
	int LV=1,EXPmax=10,EXP=0,HPmax=30,HP=30,power=0,speed=0,wisdom=0,att=10,def=5,spot=0;
	float dodge=5,burst=5;
	int AI1_HP,AI1_att,AI1_def,AI1_exp;
    int AI2_HP,AI2_att,AI2_def,AI2_exp;
	char yc1,yc2;
	char enter1,enter2;
	char name[10];
	srand(time(NULL));
	power=rand()%8+7;
	speed=rand()%8+7;
	wisdom=rand()%8+7;
	spot=45-power-speed-wisdom;
NO3:printf("你的名字是？\n");
	scanf(" %s",name);
NO2:while(1)
	{
    printf("请分配你的属性点。（a.力量b.速度c.智力）（最大HP%d,力量%d,速度%d,智力%d,攻击%d,防御%d,闪避率%f,暴击率%f,剩余%d点）\n",HPmax=30+power*2,power,speed,wisdom,att=10+power*1.5,def=5+wisdom*1.5,dodge=5+speed*0.4,burst=5+speed*0.4,spot);
	scanf(" %c",&yc1);
	switch(yc1)
	{
	case'a':power+=1,spot-=1;break;
	case'b':speed+=1,spot-=1;break;
	case'c':wisdom+=1,spot-=1;break;
	default:printf("操作错误，请重新输入。\n");
	}
    if(spot==0)break;
	}
	printf("昵称：%s\n状态：LV：%d\nEXP：%d/%d\n最大HP：%d\nHP：%d\n攻击力：%d\n防御力：%d\n闪避率：%f\n暴击率：%f\n",name,LV,EXP,EXPmax,HPmax,HP=HPmax,att,def,dodge,burst);
NO1:printf("按y继续\n");
	while(1)
	{
	scanf(" %c",&enter1);
	if(enter1!='y')printf("按y继续\n");
    if(enter1=='y')break;
	}
    AI1_HP=rand()%20+30;
    AI1_att=rand()%15+25;
    AI1_def=50-AI1_att;
	AI1_exp=15;
    printf("遇到怪物，准备进入战斗。\n怪物信息：\nHP：%d\n攻击：%d\n防御：%d\n",AI1_HP,AI1_att,AI1_def);
	while(1)
	{
	if(HP<=0)break;
	printf("a.攻击b.逃跑\n");
    scanf(" %c",&yc2);
	switch(yc2)
	{
	case'a':if(rand()%100+1<=5)printf("%s攻击，miss!\n",name);
		    else if((rand()%100+1)*10<=burst*10)
				printf("%s攻击，暴击，怪物损失%dHP,怪物剩余%dHP\n",name,att,bug1(AI1_HP-=att));
			else
				printf("%s攻击，怪物损失%dHP,怪物剩余%dHP\n",name,bug1(att-AI1_def),bug1(AI1_HP-=bug1(att-AI1_def)));
            if(AI1_HP<=0)break;
            if((rand()%100+1)*10<=dodge*10)printf("怪物攻击，miss!\n");
		    else if(rand()%100+1<=5)
				printf("怪物攻击，暴击，%s损失%dHP,%s剩余%dHP\n",name,AI1_att,name,bug1(HP-=AI1_att));
			else 
				printf("怪物攻击，%s损失%dHP,%s剩余%dHP\n",name,bug1(AI1_att-def),name,bug1(HP-=bug1(AI1_att-def)));break;
	case'b':if(rand()%2==0)goto NO1;
		    else if((rand()%100+1)*10<=dodge*10)printf("逃跑失败，怪物攻击，miss!\n");
			else if(rand()%100+1<=5)
				printf("逃跑失败，怪物攻击，暴击，%s损失%dHP,%s剩余%dHP\n",name,AI1_att,name,bug1(HP-=AI1_att));
			else 
				printf("逃跑失败，怪物攻击，%s损失%dHP,%s剩余%dHP\n",name,bug1(AI1_att-def),name,bug1(HP-=bug1(AI1_att-def)));break;
	default:printf("操作错误，请重新输入。\n");
	}
	if(AI1_HP<=0)break;
	}
    if(HP<=0)goto END;
    EXP+=AI1_exp;
	if(EXP>=EXPmax)LV+=1,spot+=5,EXP-=EXPmax,EXPmax+=20*(LV-1),printf("打败怪物，%s升级了。\n",name);
	else printf("打败怪物，输入y继续遇怪,输入q查看属性。\n");
    if(spot!=0)goto NO2;
    while(1)
	{
	scanf(" %c",&enter2);
	if(enter2!='y')printf("按y继续\n");
    if(enter2=='q')printf("昵称：%s\n状态：最大HP：%d\nHP：%d\n攻击力：%d\n防御力：%d\n闪避率：%f\n暴击率：%f\n",name,HPmax,HP=HPmax,att,def,dodge,burst);
	if(enter2=='y')goto NO1;
	}
END:printf("角色死亡，游戏结束。\n");



}

void wait()
{
    int i=0,m=4,n=3;
	void HideCursor();
	void gotoxy(int x,int y);
	printf("╔══════════════════════════════════════╗");
    printf("║                                                                            ║");
    printf("║  ╱▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏▔◥║");
    printf("║  ▏                                                                  ▏%%  ║");
    printf("║  ╲▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏▁◢║");
    printf("║                                                                            ║");
    printf("║                                                                            ║");
    printf("║                                                                            ║");
    printf("║                                                                            ║");
    printf("║            ┐╭─┼─┐　　　┌┬┌┐┐┐　　　┌─╮┬─╮                ║");
    printf("║            ┐　─┼─┐　　　┌┼┌┴┴┐　　　│└╮└└╮                ║");
    printf("║            │┌─┴──　　　╭┤│　　│　　　　──┼─　                ║");
    printf("║            │　├──┤　　　│├├──┤　　　┌──┴─┐                ║");
    printf("║            │　├──┤　　　││├──┤　　　┌─┬─┼┐                ║");
    printf("║            └╰┘　　┘      └└└　　╯      　└──╯　                ║");
    printf("║                                                                            ║");
    printf("║                                                                            ║");
    printf("║                                                                            ║");
    printf("║                                                                            ║");
    printf("║                                                                            ║");
    printf("║                                                                            ║");
    printf("║                                                                            ║");
    printf("║                                                                            ║");
    printf("╚══════════════════════════════════════╝");
    while(1)
	{

		  Sleep(50);
		  gotoxy(m+=2,n);
		  printf("■");
		  gotoxy(76,n);
		  printf("%2d",i+=3);
		  if(i==99)
		  {
			  Sleep(2000);
			  break;
		  }
	}
	system("cls");
}

void start()
{
    printf("╔══════════════════════════════════════╗");
	printf("║Beta0.1                                                                     ║");
	printf("║                                                                            ║");
	printf("║    　╗═╩═╗　╭╦═　╦╗　　　　╗　　　╔════╗　╔╗╭══╗    ║");
	printf("║    ╔╠╔═╗　　╔╩══╯╗　╚══╠═╝　║════║　║║║　　║    ║");
	printf("║    　║║　║　　　╯╦══╗　　　　║　　　╔═══╗║　║╯╝══╚    ║");
	printf("║    　║║　║╗　╚════╯　　　　║　　　╠═══╣║　║║╔╔　╗    ║");
	printf("║    　║║　║║　╚╯╦══╗　　　　║═╮　╠═══╣║　╠╯╰╰╔╯    ║");
	printf("║    ╚╩╯　╚╯  ╚════╯  ╚══╯　╝  ╚═══╯╰  ╚　══╩╝    ║");
	printf("║                                                                            ║");
	printf("║                                                                            ║");
	printf("║                              ┏━━━━┓                                  ║");
	printf("║                          ★  ┃开    始┃                                  ║");
	printf("║                              ┗━━━━┛                                  ║");
	printf("║                              ┏━━━━┓                                  ║");
	printf("║                              ┃帮    助┃                                  ║");
	printf("║                              ┗━━━━┛                                  ║");
	printf("║                              ┏━━━━┓                                  ║");
	printf("║                              ┃退    出┃                                  ║");
	printf("║                              ┗━━━━┛                                  ║");
	printf("║                                                                            ║");
	printf("║                                                             作者QQ993107057║");
	printf("║                                                             百度ID:佳砾三千║");
	printf("╚══════════════════════════════════════╝");
}

void initp1()
{
    player1.power=player1.speed=player1.wisdom=0;
	player1.att=10+player1.power*1.5;
    player1.def=5+player1.wisdom*1.5;
    player1.HPmax=30+player1.power*2;
	player1.HP=player1.HPmax;
    player1.burst=player1.dodge=5+player1.speed*0.4;
	player1.LV=1;
	player1.EXPmax=15+20*player1.LV*(player1.LV-1);
	player1.EXP=0;
	player1.spot=30;
}

void initm1()
{
	srand(time(0));
	sprintf(monster1.name,"%s","老鼠");
    monster1.HP=rand()%20+30;
    monster1.att=rand()%15+25;
    monster1.def=50-monster1.att;
    monster1.EXP=15;
	monster1.burst=monster1.dodge=5;
}

void play()
{
	int bug1(int x);
	player1.att=10+player1.power*1.5;
	player1.def=5+player1.wisdom*1.5;
	player1.HPmax=30+player1.power*2;
	player1.burst=player1.dodge=5+player1.speed*0.4;
	gotoxy(11,2);
	printf("%s",player1.name);
	gotoxy(11,3);
	printf("%3d/%3d",bug1(player1.HP),player1.HPmax);
	gotoxy(11,4);
	printf("%3d",player1.att);
	gotoxy(11,5);
	printf("%3d",player1.def);
	gotoxy(11,6);
	printf("%2.1f",player1.burst);
	gotoxy(11,7);
	printf("%2.1f",player1.dodge);
	gotoxy(11,8);
    printf("%3d",player1.power);
	gotoxy(11,9);
	printf("%3d",player1.speed);
	gotoxy(11,10);
	printf("%3d",player1.wisdom);
	gotoxy(11,11);
	printf("%3d/%3d",player1.EXP,player1.EXPmax);
	gotoxy(11,12);
	printf("%2d",player1.LV);
	gotoxy(35,5);
	printf("%2d",player1.spot);
	gotoxy(67,2);
	printf("%s",monster1.name);
	gotoxy(67,3);
	printf("%3d",bug1(monster1.HP));
	gotoxy(67,4);
	printf("%3d",monster1.att);
    gotoxy(67,5);
	printf("%3d",monster1.def);
	gotoxy(67,6);
	printf("%2.1f",monster1.burst);
	gotoxy(67,7);
	printf("%2.1f",monster1.dodge);
	gotoxy(67,8);
	printf("%3d",monster1.EXP);
}

void boundary()
{
	printf("╔══════════════════════════════════════╗");
	printf("║┌────────┐┌────────┐                ┌────────┐║");
	printf("║│  name:         ││          ┼力量│╭╮╭╮╭══╮│  name:         │║");
	printf("║│    HP:         ││          ┼速度│║║║║║╭═╯│    HP:         │║");
	printf("║│攻击力:         ││          ┼智力│║║║║║╰═╮│攻击力:         │║");
	printf("║│防御力:         ││剩余加点数:     │║║║║╰═╮║│防御力:         │║");
	printf("║│暴击率:         ││▔▔▔▔▔▔▔▔│╰╮╭╯╭═╯║│暴击率:         │║");
	printf("║│闪避率:         ││            攻击│  ╰╯　╰══╯│闪避率:         │║");
	printf("║│  力量:         ││            逃跑│                │   EXP:         │║");
	printf("║│  速度:         ││  ╭︿︿︿╮    │                │      ____()()  │║");
	printf("║│  智力:         ││  {/-●●-/}    │                │     /      @@  │║");
	printf("║│   EXP:         ││   ( (oo) )     │                │~~~~~\\_;m__m._>o│║");
	printf("║│    LV:         ││    ︶︶︶      │                │                │║");
	printf("║━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━║");
	printf("║                                                                            ║");
	printf("║                                                                            ║");
	printf("║                                                                            ║");
	printf("║                                                                            ║");
	printf("║                                                                            ║");
	printf("║                                                                            ║");
	printf("║                                                                            ║");
	printf("║                                                                            ║");
	printf("║                                                                            ║");
	printf("╚══════════════════════════════════════╝");
}


void addspot()
{        
        char c2;
	    int m=28,n=2;
		if(player1.spot>0){
	    gotoxy(2,14);
        printf("                                                   ");
		gotoxy(2,14);
		printf("请加点!");
	    gotoxy(m,n);
		printf("☆");
		gotoxy(27,2);
		while(1)
		{
			c2=getch();
			switch(c2)
			{
		case'w':
		case'W':
			printf("  ");
			n-=1;
			if(n<2)n=4;
			gotoxy(m,n);
			printf("☆");
			gotoxy(m-1,n);
			break;
		case's':
		case'S':
   			printf("  ");
			n+=1;
			if(n>4)n=2;
			gotoxy(m,n);
			printf("☆");
			gotoxy(m-1,n);
			break;
		case'j':
		case'J':
			if(n==2)player1.power+=1,player1.spot-=1;
		    if(n==3)player1.speed+=1,player1.spot-=1;
            if(n==4)player1.wisdom+=1,player1.spot-=1;
			play();
			gotoxy(m,n);
			break;
			}
			if(player1.spot==0)
			{
				gotoxy(27,2);
					printf("  ");
				gotoxy(27,3);
				printf("  ");
				gotoxy(27,4);
				printf("  ");
					break;}}
		}
}

void sure()
{
    char c3;
	int m,n;
A:  m=45,n=9;
	gotoxy(42,8);
	printf("╔══════╗");
	gotoxy(42,9);
	printf("║            ║");
	gotoxy(42,10);
	printf("║            ║");
	gotoxy(42,11);
    printf("║            ║");
	gotoxy(42,12);
    printf("╚══════╝");
	gotoxy(51,9);
	printf("继续");
	gotoxy(51,11);
	printf("退出");
	gotoxy(45,9);
	printf("★");
	gotoxy(45,9);
	while(1)
	{
		c3=getch();
		switch(c3)
		{
		case'w':
		case'W':
			printf("  ");
			n-=2;
			if(n<9)n=11;
			gotoxy(m,n);
			printf("★");
			gotoxy(m-1,n);
			break;
		case's':
		case'S':
   			printf("  ");
			n+=2;
			if(n>11)n=9;
			gotoxy(m,n);
			printf("★");
			gotoxy(m-1,n);
			break;
		case'j':
		case'J':
			if(n==9)
			{
	gotoxy(42,8);
	printf("                ");
	gotoxy(42,9);
	printf("                ");
	gotoxy(42,10);
	printf("                ");
	gotoxy(42,11);
    printf("                ");
	gotoxy(42,12);
    printf("                ");
	goto B;
			}
			if(n==11)
			{
    gotoxy(42,8);
	printf("╔══════╗");
	gotoxy(42,9);
	printf("║            ║");
	gotoxy(42,10);
	printf("║            ║");
	gotoxy(42,11);
    printf("║            ║");
	gotoxy(42,12);
    printf("╚══════╝");
	gotoxy(46,9);
	printf("否   是");
	gotoxy(46,10);
	printf("★");
	gotoxy(46,10);
	m=46,n=10;
	while(1)
	{
	    c3=getch();
		switch(c3)
		{
		case'a':
		case'A':
			printf("  ");
			m-=5;
			if(m<46)m=51;
			gotoxy(m,n);
			printf("★");
			gotoxy(m-1,n);
			break;
		case'd':
		case'D':
   			printf("  ");
			m+=5;
			if(m>51)m=46;
			gotoxy(m,n);
			printf("★");
			gotoxy(m-1,n);
			break;
		case'j':
		case'J':
			if(m==51)exit(0);
			if(m==46)goto A;
		}
	}
			}
		}
	}
B:gotoxy(28,7);
}

void p1die()
{
printf("╔══════════════════════════════════════╗");
printf("║                                                                            ║");
printf("║                           .-\"      \"-.                                     ║");	
printf("║                          /            \\                                    ║");
printf("║                         |      ★      |                                   ║");
printf("║                         |,  .-.  .-.  ,|                                   ║");
printf("║                         | )(__/  \\__)( |                                   ║");
printf("║                         |/     /\\     \\|                                   ║");
printf("║               (@_       (_     ^^     _)                                   ║");
printf("║          _     ) \\_______\\__|IIIIII|__/__________________________          ║");
printf("║         (_)@8@8{}<________|-\\IIIIII/-|___________________________>         ║");
printf("║                )_/        \\          /                                     ║");
printf("║               (@           `--------`                                      ║");
printf("║                                                                            ║");
printf("║                                                                            ║");
printf("║    ┣━━━━┓　┣━━━━┓　┏┳━━┳┓　　━━╮　　                  ║");
printf("║    ┏━━┳━┓　╯━━┳━┓　　┣━┓┃　　┳━━┻━╯                  ║");
printf("║    ┃　　┃　┃　┃　　┃　┃　┏╯　┃┃　　┃　　　　　                  ║");
printf("║    ┣━━┣━┫　┃　　┃　┃　　╰━┫┣┛　┃　　　　　                  ║");
printf("║    ┣━━┣━┫　┣━━┻━╯　　　　┃┃　　┃　　　　┃                  ║");
printf("║    ╯　　┛┗╯  ╰━━━━╯  ┗━━╯┗╯  ┗━━━━╯                  ║");
printf("║                                                                            ║");
printf("║                                                                            ║");
printf("╚══════════════════════════════════════╝");
Sleep(5000);
system("cls");
gotoxy(0,0);
exit(0);
}       

void p1order()
{
	char c4;
	int m,n;
	void addspot();
	void p1att();
	void m1att();
	void play();
	int p1run();
	gotoxy(28,7);
	printf("  ");
	gotoxy(28,8);
	printf("  ");
	gotoxy(28,7);
	printf("★");
	gotoxy(28,7);
	m=28,n=7;
	while(1)
	{
		c4=getch();
		switch(c4)
		{
		case'w':
		case'W':
			gotoxy(28,7);
            printf("  ");
	        gotoxy(28,8);
	        printf("  ");
			n-=1;
			if(n<7)n=8;
			gotoxy(m,n);
			printf("★");
			gotoxy(m-1,n);
			break;
		case's':
		case'S':
   				gotoxy(28,7);
	        printf("  ");
	       gotoxy(28,8);
	         printf("  ");
			n+=1;
			if(n>8)n=7;
			gotoxy(m,n);
			printf("★");
			gotoxy(m-1,n);
			break;
		case'j':
		case'J':
			if(n==7){p1att();addspot();if(monster1.HP<=0){sure();initm1();play();}}
			if(n==8)
			{
				p1run();
				if(player1.HP<=0)
			{
				system("cls");
				gotoxy(0,0);
				p1die();
				Sleep(3);
			}
			
			}
			
		}
	}
		

}

void p1att()
{
	void gotoxy();
	void play();
	void sure();
	int bug1(int x);
	srand(time(0));
	gotoxy(2,16);
    printf("                                                   ");
	gotoxy(2,14);
	printf("                                                   ");
    gotoxy(2,14);
	Sleep(1000);
			if(rand()%100+1<=5)printf("%s攻击，miss!\n",player1.name);
		    else if((rand()%100+1)*10<=player1.burst*10)
				printf("%s攻击，暴击，怪物损失%dHP,怪物剩余%dHP\n",player1.name,player1.att,bug1(monster1.HP-=player1.att));
			else
				printf("%s攻击，怪物损失%dHP,怪物剩余%dHP\n",player1.name,bug1(player1.att-monster1.def),bug1(monster1.HP-=bug1(player1.att-monster1.def)));
        play(); 
	gotoxy(2,15);
	printf("                                                   ");
	gotoxy(2,15);

			if(monster1.HP<=0)
			{
				player1.EXP+=monster1.EXP;
				gotoxy(2,16);
				printf("打败怪物，获得%d经验.",monster1.EXP);
				if(player1.EXP>=player1.EXPmax)
				player1.LV+=1,player1.spot+=5,player1.EXP-=player1.EXPmax,player1.EXPmax+=20*(player1.LV-1),player1.HP=player1.HPmax,printf("升级了!");
				play();
				goto NEXT;
			}
           Sleep(1000);
		    gotoxy(2,16);
            if((rand()%100+1)*10<=player1.dodge*10)printf("怪物攻击，miss!\n");
		    else if(rand()%100+1<=5)
				printf("怪物攻击，暴击，%s损失%dHP,%s剩余%dHP\n",player1.name,monster1.att,player1.name,bug1(player1.HP-=monster1.att));
			else 
				printf("怪物攻击，%s损失%dHP,%s剩余%dHP\n",player1.name,bug1(monster1.att-player1.def),player1.name,bug1(player1.HP-=bug1(monster1.att-player1.def)));
			play();
			gotoxy(28,7);
            if(player1.HP<=0)
			{
				system("cls");
				gotoxy(0,0);
				p1die();
				Sleep(3);
			}
    NEXT:;
}			

int p1run()
{

	        if(rand()%2==0)
			{
				gotoxy(2,14);
				printf("                                                   ");
				gotoxy(2,14);
				printf("逃跑成功!");
				sure();
				p1order();
			}
		    else if((rand()%100+1)*10<=player1.dodge*10)
			{
				gotoxy(2,14);
				printf("                                                   ");
				gotoxy(2,14);
				printf("逃跑失败，怪物攻击，miss!\n");
			}
			else if(rand()%100+1<=5)
			{
				gotoxy(2,14);
				printf("                                                   ");
				gotoxy(2,14);
				printf("逃跑失败，怪物攻击，暴击，%s损失%dHP,%s剩余%dHP\n",player1.name,monster1.att,player1.name,bug1(player1.HP-=monster1.att));
				play();
			}
			else 
			{
				gotoxy(2,14);
				printf("                                                   ");
				gotoxy(2,14);
				printf("逃跑失败，怪物攻击，%s损失%dHP,%s剩余%dHP\n",player1.name,bug1(monster1.att-player1.def),player1.name,bug1(player1.HP-=bug1(monster1.att-player1.def)));
				play();
			}
   return player1.HP;
}

void help()
{
printf("                    ★☆★☆★☆★☆★☆★☆★☆★☆★☆              \n");
printf("                    ☆       坑 爹 大 冒 险           ★              \n");
printf("                    ★                                ☆              \n");
printf("                    ☆     帮               助        ★              \n");
printf("                    ★                                ☆              \n");
printf("                    ☆★☆★☆★☆★☆★☆★☆★☆★☆★              \n");
printf("+力量：攻击力+1.5，HPmax+2；+速度：暴击和闪避各+0.4；+智力：防御力+1.5\n");
printf("                                                                      \n");
printf("                          因为这是第一次测试版，                      \n");
printf("         W              所以游戏还不具有坑爹元素。                    \n");
printf("         ↑               而且游戏可玩性还不高，                      \n");
printf("                        主要是因为没想好初始属性。                    \n");
printf("                     ┌──────────────┐    ┏━━┓     \n");
printf("  S←          →D   │感谢百度吧友ID6210001的指导!│    ┃确认┃     \n");
printf("                     └──────────────┘    ┗━━┛     \n");
printf("                       作者也只是个刚上大一的学生，                   \n");
printf("         ↓               刚学C语言，能力有限。             J         \n");
printf("         S                   现在是期末期间，                         \n");
printf("                         所以要做各科的复习突击，                     \n");
printf("                             没时间更新游戏，                         \n");
printf("                                 望见谅。                             \n");
printf("                                                           佳砾三千   \n");
printf("                                                        2012年12月10日\n");
}




#include <graphics.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>

#define LEFT 0x4b00
#define RIGHT 0x4d00
#define UP 0x4800
#define DOWN 0x5000
#define ESC 0x011b
#define ENTER 0x1c0b

/* 【自学去】网站收集 http://www.zixue7.com */

/*2墙壁,1可以移动地方,3自己,4敌人*/
int a[15][20]={2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	       2,1,1,1,1,1,1,0,1,0,0,0,1,0,0,0,0,1,0,2,
               2,1,2,2,2,1,1,2,1,1,0,0,0,1,1,4,1,1,0,2,
               2,1,1,0,2,1,1,2,0,1,1,2,2,2,2,2,0,0,0,2,
               2,4,1,0,2,1,1,2,1,1,1,0,1,1,1,1,0,1,1,2,
               2,1,2,1,2,1,1,2,1,3,2,2,1,1,1,1,2,2,1,2,
               2,1,2,1,2,1,1,1,1,1,1,1,1,0,0,0,1,1,1,2,
               2,1,2,1,0,1,1,1,1,2,1,0,1,2,2,2,1,1,1,2,
               2,1,0,1,0,1,2,1,1,2,1,0,1,2,1,1,4,1,1,2,
               2,1,0,2,0,1,2,1,1,2,1,0,1,2,1,1,1,1,1,2,
               2,1,0,2,1,1,2,1,1,2,1,0,2,2,1,0,0,0,1,2,
               2,1,1,2,1,1,2,1,1,2,1,0,2,1,1,2,2,1,1,2,
               2,1,2,2,1,2,2,1,1,1,1,0,1,4,1,2,0,0,1,2,
               2,1,0,0,0,0,0,4,0,1,1,0,1,1,1,1,0,0,1,2,
               2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};/*数组就是地图*/
struct play /*游戏中人物的结构体*/
{
 int x;
 int y;
};
struct play you,them[5];
int sum=0;/*统计吃的豆子个数,吃满50颗就算胜利*/
int xx[5][2];/*判定敌人方向用的结构体*/
int false=0;

void init();
void begin();
void play();
void win();
void fun(struct play *them);
void movethem(struct play *them);
void loseyes();
void drawblackdou(int x,int y);
void lose();
/*游戏玩法：当任意按下键时，游戏启动，敌人随机移动，自己开始躲避敌人和墙壁，尽可能多的吃豆豆*/
void play()
{
	int key,i;
	while(1)
	{       key=bioskey(2);
		while(!kbhit())				/*当无按键时敌人自己移动 */
		{
			setcolor(GREEN);/*重画敌人*/
			for(i=0;i<5;i++)
				circle(them[i].y*20+100,them[i].x*20+100,9);
				sleep(1);
				fun(them);/*处理敌人*/

			for(i=0;i<5;i++)
				if(them[i].x==you.x&&them[i].y==you.y)
					false=1;/*假如失败的话*/
		loseyes();/*判定是否失败*/
			if(false)
				break;
		}/*跳出while（！kbhit（））*/
	if(false)
		break;
	key=bioskey(0);
	setcolor(0);/*把自己原来位置的人给删除掉*/
	circle(100+you.y*20,100+you.x*20,9);
	if(key==ESC)
		break;
	else if(key==UP)/*这里开始的判定主要是是否吃到豆子和碰到墙壁*/
	{
		you.x--;
		if(a[you.x][you.y]==2)
			you.x++;
		else if(a[you.x][you.y]==0)
			drawblackdou(you.x,you.y);
	}
	else if(key==DOWN)
	{
		you.x++;
		if(a[you.x][you.y]==2)
			you.x--;
		else if(a[you.x][you.y]==0)
			drawblackdou(you.x,you.y);
	}
   else if(key==RIGHT)
    {
	   you.y++;
	   if(a[you.x][you.y]==2)
		   you.y--;
     else if(a[you.x][you.y]==0)
		drawblackdou(you.x,you.y);
   }
   else if(key==LEFT)
    {
	   you.y--;
	   if(a[you.x][you.y]==2)
		   you.y++;
     else if(a[you.x][you.y]==0)
		drawblackdou(you.x,you.y);
   }
   if(sum==50)
      break;
  setcolor(RED);/*执行了一次键盘后再画出自己的位置*/
  circle(100+you.y*20,100+you.x*20,9);
  loseyes();/*自己走上去碰到敌人的可能*/
   if(false)
    break;
 }

}
void fun(struct play *them)/*移动中的判定*/
{
 int i;
 setcolor(0);/*把敌人的老位置删除*/
   for(i=0;i<5;i++)
    circle(them[i].y*20+100,them[i].x*20+100,9);
  movethem(them);/*根据控制者的位置来决定敌人的移动方向*/
}


void lose()/*输的画面*/
{
	cleardevice();
	settextstyle(0,0,4);
	while(!kbhit())
	{
		setcolor(rand()%13+1);
		outtextxy(180,200,"GAME OVER!");
		delay(3);
	}

}
void win()/*赢的画面*/
{
	cleardevice();
	settextstyle(0,0,4);
	while(!kbhit())
	{
		setcolor(rand()%13+1);
		outtextxy(200,200,"YOU WIN!");
		delay(3);
	}

}
void drawblackdou(int x,int y)/*吃豆子*/
{
	setcolor(0);
	circle(100+y*20,100+x*20,3);/*画圆x，y，半径*/
	sum++;/*吃到豆子后就加一*/
	a[x][y]=1;/*吃到后这里就成为普通平地*/
}
void loseyes()/*判断是否失败*/
{
	int i;
	for(i=0;i<5;i++)
    if(them[i].x==you.x&&them[i].y==you.y)
		false=1;/*假如失败的话*/


}
void movethem(struct play *them)/*them的移动过程*/
{
	int i,loop;
	randomize();/*随机排列，任意打乱*/
	for(i=0;i<5;i++)
	{
		/*只要控制者在身边就立即靠上去*/
		if(you.x==them[i].x&&(them[i].y+1)==you.y)
			them[i].y++;
		else if(you.x==them[i].x&&(them[i].y-1)==you.y)
			them[i].y--;
		else if(you.y==them[i].y&&(them[i].x+1)==you.x)
			them[i].x++;
		else if(you.y==them[i].y&&(them[i].x-1)==you.x)
			them[i].x--;
		else
		{
		loop:
			xx[i][0]=rand()%4+1;/*这里的方向采取随机赋值,原则是新的方向不可以和原来的方向相反*/
			if(xx[i][0]==1&&xx[i][1]==2||xx[i][0]==2&&xx[i][1]==1)
				goto loop;
			if(xx[i][0]==3&&xx[i][1]==4||xx[i][0]==4&&xx[i][1]==3)
				goto loop;
			xx[i][1]=xx[i][0];
			if(xx[i][0]==1)/*四个方向*/
			{
				them[i].x--;
				if(a[them[i].x][them[i].y]==2)/*假如碰墙壁的话就回到原来的地方等待随机的方向*/
				{
					them[i].x++;
					goto loop;
				}
			}
			else if(xx[i][0]==2)
			{
				them[i].x++;
				if(a[them[i].x][them[i].y]==2)
				{
					them[i].x--;
					goto loop;
				}
			}
			else if(xx[i][0]==3)
			{
				them[i].y++;
				if(a[them[i].x][them[i].y]==2)
				{
					them[i].y--;
					goto loop;
				}
			}
			else if(xx[i][0]==4)
			{
				them[i].y--;
				if(a[them[i].x][them[i].y]==2)
				{
					them[i].y++;
					goto loop;
				}
			}
		}
	}
}



void begin()
{
	/*setbkcolor(WHITE);*/
	int i,j;
	sleep(1);
	for(i=0;i<15;i++)
		for(j=0;j<20;j++)
			if(a[i][j]==2)/*代表墙壁*/
			{
				setfillstyle(SOLID_FILL,9);
				bar(100+j*20-10,100+i*20+10,100+j*20+10,100+i*20-10);
			}
			else if(a[i][j]==3)/*代表自己*/
			{
				setcolor(RED);
				circle(100+j*20,100+i*20,9);
			}
			else if(a[i][j]==4)/*代表敌人*/
			{
				setcolor(GREEN);
				circle(100+j*20,100+i*20,9);

			}
			else if(a[i][j]==0)/*代表豆子*/
			{
				setcolor(YELLOW);
				setlinestyle(0,0,1);
				circle(100+j*20,100+i*20,3);
			}
	you.x=5;you.y=9;/*自己和敌人的初始坐标*/
	them[0].x=2;them[0].y=15;
	them[1].x=4;them[1].y=1;
	them[2].x=8;them[2].y=16;
	them[3].x=12;them[3].y=13;
	them[4].x=13;them[4].y=7;


}
void init()/*是否开始游戏*/
{
	setcolor(3);
	setlinestyle(DASHED_LINE, 0, 3);							/*设置线的宽度和形式*/
	rectangle(100, 100, 550, 350);								/*画矩形*/


	settextstyle(3,0,3);
	outtextxy(200,200,"GAME START!");
	settextstyle(3,0,3);
	outtextxy(110,300,"ESC-exit/press any key to continue");
	 getch();
}

main()
{
	int gd=DETECT,gm;
	int key;
	initgraph(&gd,&gm,"");/*图形界面初始化*/
	init();/*调用初始化函数显示进入游戏初始界面*/
	key=bioskey(0);/*接收键盘按键*/
	if(key==ESC)
		exit(0);
	else
	{
		cleardevice();
		begin();

		play();/*编写游戏实现过程*/


		if(sum==50)/*吃满豆子了*/
		{
			win();
			getch();
		}
		if(false)
		{
			lose();
			getch();

		}

	}
closegraph();
	

}

                                     
/*加载库文件*/
#pragma comment( lib, "ws2_32.lib" )
/*加载头文件*/
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

/* 【自学去】网站收集 http://www.zixue7.com */

/*定义多播常量*/
#define MCASTADDR     "224.3.5.8"
#define MCASTPORT     25000
#define BUFSIZE       1024
#define MCOUNT        10

/*定义广播常量*/
#define BCASTPORT     5050
#define BCOUNT        10

/*定义广播全局变量*/
SOCKET             socketBro;
SOCKET             socketRec;
struct sockaddr_in addrBro;
struct sockaddr_in addrRec;
BOOL               broadSendFlag;
BOOL               broadFlag;

DWORD              bCount;
DWORD              bcastAddr;
short              bPort;

/*定义多播全局变量*/
SOCKET             socketMul;
SOCKET             sockJoin;
struct sockaddr_in addrLocal;
struct sockaddr_in addrMul;

BOOL               multiSendFlag;
BOOL               bLoopBack;    
BOOL               multiFlag;

DWORD              dwInterface;  
DWORD              dwMulticastGroup;
DWORD              mCount;         
short              mPort;           

/*自定义函数*/
void initial();
void GetArgments(int argc, char **argv);

void userHelpAll();
void userHelpBro();
void userHelpMul();

void broadcastSend();
void broadcastRec();

void mulControl();
void multicastSend();
void multicastRec();

/*初始化全局变量函数*/
void initial()
{
    /*初始化广播全局变量*/
    bPort = BCASTPORT;
    bCount = BCOUNT;
    bcastAddr = INADDR_BROADCAST;
    broadSendFlag = FALSE;
    broadFlag = FALSE;
    multiFlag = FALSE;

    /*初始化多播全局变量*/
    dwInterface = INADDR_ANY;
    dwMulticastGroup = inet_addr(MCASTADDR);
    mPort = MCASTPORT;
    mCount = MCOUNT;
    multiSendFlag = FALSE;
    bLoopBack = FALSE;
}

/*参数获取函数*/
void GetArgments(int argc, char **argv)
{
    int i;
    /*如果参数个数小于2个*/
    if(argc<=1)
    {
        userHelpAll();
        return ;
    }
    /*获取广播选项*/
    if(argv[1][0]=='-'&&argv[1][1]=='b')
    {
        /*广播标志设置为真*/
        broadFlag = TRUE;
        for(i=2; i < argc ;i++)
    	{
            if (argv[i][0] == '-')
    		{
                switch (tolower(argv[i][1]))
        		{
                    /*如果是发送者*/
                	case 's': 
                        broadSendFlag = TRUE;
            			break;
                    /*广播的地址*/
                	case 'h':
                        if (strlen(argv[i]) > 3)
                            bcastAddr = inet_addr(&argv[i][3]);
            			break;
                    /*广播的端口号*/
                	case 'p':
                        if (strlen(argv[i]) > 3)
                            bPort = atoi(&argv[i][3]);
            			break;
                    /*广播(接收或者发送)的数量*/
                	case 'n': 
                        bCount = atoi(&argv[i][3]);
            			break;
                    /*其他情况显示用户帮助，终止程序*/
                	default:
        				{
                        	userHelpBro();
                        	ExitProcess(-1);
        				}
            			break;
        		}
    		}
    	}
        return ;
    }

    /*获取多播选项*/
    if(argv[1][0]=='-'&&argv[1][1]=='m')
    {
        /*多播标志设置为真*/
        multiFlag = TRUE;
        for(i=2; i < argc ;i++)
    	{
            if (argv[i][0] == '-')
    		{
                switch (tolower(argv[i][1]))
        		{
                    /*如果是发送者*/
                	case 's': 
                        multiSendFlag = TRUE;
            			break;
                    /*多播地址*/
                	case 'h': 
                        if (strlen(argv[i]) > 3)
                            dwMulticastGroup = inet_addr(&argv[i][3]);
            			break;
                    /*本地接口地址*/
                	case 'i': 
                        if (strlen(argv[i]) > 3)
                            dwInterface = inet_addr(&argv[i][3]);
            			break;
                    /*多播端口号*/
                	case 'p': 
                        if (strlen(argv[i]) > 3)
                            mPort = atoi(&argv[i][3]);
            			break;
                    /*环回标志设置为真*/
                	case 'l': 
                        bLoopBack = TRUE;
            			break;
                    /*发送(接收)的数量*/
                	case 'n':
                        mCount = atoi(&argv[i][3]);
            			break;
                    /*其他情况，显示用户帮助，终止程序*/
                	default:
                    	userHelpMul();
            			break;
        		}
    		}
    	}
    
    }
    return;
}

/*全局用户帮助函数*/
void userHelpAll()
{
    printf("Please choose broadcast[-b] or multicast[-m] !\n"); 
    printf("userHelpAll: -b [-s][p][-h][-n] | -m[-s][-h][-p][-i][-l][-n]\n");
    userHelpBro();
    userHelpMul();
}

/*广播用户帮助函数*/
void userHelpBro()
{
    printf("Broadcast: -b -s:str -p:int -h:str -n:int\n");
    printf("           -b     Start the broadcast program.\n");
    printf("           -s     Act as server (send data); otherwise\n");
    printf("                  receive data. Default is receiver.\n");
    printf("           -p:int Port number to use\n ");
    printf("                  The default port is 5050.\n");
    printf("           -h:str The decimal broadcast IP address.\n");
    printf("           -n:int The Number of messages to send/receive.\n");
    printf("                  The default number is 10.\n");
}

/*多播用户帮助函数*/
void userHelpMul()
{
    printf("Multicast: -m -s -h:str -p:int -i:str -l -n:int\n");
    printf("           -m     Start the multicast program.\n");
    printf("           -s      Act as server (send data); otherwise\n");
    printf("                   receive data. Default is receiver.\n");
    printf("           -h:str  The decimal multicast IP address to join\n");
    printf("                   The default group is: %s\n", MCASTADDR);
    printf("           -p:int  Port number to use\n");
    printf("                   The default port is: %d\n", MCASTPORT);
    printf("           -i:str  Local interface to bind to; by default \n");
    printf("                   use INADDRY_ANY\n");
    printf("           -l      Disable loopback\n");
    printf("           -n:int  Number of messages to send/receive\n");
    ExitProcess(-1);
}

/*广播消息发送函数*/
void broadcastSend()
{
    /*设置广播的消息*/
    char *smsg="The message received is from sender!";
    BOOL opt=TRUE;
    int nlen=sizeof(addrBro);
    int ret;
    DWORD i=0;
    
    /*创建UDP套接字*/
    socketBro=WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
    /*如果创建失败*/
    if(socketBro==INVALID_SOCKET)
    {
        printf("Create socket failed:%d\n",WSAGetLastError());
        WSACleanup();
        return;
    }
    
    /*设置广播地址各个选项*/
    addrBro.sin_family=AF_INET;
    addrBro.sin_addr.s_addr=bcastAddr;
    addrBro.sin_port=htons(bPort);
    
    /*设置该套接字为广播类型*/
    if (setsockopt(socketBro,SOL_SOCKET,SO_BROADCAST,(char FAR *)&opt,
                                        		sizeof(opt))==SOCKET_ERROR)
    /*如果设置失败*/
    {
        printf("setsockopt failed:%d",WSAGetLastError());
        closesocket(socketBro);
        WSACleanup();
        return;
    }
    /*循环发送消息*/
    while(i<bCount)
    {
        /*延迟1秒*/
        Sleep(1000);
        /*从广播地址发送消息*/
        ret=sendto(socketBro,smsg,256,0,(struct sockaddr*)&addrBro,nlen);
        /*如果发送失败*/
        if(ret==SOCKET_ERROR)
            printf("Send failed:%d",WSAGetLastError());
        /*如果发送成功*/
        else
        {		
            printf("Send message %d!\n",i);	
    	}
        i++;
    }
    /*发送完毕后关闭套接字、释放占用资源*/
    closesocket(socketBro);
    WSACleanup();
}

/*广播消息接收函数*/
void broadcastRec()
{   
    BOOL optval = TRUE;
    int addrBroLen;
    char buf[256];
    DWORD i=0;
    /*该地址用来绑定套接字*/
    addrRec.sin_family=AF_INET;
    addrRec.sin_addr.s_addr=0;
    addrRec.sin_port=htons(bPort);

    /*该地址用来接收网路上广播的消息*/
    addrBro.sin_family=AF_INET;
    addrBro.sin_addr.s_addr=bcastAddr;
    addrBro.sin_port=htons(bPort);
    
    addrBroLen=sizeof(addrBro);
    //创建UDP套接字
    socketRec=socket(AF_INET,SOCK_DGRAM,0);
    /*如果创建失败*/
    if(socketRec==INVALID_SOCKET)
    {
        printf("Create socket error:%d",WSAGetLastError());
        WSACleanup();
        return;
    }

    /*设置该套接字为可重用类型*/
    if(setsockopt(socketRec,SOL_SOCKET,SO_REUSEADDR,(char FAR *)&optval,
                                                			sizeof(optval))==SOCKET_ERROR)
    /*如果设置失败*/
    {
        printf("setsockopt failed:%d",WSAGetLastError());
        closesocket(socketRec);
        WSACleanup();
        return;
    }
    /*绑定套接字和地址*/
    if(bind(socketRec,(struct sockaddr *)&addrRec,
                                sizeof(struct sockaddr_in))==SOCKET_ERROR)
    /*如果绑定失败*/
    {
        printf("bind failed with: %d\n", WSAGetLastError());
        closesocket(socketRec);
        WSACleanup();
        return ;
    }
    /*从广播地址接收消息*/
    while(i<bCount)
    {
        recvfrom(socketRec,buf,256,0,(struct sockaddr FAR *)&addrBro,(int FAR *)&addrBroLen);
        /*延迟2秒钟*/
        Sleep(2000);
        /*输出接收到缓冲区的消息*/
        printf("%s\n",buf);
        /*情况缓冲区*/
        ZeroMemory(buf,256);
        i++;
    }
    /*接收完毕后关闭套接字、释放占用资源*/
    closesocket(socketRec);
    WSACleanup();
}

/*多播控制函数*/
void mulControl()
{
    int optval; 
    /*创建UDP套接字，用于多播*/
    if ((socketMul = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0,
                  WSA_FLAG_MULTIPOINT_C_LEAF 
                  | WSA_FLAG_MULTIPOINT_D_LEAF 
                  | WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        printf("socket failed with: %d\n", WSAGetLastError());
        WSACleanup();
        return ;
    }
    
    /*设置本地接口地址*/
    addrLocal.sin_family = AF_INET;
    addrLocal.sin_port = htons(mPort);
    addrLocal.sin_addr.s_addr = dwInterface;
    
    /*将UDP套接字绑定到本地地址上*/
    if (bind(socketMul, (struct sockaddr *)&addrLocal, 
                                    sizeof(addrLocal)) == SOCKET_ERROR)
    /*如果绑定失败*/
    {
        printf("bind failed with: %d\n", WSAGetLastError());
        closesocket(socketMul);
        WSACleanup();
        return ;
    }

    /*设置多播地址各个选项*/
    addrMul.sin_family      = AF_INET;
    addrMul.sin_port        = htons(mPort);
    addrMul.sin_addr.s_addr = dwMulticastGroup;

    /*重新设置TTL值*/
    optval = 8;
    /*设置多播数据的TTL(存在时间)值。默认情况下，TTL值是1*/
    if (setsockopt(socketMul, IPPROTO_IP, IP_MULTICAST_TTL, 
        (char *)&optval, sizeof(int)) == SOCKET_ERROR)
    /*如果设置失败*/
    {
        printf("setsockopt(IP_MULTICAST_TTL) failed: %d\n",WSAGetLastError());
        closesocket(socketMul);
        WSACleanup();
        return ;
    }

    /*如果指定了返还选项*/
    if (bLoopBack)
    {
        /*设置返还选项为假，禁止将发送的数据返还给本地接口*/
        optval = 0;
        if (setsockopt(socketMul, IPPROTO_IP, IP_MULTICAST_LOOP,
            (char *)&optval, sizeof(optval)) == SOCKET_ERROR)
        /*如果设置失败*/
    	{
            printf("setsockopt(IP_MULTICAST_LOOP) failed: %d\n",
                WSAGetLastError());
            closesocket(socketMul);
            WSACleanup();
            return ;
        }
    }
    
    /*加入多播组*/
    if ((sockJoin = WSAJoinLeaf(socketMul, (SOCKADDR *)&addrMul, 
                             sizeof(addrMul), NULL, NULL, NULL, NULL, 
                             JL_BOTH)) == INVALID_SOCKET)
    /*如果加入不成功*/
    {
        printf("WSAJoinLeaf() failed: %d\n", WSAGetLastError());
        closesocket(socketMul);
        WSACleanup();
        return ;
    }
}

/*多播消息发送函数*/
void multicastSend()
{
    
    TCHAR  sendbuf[BUFSIZE];
    DWORD i;
    int ret;

    mulControl();
    /*发送mCount条消息*/
    for(i = 0; i < mCount; i++)
    {
        /*将待发送的消息写入发送缓冲区*/
        sprintf(sendbuf, "server 1: This is a test: %d", i);
        ret=sendto(socketMul, (char *)sendbuf, strlen(sendbuf), 0,
                (struct sockaddr *)&addrMul, sizeof(addrMul));
        /*如果发送失败*/
        if(ret==SOCKET_ERROR)
        {
            printf("sendto failed with: %d\n",WSAGetLastError());
            closesocket(sockJoin);
            closesocket(socketMul);
            WSACleanup();
            return ;
        }
        /*如果发送成功*/
        else
            printf("Send message %d\n",i);
         Sleep(500);
     }
    /*关闭套接字、释放占用资源*/
    closesocket(socketMul);
    WSACleanup();
}

/*多播消息接收函数*/
void multicastRec()
{
    DWORD i;
    struct sockaddr_in  from;
    TCHAR recvbuf[BUFSIZE];
    int ret;
    int len = sizeof(struct sockaddr_in);

    mulControl();
    /*接收mCount条消息*/
    for(i = 0; i < mCount; i++)
    {
        /*将接收的消息写入接收缓冲区*/
        if ((ret = recvfrom(socketMul, recvbuf, BUFSIZE, 0,
                (struct sockaddr *)&from, &len)) == SOCKET_ERROR)
        /*如果接收不成功*/
    	{
            printf("recvfrom failed with: %d\n",WSAGetLastError());
            closesocket(sockJoin);
            closesocket(socketMul);
            WSACleanup();
             return ;
        }
        /*接收成功，输出接收的消息*/
        recvbuf[ret] = 0;
        printf("RECV: '%s' from <%s>\n", recvbuf,inet_ntoa(from.sin_addr));
     }
    /*关闭套接字、释放占用资源*/
    closesocket(socketMul);
    WSACleanup();
}

/*主函数*/
int main(int argc, char **argv)
{
    WSADATA wsd;

    initial();
    GetArgments(argc, argv);

    /*初始化Winsock*/
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        printf("WSAStartup() failed\n");
        return -1;
    }

    /*如果是执行广播程序*/
    if(broadFlag)
    {
        /*以发送者身份发送消息*/
        if(broadSendFlag)
    	{
            broadcastSend();
            return 0;
    	}
        /*以接收者身份接收消息*/
        else
    	{
            broadcastRec();
            return 0;
    	}
    }

    /*如果是执行多播程序*/
    if(multiFlag)
    {
        /*以发送者身份发送消息*/
        if(multiSendFlag) 
    	{
            multicastSend();
            return 0;
        }
        /*以接收者身份接收消息*/
        else    
    	{
            multicastRec();
            return 0;
    	}
    }
    return 0;
}

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h> 
#include <string.h> 
#include <conio.h>
/*字符操作函数*/
#include <ctype.h> 

#define BUFFSIZE 32
#define COL 128
#define ROW 64

/* 【自学去】网站收集 http://www.zixue7.com */
/*定义栈1*/
typedef struct node
{
    int data;
    struct node  *next;
}STACK1; 
/*定义栈2*/
typedef struct node2
{
    char data;
    struct node2 *next;
}STACK2;
/*下面定义两个栈基本操作*/
/*入栈函数*/
STACK1 *PushStack(STACK1 *top,int x)
{
    STACK1 *p;  
    p=(STACK1 *)malloc(sizeof(STACK1));
    if(p==NULL)  
    {
        printf("ERROR\n!");
        exit(0);  
    }
    p->data=x;  
    p->next=top;    
    top=p;      		
    return top;     
}
/*出栈函数*/
STACK1 *PopStack(STACK1 *top) 
{
    STACK1 *q; 
    q=top;  
    top=top->next; 
    free(q);  
    return top; 
}
/*读栈顶元素*/
int GetTop(STACK1 *top) 
{
    if(top==NULL)
    {
        printf("Stack is null\n"); 
        return 0;
    }
    /*返回栈顶元素*/
    return top->data; 
}
/*取栈顶元素，并删除栈顶元素*/
STACK1 *GetDelTop(STACK1 *top,int *x) 
{
    *x=GetTop(top);     
    top=PopStack(top); 
    return top; 
}
int EmptyStack(STACK1 *top) /*判栈是否为空*/
{
    if(top==NULL) 
        return 1; 
    return 0; 
}
/*入栈函数*/
STACK2 *PushStack2(STACK2 *top,char x) 
{
    STACK2 *p;
    p=(STACK2 *)malloc(sizeof(STACK2)); 
    if(p==NULL) 
    {
        printf("error\n!"); 
        exit(0); 
    }
    p->data=x; 
    p->next=top; 
    top=p; 
    return top; 
}
STACK2 *PopStack2(STACK2 *top) /*出栈*/
{
    STACK2 *q; 
    q=top; 
    top=top->next; 
    free(q); 
    return top; 
}
/*读栈顶元素*/
char GetTop2(STACK2 *top) 
{
    if(top==NULL) 
    {
        printf("Stack is null\n"); 
        return 0; 
    }
    return top->data; 
}
/*取栈顶元素，并删除栈顶元素*/
STACK2 *GetDelTop2(STACK2 *top,char *x) 
{
    *x=GetTop2(top); 
    top=PopStack2(top);
    return top; 
}
/*判栈是否为空*/
int EmptyStack2(STACK2 *top) 
{
    if(top==NULL)
        return 1; 
    else
        return 0; 
}
/*随机发牌函数*/
void GenCard()
{
    int num,i;
    randomize();
    for(i=0;i<4;i++)
    {
        num=random(13); /*大小随机数*/
        printf("%d ",num);
    } 
}
/*中缀字符串e转后缀字符串a函数*/
void ExpressTransform(char *expMiddle,char *expBack) 
{
    STACK2 *top=NULL; /* 定义栈顶指针*/
    int i=0,j=0;
    char ch;
    while(expMiddle[i]!='\0') 
    {
        /*判断字符是数字*/
        if(isdigit(expMiddle[i])) 
        {
            do{
                expBack[j]=expMiddle[i];
                i++;j++; 
            }while(expMiddle[i]!='.');
            expBack[j]='.';
            j++;
        }
        /*处理“(”*/
        if(expMiddle[i]=='(')  
            top=PushStack2(top,expMiddle[i]);
        /*处理“)”*/
        if(expMiddle[i]==')')  
        {
            top=GetDelTop2(top,&ch); 
            while(ch!='(')  
        	{
                expBack[j]=ch;  
                j++; 
                top=GetDelTop2(top,&ch);
        	}
        }
        /*处理加或减号*/
        if(expMiddle[i]=='+'||expMiddle[i]=='-') 
        {
            if(!EmptyStack2(top))
        	{
                ch=GetTop2(top);
                while(ch!='(') 
            	{
                    expBack[j]=ch;
                    j++; 
                    top=PopStack2(top);
                    if(EmptyStack2(top))
                        break; 
                    else
                        ch=GetTop2(top); 
            	}
        	}
            top=PushStack2(top,expMiddle[i]);
        }
        /*处理乘或除号*/
        if(expMiddle[i]=='*'||expMiddle[i]=='/') 
        {
            if(!EmptyStack2(top)) 
        	{
                ch=GetTop2(top);
                while(ch=='*'||ch=='/')
            	{
                    expBack[j]=ch;
                    j++; 
                    top=PopStack2(top);
                    if(EmptyStack2(top)) 
                        break; 
                    else
                        ch=GetTop2(top); 
            	}
        	}
            top=PushStack2(top,expMiddle[i]); 
        }
        i++; 
    }
    while(!EmptyStack2(top)) 
        top=GetDelTop2(top,&expBack[j++]);
    expBack[j]='\0';  
}
/*后缀表达式求值函数*/
int ExpressComputer(char *s)
{
    STACK1 *top=NULL;
    int i,k,num1,num2,result;
    i=0;
    while(s[i]!='\0')  /*当字符串没有结束时作以下处理*/
    {
        if(isdigit(s[i])) /*判字符是否为数字*/
        {
            k=0;  /*k初值为0*/
            do{
                k=10*k+s[i]-'0';  /*将字符连接为十进制数字*/
                i++;   /*i加1*/
            }while(s[i]!='.'); /*当字符不为‘.’时重复循环*/
            top=PushStack(top,k); /*将生成的数字压入堆栈*/
        }
        if(s[i]=='+')  /*如果为'+'号*/
        {
            top=GetDelTop(top,&num2); /*将栈顶元素取出存入num2中*/
            top=GetDelTop(top,&num1);  /*将栈顶元素取出存入num1中*/
            result=num2+num1;  /*将num1和num2相加存入result中*/
            top=PushStack(top,result);  /*将result压入堆栈*/
        }
        if(s[i]=='-')  /*如果为'-'号*/
        {
            top=GetDelTop(top,&num2); /*将栈顶元素取出存入num2中*/
            top=GetDelTop(top,&num1); /*将栈顶元素取出存入num1中*/
            result=num1-num2; /*将num1减去num2结果存入result中*/
            top=PushStack(top,result); /*将result压入堆栈*/
        }
        if(s[i]=='*')  /*如果为'*'号*/
        {
            top=GetDelTop(top,&num2); /*将栈顶元素取出存入num2中*/
            top=GetDelTop(top,&num1); /*将栈顶元素取出存入num1中*/
            result=num1*num2; /*将num1与num2相乘结果存入result中*/
            top=PushStack(top,result); /*将result压入堆栈*/
        }
        if(s[i]=='/') /*如果为'/'号*/
        {
            top=GetDelTop(top,&num2); /*将栈顶元素取出存入num2中*/
            top=GetDelTop(top,&num1); /*将栈顶元素取出存入num1中*/
            result=num1/num2;               /*将num1除num2结果存入result中*/
            top=PushStack(top,result); /*将result压入堆栈*/
        }
        i++;  /*i加1*/
    }
    top=GetDelTop(top,&result); /*最后栈顶元素的值为计算的结果*/
    return result;  /*返回结果*/
}
/*检查输入的表达式是否正确*/
int CheckExpression(char *e)
{
    char ch;
    int i=0;
    while(e[i]!='\0')
    {
        if(isdigit(e[i])) 
        {
            if(isdigit(e[i+1]))
        	{
                i++;
                continue;
        	}
            if(e[i+1]!='.')
        	{
                printf("\n The wrong express format!!\n");
                return 0;
        	}
            i++;
        }
        i++;
    }
    return 1;
}
/*主函数*/
int main()
{
    char expMiddle[BUFFSIZE],expBack[BUFFSIZE],ch;
    int i,result;
    clrscr(); 
    /*提示输入字符串格式*/
    printf("*******************************************\n");
    printf("|  Welcome to play our game : 24 points!  |\n");
    printf("|      The input format as follows:       |\n");
    printf("|              10.*(4.-3.)                |\n");
    printf("*******************************************\n");
    while(1)
    {
        printf("\n The four digits are: ");
        GenCard();
        printf("\n");
        do{
            printf(" Please input the express:\n");
            /*输入字符串压回车键*/
            scanf("%s%c",expMiddle,&ch); 
            /*检查输入的表达式是否正确*/
        }while(!CheckExpression(expMiddle));
        
        printf("%s\n",expMiddle);
        /*调用ExpressTransform函数将中缀表达式expMiddle转换为后缀表达式expBack*/
        ExpressTransform(expMiddle,expBack); 
        /*计算后缀表达式的值*/
        result=ExpressComputer(expBack); 
        printf("The value of %s is:%d.\n",expMiddle,result);
        if(result==24) 
            printf("You are right!");
        else printf("You are wrong!");
        printf(" Do you want to play again(y/n)?\n");
        scanf("%c",&ch); 
        if(ch=='n'||ch=='N') 
            break;   
    } 
    return 0;
}

/* 电子时钟程序 */

/* 【自学去】网站收集 http://www.zixue7.com */

#include <stdio.h> 
#include <stdlib.h> 
#include <conio.h> 
#include <time.h> 
typedef struct 
{ 
     int x; 
     int y; 
}Point; 
time_t now; 
struct tm *pt,t1,t2; 
int printpoint(Point p) 
{ 
     Point p1; 
     p1.x=p.x+2;  p1.y=p.y+4; 
     gotoxy(p1.x,p1.y);     printf("%c%c",2,2); 
     gotoxy(p1.x, p1.y+1);   printf("%c%c",2,2); 
     p1.y+=4; 
     gotoxy(p1.x,p1.y);     printf("%c%c",2,2); 
     gotoxy(p1.x,p1.y+1);   printf("%c%c",2,2); 
     return 0; 
} 
int print0(Point p) 
{ 
     int i=0; 
     for(;i<13;i++) 
     { 
          gotoxy(p.x+1, p.y+i); 
          if(i==0||i==12) 
                printf("%c%c%c%c%c%c",2,2,2,2,2,2); 
          else 
                printf("%c%4s%c",2," ",2); 
     } 
     return 0; 
} 
int print1(Point p) 
{ 
     int i=0; 
     for(;i<13;i++) 
     { 
          gotoxy(p.x+1, p.y+i); 
          printf("%5s%c"," ",2); 
     } 
     return 0; 
} 
int print2(Point p) 
{ 
     int i=0; 
     for(;i<13;i++) 
     { 
          gotoxy(p.x+1, p.y+i); 
          if(i==0||i==6||i==12) 
                printf("%c%c%c%c%c%c",2,2,2,2,2,2); 
          else if(i>0&&i<6) 
               printf("%5s%c"," ",2); 
               else 
                printf("%c",2); 
     } 
     return 0; 
} 
int print3(Point p) 
{ 
     int i=0; 
     for(;i<13;i++) 
     { 
          gotoxy(p.x+1, p.y+i); 
          if(i==0||i==6||i==12) 
                printf("%c%c%c%c%c%c",2,2,2,2,2,2); 
          else 
                printf("%5s%c"," ",2); 
     } 
     return 0; 
} 
int print4(Point p) 
{ 
     int i=0; 
     for(;i<13;i++) 
     { 
          gotoxy(p.x+1, p.y+i); 
          if(i<6)  printf("%c%4s%c",2," ",2); 
          else if(i==6) 
                    printf("%c%c%c%c%c%c",2,2,2,2,2,2); 
               else printf("%5s%c"," ",2); 
     } 
     return 0; 
} 
int print5(Point p) 
{ 
     int i=0; 
     for(;i<13;i++) 
     { 
          gotoxy(p.x+1, p.y+i); 
          if(i==0||i==6||i==12) 
                printf("%c%c%c%c%c%c",2,2,2,2,2,2); 
          else if(i>0&&i<6) 
                printf("%c",2); 
               else 
                printf("%5s%c"," ",2); 
     } 
     return 0; 
} 
int print6(Point p) 
{ 
     int i=0; 
     for(;i<13;i++) 
     { 
          gotoxy(p.x+1, p.y+i); 
          if(i==0||i==6||i==12) 
                printf("%c%c%c%c%c%c",2,2,2,2,2,2); 
          else if(i>0&&i<6) 
                printf("%c",2); 
               else 
                printf("%c%4s%c",2," ",2); 
     } 
     return 0; 
} 
int print7(Point p) 
{ 
     int i=0; 
     for(;i<13;i++) 
     { 
          gotoxy(p.x+1, p.y+i); 
          if(i==0)   printf("%c%c%c%c%c%c",2,2,2,2,2,2); 
          else       printf("%5s%c"," ",2); 
     } 
     return 0; 
} 
int print8(Point p) 
{ 
     int i=0; 
     for(;i<13;i++) 
     { 
          gotoxy(p.x+1, p.y+i); 
          if(i==0||i==6||i==12) 
                printf("%c%c%c%c%c%c",2,2,2,2,2,2); 
           else  printf("%c%4s%c",2," ",2); 
     } 
     return 0; 
} 
int print9(Point p) 
{ 
     int i=0; 
     for(;i<13;i++) 
     { 
          gotoxy(p.x+1, p.y+i); 
          if(i==0||i==6||i==12) 
                printf("%c%c%c%c%c%c",2,2,2,2,2,2); 
          else if(i>0&&i<6) 
                printf("%c%4s%c",2," ",2); 
               else 
                printf("%5s%c"," ",2); 
     } 
     return 0; 
} 
int clear(Point p) 
{ 
     int i=0; 
     for(;i<13;i++) 
     gotoxy(p.x,p.y+i); printf("%16s"," "); 
     return 0; 
} 
int printtime(Point p, int n) 
{ 
     int a,b; 
     Point pp; 
     a=n/10, b=n%10; 
     pp.x=p.x+8, pp.y=p.y; 
     switch(a) 
     { 
         case 0: print0(p); break; 
         case 1: print1(p); break; 
         case 2: print2(p); break; 
         case 3: print3(p); break; 
         case 4: print4(p); break; 
         case 5: print5(p); break; 
    } 
    switch(b) 
    { 
         case 0: print0(pp); break; 
         case 1: print1(pp); break; 
         case 2: print2(pp); break; 
         case 3: print3(pp); break; 
         case 4: print4(pp); break; 
         case 5: print5(pp); break; 
         case 6: print6(pp); break; 
         case 7: print7(pp); break; 
         case 8: print8(pp); break; 
         case 9: print9(pp); break; 
    } 
    return 0; 
} 
int main() 
{ 
      Point phour, pmin, psec,point1,point2; 
      phour.x=9, pmin.x=32, psec.x=55; 
      phour.y=pmin.y=psec.y=7; 
      point1.x=25, point2.x=49; 
      point1.y=point2.y=7; 
      clrscr(); 
      textbackground(YELLOW);         /* 设置背景颜色*/
      textcolor(RED);      /* 设置数字显示颜色*/
      now=time(0); 
      pt=localtime(&now); 
      t1=*pt; 
      printtime(phour, t1.tm_hour); 
      printpoint(point1); 
      printtime(pmin, t1.tm_min); 
      printpoint(point2); 
      printtime(psec, t1.tm_sec); 
      while(1) 
      { 
           now=time(0); 
       pt=localtime(&now); 
           t2=*pt; 
           if(t2.tm_sec!=t1.tm_sec) 
           { 
             t1=t2; 
             clrscr(); 
             printtime(phour, t1.tm_hour); 
             printpoint(point1); 
             printtime(pmin, t1.tm_min); 
             printpoint(point2); 
             printtime(psec, t1.tm_sec); 
           } 
           if(bioskey(1)==0) continue; 
           else exit(0); 
      } 
      return 0; 
}  
/* 【自学去】网站收集 http://www.zixue7.com */

#include<graphics.h>
#include<dos.h>
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<bios.h>
#define  R 4
#define Key_Up  0x4800
#define Key_Enter 0x1c0d
#define Key_Down  0x5000 
struct wall/*墙*/
{ int x;
  int y;
  int color;
}a[20][20];
int Keystate;
int MouseX;
int MouseY=400;
int dx=1,dy=1;	    /*计算球的反弹*/
int sizex=20,sizey=10;      /*墙的宽度和长度*/
int Ide,Key;
void draw(int x,int y)     /*画组成墙的砖*/
{
 int sizx=sizex-1;
 int sizy=sizey-1;
 setcolor(15);
 line(x,y,x+sizx,y);
 line(x,y+1,x+sizx,y+1);
 line(x,y,x,y+sizy);
 line(x+1,y,x+1,y+sizy);
 setcolor(4);
 line(x+1,y+sizy,x+sizx,y+sizy);
 line(x+2,y+sizy-1,x+sizx,y+sizy-1);
 line(x+sizx-1,y+1,x+sizx-1,y+sizy);
 line(x+sizx,y+2,x+sizx,y+sizy);
 setfillstyle(1,12);
 bar(x+2,y+2,x+sizx-2,y+sizy-2);
}

void picture(int r,int l)   /*画墙*/
{ int i,j;
  setcolor(15);
  rectangle(100,50,482,461);
  for(i=0;i<r;i++)
      for(j=0;j<l;j++)
	 { a[i][j].color=0;
	   a[i][j].x=106+j*25;
	   a[i][j].y=56+i*15;
	   draw(106+j*25,56+i*15);
         }
	     sizex=50,sizey=5;
}

void MouseOn(int x,int y)/*鼠标光标显示*/
{
  draw(x,y);
}

void MouseSetX(int lx,int rx)/*设置鼠标左右边界*/
  {
   _CX=lx;
   _DX=rx;
   _AX=0x07;
   geninterrupt(0x33);
  }

void MouseSetY(int uy,int dy)/*设置鼠标上下边界*/
  {
   _CX=uy;
   _DX=dy;
   _AX=0x08;
   geninterrupt(0x33);
  }

void MouseSetXY(int x,int y)/*设置鼠标当前位置*/
  {
   _CX=x;
   _DX=y;
   _AX=0x04;
   geninterrupt(0x33);
  }



void MouseSpeed(int vx,int vy)/*设置鼠标速度*/
  {
   _CX=vx;
   _DX=vy;
   _AX=0x0f;
   geninterrupt(0x33);
  }

void MouseGetXY()/*获取鼠标当前位置*/
  {
   _AX=0x03;
   geninterrupt(0x33);
   MouseX=_CX;
   MouseY=_DX;
  }
void MouseStatus()/*鼠标按键情况*/
{
 int x;
 int status;
 status=0;
 x=MouseX;
 if(x==MouseX&&status==0) /*判断鼠标是否移动*/
 {
  MouseGetXY();
  if(MouseX!=x)
     if(MouseX+50<482)
        status=1;
 }
 if(status)/*如果鼠标移动则重新显示鼠标*/
 {
   setfillstyle(1,0);
   bar(x,MouseY,x+sizex,MouseY+sizey);
   MouseOn(MouseX,MouseY);
 }
}


void Play(int r,int l)
{int ballX;
 int ballY=MouseY-R;
 int i,j,t=0;
 srand((unsigned long)time(0));
do
{
	ballX=rand()%477;
}while(ballX<=107||ballX>=476);/*随机产生小球的位置*/
 while(kbhit)
{
 MouseStatus();
 if(ballY<=(59-R))   /*碰上反弹*/
    dy*=(-1);
 if(ballX>=(482-R)||ballX<=(110-R)) /*碰左右反弹*/
    dx*=(-1);
 setcolor(YELLOW);
 circle(ballX+=dx,ballY-=dy,R-1);
 delay(2500);
 setcolor(0);/*将球移动后留下的印记用背景色覆盖*/
 circle(ballX,ballY,R-1);
 for(i=0;i<r;i++)
     for(j=0;j<l;j++)       /*判断是否撞到墙*/
	 if(t<l*r&&a[i][j].color==0 && ballX>=a[i][j].x && ballX<=a[i][j].x+20
              && ballY>=a[i][j].y && ballY<=a[i][j].y+10)
              {t++;
               dy*=(-1);
	       a[i][j].color=1;
	       setfillstyle(1,0);
	       bar(a[i][j].x,a[i][j].y,a[i][j].x+20,a[i][j].y+10);
              }
 if(ballX==MouseX||ballX==MouseX-1||ballX==MouseX-2&&ballX==(MouseX+50+2)||ballX==(MouseX+50+1)||ballX==(MouseX+50)) /*判断球落在板的边缘*/
    if(ballY>=(MouseY-R))
      { dx*=(-1);
        dy*=(-1);         /*原路返回*/
      }
 if(ballX>MouseX && ballX<(MouseX+50)) /*碰板反弹*/
    if(ballY>=(MouseY-R))
       dy*=(-1);
 if(t==l*r)/*判断是否将墙壁完全清除*/
   { sleep(1);
     cleardevice();
     setcolor(RED);
     settextstyle(0,0,4);
     outtextxy(100,200,"Win");
     sleep(5);
     break;
   }
 if(ballY>MouseY)
   { sleep(1);
     cleardevice();
     setcolor(RED);
     settextstyle(0,0,4);
     outtextxy(130,200,"Game Over");
     sleep(5);
     break;
   }
 }
 dx=1,dy=1;	    /*dx、dy重新置1*/
 sizex=20,sizey=10;
}
void Rule()/*游戏规则*/
{
int n;
char *s[5]={"move the mouse right or left to let the ball rebound","when the ball bounce the wall","the wall will disappear","when all the wall disappear","you will win!"};
settextstyle(0,0,1);
setcolor(GREEN);
for(n=0;n<5;n++)
outtextxy(150,170+n*20,s[n]);
}

void DrawMenu(int j)/*菜单中的选项*/
{
int n;
char *s[4]={"1.Mession One","2.Mession two","3.rule","4.Exit Game"};
settextstyle(0,0,1);
setcolor(GREEN);
for(n=0;n<4;n++)
outtextxy(250,170+n*20,s[n]);
setcolor(RED);/*选中那个菜单，那个菜单变为红色*/
outtextxy(250,170+j*20,s[j]);
}
void MainMenu()/*主菜单*/
{
void JudgeIde();
setbkcolor(BLACK);
cleardevice();
Ide=0,Key=0;
DrawMenu(Ide);
do
  {
  if(bioskey(1))   /*有键按下则处理按键*/
       {
       Key=bioskey(0);
       switch(Key)
     {
     case Key_Down:
	     {
		     Ide++;Ide=Ide%4;
		     DrawMenu(Ide);
		     break;
	     }
     case Key_Up:
	     {
		     Ide--;
		     Ide=(Ide+4)%4;
		     DrawMenu(Ide);
		     break;
	     }
     }
       }
  }while(Key!=Key_Enter);
JudgeIde();         /*调用judgeIde*/
}
void JudgeIde()
{
switch(Ide)
   {
   case 0:
	   cleardevice();
 picture(6,15);
 MouseSetX(101,431);           /*设置鼠标移动的范围*/
 MouseSetY(MouseY,MouseY);     /*鼠标只能左右移动*/
 MouseSetXY(150,MouseY);       /*鼠标的初始位置*/
 Play(6,15);
 MainMenu();
 break;
   case 1:{
	   cleardevice();
     picture(9,15);
     MouseSetX(101,431);
 MouseSetY(MouseY,MouseY);
 MouseSetXY(150,MouseY);
     Play(9,15);
     MainMenu();
     break;}
   case 2:{
	   cleardevice();
Rule();
sleep(8);
   MainMenu();
   break;}
   case 3:cleardevice();
	     settextstyle(0,0,4);
	     outtextxy(150,200,"goodbye!");
	     sleep(5);
	   exit(0);
   }
}
void main()
{int gdriver=DETECT,gmode;
 initgraph(&gdriver,&gmode,"");
 MainMenu();
 closegraph();
}


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 【自学去】网站收集 http://www.zixue7.com */

/*迷宫的数组*/
int maze[100][100];

/*迷宫的行数和列数*/
int m=0,n=0;
/*
对迷宫进行初始化，用随机数产生迷宫
*/
void InitMaze()
{
      int i,j,temp;
      srand((unsigned)time(NULL));
      for(i=1;i<=m;i++)
          for(j=1;j<=n;j++)
          {
              temp=rand()%100;
              if(temp>30)
              {
                  maze[i-1][j-1]=0;
              }else
              {
                  maze[i-1][j-1]=1;
              }
          }
      maze[0][0]=0;
      maze[m-1][n-1]=9;
}
/*
*定义栈和栈的节点
*/
typedef struct Node
{
int x;
int y;
struct Node *next;
}Node,*Stack;
/*
*初始化Stack
*/
void InitStack(Node *Stack)
{
Stack=(Node *)malloc(sizeof(Node));
if(Stack==NULL)
{
    printf("分配空间失败\n");
    exit(0);
}else
{
    Stack->next=NULL;
}
}
/*
*压栈
*/
void push(Node *Stack,int x,int y)
{
Node *temp;
temp=(Node *)malloc(sizeof(Node));
if (!temp)
{
    printf("分配内存空间错误");
    return;
}
else
{
    temp->x=x;
    temp->y=y;
    temp->next=Stack->next;
    Stack->next=temp;
}
}

/*
*出栈
*/
void pop(Node *Stack,int *x,int *y)
{
Node *temp;
temp=Stack->next;
if(!temp){
    return;
}else{
    *x=temp->x;
    *y=temp->y;
}
Stack->next=temp->next;
free(temp);
}
/*
*判断栈是否为空
*/
int isEmpty(Node *Stasck)
{
    return ((Stasck->next)==NULL);
}

/*
*判断从该点时候可以向其他方向移动，并返回移动的方向
*/
int pass(int i,int j)
{
           /*右方向*/
          if(j<n-1&&(maze[i][j+1]==0||maze[i][j+1]==9))
          {
              return 2;
          }
          /*下方向*/
          if(i<m-1&&(maze[i+1][j]==0||maze[i+1][j]==9))
          {
                return 3;
           }
           /*左方向*/
           if(j>=1&&(maze[i][j-1]==0||maze[i][j-1]==9))
           {
               return 4;
           }
           /*上方向*/
           if(i>=1&&(maze[i-1][j]==0||maze[i-1][j]==9))
           {
               return 5;
           }
           return -1;
}

/*
*对迷宫进行打印
*/
void drawMaze()
{
     int i=0,j=0;
     for(i=0;i<m;i++){
          for(j=0;j<n;j++)
          {
              if(maze[i][j]==0||maze[i][j]==9||maze[i][j]==-1)
              {
                  printf("a");
              }
              else if(maze[i][j]==1)
              {
                   printf("b");
              }
              else if(maze[i][j]==2)
              {
                   printf("D");
              }else if(maze[i][j]==3)
              {
                    printf("X");
              }else if(maze[i][j]==4)
              {
                    printf("A");
              }else if(maze[i][j]==5)
              {
                    printf("W");
              }
          }
          printf("\n");
      }
}
/*
*对迷宫进行路径搜索
*数组的数字有以下含义
*0.该点没有被探索过，且可行
*1.该点不可行
*2.该点是可行的，且进行了向东的探索
*3.该点是可行的，且进行了向南的探索
*4.该点是可行的，且进行了向西的探索
*5.该点是可行的，且进行了向北的探索
*6.该点是入口
*9.该点是出口
*-1.该点已经遍历完毕四个方向，不能找到有效的路径，则置为－1
*/
void ShowPath()
{
int curx=0,cury=0;
int count=0;
int flag=0;
Node *Stacks=NULL;
InitStack(Stacks);
do{
    if(maze[curx][cury]==9)
    {
     flag=1;
    }
    switch(pass(curx,cury)){
    case 2:
     maze[curx][cury]=2;
     push(Stacks,curx,cury);
     cury++;
     break;
    case 3:
     maze[curx][cury]=3;
     push(Stacks,curx,cury);
     curx++;
     break;
    case 4:
     maze[curx][cury]=4;
     push(Stacks,curx,cury);
     cury--;
     break;
    case 5:
     maze[curx][cury]=5;
     push(Stacks,curx,cury);
     curx--;
     break;
    case -1:
     maze[curx][cury]=-1;
     if(!isEmpty(Stacks))
      pop(Stacks,&curx,&cury);
     break;
    }
    count++;
}while(!isEmpty(Stacks)&&flag==0);
if(flag==1)
{
    printf("The path is :\n");
    printf("\n");
    drawMaze();
}else
{
    printf("\nSorry,you fail\n");
}
}
/*
*主函数
*/
int main()
{
    loop:
    printf("plase input the number of row m(m>0,m<100):");
    scanf("%d",&m);
    printf("plase input the number of line n(n>0,n<100):");
    scanf("%d",&n);
    if(m<0||m>100||n<0||n>100){
    printf("The number is error,process will exit !\n");
    exit(-1);
    }
    printf("The character is 'a',it is area.\n");
    printf("The character is 'b',it is wall.\n");
    printf("\n");
    InitMaze();
    printf("The oid Maze:\n");
    printf("\n");
    drawMaze();
    printf("\n show the path ?(y/n)");
    fflush(stdin);
    if(toupper(getch())=='Y')
    {
               printf("\n");
               ShowPath(); 
               printf("\n go on play ?(y/n)");
               fflush(stdin);
               if(toupper(getch())=='Y')
               {
                   goto loop;                 
               }
               else
               {
                   exit(1);
               }                
    }
    else
    {
             exit(1);
    }
    getch();
    return 0;
}

#include "bankDataStruct.h"
#include "bankServe.h"
#include "bankOperation.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
extern int maxAccount;
extern struct record account[99];
int accOption(int *option)
{
	printf("请输入要执行的操作：\n");
	scanf("%d",option);
	if(*option>=0 && *option<=9)
		return 0;
	else
	{
		printf("您输入的操作指令不合法。\n");
		return 1;
	}
}
int checkInput(int flag,char checkStr[])
{
	switch(flag)
	{
	case 0:
		if(strlen(checkStr)>8)
			return 1;
		else return 0;
		break;
	case 1:
		if(strlen(checkStr)>6)
			return 1;
		else return 0;
		break;
	case 2:
		if(strlen(checkStr)>12)
			return 1;
		else return 0;
		break;
	default:break;
	}
	return 0;
}
int checkPWD(int index)
{
	char accPWD[7];
	printf("请输入您的密码：\n");
	scanf("%s",accPWD);
	if(checkInput(1,accPWD))
	{
		return 1;
	}
	else
	{
		if(!strcmp(account[index].accPWD,accPWD))
		{
			return 0;
		}
		else
			return 2;
	}
}
int confirm(int flag)
{
	char input;
	switch(flag)
	{
	case 0:
		printf("确认要进行存款操作吗？[Y/N]\n");
		break;
	case 1:
		printf("确认要进行取款操作吗？[Y/N]\n");
		break;
	case 2:
		printf("确认要进行销户操作吗？[Y/N]\n");
		break;
	case 3:
		printf("确认要进行解冻操作吗？[Y/N]\n");
		break;
	case 4:
		printf("确认要进行删除操作吗？[Y/N]\n");
		break;
	case 5:
		printf("确认要进行转账操作吗？[Y/N]\n");
		break;
	case 6:
		printf("要进行开户吗？[Y/N]\n");
		break;
	case 7:
		printf("您的账户上仍有余额，必须转入其他账户才能删除，要转入吗？[Y/N]\n");
		break;
	default:
		return 2;
		break;
	}
	getchar();
	input=getchar();
	switch(tolower(input))
	{
	case 121:
		return 0;
		break;//y
	case 110:
		return 1;
		break;//n
	default:
		printf("您输入的操作指令不能识别！\n");
		return-1;
		break;
	}
}
int checkflag(int flag,int index)
{
	if(account[index].flag[0]=='1')
	{
		switch(flag)
		{
		case 0:
			printf("该账户已冻结，无法存款！\n");
			break;
		case 1:
			printf("该账户已冻结，无法取款！\n");
			break;
		case 2:
			printf("该账户已冻结，无法转账！\n");
			break;
		default:
			return -1;
			break;
		}
		return 1;
	}
	else
		return 0;
}
int createNumber()
{
	if(maxAccount==0)
		return 1001;
	else
		return account[maxAccount-1].accNumber+1;
}
int del(int index)
{
	for(;index!=maxAccount-1;++index)
	{
		account[index].accNumber=account[index+1].accNumber;
		if(!strcpy(account[index].accName,account[index+1].accName)||
		   !strcpy(account[index].accPWD,account[index+1].accPWD)||
		   !strcpy(account[index].flag,account[index+1].flag))
		{
			return 1;
		}
		account[index].balance=account[index+1].balance;
	}//从index位置开始将后面的账户统一前移
	--maxAccount;
	return 0;
}
void dispAnAccount(int number)
{
	if(number!=-1)
	{
		printf("+----+--------+------+------------+----+\n");
		printf("|账号|用户名  |密码  |余额        |状态|\n");
		printf("+--------------------------------------+\n");
		if(account[number].flag[0]=='0')
			printf("|%d|%8s|******|%12.2lf|正常|\n",account[number].accNumber,account[number].accName,
				account[number].balance);
		else
			printf("|%d|%8s|******|%12.2lf|注销|\n",account[number].accNumber,account[number].accName,
				account[number].balance);
		printf("+----+--------+------+------------+----+\n");
	}
}
int findAnAccount(int flag)
{
	int number;
	int index;
	switch(flag)
	{
	case 0:printf("请输入要查找的用户的账号：\n");break;
	case 1:printf("请输入要存款的用户的账号：\n");break;
	case 2:printf("请输入要取款的用户的账号：\n");break;
	case 3:printf("请输入要注销的用户的账号：\n");break;
	case 4:printf("请输入要解冻的用户的账号：\n");break;
	case 5:printf("请输入要删除的用户的账号：\n");break;
	case 6:printf("请输入要转账的用户的账号：\n");break;
	case 7:printf("请输入要转入的用户的账号：\n");break;
	default:break;
	}
	scanf("%d",&number);
	for(index=0;index!=maxAccount;++index)
	{
		if(account[index].accNumber==number)
		{
			return index;
		}
	}
	switch(flag)
	{
	case 0:printf("没有找到要查询的账号！\n");break;
	case 1:printf("没有找到要存入的账号！\n");break;
	case 2:printf("没有找到要取款的账号！\n");break;
	case 3:printf("没有找到要注销的账号！\n");break;
	case 4:printf("没有找到要解冻的账号！\n");break;
	case 5:printf("没有找到要删除的账号！\n");break;
	case 6:printf("没有找到要转账的账号！\n");break;
	case 7:printf("没有找到要转入的账号！\n");break;
	default:break;
	}
	return -1;
}
void getaccName(char *accName)
{
	while(1)
	{
		printf("请输入账户的用户名：\n");
		scanf("%s",accName);
		//do some validation
		if(!checkInput(0,accName))
			break;
		else
			printf("您输入的用户名不合法！\n");
	}
}
void getaccPWD(char *accPWD)
{
	while(1)
	{
		printf("请输入账户的密码：\n");
		scanf("%s",accPWD);
		//do some validation
		if(!checkInput(1,accPWD))
			break;
		else
			printf("您输入的密码不合法！\n");
	}
}
float getcash(int flag)
{
	float cash;
	char cashStr[99];
	while(1)
	{
		switch(flag)
		{
		case 0:
			printf("请输入存款金额：\n");
			break;
		case 1:
			printf("请输入取款金额：\n");
			break;
		case 2:
			printf("请输入转账金额：\n");
			break;
		default:break;
		}
		scanf("%f",&cash);
		sprintf(cashStr,"%f",cash);//格式化float型金额到字符串
		if(!checkInput(2,cashStr))
			break;
		else
		{
			printf("您输入的金额不正确！\n");
			return -1;
		}
	}
	return cash;
}
void mainmenu()
{
	int option;
	maxAccount=0;   //初始化账户数量为0
	if(readData())
	{
		printf("数据初始化失败！\n");
		exit(1);
	}
	do{
		printf("+------------------------------+\n");
		printf("|  存款  请按1     解冻  请按6 |\n");
		printf("|  取款  请按2     删除  请按7 |\n");
		printf("|  查询  请按3     转账  请按8 |\n");
		printf("|  开户  请按4                 |\n");
		printf("|  销户  请按5     退出  请按0 |\n");
		printf("|     按9输出所有用户信息      |\n");
		printf("+------------------------------+\n");
		while(accOption(&option))
		{
			//do nothing
		}
		switch(option)
		{
		case 0:
			if(writeData())
			{
				printf("保存数据失败！\n");
				break;
			}
			exit(0);
			break;
		case 1:
			deposit();
			break;
		case 2:
			withdraw();
			break;
		case 3:
			query();
			break;
		case 4:
			openAccount();
			break;
		case 5:
			closeAccount();
			break;
		case 6:
			unfreeze();
			break;
		case 7:
			deleteAccount();
			break;
		case 8:
			transfer();
			break;
		case 9:
			queryall();
			break;
		default:break;
		}
	}while(option);
}
void queryall()
{
	int index;
	printf("+----+--------+------+------------+----+\n");
	printf("|账号|用户名  |密码  |余额        |状态|\n");
	printf("+--------------------------------------+\n");
	for(index=0;index!=maxAccount;++index)
	{
		if(account[index].flag[0]=='0')
			printf("|%d|%8s|******|%12.2lf|正常|\n",account[index].accNumber,account[index].accName,
				account[index].balance);
		else
			printf("|%d|%8s|******|%12.2lf|注销|\n",account[index].accNumber,account[index].accName,
				account[index].balance);
		printf("+----+--------+------+------------+----+\n");
	}
}
int toAccount(float money)
{
	int index;
	int option;
	if((index=findAnAccount(7))!=-1)
	{
		//找到了要找的账户
		account[index].balance+=money;
		dispAnAccount(index);
		//存款成功
		return 0;
	}
	else
		return 1;
}
int  readData()
{
	FILE *fp;
	fp=fopen("account.txt","r");
	if(!fp)
		return 1;
	while(!feof(fp))
	{
		fscanf(fp,"%d%s%s%f%s\n",&account[maxAccount].accNumber,account[maxAccount].accName,account[maxAccount].accPWD,
			&account[maxAccount].balance,account[maxAccount].flag);
		++maxAccount;
	}
	fclose(fp);
	return 0;
}
int  writeData()
{
	FILE *fp;
	int num;
	fp=fopen("account.txt","w");
	if(!fp)
		return 1;
	for(num=0;num!=maxAccount;++num)
	{
		fprintf(fp,"%6d%8s%6s%12.2f%2s\n",account[num].accNumber,account[num].accName,account[num].accPWD,
			account[num].balance,account[num].flag);
	}
	fclose(fp);
	return 0;
}
#include <graphics.h>
#include <stdlib.h>
#include <dos.h>
#include<conio.h>
#define LEFT 0x4b00
#define RIGHT 0x4d00
#define DOWN 0x5000
#define UP 0x4800
#define ESC 0x011b
#define N 100/*贪吃蛇的最大长度*/
int i,key;
int speed;/*游戏速度*/
struct FOOD
{
   int x;/*食物的横坐标*/
   int y;/*食物的纵坐标*/
   int flag;/*标志是否要出现食物*/
}food;
struct Snake
{
   int x[N];
   int y[N];
   int node;/*蛇的节数*/
   int dir;/*蛇移动方向*/
   int life;/*标志是死是活*/
}snake;
void GameOver();/*结束游戏*/
void Play();/*玩游戏过程*/
void dwall();/*画墙*/
void wall(int x,int y);/*画组成墙的砖*/
int Speed();/*选择贪吃蛇的速度*/

main()
{
  int gdriver=DETECT,gmode;
   registerbgidriver(EGAVGA_driver);
   initgraph(&gdriver,&gmode,"");
   speed=Speed();/*将函数返回值赋给speed*/
   cleardevice();/*清屏*/
   dwall();/*开始画墙*/
   Play();/*开始完游戏*/
   getch();
   closegraph();/*退出图形界面*/
}

void wall(int x,int y)
{
 int sizx=9;
 int sizy=9;
 setcolor(15);/*白色画砖的上边和左边*/
 line(x,y,x+sizx,y);
 line(x,y+1,x+sizx-1,y+1);
 line(x,y,x,y+sizy);
 line(x+1,y,x+1,y+sizy-1);
 setcolor(4);/*红色画砖的右面和下面*/
 line(x+1,y+sizy,x+sizx,y+sizy);
 line(x+2,y+sizy-1,x+sizx,y+sizy-1);
 line(x+sizx-1,y+2,x+sizx-1,y+sizy-1);
 line(x+sizx,y+1,x+sizx,y+sizy);
 setfillstyle(1,12);/*用淡红色填充砖的中间部分*/
 bar(x+2,y+2,x+sizx-2,y+sizy-2);
}
void dwall()/*用前面画好的砖来画墙*/
{int j;
for(j=50;j<=600;j+=10)
{
	wall(j,40);/*画上面墙*/
	wall(j,451);/*画下面墙*/
}
for(j=40;j<=450;j+=10)
{
wall(50,j);/*画左面墙*/
wall(601,j);/*画右面墙*/
}
}
int Speed()/*选择贪吃蛇运行的速度*/
{           int m;
gotoxy(20,10);
   printf("level1\n");
   gotoxy(20,12);
   printf("level2\n");
   gotoxy(20,14);
   printf("level3\n\t\tplease choose:");
   scanf("%d",&m);
   switch(m)
   {
   case 1:return 60000;
   case 2:return 40000;
   case 3:return 20000;
   default:
   cleardevice();
   Speed();
   }
}

void Play(void)/*游戏实现过程*/
{
   srand((unsigned long)time(0));
   food.flag=1;/*1表示需出现新食物,0表示食物已存在*/
   snake.life=0;/*标志贪吃蛇活着*/
   snake.dir=1;/*方向向右*/
   snake.x[0]=300;snake.y[0]=240;/*定位蛇头初始位置*/
   snake.x[1]=300;snake.y[1]=240;
   snake.node=2;/*贪食蛇节数*/
   do
   {
      while(!kbhit())/*在没有按键的情况下,蛇自己移动身体*/
      {
	 if(food.flag==1)/*需要出现新食物*/
	 do
	 {
	     food.x=rand()%520+60;
	     food.y=rand()%370+60;
		food.flag=0;/*标志已有食物*/
	 }while(food.x%10!=0||food.y%10!=0);
	 if(food.flag==0)/*画出食物*/
	 {
              setcolor(GREEN);
	      setlinestyle(3,0,3);
              rectangle(food.x,food.y,food.x+10,food.y+10);
	 }
         for(i=snake.node-1;i>0;i--)/*实现蛇向前移动*/
         {
            snake.x[i]=snake.x[i-1];
	    snake.y[i]=snake.y[i-1];
	 }
      
	 switch(snake.dir)
	 {
	    case 1:snake.x[0]+=10;break;/*向右移*/
	    case 2: snake.x[0]-=10;break;/*向左移*/
	    case 3: snake.y[0]-=10;break;/*向上移*/
	    case 4: snake.y[0]+=10;break;/*向下移*/
	 }
	 for(i=3;i<snake.node;i++)
	 {
	    if(snake.x[i]==snake.x[0]&&snake.y[i]==snake.y[0])/*判断蛇是否吃到自己*/
	    {
               GameOver();/*游戏结束*/
               snake.life=1;/*蛇死*/
               break;
	    }
        }
	if(snake.x[0]<60||snake.x[0]>590||snake.y[0]<50||
	   snake.y[0]>440)/*蛇是否撞到墙壁*/
	{
	    GameOver();/*游戏结束*/
	    snake.life=1; /*蛇死*/
	    break;
	}
	if(snake.x[0]==food.x&&snake.y[0]==food.y)/*判断是否吃到食物*/
	{
           setcolor(0);/*用背景色遮盖调食物*/
           rectangle(food.x,food.y,food.x+10,food.y+10);
	   snake.node++;/*蛇的身体长一节*/
	   food.flag=1;/*需要出现新的食物*/
	}
	setcolor(4);/*画蛇*/
	for(i=0;i<snake.node;i++)
	 {     setlinestyle(0,0,1);
	   rectangle(snake.x[i],snake.y[i],snake.x[i]+10,
	       snake.y[i]+10);
	       }
	delay(speed);
	setcolor(0);/*用背景色遮盖蛇的的最后一节*/
	rectangle(snake.x[snake.node-1],snake.y[snake.node-1],
	snake.x[snake.node-1]+10,snake.y[snake.node-1]+10);
     }  /*endwhile（！kbhit）*/
    if(snake.life==1)/*如果蛇死就跳出循环*/
        break;
    key=bioskey(0);/*接收按键*/
       if(key==UP&&snake.dir!=4)/*判断是否往相反的方向移动*/
	  snake.dir=3;
       else
	 if(key==DOWN&&snake.dir!=3)/*判断是否往相反的方向移动*/
		   snake.dir=4;
	  else
	      if(key==RIGHT&&snake.dir!=2)/*判断是否往相反的方向移动*/
	     snake.dir=1;
	     else
	if(key==LEFT&&snake.dir!=1)/*判断是否往相反的方向移动*/
		snake.dir=2;
   }while(key!=ESC);/*ESC键退出游戏*/
}

void GameOver(void)
{
    cleardevice();
   setcolor(RED);
   settextstyle(0,0,4);
   outtextxy(50,200,"GAME OVER,BYE BYE!");
   sleep(3);
}



#include <graphics.h>
#include<math.h>
#include<time.h>
#include<bios.h>
#define PI 3.1415926
#define ESC 0x011b
float s0=-1,m0=-1,h0=-1;
main()
{
    int gdriver, gmode,key,i=3;
	char H[10];
    float n;
    gdriver = DETECT;
    registerbgidriver(EGAVGA_driver);
    initgraph(&gdriver, &gmode, "");							/*初始化图形界面*/
    setbkcolor(YELLOW);
	setcolor(BLUE);
	setfillstyle(1,1);
	circle(405,175,22);
	floodfill(405,175,BLUE);
	setfillstyle(1,1);									/*设置填充形式为红色实填充*/
    circle(235,175,22);
	floodfill(235,175,BLUE);
	setfillstyle(1,12);
	circle(320,240,85);
	circle(320, 240, 82);
	floodfill(403,240,BLUE);
    for(n=0;n<=2*PI;n+=PI/30)
	{
		putpixel(320+80 * cos(-n), 240+80 * sin(-n),GREEN); 				/*画标识分钟的小刻度*/
		
    }
    for(n=0;n<=2*PI;n+=PI/6)
	{
		setcolor(BLUE);
		circle(320+80 * cos(-n), 240+80 * sin(-n),1); 				/*画标识小时的大刻度*/
		setfillstyle(SOLID_FILL,LIGHTBLUE);
		floodfill(320+80 * cos(-n), 240+80 * sin(-n),BLUE);
		
    }
	setcolor(GREEN);
    for(n=0;n<=2*PI;n+=PI/6)
	{
		itoa(i,H,10);
		
		outtextxy(317+73 * cos(-n), 237+73 * sin(-n),H);			/*画标识小时的大刻度对应的数字*/
		i--;
		if(i<1)
			i+=12;
    }
	while(!kbhit())
	{
		timer();	
		delay(1000); 
	}
	key=bioskey(0);/*接收键盘按键*/
	if(key==ESC)
	{
		closegraph();
		exit(0);
		
	}	
    getch();
    closegraph();										/*退出图形界面*/
}

timer()
{
	
	struct tm *when;
	float s,m1,h1;
	int h,m;
	time_t now;
	time(&now);
	when= localtime(&now);
	h=when->tm_hour;
	m=when->tm_min;
	s=when->tm_sec;
	h1=(h+(m*60+s)/3600)*PI/6-PI/2;
	m1=(m+s/60)*(PI/30)-PI/2;
	if(s0!=(-1) && m0!=(-1) && h0!=(-1))
	{	setcolor(0);
	line(320,240,320+45*cos(h0),240+45*sin(h0));
	
	line(320,240,320+55*cos(m0),240+55*sin(m0));
	line(320,240,320+65*cos((s0)*(PI/30)+PI/2),240+65*sin((s0)*(PI/30)+PI/2));
	}		
	setcolor(RED);
	line(320,240,320+45*cos(h1),240+45*sin(h1));
	setcolor(LIGHTGRAY);
	line(320,240,320+55*cos(m1),240+55*sin(m1));
	setcolor(BLUE);
	line(320,240,320+65*cos((s)*(PI/30)+PI/2),240+65*sin((s)*(PI/30)+PI/2));
							
	s0=s;
	m0=m1;
	h0=h1;
	
}
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<dos.h>
#include<string.h>
#define LEN sizeof(struct student)
#define FORMAT "%-8d%-15s%-12.1lf%-12.1lf%-12.1lf%-12.1lf\n"
#define DATA stu[i].num,stu[i].name,stu[i].elec,stu[i].expe,stu[i].requ,stu[i].sum
struct student/*定义学生成绩结构体*/
{ int num;/*学号*/
  char name[15];/*姓名*/
  double elec;/*选修课*/
  double expe;/*实验课*/
  double requ;/*必修课*/
  double sum;/*总分*/
};
struct student stu[50];/*定义结构体数组*/
void in();/*录入学生成绩信息*/
void show();/*显示学生信息*/
void order();/*按总分排序*/
void del();/*删除学生成绩信息*/
void modify();/*修改学生成绩信息*/
void menu();/*主菜单*/
void insert();/*插入学生信息*/
void total();/*计算总人数*/
void search();/*查找学生信息*/

void main()/*主函数*/
{ int n;

  menu();
  scanf("%d",&n);/*输入选择功能的编号*/
  while(n)
  { switch(n)
     { case 1: in();break;
       case 2: search();break;
       case 3: del();break;
       case 4: modify();break;
       case 5: insert();break;
       case 6: order();break;
       case 7: total();break;
	   case 8: show();break;
       default:break;
     }
    getch();
    menu();/*执行完功能再次显示菜单界面*/
    scanf("%d",&n);
  }
}

void in()/*录入学生信息*/
{ int i,m=0;/*m是记录的条数*/
  char ch[2];  
  FILE *fp;/*定义文件指针*/
  if((fp=fopen("data.txt","a+"))==NULL)/*打开指定文件*/
  { 
	  printf("can not open\n");
	  return;
  }
  while(!feof(fp))
  {
	  if(fread(&stu[m] ,LEN,1,fp)==1)
		  m++;/*统计当前记录条数*/
  }
  fclose(fp);
  if(m==0)
	  printf("No record!\n");
  else
  {
	  system("cls");
          show();/*调用show函数，显示原有信息*/
  }

  if((fp=fopen("data.txt","wb"))==NULL)
  {
	  printf("can not open\n");
	  return;
  }
  for(i=0;i<m;i++)
  {
   fwrite(&stu[i] ,LEN,1,fp);/*向指定的磁盘文件写入信息*/
  }

  printf("please input(y/n):");
  scanf("%s",ch);
while(strcmp(ch,"Y")==0||strcmp(ch,"y")==0)/*判断是否要录入新信息*/
	{
    printf("number:");
	scanf("%d",&stu[m].num);/*输入学生学号*/
   for(i=0;i<m;i++)
	    if(stu[i].num==stu[m].num)
	    {
	    printf("the number is existing,press any to continue!");
	    getch();
	    fclose(fp);
	    return;
	    }
	
     printf("name:");
	 scanf("%s",stu[m].name);/*输入学生姓名*/

     printf("elective:");

     scanf("%lf",&stu[m].elec);/*输入选修课成绩*/
     printf("experiment:");
	 scanf("%lf",&stu[m].expe);/*输入实验课成绩*/
     printf("required course:");
	 scanf("%lf",&stu[m].requ);/*输入必修课成绩*/
     stu[m].sum=stu[m].elec+stu[m].expe+stu[m].requ;/*计算出总成绩*/
     if(fwrite(&stu[m],LEN,1,fp)!=1)/*将新录入的信息写入指定的磁盘文件*/
     {
		 printf("can not save!");
		 getch();
	 }
     else
	 {
		 printf("%s saved!\n",stu[m].name);
		 m++;
	 }
     printf("continue?(y/n):");/*询问是否继续*/
     scanf("%s",ch);
  }
  fclose(fp);
  printf("OK!\n");
}

void show()
 { FILE *fp;
   int i,m=0;
   fp=fopen("data.txt","rb");
   while(!feof(fp))
   {
   if(fread(&stu[m] ,LEN,1,fp)==1) 
   m++;
   }  
   fclose(fp);
   printf("number  name           elective    experiment  required    sum\t\n");
   for(i=0;i<m;i++)
       { 
	   printf(FORMAT,DATA);/*将信息按指定格式打印*/
       }
     }
 
void menu()/*自定义函数实现菜单功能*/
{
  system("cls");
  printf("\n\n\n\n\n");
  printf("\t\t|---------------------STUDENT-------------------|\n");
  printf("\t\t|\t 0. exit                                |\n");
  printf("\t\t|\t 1. input record                        |\n");
  printf("\t\t|\t 2. search record                       |\n");
  printf("\t\t|\t 3. delete record                       |\n");
  printf("\t\t|\t 4. modify record                       |\n");
  printf("\t\t|\t 5. insert record                       |\n");
  printf("\t\t|\t 6. order                               |\n");
  printf("\t\t|\t 7. number                              |\n");
  printf("\t\t|\t 8. show                                |\n");
  printf("\t\t|-----------------------------------------------|\n\n");
  printf("\t\t\tchoose(0-8):");
}

void order()/*自定义排序函数*/
{ FILE *fp;
  struct student t;
  int i=0,j=0,m=0;
  if((fp=fopen("data.txt","r+"))==NULL)
     { 
	printf("can not open!\n");
        return;
  }
  while(!feof(fp)) 
  if(fread(&stu[m] ,LEN,1,fp)==1) 
	  m++;
  fclose(fp);
  if(m==0) 
  {
	  printf("no record!\n");
	  return;
  }
  if((fp=fopen("data.txt","wb"))==NULL)
     {
	  printf("can not open\n");
	  return;}
  for(i=0;i<m-1;i++)
      for(j=i+1;j<m;j++)/*双重循环实现成绩比较并交换*/
          if(stu[i].sum<stu[j].sum)
          { t=stu[i];stu[i]=stu[j];stu[j]=t;}
	  if((fp=fopen("data.txt","wb"))==NULL)
     { printf("can not open\n");return;}
  for(i=0;i<m;i++)/*将重新排好序的内容重新写入指定的磁盘文件中*/
      if(fwrite(&stu[i] ,LEN,1,fp)!=1)
       { 
        printf("%s can not save!\n"); 
        getch();
      }
  fclose(fp);
  printf("save successfully\n");
}


void del()/*自定义删除函数*/
{
	FILE *fp;
	int snum,i,j,m=0;
	char ch[2];
	if((fp=fopen("data.txt","r+"))==NULL)
	{
		printf("can not open\n");return;}
		while( !feof(fp) )  
			if(fread(&stu[m],LEN,1,fp)==1) m++;
			fclose(fp);
		if(m==0) 
		{
			printf("no record!\n");
			return;
		}
  show();
		printf("please input the number:");
		scanf("%d",&snum);

		for(i=0;i<m;i++)
		{
			if(snum==stu[i].num)
			{    
				printf("find the student,delete?(y/n)");
				scanf("%s",ch);
				if(strcmp(ch,"Y")==0||strcmp(ch,"y")==0)/*判断是否要进行删除*/
				for(j=i;j<m;j++)
				stu[j]=stu[j+1];/*将后一个记录移到前一个记录的位置*/
				m--;/*记录的总个数减1*/
				if((fp=fopen("data.txt","wb"))==NULL)
				{ printf("can not open\n");return;}
				for(j=0;j<m;j++)/*将更改后的记录重新写入指定的磁盘文件中*/
				if(fwrite(&stu[j] ,LEN,1,fp)!=1)
				{ printf("can not save!\n");
				getch();}
				fclose(fp);
				printf("delete successfully!\n");
				return;
			}
		}
		printf("没有找到要删除的信息！\n");
}



void search()/*自定义查找函数*/
{ FILE *fp;
  int snum,i,m=0;
  char ch[2];
  if((fp=fopen("data.txt","rb"))==NULL)
     { printf("can not open\n");return;}
  while(!feof(fp))  if(fread(&stu[m],LEN,1,fp)==1) m++;
  fclose(fp);
  if(m==0) {printf("no record!\n");return;}
  printf("please input the number:");
  scanf("%d",&snum);
  for(i=0;i<m;i++)
     if(snum==stu[i].num)/*查找输入的学号是否在记录中*/
     { printf("find the student,show?(y/n)");
     scanf("%s",ch);
      if(strcmp(ch,"Y")==0||strcmp(ch,"y")==0) 
        {
          printf("number  name           elective    experiment  required    sum\t\n");
          printf(FORMAT,DATA);/*将查找出的结果按指定格式输出*/
	  break;
     }
     }   
  if(i==m) printf("can not find the student!\n");/*未找到要查找的信息*/
}

void modify()/*自定义修改函数*/
{ FILE *fp;
  int i,j,m=0,snum;
  if((fp=fopen("data.txt","r+"))==NULL)
     { printf("can not open\n");return;}
  while(!feof(fp))  
 if(fread(&stu[m],LEN,1,fp)==1) m++;
  if(m==0) 
  {
	printf("no record!\n");
	fclose(fp);
	return;
  }
  show();
  printf("please input the number of the student which do you want to modify!\n");
  printf("modify number:");
  scanf("%d",&snum);
  for(i=0;i<m;i++)
  {
	if(snum==stu[i].num)/*检索记录中是否有要修改的信息*/
	{
	  printf("find the student!you can modify!\n");
	 
	  printf("name:");
	  scanf("%s",stu[i].name);/*输入名字*/
          printf("elective:");
	  scanf("%lf",&stu[i].elec);/*输入选修课成绩*/
          printf("experiment:");
	  scanf("%lf",&stu[i].expe);/*输入实验课成绩*/
          printf("required course:");
	  scanf("%lf",&stu[i].requ);/*输入必修课成绩*/
	  printf("modify successful!");
	  
		stu[i].sum=stu[i].elec+stu[i].expe+stu[i].requ;
	  if((fp=fopen("data.txt","wb"))==NULL)
     { printf("can not open\n");return;}
	  for(j=0;j<m;j++)/*将新修改的信息写入指定的磁盘文件中*/
	  if(fwrite(&stu[j] ,LEN,1,fp)!=1)
       { printf("can not save!"); getch(); }
	fclose(fp);
	return ;
	}	 
  }
	printf("没有找到匹配信息！\n");
 }

void insert()/*自定义插入函数*/
{ FILE *fp;
  int i,j,k,m=0,snum;
  if((fp=fopen("data.txt","r+"))==NULL)
     { printf("can not open\n");return;}
  while(!feof(fp))  
 if(fread(&stu[m],LEN,1,fp)==1) m++;
  if(m==0) {printf("no record!\n");
  fclose(fp);
  return;
  }
  printf("please input position where do you want to insert!(input the number)\n");
  scanf("%d",&snum);/*输入要插入的位置*/
  for(i=0;i<m;i++)
	  if(snum==stu[i].num)
		  break;
	  for(j=m-1;j>i;j--)
           stu[j+1]=stu[j];/*从最后一条记录开始均向后移一位*/
	  printf("now please input the new information.\n");
          printf("number:");
	  scanf("%d",&stu[i+1].num);
	  for(k=0;k<m;k++)
	    if(stu[k].num==stu[m].num)
	    {
	    printf("the number is existing,press any to continue!");
	    getch();
	    fclose(fp);
	    return;
	    }
	  printf("name:");
	  scanf("%s",stu[i+1].name);
          printf("elective:");
	  scanf("%lf",&stu[i+1].elec);
          printf("experiment:");
	  scanf("%lf",&stu[i+1].expe);
          printf("required course:");
	  scanf("%lf",&stu[i+1].requ);
	  stu[i+1].sum=stu[i+1].elec+stu[i+1].expe+stu[i+1].requ;
	  if((fp=fopen("data.txt","wb"))==NULL)
     { printf("can not open\n");return;}
	  for(k=0;k<=m;k++)
	  if(fwrite(&stu[k] ,LEN,1,fp)!=1)/*将修改后的记录写入磁盘文件中*/
       { printf("can not save!"); getch(); }
  fclose(fp);
 }

void total()
{ FILE *fp;
  int m=0;
  if((fp=fopen("data.txt","r+"))==NULL)
     { printf("can not open\n");return;}
  while(!feof(fp))  
	  if(fread(&stu[m],LEN,1,fp)==1) 
		  m++;/*统计记录个数即学生个数*/
  if(m==0) {printf("no record!\n");fclose(fp);return;}
  printf("the class are %d students!\n",m);/*将统计的个数输出*/
  fclose(fp);
 }
/*#include <malloc.h>*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN sizeof(struct scorenode)
#define DEBUG

struct scorenode
{int number;/*学号*/
char name[10];/*姓名*/
int xiaofei;/*消费情况*/
struct scorenode *next;
};
typedef struct scorenode score;
int n,k;/*n,k为全局变量，本程序中的函数均可*p3以使用它*/
void menu();
score *creat(void);
score *load(score *head);
score *search(score *head);
score *del(score *head);
score *add(score *head,score *stu);
void print(score *head);
save(score *p1);
/*==============================================================================================*/
/*=========================创建链表，此函数带回一个指向链表头的指针=============================*/
score *creat(void)
{

	score *head;
    score *p1,*p2,*p3,*max;
	int i,j;
    char t[10];
	n=0;
    p1=p2=p3=(score *)malloc(LEN);/*head=p3; 开辟一个新单元*/
      printf("please input student's information,input 0 exit!\n");
repeat1: printf("please input student's number(number>0):");/*输入学号，学号应大于0*/
       scanf("   %d",&p1->number);
     while(p1->number<0)
	 {
		 getchar();
		 printf("error，please input number again:");
         scanf("%d",&p1->number);
	 }
  /*输入学号为字符或小于0时，程序报错，提示重新输入学号*/
	if(p1->number==0)
		goto end;/*当输入的学号为0时，转到末尾，结束创建链表*/
	else
	{
		p3=head;
		if(n>0)
		{
			for(i=0;i<n;i++)
			{
				if(p1->number!=p3->number)
					p3=p3->next;
				else
				{
					printf("number repeate,please input again!\n");
					goto repeat1;
     /*当输入的学号已经存在，程序报错，返回前面重新输入*/
			}
			}
		}
	 }
       printf("please input student's name:");
       scanf("%s",&p1->name);/*输入学生姓名*/
       printf("please input student's consume money:");/*输入消费情况;*/
	 scanf("%d",&p1->xiaofei);

 while(p1->number!=0)
  {
    n=n+1;
 if(n==1)
  head=p1;
 else

    p2->next=p1;
    p2=p1;
    p1=(score *)malloc(LEN);
    printf("please input student's information,input 0 exit!\n");
repeat2:printf("please input student's number(number>0):");
      scanf("%d",&p1->number);/*输入学号，学号应大于0*/

   while(p1->number<0)
   {getchar();
    printf("error,please input number again:");
       scanf("%d",&p1->number);}
   /*输入学号为字符或小于0时，程序报错，提示重新输入学号*/
  if(p1->number==0)
   goto end;/*当输入的学号为0时，转到末尾，结束创建链表*/
  else
  {
  p3=head;
  if(n>0)
   {for(i=0;i<n;i++)
      {if(p1->number!=p3->number)
    p3=p3->next;
    else
    {printf("number repeate,please input again!\n");
     goto repeat2;
     /*当输入的学号已经存在，程序报错，返回前面重新输入*/
    }
    }
   }
  }
  printf("please input student's name:");
       scanf("%s",&p1->name);/*输入学生姓名*/
     printf("please input student's consume money:");
       scanf("%d",&p1->xiaofei);/*输入消费情况; */
}
end: p1=head;
     p3=p1;
  for(i=1;i<n;i++)
               {
             for(j=i+1;j<=n;j++)
                 {
       max=p1;
       p1=p1->next;

       if(max->number>p1->number)
        {
        k=max->number;
        max->number=p1->number;
        p1->number=k;
         /*交换前后结点中的学号值，使得学号大者移到后面的结点中*/

        strcpy(t,max->name);
        strcpy(max->name,p1->name);
        strcpy(p1->name,t);
     /*交换前后结点中的姓名，使之与学号相匹配*/
                        /*交换前后结点中的消费情况，使之与学号相匹配*/
           }
                 }
    max=head;p1=head;/*重新使max,p指向链表头*/
  }
p2->next=NULL;/*链表结尾*/
       printf("input student's num:%d ge!\n",n);
	   getch();
       return(head);

}
/*==============================================================================================*/
/*===========================从文件读入学生记录=================================================*/
score *load(score *head)
{
		score *p1,*p2;
        int m=0;
        char filepn[10];
		FILE *fp;

		printf("please input file's postion and its name:");
		scanf("%s",filepn);/*输入文件路径及名称*/
		if((fp=fopen(filepn,"r+"))==NULL)
		{
			printf("can't open this file!\n");
			getch();
			return 0;
		}
		else
		{
			p1=(score *)malloc(LEN); /*开辟一个新单元*/
			fscanf(fp,"%d%s%d\n",&p1->number,p1->name,&p1->xiaofei);
			printf("|%d\t|%s\t|%d\t\n",p1->number,p1->name,p1->xiaofei);
			/*文件读入与显示*/
			head=NULL;
			do
			{
				n=n+1;
				if(n==1)
					head=p1;
				else
					p2->next=p1;
				p2=p1;
				p1=(score *)malloc(LEN);  /*开辟一个新单元*/
				fscanf(fp,"%d%s%d\n",&p1->number,p1->name,&p1->xiaofei);
				printf("|%d\t|%s\t|%d\t\n",p1->number,p1->name,p1->xiaofei);
				/*文件读入与显示*/
			}while(!feof(fp));
			p2->next=p1;
			p1->next=NULL;
			n=n+1;
		}
		printf("-----------------------------------------\n");/*表格下线*/
		getch();
		fclose(fp);/*结束读入，关闭文件*/

		return (head);
}

/*==============================================================================================*/
/*=====================查询学生消费=====================================================*/
score *search(score *head)
{
	int number;
	score *p1,*p2;
	printf("input the student's number of searching:");
	scanf("%d",&number);
	getchar();
	while(number!=0)
	{
		if(head==NULL)
		{
			printf("\n nobody information!\n");
			return(head);
		}
		printf("-----------------------------------------\n");
		printf("|number\t|name\t|consume\t \n");
		printf("-----------------------------------------\n");/*打印表格域*/
		p1=head;
		while(number!=p1->number&&p1->next!=NULL)
		{
			p2=p1;
			p1=p1->next;
		}
        if(number==p1->number)
		{
			 printf("|%d\t|%s\t|%d\t\n",p1->number,p1->name,p1->xiaofei);
			 printf("-----------------------------------------\n");

		}/*打印表格域*/
        else

			printf("%dthis student not exist!\n",number);
		printf("input the student's number of searching:");
		scanf("%d",&number);
		getchar();
	}
		printf("already exit!\n");
		getchar();
		return(head);
}
/*==============================================================================================*/
/*=======================删除学生资料================================================*/
score *del(score *head)
{
	score *p1,*p2;
	int number;
	printf("input the student's number of deleting(input 0 exit):");
	scanf("%d",&number);
	getchar();
	while(number!=0)/*输入学号为0时退出*/
	{

		if(head==NULL)
		{
		printf("\nnobody information!\n");
		return(head);
		}

		p1=head;

		while(number!=p1->number&&p1->next!=NULL)
   /*p1指向的不是所要找的首结点，并且后面还有结点*/
		{
		p2=p1;
		p1=p1->next;
		} /*p1后移一个结点*/

		if(number==p1->number)
   /*找到了*/
		{
		if(p1==head)
		head=p1->next;
   /*若p1指向的是首结点，把地二个结点地址赋予head*/
		else
		p2->next=p1->next;
   /*否则将下一个结点地址 赋给前一结点地址*/
		printf("delete number:%d\n",number);
		n=n-1;
		}
		else
		printf("%d student not exist!\n",number);
  /*找不到该结点*/


	printf("input the student's number of deleting:");
	scanf("%d",&number);
	getchar();
	}
#ifdef DEBUG
 printf("already exit\n");
#endif
 printf("now how many students:%d ge!\n",n);
 getch();
 return(head);
}
/*==============================================================================================*/
/*==============================================================================================*/
score *add(score *head,score *stu)

{
	score *p0,*p1,*p2,*p3,*max;
    int i,j;
    char t[10];
	p3=stu=(score *)malloc(LEN);/*开辟一个新单元*/
	printf("\ninput the student's information of adding!");
	repeat4: printf("please input the student's number(number>0):");
	scanf("%d",&stu->number);
  /*输入学号，学号应大于0*/
    while(stu->number<0)
	{
		getchar();
		printf("error,please input number again:");
		scanf("%d",&stu->number);
	}/*输入错误，重新输入学号*/
     /******************************************************/
    if(stu->number==0)
	goto end2;/*当输入的学号为0时，转到末尾，结束追加*/
	else
	{
		p3=head;
		if(n>0)
		{
			for(i=0;i<n;i++)
			{
				if(stu->number!=p3->number)
					p3=p3->next;
				else
				{
					printf("number repeat,please input again!\n");
					goto repeat4;
     /*当输入的学号已经存在，程序报错，返回前面重新输入*/
				}
			}
		}
	}

  /******************************************************/
   printf("input the student's name:");
   scanf("%s",stu->name);            /*输入学生姓名*/
   printf("please input the consuming:");
   scanf("%d",&stu->xiaofei);    
   p1=head;
   p0=stu;
	if(head==NULL)
	{
	   head=p0;
	   p0->next=NULL;
	}/*当原来链表为空时，从首结点开始存放资料*/
	else/*原来链表不为空*/
	{
		if(p1->next==NULL)/*找到原来链表的末尾*/
		{
			p1->next=p0;
			p0->next=NULL;/*将它与新开单元相连接*/
		}
		else
		{
			while(p1->next!=NULL)/*还没找到末尾，继续找*/
			{
				p2=p1;
				p1=p1->next;
			}
			p1->next=p0;
			p0->next=NULL;
		}
	}
	n=n+1;
	p1=head;
	p0=stu;
	for(i=1;i<n;i++)
	{
		for(j=i+1;j<=n;j++)
        {
			max=p1;
			p1=p1->next;
			if(max->number>p1->number)
			{
			k=max->number;
			max->number=p1->number;
			p1->number=k;
         /*交换前后结点中的学号值，使得学号大者移到后面的结点中*/

			strcpy(t,max->name);
			strcpy(max->name,p1->name);
			strcpy(p1->name,t);
     /*交换前后结点中的姓名，使之与学号相匹配*/
                  /*交换前后结点中的消费情况，使之与学号相匹配*/
            }
			max=head;
			p1=head;/*重新使max,p指向链表头*/
		}
	}
	end2:
		printf("now how many students are they:%d ge!\n",n);
		getch();
		return(head);
}


 /*==============================================================================================*/
 /*==============================================================================================*/
void print(score *head)

{
 score *p;
 if(head==NULL)
         printf("\nnobody information!\n");
 else
 {
	 printf("%d\n",n);
 printf("-----------------------------------------\n");
 printf("|number\t|name\t|consume\t |\n");
 printf("-----------------------------------------\n");/*打印表格域*/
 p=head;
     do
    {printf("|%d\t|%s\t|%d\t\n",p->number,p->name,p->xiaofei);
        printf("-----------------------------------------\n");/*打印表格域*/
       p=p->next;


	}while (p!=NULL);/*打印完成了*/
	getch();
 }
}


/*==============================================================================================*/
/*==============================================================================================*/
save(score *p1)
{
 FILE *fp;
 if((fp=fopen("f:\\consume","wb"))==NULL)
 {
  printf("can't open this file!\n");
  return 0;
 }
 else
 {
	while(p1!=NULL)
	{
	fprintf(fp,"%d,%s,%d\t\t\t",p1->number,p1->name,p1->xiaofei);

	/*	printf("file write error\n");*/
	p1=p1->next;
	}
	printf("file save complete!please enter return!\n");
	getch();
 }
	 fclose(fp);


}





/*==============================================================================================*/
/*==================================主菜单===================================================*/
void menu()
{

	system("cls");
	printf("\n\n\n");
	printf("\t\t-------------STUDENT CONSUME-------------\n");
	printf("\t\t\t0  exit                                \n");
	printf("\t\t\t1  creat                               \n");
	printf("\t\t\t2  load                                \n");
	printf("\t\t\t3  search                              \n");
	printf("\t\t\t4  delete                              \n");
	printf("\t\t\t5  add                                 \n");
	printf("\t\t\t6  show                                \n");
	printf("\t\t\t7  save                                \n");
	printf("\t\t-----------------------------------------\n\n");
	printf("\t\tchoose(0-7):");


}
/*===============================主函数================================================*/
main()
{
	int num;
	score *head=0,*stu=0;
	menu();
	scanf("%d",&num);
	while(1)
	{
		switch(num)
		{
		case 1: head=creat();break;
		case 2: head=load(head);break;
		case 3: head=search(head);break;
		case 4: head=del(head);break;
		case 5: head=add(head,stu);break;
		case 6: print(head);break;
		case 7: save(head);break;
	
		case 0: exit(0);
		default:printf("Input error,please again!");

		}

		menu();
		scanf("%d",&num);

}



}
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#define HEADER1 " -------------------------------BOOK TICKET----------------------------------\n"
#define HEADER2 " |  number  |start city|reach city|takeofftime|receivetime|price|ticketnumber|\n"
#define HEADER3 " |----------|----------|----------|-----------|-----------|-----|------------|\n"
#define FORMAT  " |%-10s|%-10s|%-10s|%-10s |%-10s |%5d|  %5d     |\n"
#define DATA p->data.num,p->data.startcity,p->data.reachcity,p->data.takeofftime,p->data.receivetime,p->data.price,p->data.ticketnum
int saveflag=0 ;
/*定义存储火车信息的结构体*/
struct train 
{
    char num[10];/*列车号*/
    char startcity[10];/*出发城市*/
    char reachcity[10];/*目的城市*/
    char takeofftime[10];/*发车时间*/
    char receivetime[10];/*到达时间*/
    int  price;/*票价*/
    int  ticketnum ;/*票数*/
};
/*订票人的信息*/
struct man 
{
    char num[10];/*ID*/
    char name[10];/*姓名*/
    int  bookNum ;/*订的票数*/
};
/*定义火车信息链表的结点结构*/
typedef struct node 
{
    struct train data ;
    struct node * next ;
}Node,*Link ;
/*定义订票人链表的结点结构*/
typedef struct Man 
{
    struct man data ;
    struct Man *next ;
}book,*bookLink ;
/* 初始界面*/
void menu()
{
    puts("\n\n");
    puts("\t\t|------------------------------------------------------|");
    puts("\t\t|                   Booking Tickets                    |");
    puts("\t\t|------------------------------------------------------|");
    puts("\t\t|       0:quit the system                              |");
    puts("\t\t|       1:Insert a train information                   |");
    puts("\t\t|       2:Search a train information                   |");
    puts("\t\t|       3:Book a train ticket                          |");
    puts("\t\t|       4:Modify the train information                 |");
    puts("\t\t|       5:Show the train information                   |");
    puts("\t\t|       6:save information to file                     |");
    puts("\t\t|------------------------------------------------------|");
}
/*添加一个火车信息*/
void Traininfo(Link linkhead)
{
    struct node *p,*r,*s ;
    char num[10];
    r = linkhead ;
    s = linkhead->next ;
    while(r->next!=NULL)
    r=r->next ;
    while(1)
    {
        printf("please input the number of the train(0-return)");
        scanf("%s",num);
        if(strcmp(num,"0")==0)
          break ;
        /*判断是否已经存在*/
        while(s)
        {
            if(strcmp(s->data.num,num)==0)
            {
                printf("the train '%s'is existing!\n",num);
                return ;
            }
            s = s->next ;
        }
        p = (struct node*)malloc(sizeof(struct node));
        strcpy(p->data.num,num);/*输入车号*/
	 printf("Input the city where the train will start:");
        scanf("%s",p->data.startcity);/*输入出发城市*/
        printf("Input the city where the train will reach:");
        scanf("%s",p->data.reachcity);/*输入到站城市*/
        printf("Input the time which the train take off:");
	scanf("%s",p->data.takeofftime);/*输入出发时间*/
        printf("Input the time which the train receive:");
	scanf("%s",&p->data.receivetime);/*输入到站时间*/
        printf("Input the price of ticket:");
        scanf("%d",&p->data.price);/*输入火车票价*/
        printf("Input the number of booked tickets:");
	scanf("%d",&p->data.ticketnum);/*输入预定票数*/
        p->next=NULL ;
        r->next=p ;/*插入到链表中*/
        r=p ;
       saveflag = 1 ;
    }
}
/*打印火车票信息*/
void printheader() /*格式化输出表头*/
{
printf(HEADER1);
printf(HEADER2);
printf(HEADER3);
}
void printdata(Node *q) /*格式化输出表中数据*/
{
Node* p;
p=q;
printf(FORMAT,DATA);
}

/*查询火车信息*/
void searchtrain(Link l)

{
    Node *s[10],*r;
    int sel,k,i=0 ;
    char str1[5],str2[10];
    if(!l->next)
    {
        printf("There is not any record !");
        return ;
    }
    printf("Choose the way:\n1:according to the number of train;\n2:according to the city:\n");
    scanf("%d",&sel);/*输入选择的序号*/
    if(sel==1)
    {
        printf("Input the the number of train:");
        scanf("%s",str1);
        r=l->next;
	while(r!=NULL)
        if(strcmp(r->data.num,str1)==0)/*检索是否有与输入的车号相匹配的*/
        {
            s[i]=r;
	    i++;
	    break;
        }
        else 
            r=r->next;
    }
    else if(sel==2)
    {
        printf("Input the city  you want to go:");
        scanf("%s",str2);
        r=l->next;
	while(r!=NULL)
        if(strcmp(r->data.reachcity,str2)==0)/*检索是否有与输入的城市相匹配的火车*/
        {
            s[i]=r;
	    i++;
	    r=r->next;
        }
        else 
            r=r->next;
    }
    	if(i==0)
		printf("can not find!");
	else
	{
		printheader();
    for(k=0;k<i;k++)
printdata(s[k]);
	}
}

/*订票子模块*/
void Bookticket(Link l,bookLink k)
{
    Node *r[10],*p ;
    char ch[2],tnum[10],str[10],str1[10],str2[10];
    book *q,*h ;
    int i=0,t=0,flag=0,dnum;
    q=k ;
    while(q->next!=NULL)
    q=q->next ;
    printf("Input the city you want to go: ");
    scanf("%s",&str);/*输入要到达的城市*/
    p=l->next ;
    while(p!=NULL)
    {
        if(strcmp(p->data.reachcity,str)==0)
        {
            r[i]=p ;/*将满足条件的记录存到数组r中*/
            i++;
        }
        p=p->next ;
    }
    printf("\n\nthe number of record have %d\n",i);
       printheader();
    for(t=0;t<i;t++)
	    printdata(r[t]);
    if(i==0)
    printf("\nSorry!Can't find the train for you!\n");
    else
    {
        printf("\ndo you want to book it?<y/n>\n");
        scanf("%s",ch);
	if(strcmp(ch,"Y")==0||strcmp(ch,"y")==0)/*判断是否订票*/
        {
	    h=(book*)malloc(sizeof(book));
            printf("Input your name: ");
            scanf("%s",&str1);
            strcpy(h->data.name,str1);
            printf("Input your id: ");
            scanf("%s",&str2);
            strcpy(h->data.num,str2);
	    printf("please input the number of the train:");
	    scanf("%s",tnum);
	    for(t=0;t<i;t++)
	    if(strcmp(r[t]->data.num,tnum)==0)
	    {
	       if(r[t]->data.ticketnum<1)/*判断剩余的供订票的票数是否为0*/
	       {
		       printf("sorry,no ticket!");
		       sleep(2);
		       return;
	       }
	      printf("remain %d tickets\n",r[t]->data.ticketnum);
               flag=1;
	       break;
	    }
	    if(flag==0)
	    {
		    printf("input error");
		   sleep(2);
                    return;
	    }
	    printf("Input your bookNum: ");
            scanf("%d",&dnum);
            r[t]->data.ticketnum=r[t]->data.ticketnum-dnum;/*定票成功则可供订的票数相应减少*/
	    h->data.bookNum=dnum ;
            h->next=NULL ;
	    q->next=h ;
	    q=h ;
            printf("\nLucky!you have booked a ticket!");
            getch();
            saveflag=1 ;
        }
    }
}
/*修改火车信息*/
void Modify(Link l)
{
    Node *p ;
    char tnum[10],ch ;
    p=l->next;
    if(!p)
    {
        printf("\nthere isn't record for you to modify!\n");
        return ;
    }
    else
    {
	    printf("\nDo you want to modify it?(y/n)\n");
            getchar();
            scanf("%c",&ch);
            if(ch=='y'||ch=='Y')
            {
                printf("\nInput the number of the train:");
		scanf("%s",tnum);
	while(p!=NULL)
	if(strcmp(p->data.num,tnum)==0)/*查找与输入的车号相匹配的记录*/
	    break;
        else
            p=p->next;
                if(p)
                {
                    printf("Input new number of train:");
                    scanf("%s",&p->data.num);
		    printf("Input new city the train will start:");
                    scanf("%s",&p->data.startcity);
                    printf("Input new city the train will reach:");
                    scanf("%s",&p->data.reachcity);
                    printf("Input new time the train take off");
		    scanf("%s",&p->data.takeofftime);
                    printf("Input new time the train reach:");
		    scanf("%s",&p->data.receivetime);
                    printf("Input new price of the ticket::");
                    scanf("%d",&p->data.price);
                    printf("Input new number of people who have booked ticket:");
                    scanf("%d",&p->data.ticketnum);
                    printf("\nmodifying record is sucessful!\n");
                    saveflag=1 ;
                }
                else
                printf("\tcan't find the record!");
            }
    }
}
void showtrain(Link l)/*自定义函数显示列车信息*/
{
Node *p;
p=l->next;
printheader();
if(l->next==NULL)
printf("no records!");
else
 while(p!=NULL)
{
	printdata(p);
	p=p->next;
}
}
/*保存火车信息*/
void SaveTrainInfo(Link l)
{
    FILE*fp ;
    Node*p ;
    int count=0,flag=1 ;
    fp=fopen("f:\\train.txt","wb");
    if(fp==NULL)
    {
        printf("the file can't be opened!");
        return ;
    }
    p=l->next ;
    while(p)
    {
        if(fwrite(p,sizeof(Node),1,fp)==1)
        {
            p=p->next ;
            count++;
        }
        else
        {
            flag=0 ;
            break ;
        }
    }
    if(flag)
    {
        printf(" saved %d train records\n",count);
        saveflag=0 ;
    }
    fclose(fp);
}
/*保存订票人的信息*/
void SaveBookInfo(bookLink k)
{
    FILE*fp ;
    book *p ;
    int count=0,flag=1 ;
    fp=fopen("f:\\man.txt","wb");
    if(fp==NULL)
    {
        printf("the file can't be opened!");
        return ;
    }
    p=k->next ;
    while(p)
    {
	if(fwrite(p,sizeof(book),1,fp)==1)
        {
            p=p->next ;
            count++;
        }
        else
        {
            flag=0 ;
            break ;
        }
    }
    if(flag)
    {
        printf(" saved %d booking records\n",count);
        saveflag=0 ;
    }
    fclose(fp);
}

main()
{
    FILE*fp1,*fp2 ;
    Node *p,*r ;
    char ch1,ch2 ;
    Link l ;
    bookLink k ;
    book *t,*h ;
    int sel ;
    l=(Node*)malloc(sizeof(Node));
    l->next=NULL ;
    r=l ;
    k=(book*)malloc(sizeof(book));
    k->next=NULL ;
    h=k ;
    fp1=fopen("f:\\train.txt","ab+");/*打开存储车票信息的文件*/
    if((fp1==NULL))
    {
        printf("can't open the file!");
        return 0 ;
    }
    while(!feof(fp1))
    {
        p=(Node*)malloc(sizeof(Node));
        if(fread(p,sizeof(Node),1,fp1)==1)/*从指定磁盘文件读取记录*/
        {
            p->next=NULL ;
            r->next=p ;/*构造链表*/
            r=p ;
       
        }
    }
    fclose(fp1);
    fp2=fopen("f:\\man.txt","ab+");
    if((fp2==NULL))
    {
        printf("can't open the file!");
        return 0 ;
    }
    
    while(!feof(fp2))
    {
        t=(book*)malloc(sizeof(book));
        if(fread(t,sizeof(book),1,fp2)==1)
        {
            t->next=NULL ;
            h->next=t ;
            h=t ;
           
        }
    }
    fclose(fp2);
    while(1)
    {
        clrscr();
        menu();
        printf("\tplease choose (0~6):  ");
        scanf("%d",&sel);
        clrscr();
        if(sel==0)
        {
	    if(saveflag==1)/*当退出时判断信息是否保存*/
            {
                getchar();
                printf("\nthe file have been changed!do you want to save it(y/n)?\n");
                scanf("%c",&ch1);
                if(ch1=='y'||ch1=='Y')
                {
		    SaveBookInfo(k);
                    SaveTrainInfo(l);
                }
            }
            printf("\nThank you!!You are welcome too\n");
            break ;

        }
        switch(sel)/*根据输入的sel值不同选择相应操作*/
        {
            case 1 :
              Traininfo(l);break ;
            case 2 :
              searchtrain(l);break ;
            case 3 :
              Bookticket(l,k);break ;
            case 4 :
              Modify(l);break ;
	    case 5:
		    showtrain(l);break;
            case 6 :
	      SaveTrainInfo(l);SaveBookInfo(k);break ;
            case 0:
            return 0;
        }
        printf("\nplease press any key to continue.......");
        getch();
    }
}
#include<stdio.h>
#include<stdlib.h>
#include<dos.h>
#include<string.h>
typedef struct Info
{
char name[15];/*姓名*/
char city[10];/*城市*/
char province[10];/*省*/
char state[10];/*国家*/
char tel[15];/*电话*/
};
typedef struct node/*定义通讯录链表的结点结构*/
{
struct Info data;
struct node *next;
}Node,*link;

void stringinput(char *t,int lens,char *notice)
{
   char n[50];
   do{
      printf(notice); /*显示提示信息*/
      scanf("%s",n); /*输入字符串*/
      if(strlen(n)>lens)printf("\n exceed the required length! \n"); /*超过lens值重新输入*/
     }while(strlen(n)>lens);
   strcpy(t,n); /*将输入的字符串拷贝到字符串t中*/
}
void enter(link l)/*输入记录*/
{
Node *p,*q;
q=l;
while(1)
{
p=(Node*)malloc(sizeof(Node));/*申请结点空间*/
if(!p)/*未申请成功输出提示信息*/
{
printf("memory malloc fail\n");
return;
}
stringinput(p->data.name,15,"enter name:");/*输入姓名*/
if(strcmp(p->data.name,"0")==0)/*检测输入的姓名是否为0*/
break;
stringinput(p->data.city,10,"enter city:");/*输入城市*/
stringinput(p->data.province,10,"enter province:");/*输入省*/
stringinput(p->data.state,10,"enter status:");/*输入国家*/
stringinput(p->data.tel,15,"enter telephone:");/*输入电话号码*/
p->next=NULL;
 q->next=p;
 q=p;
}
}

void del(link l)
{
Node *p,*q;
char s[20];
q=l;
p=q->next;
printf("enter name:");
scanf("%s",s);/*输入要删除的姓名*/
while(p)
{
if(strcmp(s,p->data.name)==0)/*查找记录中与输入名字匹配的记录*/
{q->next=p->next;/*删除p结点*/
free(p);/*将p结点空间释放*/
printf("delete successfully!");
break;
}
else
{
q=p;
p=q->next;
}
}
getch();
}
void display(Node *p)
{
printf("MESSAGE \n");
	printf("name:%15s\n",p->data.name);
	printf("city:    %10s\n",p->data.city);
	printf("province:%10s\n",p->data.province);
	printf("state:   %10s\n",p->data.state);
	printf("telphone:%15s\n",p->data.tel);

}
void search(link l)
{
char name[20];
Node *p;
p=l->next;
printf("enter name to find:");
scanf("%s",name);/*输入要查找的名字*/
while(p)
{if(strcmp(p->data.name,name)==0)/*查找与输入的名字相匹配的记录*/
{
display(p);/*调用函数显示信息*/
getch();
break;
}
else
p=p->next;
}
}
void list(link l)
{
Node *p;
p=l->next;
while(p!=NULL)/*从首节点一直遍历到链表最后*/
{
display(p);
p=p->next;
}
getch();
}

void save(link l)
{
Node *p;
FILE *fp;
p=l->next;
if((fp=fopen("f:\\adresslist","wb"))==NULL)
{
printf("can not open file\n");
exit(1);
}
printf("\nSaving file\n");
while(p)/*将节点内容逐个写入磁盘文件中*/
{
fwrite(p,sizeof(Node),1,fp);
p=p->next;
}
fclose(fp);
getch();
}
void load(link l)
{
Node *p,*r;
FILE *fp;
l->next=NULL;
r=l;
if((fp=fopen("f:\\adresslist","rb"))==NULL)
{
printf("can not open file\n");
exit(1);
};
printf("\nLoading file\n");
while(!feof(fp))
{
p=(Node*)malloc(sizeof(Node));/*申请节点空间*/
if(!p)
{
printf("memory malloc fail!");
return;
}
if(fread(p,sizeof(Node),1,fp)!=1)/*读记录到节点p中*/
break;
else
{
	p->next=NULL;
	r->next=p;/*插入链表中*/
	r=p;
	}
}
fclose(fp);
getch();
}
menu_select()
{
int i;
printf("\n\n\t *************************ADDRESS LIST*************************\n");
printf("\t|*			1.input record			      *|\n");
printf("\t|*			2.delete record			      *|\n");
printf("\t|*			3.list record			      *|\n");
printf("\t|*			4.search record			      *|\n");
printf("\t|*			5.save record			      *|\n");
printf("\t|*			6.load record			      *|\n");
printf("\t|*			7.Quit                                *|\n");
printf("\t **************************************************************\n");
do
{
printf("\n\tEnter your choice:");
scanf("%d",&i);
}while(i<0||i>7);
return i;
}
main()
{
link l;
l=(Node*)malloc(sizeof(Node));
if(!l)
   {
      printf("\n allocate memory failure "); /*如没有申请到，输出提示信息*/
      return ;             /*返回主界面*/
   }
l->next=NULL;
system("cls");
while(1)
{
system("cls");
switch(menu_select())
{
case 1:
enter(l);
break;
case 2:
del(l);
break;
case 3:
list(l);
break;
case 4:
search(l);
break;
case 5:
save(l);
break;
case 6:
load(l);
break;
case 7:
exit(0);
}
}
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct employee
{
    int num;
    char duty[10];
    char name[10];
    char sex[3];
    unsigned char age;
    char edu[10];
    int salary;
    char tel_office[13];
    char tel_home[13];
    char mobile[13];
    char qq[11];
    char address[31];
    struct employee *next;    
}EMP;
    
    char password[9];
    EMP *emp_first,*emp_end;
    char gsave,gfirst;
    
    void addemp(void);
    void findemp(void);
    void listemp(void);
    void modifyemp(void);
    void summaryemp(void);
    void delemp(void);
    void resetpwd(void);
    void readdata(void);
    void savedata(void);
    int modi_age(int s);
    int modi_salary(int s);
    char *modi_field(char *field,char *s,int n);
    EMP *findname(char *name);
    EMP *findnum(int num);
    EMP *findtelephone(char *name);
    EMP *findqq(char *name);
    void displayemp(EMP *emp,char *field,char *name);
    void checkfirst(void);
    void bound(char ch,int n);
    void login();
    void menu();



void addemp()
{
    FILE *fp;
    EMP *emp1;
    int i=0;
    char choice='y';
    
    if((fp=fopen("employee.dat","ab"))==NULL)
    {
        printf("打开文件employee.dat出错！\n");
        getch();
        return;                                         
    }     
    
    do{
         i++;
         emp1=(EMP *)malloc(sizeof(EMP));
         
         if(emp1==NULL)
         {
             printf("内存分配失败，按任意键退出！\n");
             getch();
             return;              
         }
         printf("请输入第%d个员工的信息，\n",i);
         bound('_',30);
         printf("工号：");
         scanf("%d",&emp1->num);
         
         printf("职务：");
         scanf("%s",&emp1->duty);
         
         printf("姓名：");
         scanf("%s",&emp1->name);
         
         printf("性别：");
         scanf("%s",&emp1->sex);
         
         printf("年龄：");
         scanf("%d",&emp1->age);
         
         printf("文化程度：");
         scanf("%s",&emp1->edu);
         
         printf("工资：");
         scanf("%d",&emp1->salary);
         
         printf("办公电话：");
         scanf("%s",&emp1->tel_office);
         
         printf("家庭电话：");
         scanf("%s",&emp1->tel_home);
         
         printf("移动电话：");
         scanf("%s",&emp1->mobile);
         
         printf("QQ:");
         scanf("%s",&emp1->qq);
         
         printf("地址：");
         scanf("%s",&emp1->address);
         
         emp1->next=NULL;
         if(emp_first==NULL)
         {
             emp_first=emp1;
             emp_end=emp1;                   
         }else {
             emp_end->next=emp1;
             emp_end=emp1;      
         }
         
         fwrite(emp_end,sizeof(EMP),1,fp);
         
         gfirst=0;
         printf("\n");
         bound('_',30);
         printf("\n是否继续输入?(y/n)");
         fflush(stdin);
         choice=getch();
         
         if(toupper(choice)!='Y')
         {
             fclose(fp);
             printf("\n输入完毕，按任意键返回\n");
             getch();
             return;                        
         }
         
         system("cls");      
    }while(1);
}



void bound(char ch,int n)
{
   while(n--)
       putch(ch);
       
   printf("\n");
   return;     
}


/*首次使用，进行用户信息初始化*/
void checkfirst()
{
    FILE *fp,*fp1; 
    char pwd[9],pwd1[9],pwd2[9],pwd3[9],ch;
    int i;
    char strt='8';    
    
    if((fp=fopen("config.bat","rb"))==NULL)
    {
        printf("\n新系统，请进行相应的初始化操作！\n");
        bound('_',50);
        getch();
        
        do{
            printf("\n设置密码必须为8位：");
            for(i=0;i<8&&((pwd[i]=getch())!=13);i++)
                putch('*');
            printf("\n再确认一次密码：");
            for(i=0;i<8&&((pwd1[i]=getch())!=13);i++) 
                putch('*');    
                
            pwd[i]='\0';  
            pwd1[i]='\0';
            
            if(strcmp(pwd,pwd1)!=0)
                printf("\n两次密码输入不一致，请重新输入！\n\n");
            else break;
            
        }while(1);
        
        if((fp1=fopen("config.bat","wb"))==NULL)
        {
            printf("\n系统创建失败，请按任意键退出！");
            getch();
            exit(1);                                        
        }
        
        i=0;
        while(pwd[i])
        {
                 
            pwd2[i]=(pwd[i]^ strt);        
            putw(pwd2[i],fp1);
            i++;             
        }
        
        fclose(fp1);
        printf("\n\n系统初始化成功，按任意键退出后，再重新进入！\n");
        getch();
        exit(1);
        
    }else{
        i=0;
        while(!feof(fp)&&i<8)
            pwd[i++]=(getw(fp)^strt);
            
            
        pwd[i]='\0';
        
        if(i>=8) i--;
        while(pwd[i]!=-1&&i>=0)
            i--;
        
        pwd[i]='\0';
        strcpy(password,pwd); 
    } 
}


void delemp()
{
    int findok=0;
    EMP *emp1,*emp2;
    char name[10],choice;
    
    system("cls");
    printf("\n输入要删除的员工姓名：");
    scanf("%s",name);
    
    emp1=emp_first;
    emp2=emp1;
    while(emp1)
    {
        if(strcmp(emp1->name,name)==0)
        {
            findok=1;
            system("cls");
            
            printf("员工：%s的信息如下：",emp1->name);
            bound('_',40);
            printf("工号：%d\n",emp1->num);
            printf("职务：%s\n",emp1->duty);
            printf("姓名：%s\n",emp1->name);
            printf("性别：%s\n",emp1->sex);
            printf("年龄：%d\n",emp1->age);
            printf("文化程度：%s\n",emp1->edu);
            printf("工资：%d\n",emp1->salary);
            printf("办公电话：%s\n",emp1->tel_office);
            printf("家庭电话：%s\n",emp1->tel_home);
            printf("移动电话：%s\n",emp1->mobile);
            printf("QQ号码：%s\n",emp1->qq);
            printf("住址:%\ns",emp1->address);  
            bound('_',40);
            printf("您真的要删除该员工吗？(y/n)");
            
            fflush(stdin);
            choice=getchar();
            
            if(choice!='y' && choice!='Y') return;
            if(emp1==emp_first) emp_first=emp1->next;
            else emp2->next=emp1->next;
            
            free(emp1);
            gsave=1;
            savedata();
            return;                           
        }  else{        
            emp2=emp1;
            emp1=emp1->next;
        }
    }    
    if(!findok)
    {
        bound('_',40);
        printf("\n没有找到姓名是：%s的信息！\n",name);
        getch();           
    } 
    return;
}


void displayemp(EMP *emp,char *field,char *name)
{
    if(emp)
    {
        printf("\n%s:%s信息如下：\n",field,name);
        bound('_',30); 
        printf("工号：%d\n",emp->num);
        printf("职务：%s\n",emp->duty);
        printf("姓名：%s\n",emp->name);
        printf("性别：%s\n",emp->sex);
        printf("年龄：%d\n",emp->age);
        printf("文化程度：%s\n",emp->edu);
        printf("工资：%d\n",emp->salary);
        printf("办公电话：%s\n",emp->tel_office);
        printf("家庭电话：%s\n",emp->tel_home);
        printf("移动电话：%s\n",emp->mobile);
        printf("QQ号码：%s\n",emp->qq);
        printf("住址:%s\n",emp->address);
        bound('_',30);      
    }else {
    bound('_',40);
    printf("资料库中没有%s为：%s的员工！请重新确认！",field,name);
    }
    return;
}


EMP *findname(char *name)
{
    EMP *emp1;
    emp1=emp_first;
    
    while(emp1)
    {
        if(strcmp(name,emp1->name)==0)  return emp1;
        emp1=emp1->next;          
    }    
    return NULL;
}


EMP *findnum(int num)
{
   EMP *emp1;
   
   emp1=emp_first;
   while(emp1)
   {
       if(num==emp1->num)  return emp1;
       emp1=emp1->next;           
   }  
   return NULL;  
}

EMP *findtelephone(char *name)
{
     EMP *emp1;
     
     emp1=emp_first;
     while(emp1)
     {
         if((strcmp(name,emp1->tel_office)==0)||
         (strcmp(name,emp1->tel_home)==0)||
         (strcmp(name,emp1->mobile)==0))
         return emp1;
         emp1=emp1->next;
                 
     }  
      return NULL;       
}


EMP *findqq(char *name)
{
    EMP *emp1;
    
    emp1=emp_first;
    while(emp1)
    {
        if(strcmp(name,emp1->qq)==0)  return emp1;
        emp1=emp1->next;           
    }  
    return NULL;  
}


void findemp()
{
     int choice,ret=0,num;
     char str[13];
     EMP *emp1;
     
     system("cls");
     
     do{
         printf("\t查询员工信息\n");
         bound('_',30);
         printf("\t1.按姓名查询\n");
         printf("\t2.按工号查询\n");
         printf("\t3.按电话查询\n");
         printf("\t4.按QQ号查询\n");
         printf("\t0.返回主菜单\n");
         bound('_',30);
         printf("\n请选择菜单：");
         
         do{
             fflush(stdin);
             choice=getchar();
             system("cls");
             
             switch(choice)
             {
                 case '1':
                      printf("\n输入要查询的员工姓名：");
                      scanf("%s",str);
                      
                      emp1=findname(str);
                      displayemp(emp1,"姓名",str);
                      getch();
                      break; 
                      
                 case '2':
                      printf("\n请输入要查询的员工的工号"); 
                      scanf("%d",&num); 
                      
                      emp1=findnum(num);
                      itoa(num,str,10);
                      displayemp(emp1,"工号",str);
                      getch();
                      break;
                      
                  case '3':
                       printf("\n输入要查询员工的电话:");
                       scanf("%s",str);
                       
                       emp1=findtelephone(str); 
                       displayemp(emp1,"电话",str);
                       getch();
                       break;
                       
                  case '4':
                       printf("\n输入要查询的员工的QQ号：");
                       scanf("%s",str);
                       
                       emp1=findqq(str);
                       displayemp(emp1,"QQ号码",str);
                       getch();
                       break;
                       
                   case '0':
                        ret=1;
                        break;          
             }                         
         }while(choice<'0'||choice>'4');
         
         system("cls");
         if(ret) break;
     }while(1);         
}


void listemp()
{
    EMP *emp1;
    
    printf("\n资料库中的员工信息列表\n");
    bound('_',40);
    emp1=emp_first;
    
    while(emp1) 
    {
        printf("工号：%d\n",emp1->num);
        printf("职务：%s\n",emp1->duty);
        printf("姓名：%s\n",emp1->name);
        printf("性别：%s\n",emp1->sex);
        printf("年龄：%d\n",emp1->age);
        printf("文化程度：%s\n",emp1->edu);
        printf("工资：%d\n",emp1->salary);
        printf("办公电话：%s\n",emp1->tel_office);
        printf("家庭电话：%s\n",emp1->tel_home);
        printf("移动电话：%s\n",emp1->mobile);
        printf("QQ号码：%s\n",emp1->qq);
        printf("住址:%s\n",emp1->address);  
        bound('_',40);
        emp1=emp1->next;      
    }    
    printf("\n显示完毕，按任意键退出！\n");
    getch();
    return;
}


/*检测登录密码*/
void login()
{
    int i,n=3;
    char pwd[9];
    
    do{
        printf("请输入密码：");
        for(i=0;i<8 && ((pwd[i]=getch())!=13);i++)
            putch('*');
            
        pwd[i]='\0';
        if(strcmp(pwd,password))
        {
            printf("\n密码错误，请重新输入！\n");
            system("cls");
            n--;                        
        }else break;
    } while(n>0);
    
    if(!n)
    {
        printf("请退出，你的三次输入密码错误！");
        getch();
        exit(1);      
    }
}


void menu()
{
    char choice;
    
    system("cls");
    
    do{
        printf("\t 企业员工管理系统\n");
        bound('_',40);
        printf("\t1.输入员工信息\n");
        printf("\t2.查询员工信息\n");
        printf("\t3.显示员工信息\n");
        printf("\t4.修改员工信息\n");
        printf("\t5.删除员工信息\n");
        printf("\t6.统计员工信息\n");
        printf("\t7.重置系统密码\n");
        printf("\t0.退出系统\n");
        bound('_',40);
        printf("\n请选择您需要的操作！");
        
        do{
            fflush(stdin);
            choice=getchar();
            system("cls");
            
            switch(choice)
            {
                case '1':
                   addemp();
                   break;
                case '2':
                   if(gfirst)
                   {
                       printf("系统信息中无员工信息，请先添加员工信息！\n");
                       getch();
                       break;          
                   }  
                   
                   findemp();
                   break;
                case '3':
                   if(gfirst)
                   {
                       printf("系统信息中无员工信息，请先添加员工信息！\n");
                       getch();
                       break;       
                   }
                   
                   listemp();
                   break;
               case '4':
                   if(gfirst)
                   {
                       printf("系统信息中无员工信息，请先添加员工信息！\n");
                       getch();
                       break;       
                   }
                   
                   modifyemp();
                   break;
               case '5':
                    if(gfirst)
                   {
                       printf("系统信息中无员工信息，请先添加员工信息！\n");
                       getch();
                       break;       
                   }
                   
                   delemp();
                   break;
               case '6':
                    if(gfirst)
                   {
                       printf("系统信息中无员工信息，请先添加员工信息！\n");
                       getch();
                       break;       
                   }
                   
                   summaryemp();
                   break; 
               case '7':
                   resetpwd();
                   break;
               case '0':
                   savedata();
                   exit(0);        
            }                             
        } while(choice<'0'||choice>'7');  
        
        
         system("cls");        
    }while(1);    
}


int modi_salary(int salary){
    int newsalary;
    printf("原来的工资数为：%d",salary);
    printf("新的工资数：");
    scanf("%d",&newsalary);
    
    return(newsalary);
}



int modi_age(int age){
    int newage;
    printf("原来的年龄为：%d",age);
    printf("新的年龄：");
    scanf("%d",&newage);
    
    return(newage);
}
char *modi_field(char *field,char *content,int len)
{
     char *str;
     str=malloc(sizeof(char)*len);
     if(str==NULL)
     {
         printf("内存分配失败，按任意键退出！");
         getch();
         return NULL;             
     }
     printf("原来%s为：%s\n",field,content);
     printf("修改为（内容不要超过%d个字符！）：",len);
     scanf("%s",str);
     
     return str;
}

void modifyemp()
{
     EMP *emp1;
     char name[10],*newcontent;
     int choice;
     
     printf("\n请输入您要修改的员工的信息:");
     scanf("%s",&name);
     
     emp1=findname(name);
     displayemp(emp1,"姓名",name);
     
     if(emp1)
     {
         printf("\n 请输入你要修改的内容选项！\n");
         bound('_',40);
         printf("1.修改职务                2.修改年龄\n");
         printf("3.修改文化程度            4.修改工资\n");
         printf("5.修改办公室电话          6.修改家庭电话\n");
         printf("7.修改移动电话            8.修改QQ号码 \n");
         printf("9.修改住址                0.返回\n  ");
         bound('_',40);
         
         do{
             fflush(stdin);
             choice=getchar();
             switch(choice) 
             {
                 case '1':
                      newcontent=modi_field("职务",emp1->duty,10);
                      if(newcontent!=NULL)
                      {
                          strcpy(emp1->duty,newcontent);
                          free(newcontent);                    
                      } 
                      break;
                 case '2':
                      emp1->age=modi_age(emp1->age);
                      break;
                  case '3':
                      newcontent=modi_field("文化程度",emp1->edu,10);
                      if(newcontent!=NULL)
                      {
                          strcpy(emp1->edu,newcontent);
                          free(newcontent);                    
                      } 
                      break;
                  case '4':
                      emp1->salary=modi_salary(emp1->salary);
                      break;
                 case '5':
                      newcontent=modi_field("办公室电话",emp1->tel_office,13);
                      if(newcontent!=NULL)
                      {
                          strcpy(emp1->tel_office,newcontent);
                          free(newcontent);                    
                      } 
                      break;
                 case '6':
                      newcontent=modi_field("家庭电话",emp1->tel_home,13);
                      if(newcontent!=NULL)
                      {
                          strcpy(emp1->tel_home,newcontent);
                          free(newcontent);                    
                      } 
                      break;
                  case '7':
                      newcontent=modi_field("移动电话",emp1->mobile,12);
                      if(newcontent!=NULL)
                      {
                          strcpy(emp1->mobile,newcontent);
                          free(newcontent);                    
                      } 
                      break;
                  case '8':
                      newcontent=modi_field("QQ号码",emp1->qq,10);
                      if(newcontent==NULL)
                      {
                          strcpy(emp1->qq,newcontent);
                          free(newcontent);                    
                      } 
                      break;  
                  case '9':
                      newcontent=modi_field("住址",emp1->address,30);
                      if(newcontent!=NULL)
                      {
                          strcpy(emp1->address,newcontent);
                          free(newcontent);                    
                      } 
                      break;     
                  case '0':
                      return;             
             }             
         }while(choice<'0' || choice>'9');
         
         gsave=1;
         savedata();
         printf("\n修改完毕，按任意键退出！\n");
         getch();
     }  
     return;   
}



/*读取数据*/ 
void readdata(void)
{
    FILE *fp;
    EMP *emp1;
    
    if((fp=fopen("employee.dat","rb"))==NULL)
    {
        gfirst=1;
        return;                                        
    }     
    
    while(!feof(fp))
    {
        emp1=(EMP *)malloc(sizeof(EMP));
        if(emp1==NULL)
        {
            printf("内存分配失败！\n");
            getch();
            return;              
        }          
        
        fread(emp1,sizeof(EMP),1,fp);
        if(feof(fp))  break;
        
        if(emp_first==NULL)
        {
           emp_first=emp1;
           emp_end=emp1;                   
        }else{
            emp_end->next=emp1;
            emp_end=emp1;   
        }
        emp_end->next=NULL;
    }
    gfirst=0;
    fclose(fp);
}


void resetpwd()
{
    char pwd[9],pwd1[9],ch;
    int i;
    FILE *fp1;
    
    system("cls");
    
    printf("\n请输入旧密码：\n");
     for(i=0;i<8 && ((pwd[i]=getch())!=13);i++)
            putch('*');
            
        pwd[i]='\0';
        if(strcmp(password,pwd)!=0)
        {
            printf("\n密码错误，请按任意键退出！\n");
            getch();
            return;             
        }   
     do{
         printf("\n设置新密码，请不要超过8位：");
            for(i=0;i<8&&((pwd[i]=getch())!=13);i++)
                putch('*');
            printf("\n再确认一次密码：");
            for(i=0;i<8&&((pwd1[i]=getch())!=13);i++) 
                putch('*');    
                
            pwd[i]='\0';  
            pwd1[i]='\0';
            
            if(strcmp(pwd,pwd1)!=0)
                printf("\n两次密码输入不一致，请重新输入！\n\n");
            else break;
            
        }while(1);
        
        if((fp1=fopen("config.bat","wb"))==NULL)
        {
            printf("\n系统创建失败，请按任意键退出！");
            getch();
            exit(1);                                        
        }
        
        i=0;
        while(pwd[i])
        {
            putw(pwd[i],fp1);
            i++;             
        }
        
        fclose(fp1);
        printf("\n密码修改成功，按任意键退出！\n");
        getch();
        return;       
}


void savedata()
{
      FILE *fp;
      EMP *emp1;
      
      if(gsave==0) return;
      
      if((fp=fopen("employee.dat","wb"))==NULL)
      {
          printf("打开文件employee.dat出错！\n");
          getch();
          return;                                         
      }   
      
      emp1=emp_first;
      while(emp1)
      {
          fwrite(emp1,sizeof(EMP),1,fp);
          emp1=emp1->next;          
      }
      
      gsave=0;
      fclose(fp);
}


void summaryemp()
{
   EMP *emp1;
   int sum=0,num=0,man=0,woman=0;
   
   emp1=emp_first;
   while(emp1)
   {
       num++;
       sum+=emp1->salary;
       char strw[2];
       strncpy(strw,emp1->sex,2);
       if((strcmp(strw,"ma")==0)||(strcmp(emp1->sex,"男")==0)) man++;
       else woman++;
       emp1=emp1->next;          
   }     
   
   printf("\n下面是相关员工的统计信息！\n");
   bound('_',40);
   printf("员工总数是：%d\n",num);
   printf("员工的工资总数是：%d\n",sum);
   printf("男员工数为：%d\n",man);
   printf("女员工数为：%d\n",woman);
   bound('_',40);
   printf("按任意键退出！\n");
   getch();
   return;
}


int main(void)
{
  emp_first=emp_end=NULL;
  gsave=gfirst=0;
  
  checkfirst();
  login();
  readdata();
  menu();
  system("PAUSE");	
  return 0;
}
#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib,"ws2_32.lib")

#define CLIENTSEND_EXIT 1
#define CLIENTSEND_TRAN 2
#define CLIENTSEND_LIST 3

#define SERVERSEND_SELFID 1
#define SERVERSEND_NEWUSR 2
#define SERVERSEND_SHOWMSG 3
#define SERVERSEND_ONLINE 4

FILE *ioutfileServer;
FILE *ioutfileClient;

struct CReceivePackage
{
	int iType;			//存放消息类型
	int iToID;			//存放目标用户ID
	int iFromID;		//存放原用户ID
	char cBuffer[1024];	//存放消息内容
};

struct CSendPackage
{
	int iType;			//消息类型
	int iCurConn;		//当前在线用户数量
	char cBuffer[1024];	//存放消息内容
};

struct CUserSocketInfo
{
	int ID;				//用户的ID
	char cDstIP[64];	//用户的IP地址,扩展使用
	int iPort;			//用户应用程序端口扩展使用
	SOCKET sUserSocket;	//网络句柄
};

struct CUser
{
	int ID;				//用户的ID
	char cDstIP[64];	//用户的IP地址 扩展时使用
};

struct CUser usr[20];
int bSend=0;//是否可以发送消息
int iMyself;//自己的id
int iNew=0;//在线用户数

struct CUserSocketInfo usrinfo[20]; 


void ExitSystem()
{
	if(ioutfileServer!=NULL)
		fclose(ioutfileServer);
	if(ioutfileClient!=NULL)
		fclose(ioutfileClient);
	WSACleanup();
	exit(0);
}

DWORD WINAPI threadproClient(LPVOID pParam)
{
	SOCKET hsock=(SOCKET)pParam;
	char cRecvBuffer[1024];
	char cShowBuffer[1024];
	int num=0;                                                                                                                                                                                                                                                                                                                                                                                  
	if(hsock!=INVALID_SOCKET)
		printf("start:\n");
	while(1)
	{
		num = recv(hsock,cRecvBuffer,1024,0);
		if(num >= 0)
		{		
			cRecvBuffer[num]='\0';
			sprintf(cShowBuffer,"to me : %s\n",cRecvBuffer);
			printf("%s",cShowBuffer);
			fwrite(cShowBuffer ,sizeof(char),strlen(cShowBuffer),ioutfileClient);
			fflush(ioutfileClient);
			if(strcmp("exit",cRecvBuffer)==0)
			{
				ExitSystem();
			}
		}
	}
	return 0;
}

DWORD WINAPI threadproServer(LPVOID pParam)
{
	SOCKET hsock=(SOCKET)pParam;
	char cRecvBuffer[1024];
	char cShowBuffer[1024];
	int num=0;
	if(hsock!=INVALID_SOCKET)
		printf("start:\n");
	while(1)
	{
		num = recv(hsock,cRecvBuffer,1024,0);
		if(num >= 0)
		{		
			cRecvBuffer[num]='\0';
			sprintf(cShowBuffer,"to me : %s\n",cRecvBuffer);
			printf("%s",cShowBuffer);
			fwrite(cShowBuffer ,sizeof(char),strlen(cShowBuffer),ioutfileServer);
			fflush(ioutfileServer);
			if(strcmp("exit",cRecvBuffer)==0)
			{
				ExitSystem();
			}
		}
	}
	return 0;
}


void CreateServer()
{
	SOCKET m_SockServer;
	struct sockaddr_in serveraddr;//本地地址信息
	struct sockaddr_in serveraddrfrom;//连接的地址信息

	int iPort=4600;//设定为固定端口
	int iBindResult=-1;//绑定结果
	int iWhileCount=200;

	struct hostent* localHost;
	char* localIP;

	SOCKET m_Server;

	char cWelcomBuffer[]="Welcome to you\0";

	int len=sizeof(struct sockaddr);
	int iWhileListenCount=10;

	DWORD nThreadId = 0;
	int ires;//发送的返回值

	char cSendBuffer[1024];//发送消息缓存
	char cShowBuffer[1024];//接收消息缓存

	ioutfileServer= fopen("MessageServer.txt","a");//打开记录消息的文件
	m_SockServer = socket ( AF_INET,SOCK_STREAM,  0);
	
	printf("本机绑定的端口号(大于1024)：");
	scanf("%d",&iPort);
	localHost = gethostbyname("");
	localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);
	//设置网络地址信息
	serveraddr.sin_family = AF_INET;					
	serveraddr.sin_port = htons(iPort);	//端口			
	serveraddr.sin_addr.S_un.S_addr = inet_addr(localIP);//地址
	//绑定地址信息
	iBindResult=bind(m_SockServer,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr));
	//如果端口不能被绑定，重新设置端口
	while(iBindResult!=0 && iWhileCount > 0)
	{
		printf("绑定失败，重新输入：");
		scanf("%d",iPort);
		//设置网络地址信息
		serveraddr.sin_family = AF_INET;					
		serveraddr.sin_port = htons(iPort);//端口	
		serveraddr.sin_addr.S_un.S_addr = inet_addr(localIP);//IP
		//绑定地址信息
		iBindResult = bind(m_SockServer,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr));
		iWhileCount--;
		if(iWhileCount<=0)
		{
			printf("端口绑定失败,重新运行程序\n");
			exit(0);
		}
	}

	while(iWhileListenCount>0)
	{
		printf("start listen\n");
		listen(m_SockServer,0);//返回值判断单个监听是否超时
		m_Server=accept(m_SockServer,(struct sockaddr*)&serveraddrfrom,&len);
			if(m_Server!=INVALID_SOCKET)
			{
				//有连接成功，发送欢迎信息
				send(m_Server,cWelcomBuffer,sizeof(cWelcomBuffer),0);
				//启动接收消息的线程
				CreateThread(NULL,0,threadproServer,(LPVOID)m_Server,0,&nThreadId );
				break;
			}
		printf(".");
		iWhileListenCount--;
		if(iWhileListenCount<=0)
		{
			printf("\n建立连接失败\n");
			exit(0);
		}
	}
	
		
	while(1)
	{
		memset(cSendBuffer,0,1024);
		scanf("%s",cSendBuffer);//输入消息
		if(strlen(cSendBuffer)>0)//输入消息不能为空
		{
			ires = send(m_Server,cSendBuffer,strlen(cSendBuffer),0);//发送消息
			if(ires<0)
			{
				printf("发送失败");
			}
			else
			{
				sprintf(cShowBuffer,"Send to : %s\n",cSendBuffer);
				printf("%s",cShowBuffer);
				fwrite(cShowBuffer ,sizeof(char),strlen(cShowBuffer),ioutfileServer);//将消息写入日志
			}
			if(strcmp("exit",cSendBuffer)==0)
			{
				ExitSystem();
			}
		}
	}
}
int CheckIP(char *cIP)
{
	char IPAddress[128];//IP地址字符串
	char IPNumber[4];//IP地址中每组的数值
	int iSubIP=0;//IP地址中4段之一
	int iDot=0;//IP地址中'.'的个数
	int iResult=0;
	int iIPResult=1;
	int i;//循环控制变量
	memset(IPNumber,0,4);
	strncpy(IPAddress,cIP,128);
	for(i=0;i<128;i++)
	{
		if(IPAddress[i]=='.')
		{
			iDot++;
			iSubIP=0;
			if(atoi(IPNumber)>255)
				iIPResult = 0;
			memset(IPNumber,0,4);
		}
		else
		{
			IPNumber[iSubIP++]=IPAddress[i];
		}
		if(iDot==3 && iIPResult!=0)
			iResult= 1;
	}
	return iResult;
}
void CreateClient()
{
	SOCKET m_SockClient;
	struct sockaddr_in clientaddr;
	char cServerIP[128];
	int iWhileIP=10;//循环次数
	int iCnnRes;//连接结果
	DWORD nThreadId = 0;//线程ID值
	char cSendBuffer[1024];//发送缓存
	char cShowBuffer[1024];//显示缓存
	char cRecvBuffer[1024];//接收缓存
	int num;//接收的字符个数
	int ires;//发送消息的结果
	int iIPRes;//检测IP是否正确
	m_SockClient = socket ( AF_INET,SOCK_STREAM, 0 );
	printf("请输入服务器地址：");
	scanf("%s",cServerIP);
	//IP地址判断
	if(strlen(cServerIP)==0)
		strcpy(cServerIP,"127.0.0.1");//没有输入地址，使用回环地址
	else
	{
		iIPRes=CheckIP(cServerIP);
		while(!iIPRes && iWhileIP>0)
		{
			printf("请重新输入服务器地址：\n");
			scanf("%s",cServerIP);//重新输入IP地址
			iIPRes=CheckIP(cServerIP);//检测IP的合法性
			iWhileIP--;
			if(iWhileIP<=0)
			{
				printf("输入次数过多\n");
				exit(0);
			}
		}
	}

	ioutfileClient= fopen("MessageServerClient.txt","a");//打开记录消息的文件
	clientaddr.sin_family = AF_INET;
	//客户端向服务端请求的端口好，应该和服务端绑定的一致			
	clientaddr.sin_port = htons(4600);
	clientaddr.sin_addr.S_un.S_addr = inet_addr(cServerIP);

	iCnnRes = connect(m_SockClient,(struct sockaddr*)&clientaddr,sizeof(struct sockaddr));
	if(iCnnRes==0)//连接成功
	{
		num = recv(m_SockClient,cRecvBuffer,1024,0);//接收消息
		if( num > 0 )
		{
			printf("Receive form server : %s\n",cRecvBuffer);
			//启动接收消息的线程
			CreateThread(NULL,0,threadproClient,(LPVOID)m_SockClient,0,&nThreadId );
		}
		while(1)
		{
			memset(cSendBuffer,0,1024);
			scanf("%s",cSendBuffer);	
			if(strlen(cSendBuffer)>0)
			{
				ires=send(m_SockClient,cSendBuffer,strlen(cSendBuffer),0);
				if(ires<0)
				{
					printf("发送失败\n");
				}
				else
				{
					sprintf(cShowBuffer,"Send to : %s\n",cSendBuffer);//整理要显示的字符串
					printf("%s",cShowBuffer);
					fwrite(cShowBuffer ,sizeof(char),strlen(cShowBuffer),ioutfileClient);//记录发送消息
					fflush(ioutfileClient);
				}
				if(strcmp("exit",cSendBuffer)==0)
				{
					ExitSystem();
				}
			}
		}
	}//iCnnRes
	else
	{
		printf("连接不正确\n");
	}
}

DWORD WINAPI threadTranServer(LPVOID pParam) 
{
	SOCKET hsock=(SOCKET)pParam;//获取SOCKET句柄
	SOCKET sTmp;//临时存放用户的SOCKET句柄
	char cRecvBuffer[1024];//接收消息的缓存
	int num=0;//发送的字符串
	int m,j;//循环控制变量
	//char cTmp[2];//临时存放用户ID
	int ires;
	struct CSendPackage sp;//发包
	struct CReceivePackage *p;
	if(hsock!=INVALID_SOCKET)
		printf("start:%d\n",hsock);
	while(1)
	{
		num=recv(hsock,cRecvBuffer,1024,0);//接收发送过来的信息
		if(num>=0)
		{		
			p = (struct CReceivePackage*)cRecvBuffer;
			switch(p->iType)
			{
			case CLIENTSEND_TRAN://对消息进行中转
				for(m=0;m<2;m++)
				{
					if(usrinfo[m].ID==p->iToID)
					{
						//组包
						sTmp=usrinfo[m].sUserSocket;
						memset(&sp,0,sizeof(sp));
						sp.iType=SERVERSEND_SHOWMSG;
						strcpy(sp.cBuffer,p->cBuffer);
						ires = send(sTmp,(char*)&sp,sizeof(sp),0);//发送内容
						if(ires<0)
							printf("发送失败\n");
					}
				}
				break;
			case CLIENTSEND_LIST://发送在线用户
				memset(&sp,0,sizeof(sp));
				for(j=0;j<2;j++)
				{
					if(usrinfo[j].ID!=p->iFromID && usrinfo[j].ID!=0)
					{
						sp.cBuffer[j]=usrinfo[j].ID;
					}
				}
				sp.iType=SERVERSEND_ONLINE;
				send(hsock,(char*)&sp,sizeof(sp),0);
				break;
			case CLIENTSEND_EXIT:
				printf("退出系统\n");
				return 0;//结束线程
				break;
			}
		}
	}
	return 0;
}

DWORD WINAPI NotyifyProc(LPVOID pParam)
{
	struct CSendPackage sp;//发送包
	SOCKET sTemp;//连接用户的socket句柄
	int *p;//接收主线程发送过来的ID值
	int j;//循环控制变量
	p=(int*)pParam;//新用户ID
	
	for(j=0;j<2;j++)//去除新登录的,已经连接的
	{
		if(usrinfo[j].ID !=  (*p))
		{
			sTemp=usrinfo[j].sUserSocket;
			sp.iType=SERVERSEND_NEWUSR;//新上线通知
			sprintf(sp.cBuffer,"%d\n",(*p));
			send(sTemp,(char*)&sp,sizeof(sp),0);//发送新用户上线通知
		}
	}
	return 0;
}

void CreateTranServer()
{
	SOCKET m_SockServer;//开始监听的SOCKET句柄
	struct sockaddr_in serveraddr;//用于绑定的地址信息
	struct sockaddr_in serveraddrfrom;//接收到的连接的地址信息
	int iRes;//获取绑定的结果
	SOCKET m_Server;//已建立连接的SOCKET句柄
	struct hostent* localHost;//主机环境指针
	char* localIP;//本地IP地址
	struct CSendPackage sp;//发送包
	int iMaxConnect=20;//允许的最大连接个数
	int iConnect=0;//建立连接的个数
	DWORD nThreadId = 0;//获取线程的ID值
	char cWarnBuffer[]="It is voer Max connect\0";//警告字符串
	int len=sizeof(struct sockaddr);
	int id;//新分配的客户ID
	localHost = gethostbyname("");
	localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);//获取本地IP
	serveraddr.sin_family = AF_INET;					
	serveraddr.sin_port = htons(4600);//设置绑定的端口号			
	serveraddr.sin_addr.S_un.S_addr = inet_addr(localIP);//设置本地IP
	//创建套接字
	m_SockServer = socket ( AF_INET,SOCK_STREAM,  0);
	if(m_SockServer == INVALID_SOCKET)
	{
		printf("建立套接字失败\n");
		exit(0);
	}
	//绑定本地IP地址
	iRes=bind(m_SockServer,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr));
	if(iRes < 0)
	{
		printf("建立套接字失败\n");
		exit(0);
	}
	//程序主循环
	while(1)
	{
		listen(m_SockServer,0);//开始监听
		m_Server=accept(m_SockServer,(struct sockaddr*)&serveraddrfrom,&len);//接收连接
		if(m_Server!=INVALID_SOCKET)
		{
			printf("有新用户登录");//对方已登录 
			if(iConnect < iMaxConnect)
			{
				//启动接收消息线程
				CreateThread(NULL,0,threadTranServer,(LPVOID)m_Server,0,&nThreadId );
				//构建连接用户的信息
				usrinfo[iConnect].ID=iConnect+1;//存放用户ID
				usrinfo[iConnect].sUserSocket=m_Server;
				usrinfo[iConnect].iPort=0;//存放端口，扩展用
				//构建发包信息
				sp.iType=SERVERSEND_SELFID;//获取的ID值，返回信息
				sp.iCurConn=iConnect;//在线个数
				id=iConnect+1;
				sprintf(sp.cBuffer,"%d\0",id);
				send(m_Server,(char*)&sp,sizeof(sp),0);//发送客户端的ID值
				//通知各个客户端
				if(iConnect>0)
					CreateThread(NULL,0,NotyifyProc,(LPVOID)&id,0,&nThreadId );
				iConnect++;
			}
			else
				send(m_Server,cWarnBuffer,sizeof(cWarnBuffer),0);//已超出最大连接数
		}
	}
	WSACleanup();
}



DWORD WINAPI threadTranClient(LPVOID pParam) 
{
	SOCKET hsock=(SOCKET)pParam;
	int i;//循环控制变量
	char cRecvBuffer[2048];//接收消息的缓存
	int	num;//接收消息的字符数
	//char cTmp[2];//临时存放在线用户ID
	struct CReceivePackage sp;//服务端的接收包是，客户端的发送包
	struct CSendPackage *p;//服务端的发送包是，客户端的接收包
	int iTemp;//临时存放接收到的ID值
	while(1)
	{
		num = recv(hsock,cRecvBuffer,2048,0);//接收消息
		if(num>=0) 
		{
			p = (struct CSendPackage*)cRecvBuffer;
			if(p->iType==SERVERSEND_SELFID)
			{
				iMyself=atoi(p->cBuffer);
				sp.iType=CLIENTSEND_LIST;//请求在线人员列表
				send(hsock,(char*)&sp,sizeof(sp),0);
			}
			if(p->iType==SERVERSEND_NEWUSR)//登录用户ID
			{
				iTemp = atoi(p->cBuffer);
				usr[iNew++].ID=iTemp;//iNew表示有多少个新用户登录 
				printf("有新用户登录,可以与其聊天\n");
				bSend=1;//可以发送消息聊天
			}
			if(p->iType==SERVERSEND_SHOWMSG)//显示接受的消息
			{
				printf("rec:%s\n",p->cBuffer);
			}
			if(p->iType==SERVERSEND_ONLINE)//获取在线列表
			{
				for(i=0;i<2;i++)
					{
						if(p->cBuffer[i]!=iMyself && p->cBuffer[i]!=0)
						{
							usr[iNew++].ID=p->cBuffer[i];
							printf("有用户在线,可以与其聊天\n");
							bSend=1;//可以发送消息聊天
						}
					}
					if(!bSend)
						printf("在线列表为空\n");
			}
		}
	}
	return 0;
}

void ExitTranSystem()
{
	WSACleanup();
	exit(0);
}
void CreateTranClient()
{
	SOCKET m_SockClient;//建立连接的socket
	struct sockaddr_in clientaddr;//目标的地址信息
	int iRes;//函数执行情况
	char cSendBuffer[1024];//发送消息的缓存
	DWORD nThreadId = 0;//保存线程的ID值
	struct CReceivePackage sp;//发包结构
	char IPBuffer[128];
	printf("输入服务器IP地址\n");
	scanf("%s",IPBuffer);

	clientaddr.sin_family = AF_INET;				
	clientaddr.sin_port = htons(4600);//连接的端口号
	clientaddr.sin_addr.S_un.S_addr = inet_addr(IPBuffer);
	m_SockClient = socket ( AF_INET,SOCK_STREAM, 0 );//创建socket
	//建立与服务端的连接
	iRes = connect(m_SockClient,(struct sockaddr*)&clientaddr,sizeof(struct sockaddr));
	if(iRes < 0)
	{
		printf("连接错误\n");
		exit(0);
	}
	//启动接收消息的线程
	CreateThread(NULL,0,threadTranClient,(LPVOID)m_SockClient,0,&nThreadId );
	while(1)//接收到自己ID
	{
		memset(cSendBuffer,0,1024);
		scanf("%s",cSendBuffer);//输入发送内容
		if(bSend)
		{
			if(sizeof(cSendBuffer)>0)
			{
				memset(&sp,0,sizeof(sp));
				strcpy(sp.cBuffer,cSendBuffer);
				sp.iToID=usr[0].ID;//聊天对象是固定的
				sp.iFromID=iMyself;//自己
				sp.iType=CLIENTSEND_TRAN;
				send(m_SockClient,(char*)&sp,sizeof(sp),0);//发送消息
			}
			if(strcmp("exit",cSendBuffer)==0)
			{
				memset(&sp,0,sizeof(sp));
				strcpy(sp.cBuffer,"退出");//设置发送消息的文本内容
				sp.iFromID=iMyself;
				sp.iType=CLIENTSEND_EXIT;//退出
				send(m_SockClient,(char*)&sp,sizeof(sp),0);//发送消息
				ExitTranSystem();
			}
		}
		else
			printf("没有接收对象,发送失败\n");
	Sleep(10);
	}
}

int main(void)
{
	int iSel=0;
	WSADATA wsd;									
	WSAStartup(MAKEWORD(2,2),&wsd);
	do
	{
		printf("选择程序类型：\n");
		printf("点对点服务端: 1\n");
		printf("点对点客户端: 2\n");
		printf("服务器中转服务端: 3\n");
		printf("服务器中转客户端: 4\n");
		scanf("%d",&iSel);
	}while(iSel<0 || iSel >4);
	switch(iSel)
	{
	case 1:
		CreateServer();
		break;
	case 2:
		CreateClient();
		break;
	case 3:
		CreateTranServer();
		break;
	case 4:
		CreateTranClient();
		break;
	}
	printf("退出系统\n");
	
	return 0;
}
#include <stdio.h>
#include <windows.h>
#include <mysql.h>
#pragma  comment(lib,"libmysql.lib")

/*定义数据库相关操作变量*/
MYSQL mysql;          //
MYSQL_RES *result;    //定义结果集变量
MYSQL_ROW row;        //定义行变量
char ch[2];


void ShowAll();       /*显示所有的图书信息*/
void AddBook();       /*添加图书信息*/
void ModifyBook();    /*修改图书信息*/
void DeleteBook();    /*删除图书信息*/
void QueryBook();     /*查询图书信息*/


/*显示菜单*/
void showmenu()
{
    system("cls");
    printf("\n\n\n\n\n");
    printf("\t╔═══════════════════════════╗\n");
    printf("\t║            Welcome To Lemon Books System             ║\n");
    printf("\t╠═══════════════════════════╣\n");
    printf("\t║\t 1 - 显示所有图书信息                           ║\n");
    printf("\t║\t 2 - 添加图书信息                               ║\n");
    printf("\t║\t 3 - 修改图书信息                               ║\n");
    printf("\t║\t 4 - 删除图书信息                               ║\n");
    printf("\t║\t 5 - 查询图书信息                               ║\n");
    printf("\t║\t 6 - 退出                                       ║\n");
    printf("\t╚═══════════════════════════╝\n");
    printf("\n              ENTER YOUR CHOICE(1-6):");
    
}

void inquire()  /*询问用户是否显示主菜单*/
{
    printf("\t 显示主菜单?(y/n):");
    scanf("%s",ch);
    if(strcmp(ch,"Y")==0||strcmp(ch,"y")==0)/*判断是否要显示查找到的信息*/
    {
        showmenu();   /*显示菜单*/ 
    }
    else
    {
        exit(0);
    }
}

/*显示主菜单，连接数据库*/
int main()
{
    int n ;   //定义变量  存储用户输入的编号
    
    mysql_init(&mysql);  //初始化mysql结构
    
    showmenu();   /*显示菜单*/ 
    
    scanf("%d",&n);/*输入选择功能的编号*/
    
    while(n)
    { 
        switch(n)
        {
            case 1: 
                ShowAll();    /*调用显示所有图书数据的过程*/
                break;
            case 2: 
                AddBook();   /*添加图书信息*/
                break;
            case 3: 
                ModifyBook();     /*修改图书信息*/
                break;
            case 4:
                DeleteBook();   /*删除图书信息*/
                break;
            case 5:
                QueryBook();   /*查询图书信息*/
                break;
            case 6:
                exit(0);       /*退出*/
            default:break;
        }
        scanf("%d",&n);
    }
}



void ShowAll()    /*调用显示所有图书数据的过程*/
{
    /*连接数据库*/
    if(!mysql_real_connect(&mysql,"127.0.0.1","root","111","db_books",0,NULL,0))
    { 
        printf("\n\t 不能连接数据库!\n");
    }
    else
    {
        /*数据库连接成功*/
        if(mysql_query(&mysql,"select * from tb_book"))
        {   //如果查询失败
            printf("\n\t 查询 tb_book 数据表失败!\n");
        }
        else
        {
            result=mysql_store_result(&mysql); //获得结果集
            if(mysql_num_rows(result)!=NULL)
            {  //有记录的情况,只有有记录取数据才有意义
                printf("\t ════════════════════════════ \n");
                printf("\t                 显 示 所 有 图 书 信 息                  \n");
                printf("\t ════════════════════════════ \n");
                printf("\t图书编号       图书名         作者        出版社      \n");
                printf("\t -------------------------------------------------------- \n");
                while((row=mysql_fetch_row(result)))
                {   //取出结果集中记录
                    fprintf(stdout,"\t   %s             %s             %s          %s   \n",row[0],row[1],row[2],row[3]);  //输出这行记录
                }
                printf("\t ════════════════════════════ \n");
            }
            else
            {
                printf("\n\t 没有记录信息 !\n");
            }
            mysql_free_result(result);    //释放结果集
        }
        mysql_close(&mysql);          //释放连接
    }
    inquire();   /*询问是否显示主菜单*/
}







void AddBook()    /*添加图书信息*/
{
    int rowcount;   /*结果集中的行数*/
    
    char id[10];       /*编号*/
    char *bookname;
    char *author;
    char *bookconcern;
    
    char *sql;    
    char dest[500] ={"  "};    
    
    /*连接数据库*/
    if(!mysql_real_connect(&mysql,"127.0.0.1","root","111","db_books",0,NULL,0))
    { 
        printf("\n\t 不能连接数据库!\n");
    }
    else
    {
        /*数据库连接成功，插入数据*/   
        printf("\t ════════════════════════════ \n");
        printf("\t                     添 加 图 书 信 息                    \n");
        printf("\t ════════════════════════════ \n");
        if(mysql_query(&mysql,"select * from tb_book"))
        {   //如果查询失败
            printf("\n\t 查询 tb_book 数据表失败!\n");
        }
        else
        {
            result=mysql_store_result(&mysql); //获得结果集
            rowcount=mysql_num_rows(result) ;  //获得行数
            row=mysql_fetch_row(result);       //获取结果集的行  
            
            printf("\t 图书编号:");
            scanf("%s",id);   /*输入图书编号*/    
            
            sql="insert into tb_book (ID,bookname,author,bookconcern) values (";
            strcat(dest,sql);
            strcat(dest,"'");
            strcat(dest,id);
            strcat(dest,"', '");


            if(mysql_num_rows(result)!=NULL)
            {                
                /*判断输入的编号是否存在*/
                do
                {   //存在相同编号
                    if(!strcmp(id,row[0]))
                    {
                      //  printf("%s",row[0]);
                        printf("\n\t 记录存在，按任意键继续!\n");
                        getch();
                        mysql_free_result(result);   /*释放结果集*/ 
                        mysql_close(&mysql);         /*释放连接*/  
                        inquire();   /*询问是否显示主菜单*/
                        return;
                    }
                }while(row=mysql_fetch_row(result));  
            }
            
            bookname = (char*)malloc(50);
            author = (char*)malloc(50);
            bookconcern = (char*)malloc(50);

            
            //不存在相同的编号            
            printf("\t 图书名:");
            scanf("%s",bookname);   /*输入图书名*/
            strcat(dest,bookname);    /*将图书编号追加到sql语句后面*/
            
            printf("\t 作者:");
            scanf("%s",author);    /*输入作者*/
            strcat(dest,"', '");
            strcat(dest,author);
            
            printf("\t 出版社:");
            scanf("%s",bookconcern);    /*输入出版社*/
            strcat(dest,"', '");
            strcat(dest,bookconcern);
            strcat(dest,"')");
            //printf("%s",dest);
            
            if ( mysql_query(&mysql,dest)!=0)   
            {
                fprintf(stderr,"\t 不能插入记录!",mysql_error(&mysql));
            }
            else
            {
                printf("\t 插入成功!\n");                
            }
            mysql_free_result(result);    //释放结果集
        }
        mysql_close(&mysql);          //释放连接
    }
    inquire();   /*询问是否显示主菜单*/
}   




void ModifyBook()   /*修改图书信息*/
{
    char id[10];   /*结果集中的行数*/
    char *sql;    
    char dest[500] ={"  "};
    char dest1[500] ={"  "};
    
    char *bookname;
    char *author;
    char *bookconcern;
    
    if (!mysql_real_connect(&mysql,"127.0.0.1","root","111","db_books",0,NULL,0))
    {
        printf("\t 不能连接数据库!\n");
    }
    else
    {
        /*数据库连接成功*/
        // printf("连接成功");
        printf("\t 请输入您想要修改的图书编号.");
        
        scanf("%s",id);   /*输入图书编号*/
        sql = "select * from tb_book where id=";
        strcat(dest,sql);
        strcat(dest,id);    /*将图书编号追加到sql语句后面*/
        
        //printf("%s\n",dest);
        
        /*查询该图书信息是否存在*/
        if(mysql_query(&mysql,dest))
        {   //如果查询失败
            printf("\n  查询 tb_book 数据表失败! \n");
        }
        else
        {
            result=mysql_store_result(&mysql); //获得结果集
            if(mysql_num_rows(result)!=NULL)
            {
                //有记录的情况,只有有记录取数据才有意义
                printf("\t 发现记录信息,是否显示?(y/n) ");
                scanf("%s",ch);
                if(strcmp(ch,"Y")==0||strcmp(ch,"y")==0)/*判断是否要显示查找到的信息*/
                {
                    printf("\t ════════════════════════════ \n");
                    printf("\t               ***** 显示图书信息 *****                   \n");
                    printf("\t ════════════════════════════ \n");
                    printf("\t图书编号       图书名         作者        出版社      \n");
                    printf("\t -------------------------------------------------------- \n");
                    while((row=mysql_fetch_row(result)))
                    {   //取出结果集中记录
                        fprintf(stdout,"\t   %s             %s             %s          %s   \n",row[0],row[1],row[2],row[3]);  //输出这行记录
                    }
                    printf("\t ════════════════════════════ \n");
                }
                
                
                printf("\t Modify?(y/n)");
                scanf("%s",ch);
                if (strcmp(ch,"Y")==0||strcmp(ch,"y")==0)    /*判断是否需要录入*/
                {

 
                    sql = "update tb_book set bookname= '";
                    strcat(dest1,sql);
                    //   printf("%s",sql);
                    bookname = (char*)malloc(20);
                    author = (char*)malloc(20);
                    bookconcern = (char*)malloc(20);

                    printf("\t 图书名:");
                    scanf("%s",bookname);   /*输入图书名*/
                    
                    strcat(dest1,bookname); 
                    //  printf("%s",&bookname);

                    printf("\t 作者:");
                    scanf("%s",author);    /*输入作者*/
                    strcat(dest1,"', author= '");
                    strcat(dest1,author);    /*追加sql语句*/                        
                    
                    printf("\t 出版社:");
                    scanf("%s",bookconcern);    /*输入出版社*/
                    strcat(dest1,"', bookconcern = '");
                    strcat(dest1,bookconcern);    /*追加sql语句*/
                    
                    strcat(dest1,"' where id= ");
                    strcat(dest1,id);                        
                    
                    //printf("%s",dest1);

                    if(mysql_query(&mysql,dest1)!=0)
                    {   
                        fprintf(stderr,"\t 不能修改记录!\n",mysql_error(&mysql));                            
                    }
                    else
                    {
                        printf("\t 修改成功!\n");
                    }
                }
                
            }
            else
            {
                printf("\t 没有发现要修改的信息!\n");
            }
        }
        mysql_free_result(result);          //释放结果集
    }
    mysql_close(&mysql);     //释放连接
    inquire();   /*询问是否显示主菜单*/
}



void DeleteBook()   /*删除图书信息*/
{
    char id[10];   /*结果集中的行数*/
    char *sql;
    char dest[100] ={"  "};
    char dest1[100] ={"  "};
    if(!mysql_real_connect(&mysql,"127.0.0.1","root","111","db_books",0,NULL,0))
    {
        printf("\t 不能连接数据库!\n");
    }
    else
    {
        printf("\t 请输入您想要删除的图书编号. ");        
        scanf("%s",id);   /*输入图书编号*/
        sql = "select * from tb_book where id=";
        strcat(dest,sql);
        strcat(dest,id);    /*将图书编号追加到sql语句后面*/        
        //printf("%s\n",dest);
        
        /*查询该图书信息是否存在*/
        if(mysql_query(&mysql,dest))
        {   //如果查询失败
            printf("\n 查询 tb_book 数据表失败! \n");
        }
        else
        { 
            result=mysql_store_result(&mysql); //获得结果集
            if(mysql_num_rows(result)!=NULL)
            {  //有记录的情况,只有有记录取数据才有意义
                printf("\t 发现记录信息,是否显示?(y/n) ");
                scanf("%s",ch);
                if(strcmp(ch,"Y")==0||strcmp(ch,"y")==0)/*判断是否要显示查找到的信息*/
                {
                    printf("\t ════════════════════════════ \n");
                    printf("\t               ***** 显示图书信息 *****                   \n");
                    printf("\t ════════════════════════════ \n");
                    printf("\t图书编号       图书名         作者        出版社      \n");
                    printf("\t -------------------------------------------------------- \n");
                    while((row=mysql_fetch_row(result)))
                    {   //取出结果集中记录
                        fprintf(stdout,"\t   %s             %s             %s          %s   \n",row[0],row[1],row[2],row[3]);  //输出这行记录
                    }
                    printf("\t ════════════════════════════ \n");
                }
                                    
                    printf("\t 是否删除?(y/n) ");
                    scanf("%s",ch);
                    if (strcmp(ch,"Y")==0||strcmp(ch,"y")==0)    /*判断是否需要录入*/
                    {
                        sql = "delete from tb_book where ID= ";
                        printf("%s",dest1);
                        strcat(dest1,sql);
                        strcat(dest1,id);
                       // printf("%s",dest1);
                        
                        if(mysql_query(&mysql,dest1)!=0)
                        {
                            fprintf(stderr,"\t 不能删除记录! \n",mysql_error(&mysql));
                        }
                        else
                        {
                            printf("\t 删除成功!\n");
                        }
                    }
            }
            else
            {
                printf("\t 没有发现要删除的信息!\n");
            }
        }
        mysql_free_result(result);    //释放结果集
    }
    mysql_close(&mysql);
    inquire();   /*询问是否显示主菜单*/
}



void QueryBook()   /*查询图书信息*/
{     
    char id[10];   /*结果集中的行数*/
    char *sql;
    char dest[100] ={"  "};
    
    if(!mysql_real_connect(&mysql,"127.0.0.1","root","111","db_books",0,NULL,0))
    {
        printf("\t 不能连接数据库!\n");
    }
    else
    {  
        printf("\t 请输入您想要查询的图书编号:");        
        scanf("%s",id);   /*输入图书编号*/
        sql = "select * from tb_book where id=";
        strcat(dest,sql);
        strcat(dest,id);    /*将图书编号追加到sql语句后面*/        
        
        if(mysql_query(&mysql,dest))
        {   //如果查询失败
            printf("\n 查询 tb_book 数据表失败!\n");
        }
        else
        {
            result=mysql_store_result(&mysql); //获得结果集
            if(mysql_num_rows(result)!=NULL)
            {  //有记录的情况,只有有记录取数据才有意义
                printf("\t ════════════════════════════ \n");
                printf("\t                          显示图书信息                      \n");
                printf("\t ════════════════════════════ \n");
                printf("\t图书编号       图书名         作者        出版社      \n");
                printf("\t -------------------------------------------------------- \n");
                while((row=mysql_fetch_row(result)))
                {   //取出结果集中记录
                    fprintf(stdout,"\t   %s             %s             %s          %s   \n",row[0],row[1],row[2],row[3]);  //输出这行记录
                }
                printf("\t ════════════════════════════ \n");                
            }
            else
            {
                printf("\t 没有发现要查询的信息!\n");
            }
            mysql_free_result(result);    //释放结果集
        }
        mysql_close(&mysql);          //释放连接
    }
    inquire();   /*询问是否显示主菜单*/
}
