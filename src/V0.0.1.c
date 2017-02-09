#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>
#define POSTURL "http://172.16.154.130:69/cgi-bin/srun_portal"  //定义POST地址  


int main()
{
	char username[20];
	char post[200] = "action:login&drop:0&pop:1&type:2&n:117&acid:1&username:{SRUN3}\r\n";//学长抓包分析得到的头文件
	char password[20];
	char key[] = "1234567890";//加密key
	char password_encrypt[50] = "";
	unsigned int i;
	printf("+++++++欢迎使用校园网登陆器C语言版+++++++\n");
	printf("+++++++Made By CHN-STUDENT V0.0.1+++++++\n");
	printf("请输入你要登录的用户名:");
	gets(username);
	/*用户名加密部分开始*/
	for (i = 0; i<strlen(username); ++i)
	{
		username[i] = username[i] + 4;
	}
	strcat(post, username);//复制加密后用户名到post数据字符串
	strcat(post, "&password:");
	/*用户名加密部分结束*/
	printf("请输入你要登录的密码（不显示）:");
	/*密码输入部分开始*/
	char ch;
	for (i = 0; (ch = _getch()) != 13; )
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
	strcat(post, password_encrypt);//复制密码到post数据字符串
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();//初始化libcurl
	if (!curl)
	{
		fprintf(stderr, "libcurl初始化失败！\n");
		return -1;
	}
	curl_easy_setopt(curl, CURLOPT_URL, POSTURL); //设置POST地址
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);//设置POST参数
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		printf("POST错误!\n");
		return -1;
	}
	curl_easy_cleanup(curl);//清理
	return 0;
}
