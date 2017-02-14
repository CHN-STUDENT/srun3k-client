# 研究如何处理服务器GET和POST回传。

## 下面这个是处理服务器POST回传的消息。

```c
/*****************************研究如何处理服务器回传***************************** 
		    				一般服务器回传有以下信息 
login_ok,1573330604,0,0,0.0,0.0,0,0,0,0,0,1.01.20160715 //登陆成功 
logout_ok											    //注销成功 
missing_required_parameters_error                       //缺少参数 
login_error#E2553: Password is error.                   //密码错误 
login_error#E2531: User not found.                      //用户未找到 
login_error#INFO failed, BAS respond timeout.           //ACID错误 
login_error#You are not online.							//你不在线 
*********************************************************************************/
#include <stdio.h>
#include <string.h>

char login_ok[]="login_ok,1573330604,0,0,0.0,0.0,0,0,0,0,0,1.01.20160715"; //登陆成功 
void *ptr1=login_ok;
char logout_ok[]="logout_ok";										      //注销成功 
void *ptr2=logout_ok;
char error1[]="missing_required_parameters_error";                         //缺少参数
void *ptr3=error1;
char error2[]="login_error#E2553: Password is error.";                     //密码错误
void *ptr4=error2; 
char error3[]="login_error#E2531: User not found.";                        //用户未找到 
void *ptr5=error3; 
char error4[]="login_error#INFO failed, BAS respond timeout.";            //ACID错误 
void *ptr6=error4; 
char error5[]="login_error#You are not online.";	                  //你不在线 
void *ptr7=error5; 

int main()
{
	char *k=(char*)ptr;//将void *空指针强制转为char *指针
	for(;*k!='\0';k++) 
	{
		if(*k=='m')
		{//参数错误情况 
			printf("\n缺少参数，请检查输入。\n");
			break;
		} 
		else if(*k=='t'&&*(k+2)=='o')
		{//注销成功情况
			printf("\n注销成功，感谢您的使用。\n");
			break;
		}
		else if(*k=='n'&&*(k+2)=='o')//
		{//登陆成功情况
			printf("\n登陆成功，感谢您的使用。\n");
			break;
		} 
		else if(*k=='P'&&*(k+1)=='a') 
		{//密码错误情况
			printf("\n密码错误，请检查输入。\n");
			break;
		} 
		else if(*k=='U'&&*(k+1)=='s')//
		{//用户名错误情况
			printf("\n用户名不存在，请检查输入。\n");
			break;
		} 
		else if(*k=='I'&&*(k+1)=='N') 
		{//用户名错误情况
			printf("\nACID错误，请检查输入。\n");
			break;
		}
		else if(*k=='Y'&&*(k+1)=='o')
		{//不在线情况 
			printf("\n您不在线。\n");
			break;
		} 
	}
	return 0;
}
```

## 下面这个是处理服务器GET回传的消息(用户数据)。

V2版，多了一些信息转换。

```c
#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>
//如果遇到逗号，复制逗号前数据给temp 
char info1[]="201416010000,1474719765,1474719771,0,0,0,78570982665,1010177,172.30.85.5,0,,0.000000,0,0,0,0,0,0,0,0,0,1.01.20160715";
char info2[]="not_online"; 
void *ptr=info1;

int main()
{
  	char *k=(char*)ptr;//将void *空指针强制转为char *指针 
  	if(*k=='n')
	{ 
		printf("您不在线!");
	}
	else
	{ 
		printf("\n+++++++++++++您已在线，下面是您的信息:+++++++++++++\n");
		char *j=(char*)ptr;//备份指针,做计数用 
		for(int flag=0;k!='\0';k++)//开始循环直到数据末尾 
		{
			char temp[20]; //定义一个临时变量
			if(*k==',')//如果当前位为,分隔符 
			{	
				flag++;//,分隔符计数 
				char *t=temp;//定义一个临时指针指向t
				for(;*j!=*k;j++,t++)
					*t=*j; 
				*t='\0';
				if(flag==1)
					printf("\n您的登入用户名是: %s\n",temp);
				if(flag==7)
					printf("\n您这个月所用流量: %f GB\n",1.0*atof(temp)/1073741824);
				if(flag==8)
					printf("\n您这个月所用时间: %03d小时%02d分钟%02d秒\n",atoi(temp)/3600,atoi(temp)/60%60,atoi(temp)%60); 
				if(flag==9)
				{
					printf("\n您 的 登 入  IP : %s\n",temp);
					break;
				}
				j++; 
				continue;
			}	
		}
	} 
	return 0;
}
```

V1版，没有信息转换。

```c
#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>
//如果遇到逗号，复制逗号前数据给temp 
char info1[]="201416010000,1474719765,1474719771,0,0,0,78570982665,1010177,172.30.85.5,0,,0.000000,0,0,0,0,0,0,0,0,0,1.01.20160715";
char info2[]="not_online"; 
void *ptr=info1;

int main()
{
  	char *k=(char*)ptr;//将void *空指针强制转为char *指针 
  	if(*k=='n')
	{ 
		printf("您不在线!");
	}
	else
	{ 
		printf("\n+++++++++++++您已在线，下面是您的信息:+++++++++++++\n");
		char *j=(char*)ptr;//备份指针,做计数用 
		for(int flag=0;k!='\0';k++)//开始循环直到数据末尾 
		{
			char temp[20]; //定义一个临时变量
			if(*k==',')//如果当前位为,分隔符 
			{	
				flag++;//,分隔符计数 
				char *t=temp;//定义一个临时指针指向t
				for(;*j!=*k;j++,t++)
					*t=*j; 
				*t='\0';
				if(flag==1)
					printf("\n您的登入用户名是: %s\n",temp);
				if(flag==7)
					printf("\n您这个月所用流量: %s Byte\n",temp);
				if(flag==8)
					printf("\n您这个月所用时间: %s 秒\n",temp);
				if(flag==9)
				{
					printf("\n您 的 登 入  IP : %s\n",temp);
					break;
				}
				j++; 
				continue;
			}	
		}
	} 
	return 0;
}
```
