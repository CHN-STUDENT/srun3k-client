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
			printf("\n您不在线。\n");
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
	CURL *curl = curl_easy_init();
	CURL *curl1 = curl_easy_init();//定义CURL句柄，初始化为urlencode做准备
	if(!(curl&&curl1))
	{//如果非正常初始化 
	 	printf("\nLibcurl初始化失败，请重新打开本程序重试，如果多次失败，请向我反馈。\n");
	 	system("pause"); 
		exit(-1); 
	} 
	if(curl&&curl1)
	{//如果正常初始化
		char acid[8]="&ac_id=";
		char username[20];
		char username_Post[50]="{SRUN3}\r\n"; 
		char LOGIN[200]="action=login&drop=0&pop=1&type=2&n=117&mbytes=0&minutes=0&mac=&username=";
		char *ptr=LOGIN;
		while(*ptr!='\0')
 		  	ptr++;  
		printf("请输入你的用户名:");
		gets(username); 
		for (int i = 0; i<strlen(username); ++i)
		{//用户名输入加密 
			username[i] = username[i] + 4;
		}
		strcat(username_Post,username);
		char *name_urlencode = curl_easy_escape(curl,username_Post,0);
		while(*name_urlencode!='\0')
		{
			*ptr=*name_urlencode;
			ptr++;
			name_urlencode++;
		}
		for(char *password_Post="&password=";*password_Post!='\0';password_Post++,ptr++)
		{
			*ptr=*password_Post;
		}
		char password[20];
		printf("\n请输入你的密码（不显示）:");
		char ch;
		unsigned int i;
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
		char key[] = "1234567890";//加密key
		char password_encrypt[50] ="";
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
		char *password_urlencode = curl_easy_escape(curl1,password_encrypt,0);
		while(*password_urlencode !='\0')
		{
			*ptr=*password_urlencode ;
			ptr++;
			password_urlencode++;
		}
		*ptr='\0'; 
		curl_easy_cleanup(curl);//清理
		curl_easy_cleanup(curl1);//清理
		ACID_INPUT(acid);//ACID输入
		strcat(LOGIN,acid);
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
		curl_easy_setopt(curl_post, CURLOPT_POSTFIELDS,LOGIN);//设置POST参数
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
	}
	return 0;
}
