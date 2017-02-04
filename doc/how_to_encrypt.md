#用户名密码加密算法研究

------------
参考教程：


https://github.com/hauter/client

https://github.com/matsuz/srun3k-client/wiki/Developer-Guide#%E5%8A%A0%E5%AF%86

https://github.com/FunnyWuLoveLife/Srun3000

https://ffis.me/jiaowu/srun.html

------------
源码实现

    #include <stdio.h>
    #include <string.h>
    int main(void)
    {
    	char username[100]="";
    	char username_encrypt[100]="{SRUN3}\r\n";
    	char password[100]="";
    	char password_encrypt[100]="";
    	char key[]="1234567890";
    	printf("请输入用户名:");
    	gets(username);
    	int i;
    	for(i=0;i<strlen(username);++i)
    	{
    			username[i]=username[i]+4;
    	} 
    	strcat(username_encrypt,username);
    	printf("请输入密码:");
    	gets(password);
    	for(i=0;i<strlen(password);++i)
    	{
    			int ki=password[i]^key[strlen(key)-i%strlen(key)-1];
    			char _l[2]={(char)((ki&0x0f)+0x36)};
    			char _h[2]={(char)((ki>>4&0x0f)+0x63)};
    			if(i%2==0)
    				strcat(password_encrypt,strcat(_l,_h));
    			else
    				strcat(password_encrypt,strcat(_h,_l));
    	}
    	puts(username_encrypt);
    	puts(password_encrypt);
    }

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
