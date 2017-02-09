#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>
const char POSTURL[]="http://172.16.154.130:69/cgi-bin/srun_portal";  //定义POST地址  

void username_pt(char *username) //用户名加密函数 
{
	int i;
	/*用户名加密部分开始*/
	for (i = 0; i<strlen(username); ++i)
	{
		username[i] = username[i] + 4;
	}
}
void password_input(char *password) //密码输入函数 
{
	/*密码输入部分开始*/
	char ch;
	int i;
	for (i = 0; (ch = getch()) != 13; )
	{
		if (ch == 8)
		{
			if (i == 0)
				password[0] = '\0';
			else
			{
				password[i - 1] = '\0';
				i--;
			}
		}
		else
		{
			password[i] = ch;
			i++;
		}

	}
	password[i] = '\0';
	printf("\n");
	/*密码输入部分结束*/
}
void passwd_pt(char *password) //密码加密函数 
{
	unsigned int i;
	char key[] = "1234567890";//加密key
	char password_encrypt[50] = "";
	/*密码加密部分开始*/
	for (i = 0; i<strlen(password); ++i)
	{
		int ki = password[i] ^ key[strlen(key) - i%strlen(key) - 1];
		char _l[4] = { (char)((ki & 0x0f) + 0x36) };
		char _h[4] = { (char)((ki >> 4 & 0x0f) + 0x63) };
		if (i % 2 == 0)
		{
			strcat(_l, _h);
			strcat(password_encrypt, _l);
		}
		else
		{
			strcat(_h, _l);
			strcat(password_encrypt, _h);
		}
	}
	/*密码加密部分结束*/
	strcpy(password,password_encrypt);
}
void acid_input(char *acid) //acid输入函数 
{
	char ac_id[2]= {0};
    do
    {
        if (ac_id[0]!= '\0')
            printf("\n对不起，输入错误，请重试!\n");
        printf("\n请输入您的ACID<1/2>:");
        scanf("%1s",&ac_id[0]);
    } 
    while ( ac_id[0]!= '1'&&ac_id[0]!= '2');
    strcat(acid,ac_id);
} 
int libcurl(const char *POSTURL,char *post) //libcurl
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();//初始化libcurl
	if (!curl)
	{
		fprintf(stderr, "\nlibcurl初始化失败！\n");
		return -1;
	}
	curl_easy_setopt(curl, CURLOPT_URL, POSTURL); //设置POST地址
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);//设置POST参数
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		printf("\nPOST错误!\n");
		return -1;
	}
	curl_easy_cleanup(curl);//清理
	return 0;
}
int main()
{
	char username[20];
	char post[200] = "drop:0&pop:1&type:2&n:117&username:{SRUN3}\r\n";//学长抓包分析得到的头文件
	char password[20];
	const char action_login[]="&action:login";
	const char action_logout[]="&action:logout";
	char acid[8]="&acid:";
	unsigned int i;
	printf("+++++++++++++欢迎使用校园网登陆器C语言版+++++++++++++\n");
	printf("+++++++Made By CHN-STUDENT && MouYouLing V0.0.3+++++++\n\n"); 
	printf("请输入你的用户名:");
	gets(username);
	username_pt(username);//用户名加密 
	printf("\n请输入你的密码（不显示）:");
	password_input(password);//密码输入 
	passwd_pt(password);//密码加密
	strcat(post,username);//复制加密后用户名到post数据字符串
	strcat(post, "&password:");
	strcat(post,password);//复制密码到post数据字符串
	acid_input(acid);//acid输入 
	strcat(post,acid);//复制acid到post 
	char login_or_logout = '\0';
    do
    {
        if (login_or_logout!= '\0')
            printf("\n对不起，输入错误，请重试!\n");
        printf("\n请输入<1.登陆/2.注销>:");
        scanf("%1s",&login_or_logout);
    } 
    while (login_or_logout!= '1'&&login_or_logout!= '2');
    if(login_or_logout== '1')
    	strcat(post,action_login);
    if(login_or_logout== '2')
    	strcat(post,action_logout);
	libcurl(POSTURL,post);//libcurl
	return 0;
}


