# 校园网登陆器C语言版如何储存和读取配置文件

```c
#include <stdio.h>
//#include <curl\curl.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

typedef struct login_info
{
	char username[20];
	char password[20];
	char acid;
	char auto_login;	
}info;

int save_config_file(struct login_info *input)
{
	FILE *out=fopen("config.json","w+");
	if(!out)
	{
		printf("\n----------------------------------------\n");
		printf("\n		  	    文件另存失败 	  		  \n");
		printf("\n----------------------------------------\n");
		return -1; 
	}
	fprintf(out,"/**************************************\n");
	fprintf(out,"这是校园网登陆器配置文件\n");
	fprintf(out,"为防止错误，请勿更改！\n");
	fprintf(out,"**************************************/\n");
	fprintf(out,"\n");
	fprintf(out,"{\n");
	fprintf(out,"#username:%s\n",input->username);
	fprintf(out,"#password:%s\n",input->password);
	fprintf(out,"#acid:%c\n",input->acid);
	fprintf(out,"#auto_login:%c\n",input->auto_login);
	fprintf(out,"}");
	fclose(out); 
}

struct login_info *read_config_file(void)
{
	FILE *read=fopen("config.json","rb");
	if(!read)
	{
		printf("\n----------------------------------------\n");
		printf("\n			  读取不到配置文件			  \n"); 
		printf("\n----------------------------------------\n");
		return ((info *)-1); 
	}
	info *data=(info *)malloc(sizeof(info));
	if(!data)
	{
		printf("\n----------------------------------------\n");
		printf("\n          为程序分配内存失败！          \n"); 
		printf("\n----------------------------------------\n");
		system("pause"); 
		exit(-1);
	}
	char temp[256];
	while(feof(read)==0)
	{
		fscanf(read,"%s",temp);
		if(strcmp(temp,"{")==0)
		{
			break;
		}
	}
	fscanf(read,"%*10s%s",data->username);
	fscanf(read,"%*10s%s",data->password);
	fscanf(read,"%*6s%c",&data->acid);
	fscanf(read,"%*12s%c",&data->auto_login);
	fclose(read);
	return data;
}



int main(void)
{
	info *input=(info *)malloc(sizeof(info));
	strcpy(input->username,"201600000000");
	strcpy(input->password,"111111");
	input->acid='1';
	input->auto_login='1';
	save_config_file(input);
	info *read=read_config_file(); 
	printf("%s %s %c %c\n",read->username,read->password,read->acid,read->auto_login);
	return 0;
}
```
