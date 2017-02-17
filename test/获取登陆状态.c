#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>

size_t GET_INFO(void *ptr)
{//处理得到用户信息函数
	char *k=(char*)ptr;//将void *空指针强制转为char *指针 
		  	if(*k=='n')
			{ //如果未登陆 
				printf("\n您不在线\n"); 
				printf("\n");
				system("pause");
				exit(0);
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
				printf("\n");
				system("pause");
				exit(0);
			} 		
}

int main(void)
{//获取当前登陆状态函数
	printf("+++++++++++++正在获取您的信息,请稍后:+++++++++++++\n"); 
	const char GET_INFO_URL[]="http://172.16.154.130/cgi-bin/rad_user_info";//定义获取登陆状态函数
	CURL *curl;//定义CURL的句柄 
  	CURLcode res;//定义CURL反馈结果
  	curl = curl_easy_init(); //CURL句柄初始化 
  	if(!(curl))
  	{//如果非正常初始化 
	 	printf("\nLibcurl初始化失败，请重新打开本程序重试，如果多次失败，请向我反馈。\n");
	 	system("pause");
	 	exit(-1); 
	}
	if(curl)
	{//如果正常初始化

		curl_easy_setopt(curl,CURLOPT_URL,GET_INFO_URL);//设置得到用户信息的GET地址
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,GET_INFO);//设置处理得到用户信息函数
		curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//超时设置成1s
		//curl_easy_setopt(curl,CURLOPT_VERBOSE,1); //啰嗦模式
		res = curl_easy_perform(curl);//启动libcurl 
		if (res != CURLE_OK)
		{
			printf("\nGET错误!请检查你的网络连接!\n");
			curl_easy_cleanup(curl);//清理
			printf("\n");
			system("pause");
			exit(-1); 
		}
		curl_easy_cleanup(curl);//清理
	}
}
