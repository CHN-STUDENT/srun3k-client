#这是如何在VS2015中新建libcurl项目的笔记

1.首先用VS2015新建一个`C++ WIN32控制台项目`，将`预编辑头`和`安全开发周期检查`关掉。

2.右边`解决方案管理器`-`属性`-`C/C++`-`常规`-`附加包含目录`加入编译的libcurl的`include`目录

3. `解决方案管理器`-`属性`-`C/C++`-`预处理器`-加入`;CURL_STATICLIB;`

4. `解决方案管理器`-`属性`-`C/C++`-`代码生成`-`运行库`-改成`多线程调试 (/MTd)`

5. `解决方案管理器`-`属性`-`链接器`-`常规`-`附加库目录`加入编译的libcurl的`lib`目录

6. `解决方案管理器`-`属性`-`链接器`-`输入`-`附加依赖项`-加入`libcurl.lib;wldap32.lib;ws2_32.lib;`

然后就可以写一个例子试试了，这是一个HTTP-GET得到百度源码的例子。
```c
#include <stdio.h>
#include <curl/curl.h>

int main(void)
{
	CURL *curl; //定义curl指针
	CURLcode res; //定义curl结果

	curl = curl_easy_init(); //curl初始化
	if (curl) //正常初始化
	{ 
		curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com/");//定义get地址
		res = curl_easy_perform(curl);//进行get操作，并反馈结果到res

		if (CURLE_OK == res)//正常 
		{
			char *get;//声明指针
					  /* ask for the content-type */
			res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &get);//get到的值给get这个指针
																	   //(参数1:curl指针;参数2:需要提取curl信息;参数3:字符指针）

			if ((CURLE_OK == res) && get)//如果正常并且指针指代成功，输出
				puts(get);
		}

		/* always cleanup */
		curl_easy_cleanup(curl);//进行清理
	}
	return 0;
}
```
