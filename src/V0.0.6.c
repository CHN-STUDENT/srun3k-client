#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

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

int HTTP_POST_LOGIN(void)
{//登入函数
	CURL *curl = curl_easy_init();
	CURL *curl1 = curl_easy_init();//定义CURL句柄，初始化为urlencode做准备
	if(!(curl&&curl1))
	{//如果非正常初始化 
	 	printf("\nLibcurl初始化失败，请重新打开本程序重试，如果多次失败，请向我反馈。\n");
		exit(-1); 
	} 
	if(curl&&curl1)
	{//如果正常初始化
		char acid[8]="&acid:";
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
		 	exit(-1); 
		}
		/*以下是设置参数*/ 
		curl_easy_setopt(curl_post, CURLOPT_URL, POSTURL); //设置POST地址
		curl_easy_setopt(curl_post, CURLOPT_POSTFIELDS,LOGIN);//设置POST参数
		//curl_easy_setopt(curl_post,CURLOPT_TIMEOUT,1L);//超时设置成1s
		//curl_easy_setopt(curl_post,CURLOPT_VERBOSE,1); //啰嗦模式
		curl_easy_setopt(curl_post, CURLOPT_WRITEFUNCTION,POST_FEEDBACK);//接受回传 
		/*设置参数结束*/ 
		res = curl_easy_perform(curl_post);//启动libcurl 
		if (res != CURLE_OK)
		{
			printf("\nPOST错误!\n");
			curl_easy_cleanup(curl_post);//清理
			exit(-1); 
		}
		curl_easy_cleanup(curl_post);//清理
	}
	exit(0); 
}

int HTTP_POST_LOGOUT(void)
{//登出函数
 	char LOGOUT[50] = "username=0&mac=&type=2&action=logout";//学长抓包分析得到的头文件
 	char acid[8]="&acid:";
 	ACID_INPUT(acid);//ACID输入
	strcat(LOGOUT,acid);
	CURL *curl_post;
	CURLcode res;
	curl_post = curl_easy_init();//初始化libcurl
	if (!curl_post)
	{
			printf("\nLibcurl初始化失败，请重新打开本程序重试，如果多次失败，请向我反馈。\n");
		 	exit(-1); 
	}
	/*以下是设置参数*/ 
	curl_easy_setopt(curl_post, CURLOPT_URL, POSTURL); //设置POST地址
	curl_easy_setopt(curl_post, CURLOPT_POSTFIELDS,LOGOUT);//设置POST参数
	//curl_easy_setopt(curl_post,CURLOPT_TIMEOUT,1L);//超时设置成1s
	//curl_easy_setopt(curl_post,CURLOPT_VERBOSE,1); //啰嗦模式
	curl_easy_setopt(curl_post, CURLOPT_WRITEFUNCTION,POST_FEEDBACK);//接受回传 
	/*设置参数结束*/ 
	res = curl_easy_perform(curl_post);//启动libcurl 
	if (res != CURLE_OK)
		{
			printf("\nPOST错误!\n");
			curl_easy_cleanup(curl_post);//清理
			exit(-1); 
		}
	curl_easy_cleanup(curl_post);//清理
	exit(0); 
}

void ASK_LOGIN_OR_EXIT(void)
{//询问是否登入或者退出函数 
	char login_or_exit = '\0';
    do
    {
        if (login_or_exit!= '\0')
            printf("\n对不起，输入错误，请重试!\n");
        printf("\n是否登入或者退出。\n输入<1.登入/2.退出>:\n");
        scanf("%1s",&login_or_exit);
    } 
    while (login_or_exit!= '1'&&login_or_exit!= '2');
	if(login_or_exit==1)
		//如果要登入 
		HTTP_POST_LOGIN();//执行登入  
	if(login_or_exit==2)
		//如果要退出
		exit(0); 
}

void ASK_LOGOUT_OR_EXIT(void)
{//询问是否登出或者退出函数 
	char logout_or_exit = '\0';
    do
    {
        if (logout_or_exit!= '\0')
            printf("\n对不起，输入错误，请重试!\n");
        printf("\n是否登出或者退出。\n输入<1.登出/2.退出>:\n");
        scanf("%1s",&logout_or_exit);
    } 
    while (logout_or_exit!= '1'&&logout_or_exit!= '2');
	if(logout_or_exit==1)
	{//如果要登出
		HTTP_POST_LOGOUT();//执行登出 
		ASK_LOGIN_OR_EXIT();//询问是否登入或者退出 
	}
	if(logout_or_exit==2)
	//如果要退出
		exit(0); 	 	 
}

size_t GET_INFO(void *ptr, size_t size, size_t nmemb, void *stream)
{//处理得到用户信息函数
	char *k=(char*)ptr;//将void *空指针强制转为char *指针 
		  	if(*k=='n')
			{ //如果未登陆 
				HTTP_POST_LOGIN();//执行登陆;
			}
			else
			{ //如果已登录 
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
							printf("\n您这个月所用流量: %f GB\n",1.0*atof(temp)/1073741824);//atof(string)转浮点 
						if(flag==8)
							printf("\n您这个月所用时间: %03d小时%02d分钟%02d秒\n",atoi(temp)/3600,atoi(temp)/60%60,atoi(temp)%60); //atoi(string)转整数 
						if(flag==9)
						{
							printf("\n您 的 登 入  IP : %s\n",temp);
							break;
						}
						j++; 
						continue;
					}	
				}
				ASK_LOGOUT_OR_EXIT();//询问是否登出或者退出
			} 		
}

int HTTP_GET_INFO(void)
{//获取当前登陆状态函数
	printf("+++++++++++++正在获取您的信息,请稍后:+++++++++++++\n"); 
	const char GET_INFO_URL[]="http://172.16.154.130/cgi-bin/rad_user_info";//定义获取登陆状态函数
	CURL *curl;//定义CURL的句柄 
  	CURLcode res;//定义CURL反馈结果
  	curl = curl_easy_init(); //CURL句柄初始化 
  	if(!(curl))
  	{//如果非正常初始化 
	 	printf("\nLibcurl初始化失败，请重新打开本程序重试，如果多次失败，请向我反馈。\n");
	 	exit(-1); 
	}
	if(curl)
	{//如果正常初始化
		curl_easy_setopt(curl,CURLOPT_URL,GET_INFO_URL);//设置得到用户信息的GET地址
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,GET_INFO);//设置处理得到用户信息函数
		//curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//超时设置成1s
		//curl_easy_setopt(curl,CURLOPT_VERBOSE,1); //啰嗦模式
		res = curl_easy_perform(curl);//启动libcurl 
		if (res != CURLE_OK)
		{
			printf("\nPOST错误!\n");
			curl_easy_cleanup(curl);//清理
			exit(-1); 
		}
		curl_easy_cleanup(curl);//清理
	}
}
		
int main()
{ 
	printf("+++++++++++++欢迎使用校园网登陆器C语言版+++++++++++++\n");
	printf("+++++++Made By CHN-STUDENT && MouYouLing V0.0.6+++++++\n"); 
	HTTP_GET_INFO();//获取当前登陆状态 
	system("pause"); 
	return 0;
}
