#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>

const char POSTURL[]="http://172.16.154.130:69/cgi-bin/srun_portal";  //定义POST地址

struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "\n            内存错误！            \n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "\n            内存错误!            \n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

int HTTP_LOGIN(CURL *curl,CURLcode res) 
{
	CURL *curl1 = curl_easy_init();
	CURL *curl_post = curl_easy_init();
	if(!(curl&&curl1&&curl_post))
  	{//如果非正常初始化 
	 	printf("\nLibcurl初始化失败，请重新打开本程序重试!\n");
	 	system("pause"); 
	 	exit(-1); 
	}
	int set=0;
	char username[20];
	char username_Post[50]="{SRUN3}\r\n";  
		do
		{		
				char LOGIN[200]="&ac_id=1&action=login&drop=0&pop=1&type=3&n=117&mbytes=0&minutes=0&mac=&username=";	
				char *ptr=LOGIN;
				while(*ptr!='\0')
			 		  	ptr++;  
				if(set==0||set==-2)
				{//如果是第一次或者用户名输入错误要求重输则输入用户名并加密 
			 		printf("\n----------------------------------------\n");
					printf("\n            请输入你的用户名:           \n");
					gets(username); 
					for (int i = 0; i<strlen(username); ++i)
					{//用户名输入加密 
						username[i] = username[i] + 4;
					}
					strcat(username_Post,username);
				} 
					char *name_urlencode = curl_easy_escape(curl,username_Post,0);
					while(*name_urlencode!='\0')
					{
						*ptr=*name_urlencode;
						ptr++;
						name_urlencode++;
					}
					char password[20];
					printf("\n----------------------------------------\n");
					printf("\n         请输入你的密码（不显示）:      \n");
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
					printf("\n----------------------------------------\n");
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
					for(char *password_Post="&password=";*password_Post!='\0';password_Post++,ptr++)
					{
						*ptr=*password_Post;
					}
					char *password_urlencode = curl_easy_escape(curl1,password_encrypt,0);
					while(*password_urlencode !='\0')
					{
						*ptr=*password_urlencode ;
						ptr++;
						password_urlencode++;
					}
					*ptr='\0'; 
					struct string in;
			    	init_string(&in);
			    	if(!(curl_post))
				  	{//如果非正常初始化 
					 	printf("\nLibcurl初始化失败，请重新打开本程序重试!\n");
					 	system("pause"); 
					 	exit(-1); 
					}
					curl_easy_setopt(curl_post, CURLOPT_URL, POSTURL); //设置POST地址
					curl_easy_setopt(curl_post, CURLOPT_POSTFIELDS,LOGIN);//设置POST参数
					curl_easy_setopt(curl_post,CURLOPT_TIMEOUT,1L);//超时设置成1s
					curl_easy_setopt(curl_post, CURLOPT_WRITEFUNCTION, writefunc);
				    curl_easy_setopt(curl_post, CURLOPT_WRITEDATA, &in);
					 while(1)
			    	{
						res = curl_easy_perform(curl_post);//启动libcurl 
						if (res != CURLE_OK)
						{
							printf("\n----------------------------------------\n");
							printf("\n              *** 错误: ***             \n");
							printf("\n  无法获取您的信息,请检查您的网络连接!  \n");
							printf("\n----------------------------------------\n");
							char input;
							while(1)
							{
								printf("\n     您是否要重试？<1.重试/2.退出>:     \n");
								char input=getch();
								if(input=='1')
								{ 
								   	break;
								}
							   	if(input=='2')
							   		exit (0);
							   	else
								{
									printf("\n----------------------------------------\n");
									printf("\n             输入错误请重试!            \n"); 
									printf("\n----------------------------------------\n");
								}
							}
						}
						if(res == CURLE_OK)
							break;
					}
					char *k=(char*)in.ptr;
					printf("\n----------------------------------------\n");
					for(;*k!='\0';k++) 
					{
						if(*k=='n'&&*(k+2)=='o')//
						{//登陆成功情况
							set=0;
							printf("\n              登陆成功              \n");
							printf("\n----------------------------------------\n");
							break;
						} 
						else
						{  
							if(*k=='P'&&*(k+1)=='a') 
							{//密码错误情况
								printf("\n        密码错误，请检查输入。      \n");
								set=-1;
								break;
							} 
							if(*k=='U'&&*(k+1)=='s')
							{//用户名错误情况
								printf("\n      用户名不存在，请检查输入。    \n");
								set=-2;
								break;
							} 
							if(*k=='I'&&*(k+1)=='N') 
							{//ACID错误情况
								printf("\n              ACID错误              \n");
								break;
							}
							if(*k=='m')
							{//参数错误情况 
								printf("\n              缺少参数              \n");
								break;
							} 
							
						} 
					}
				printf("\n----------------------------------------\n");	
				if(set!=0)
				{
					while(1)
					{
							printf("\n     您是否要重试？<1.重试/2.退出>:     \n");
							char input=getch();
							if(input=='1')
							{
								break;
							}
						   	if(input=='2')
						   		exit (0);
						   	else
							{
								printf("\n----------------------------------------\n");
								printf("\n             输入错误请重试!            \n"); 
								printf("\n----------------------------------------\n");
							}
					}
				}
		}
		while(set==-2||set==-1); 
		curl_easy_cleanup(curl);//清理
		curl_easy_cleanup(curl1);//清理
		curl_easy_cleanup(curl_post);//清理 
	
}; 

int HTTP_LOGOUT(CURL *curl,CURLcode res) 
{
	char LOGOUT[50] = "username=0&mac=&type=2&action=logout&ac_id=1";//学长抓包分析得到的头文件
	struct string out;
    init_string(&out);
	curl_easy_setopt(curl, CURLOPT_URL, POSTURL); //设置POST地址
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS,LOGOUT);//设置POST参数
	curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//超时设置成1s
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out); 
  	while(1)
    {
		res = curl_easy_perform(curl);//启动libcurl 
		if (res != CURLE_OK)
		{
			printf("\n----------------------------------------\n");
			printf("\n              *** 错误: ***             \n");
			printf("\n  无法获取您的信息,请检查您的网络连接!  \n");
			printf("\n----------------------------------------\n");
			char input;
			while(1)
			{
				printf("\n     您是否要重试？<1.重试/2.退出>:     \n");
				char input=getch();
				if(input=='1')
				{ 
				   	break;
				}
			   	if(input=='2')
			   		exit (0);
			   	else
				{
					printf("\n----------------------------------------\n");
					printf("\n             输入错误请重试!            \n"); 
					printf("\n----------------------------------------\n");
				}
			}
		}
		if(res == CURLE_OK)
			break;
	}
	printf("\n----------------------------------------\n");
	char *k=(char *)out.ptr;
	for(;;k++) 
	{
		if(*k=='m')
		{//参数错误情况 
			printf("\n              缺少参数              \n");
			break;
		} 
		else if(*k=='t'&&*(k+2)=='o')
		{//注销成功情况
			printf("\n              注销成功              \n");
			return 0;
			break;
		}
		else if(*k=='I'&&*(k+1)=='N') 
		{//ACID错误情况
			printf("\n              ACID错误              \n");
			break;
		}
		else if(*k=='Y'&&*(k+1)=='o')
		{//不在线情况 
			printf("\n         您不在线，登出失败         \n");
			return 0; 
			break;
		} 
	}
	printf("\n----------------------------------------\n");
	curl_easy_cleanup(curl);//清理
}; 

int HTTP_GET_INFO(CURL *curl,CURLcode res)
{
	printf("\n-------正在获取您的信息，请稍后:--------\n"); 
	struct string s;
    init_string(&s);
	const char GET_INFO_URL[]="http://172.16.154.130/cgi-bin/rad_user_info";//定义获取登陆状态函数
	curl_easy_setopt(curl,CURLOPT_URL,GET_INFO_URL);//设置得到用户信息的GET地址
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//超时设置成1s
    while(1)
    {
		res = curl_easy_perform(curl);//启动libcurl 
		if (res != CURLE_OK)
		{
			printf("\n----------------------------------------\n");
			printf("\n              *** 错误: ***             \n");
			printf("\n  无法获取您的信息,请检查您的网络连接!  \n");
			printf("\n----------------------------------------\n");
			char input;
			while(1)
			{
				printf("\n     您是否要重试？<1.重试/2.退出>:     \n");
				char input=getch();
				if(input=='1')
				{ 
				   	break;
				}
			   	if(input=='2')
			   		exit (0);
			   	else
				{
					printf("\n----------------------------------------\n");
					printf("\n             输入错误请重试!            \n"); 
					printf("\n----------------------------------------\n");
				}
			}
		}
		if(res == CURLE_OK)
			break;
	}
	(char *)s.ptr;
	if(*s.ptr=='n')
	{//如果未登陆 
		printf("\n       您不在线，请你输入信息登陆:        \n");
		return 0;
	}
	else
		{//如果已登录 
			printf("\n       您已在线，下面是您的信息:        \n");
			printf("\n----------------------------------------\n");
			char *j=(char *)s.ptr;
			for(int flag=0;;s.ptr++)//开始循环直到数据末尾 
				{
					char temp[20]; //定义一个临时变量
					if(*s.ptr==',')//如果当前位为,分隔符 
					{	
						flag++;//,分隔符计数 
						char *t=temp;//定义一个临时指针指向t
						for(;*j!=*s.ptr;j++,t++)
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
			printf("\n----------------------------------------\n");
			return 1;
		} 
	curl_easy_cleanup(curl);//清理
}

void PRINT_WELCOME_INFO(void)
{//输出欢迎信息
	printf("\n----------------------------------------\n");
	printf("      欢迎使用校园网登陆器 C 语言版     \n");	
	printf("Made By CHN-STUDENT & MouYouLing V0.0.8\n");
	printf("\n----------------------------------------\n");
}

int main()
{ 
	system("mode con cols=40");//设置宽度40
	system("color A");//字体设置为绿色 
	PRINT_WELCOME_INFO();
	int state=0;//定义状态，0代表未登录，1代表已经登陆，默认未登录 
  	CURL *curl;//定义CURL的句柄 
  	CURLcode res;//定义CURL反馈结果
  	curl_global_init(CURL_GLOBAL_ALL);//全局初始化 
  	curl = curl_easy_init(); //CURL句柄初始化 
  	if(!(curl))
  	{//如果非正常初始化 
	 	printf("\nLibcurl初始化失败，请重新打开本程序重试!\n");
	 	system("pause"); 
	 	exit(-1); 
	}
	if(curl)
	{//如果正常初始化
		state=HTTP_GET_INFO(curl,res);//HTTP-GET得到用户信息
	   	if(state==0)
	   	{//未登录 
		    HTTP_LOGIN(curl,res); 
	   	} 
	   	if(state==1)
	   	{//已登录 
	   		while(1)
	   		{
			   	printf("\n     您是否要注销？<1.注销/2.退出>:     \n");
	   			char input=getch();
	   			if(input=='1')
	   			{ 
	   				state=HTTP_LOGOUT(curl,res);
	   				break;
	   			}
	   			if(input=='2')
	   				exit (0);
				else
				{
					printf("\n----------------------------------------\n");
					printf("\n             输入错误请重试!            \n"); 
					printf("\n----------------------------------------\n");
				}
			}
			if(state==0)
			{//注销成功后 
				printf("\n     您是否要重登？<1.重登/2.退出>:     \n");
				while(1)
				{
					char input=getch();
		   			if(input=='1')
		   			{ 
		   				state=HTTP_LOGIN(curl,res);
		   				break;
		   			}
	   				if(input=='2')
	   					exit (0);
	   				else
					{
						printf("\n----------------------------------------\n");
						printf("\n             输入错误请重试!            \n"); 
						printf("\n----------------------------------------\n");
					}
				} 
			}
		} 
	}
	curl_global_cleanup();
	system("pause"); 
	return 0;
}
