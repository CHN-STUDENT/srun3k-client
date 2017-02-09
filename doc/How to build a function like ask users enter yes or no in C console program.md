#这是研究如何实现 Yes or No 输入的笔记
我想实现一个功能，程序要求用户输入 `Y` 或者 `N` 判断是否执行某些函数，如果他们输入不是这两个字符，程序会告诉他们 *输入错误，请重试。*并且要求他们重新输入直到他们输入正确的字符为止。

我哥们给我一个意见，然后我又跑到stackoverflow询问，最终用了以下方法实现。

```c
#include <stdio.h>
int main(void)
{
    char yon = '\0';
    do
    {
        if (yon != '\0')
            printf("\nSorry,Please try again.\n");
        printf("\nEnter Y/N?:");
        scanf("%1s",&yon);
    } 
    while ( yon!= 'y'&&yon != 'Y'&&yon != 'n'&&yon != 'N');
    return 0;
}
```

程序很简单先执行循环语句内内容，要求用户输入`Y`和`N`，并且只取他们输入字符串（可能有人这样做）第一个字符，判断是否是`Y`和`N`,如果不是重新进循环输出输入错误并且要求重输，直到获取到用户正确输入为止。
