#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>

const char POSTURL[]="http://172.16.154.130:69/cgi-bin/srun_portal";  //定义POST地址

size_t POST_FEEDBACK(void *ptr, size_t size, size_t nmemb, void *stream)
{//处理得到的服务器回传 
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
			printf("\n您不在线，登出失败。\n");
			break;
		} 
	}
	printf("\n");
	system("pause"); 
	exit(0);
}

void ACID_INPUT(char *acid) 
{//ACID输入入函数 
	char ac_id[2]= {0};
    do
    {
        if (ac_id[0]!= '\0')
            printf("\n对不起，输入错误，请重试!\n");
        printf("\n请输入您的ACID<1/2>:");
        scanf("%1s",&ac_id[0]);
    } 
    while (ac_id[0]!= '1'&&ac_id[0]!= '2');
    strcat(acid,ac_id);
} 

int main(void)
{//登入函数
	char LOGOUT[50] = "username=0&mac=&type=2&action=logout";//学长抓包分析得到的头文件
 	char acid[8]="&ac_id=";
 	ACID_INPUT(acid);//ACID输入
	strcat(LOGOUT,acid);
	CURL *curl_post;
	CURLcode res;
	curl_post = curl_easy_init();//初始化libcurl
	if (!curl_post)
	{
	    printf("\nLibcurl初始化失败，请重新打开本程序重试，如果多次失败，请向我反馈。\n");
	    system("pause"); 
		exit(-1); 
	}
	/*以下是设置参数*/ 
	curl_easy_setopt(curl_post, CURLOPT_URL, POSTURL); //设置POST地址
	curl_easy_setopt(curl_post, CURLOPT_POSTFIELDS,LOGOUT);//设置POST参数
	curl_easy_setopt(curl_post,CURLOPT_TIMEOUT,1L);//超时设置成1s
	//curl_easy_setopt(curl_post,CURLOPT_VERBOSE,1); //啰嗦模式
	curl_easy_setopt(curl_post, CURLOPT_WRITEFUNCTION,POST_FEEDBACK);//接受回传 
	/*设置参数结束*/ 
	res = curl_easy_perform(curl_post);//启动libcurl 
	if (res != CURLE_OK)
		{
			printf("\nPOST错误!请检查你的网络连接!\n");
			curl_easy_cleanup(curl_post);//清理
			printf("\n");
			system("pause"); 
			exit(-1); 
		}
	curl_easy_cleanup(curl_post);//清理
	return 0;
}
