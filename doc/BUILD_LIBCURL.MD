#如何在 VS2015 中静态编译libcurl
- 这是如何在 VS2015 编译安装笔记
- 我用的`libcurl 7.28.1`版本编译成功，其他版本我均未成功。
- 参考教程：官网的[https://curl.haxx.se/libcurl/c/](https://curl.haxx.se/libcurl/c/ "https://curl.haxx.se/libcurl/c/")中的Andrei Jakab's[ Using libcurl with SSH support in Visual Studio 2010](https://curl.haxx.se/libcurl/c/Using-libcurl-with-SSH-support-in-Visual-Studio-2010.pdf " Using libcurl with SSH support in Visual Studio 2010") [PDF]

------------

- libcurl下载地址：https://curl.haxx.se/download/curl-7.28.1.zip


------------
1. 首先解压libcurl
2. 进入`curl-7.28.1\lib`目录打开`libcurl.vcxproj`
   提示升级点击是
3. 右边-`解决方案管理器`-`属性`-`C/C++`-`预处理器`-加入`;CURL_STATICLIB;CURL_STATICLIB;BUILDING_LIBCURL;`
<del> 4. `解决方案管理器`-`属性`-`C/C++`-`代码生成`-`运行库`-改成`多线程调试 (/MTd)` </del>
5. 菜单-`生成`-`生成libcurl`
6. 新建文件夹lib 
7. 进入`\curl-7.28.1\lib\Debug`找到`libcurl.lib`拷出来到lib文件夹备用
8. 拷出来`\curl-7.28.1\include`文件夹备用

------------

编译过程结束，下面是如何新建libcurl项目。
- 如果需要编译好的libcurl，请下载本项目libcurl文件夹下的东西。
