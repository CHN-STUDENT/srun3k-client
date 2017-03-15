#用户名密码加密算法研究

------------
参考教程：


https://github.com/hauter/client

https://github.com/matsuz/srun3k-client/wiki/Developer-Guide#%E5%8A%A0%E5%AF%86

https://github.com/FunnyWuLoveLife/Srun3000

https://ffis.me/jiaowu/srun.html

------------
源码实现

2016.2.14补充：测试发现用户名和密码的数据必须要urlencode转码，而libcurl刚好有这个函数，所以重新修改，但是有个问题，貌似一个libcurl只能处理一个数据？我多个测试时处理出现了BUG。所以我用了两个libcurl句柄。


```c
#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>

int main()
{
  	CURL *curl = curl_easy_init();
	CURL *curl1 = curl_easy_init();//定义CURL句柄，初始化为urlencode做准备
	if(!(curl&&curl1))
	//如果非正常初始化 
	 	printf("\nLibcurl初始化失败，请重新打开本程序重试，如果多次失败，请向我反馈。");
	if(curl&&curl1)
	{//如果正常初始化
		char username[20];
		char username_Post[50]="{SRUN3}\r\n"; 
		char POST_LOGIN[200]="&ac_id=1&action=login&drop=0&pop=1&type=2&n=117&mbytes=0&minutes=0&mac=&username=";
		char *ptr=POST_LOGIN;
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
		puts(POST_LOGIN);
	} 
	return 0;
}
```

下面是原来版本，密码和用户名没有经过urlencode转码。

```c
#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
int main()
{
	char username[20];
	char password[20];
	char key[] = "1234567890";//加密key
	char password_encrypt[50] = "";
	unsigned int i;
	printf("请输入你要用户名:");
	gets_s(username);
	/*用户名加密部分开始*/
	for (i = 0; i<strlen(username); ++i)
	{
		username[i] = username[i] + 4;
	}
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
			strcat_s(_l,4, _h);
			strcat_s(password_encrypt,50,_l);
		}
		else
		{
			strcat_s(_h,4,_l);
			strcat_s(password_encrypt,50, _h);
		}
	}
	/*密码加密部分结束*/
	puts(username);
	puts(password_encrypt);
	return 0;
}
```


过程：
---

首先，查看学长写的html版登陆器核心部分是个js

    function username_encrypt(username) {
    			var username_crypto = "";
    			for (var i = 0; i < username.length; ++i) {
    				username_crypto += String.fromCharCode(username.charCodeAt(i) + 4);
    			}
    			return "{SRUN3}\r\n" + username_crypto;
    		}
    
    		function password_encrypt(password, key = '1234567890') {
    			var password_crypto = "";
    			for (var i = 0; i < password.length; ++i) {
    				var ki = password.charCodeAt(i) ^ key.charCodeAt(key.length - i % key.length - 1);
    				var _l = String.fromCharCode((ki & 0x0f) + 0x36);
    				var _h = String.fromCharCode((ki >> 4 & 0x0f) + 0x63);
    				if (i % 2 == 0) {
    					password_crypto += _l + _h;
    				} else {
    					password_crypto += _h + _l;
    				}
    			}
    			return password_crypto;
    		}

我了解一`var`是js定义变量的方法，`xxx.length`是统计xxx字符串的函数，`xxx.charCodeAt(i)`是返回xxx字符串第i位Unicode码值(其实也是ASCII值）,`String.fromCharCode(x)`是根据Unicode值返回字符，`^`是与或运算符，`&`是与运算符，`>>`位移运算符，其他也跟C差不多。

由于C方便的可以讲字符当成整数加减，所以用户名就简单加密一下（向后移动4位），并且用字符串合并函数加上服务器认证头。
研究密码时当时不是很了解这些`ki`,`_h`,`_l`是什么类型，所以不好定义，但是复习了计算机基础之后，渐渐有些眉目。
经过测试发现`ki`是个数值，这句话是密码中第`i`位和字符串中第`strlen(key)(加密key位数，10位)-i%strlen(key)-1`做与非运算返回一个数值。
然后`_h`和`_l`是根据数值返回的字符，用上面求的数值ki经过运算而得到，C语言中含有这些运算符，而且也能跨进制计算，这很方便。
但是此时有个问题，这时候`ki`经过运算之后还是个数值，虽然可以强制进制转换`(char)xx`但是得到仅仅是单单一个字符，没法进行字符串操作（内存溢出），后来查资料得到字符强制转字符串的方法`char a[10]={b};//b为字符`，然后就想到了转成字符串之后使用字符串合并函数拼接循环加密后的密码。
