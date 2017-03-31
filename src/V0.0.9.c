#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

const char POSTURL[]="http://172.16.154.130:69/cgi-bin/srun_portal";  //����POST��ַ
char LOGOUT[] = "username=0&mac=&type=2&action=logout&ac_id=1";//ע�����Ͳ��� 
struct login_info *data=NULL;//ȫ�ֵ�½����ָ�� 
int file=0;//ȫ���ļ�����ָʾ 
int login_num=0;//��½�������� 

/*���洦������������STACKOVERFLOW������*/ 

struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "\n            �ڴ����            \n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{//���������� 
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "\n            �ڴ����!            \n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

/*���洦������������STACKOVERFLOW������*/ 

typedef struct login_info
{//��½���ݽṹ�� 
	char username[20];
	char password[20];
	char acid; 
	char auto_login;	
}info;

int save_config_file(struct login_info *data)
{//�洢�����ļ����� 
	FILE *out=fopen("config.json","wb");
	if(!out)
	{
		printf("\n----------------------------------------\n");
		printf("\n		  	    �ļ����ʧ�� 	  		  \n");
		printf("\n----------------------------------------\n");
		return -1; 
	}
	fprintf(out,"/**************************************\n");
	fprintf(out,"����У԰����½�������ļ�\n");
	fprintf(out,"Ϊ��ֹ����������ģ�\n");
	fprintf(out,"**************************************/\n");
	fprintf(out,"\n");
	fprintf(out,"{\n");
	fprintf(out,"#username:%s\n",data->username);
	fprintf(out,"#password:%s\n",data->password);
	fprintf(out,"#acid:%c\n",data->acid);
	fprintf(out,"#auto_login:%c\n",data->auto_login);
	fprintf(out,"}");
	fclose(out); 
}

int read_config_file(void)
{//��ȡ�����ļ����� 
	FILE *read=fopen("config.json","rb");
	data=(info *)malloc(sizeof(info));//��ȡ��½��Ϣ�� 
	if(!data)
	{
		printf("\n----------------------------------------\n");
		printf("\n          Ϊ��������ڴ�ʧ�ܣ�          \n"); 
		printf("\n----------------------------------------\n");
		system("pause"); 
		exit(-1);
	}
	if(!read)
	{
		printf("\n----------------------------------------\n");
		printf("\n	    ��ȡ���������ļ�    \n"); 
		printf("\n----------------------------------------\n");
		return (-1); 
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
	return 0;
}

int input_info(int state)
{//������Ϣ���� 
	if(state==-2||state==0)
	{//��һ����������û������� 
		printf("\n----------------------------------------\n");
		printf("\n            ����������û���:           \n");
		gets(data->username); 
	} 
	if(state==-1||state==-2||state==0) 
	{//��һ�������������������� 
		printf("\n----------------------------------------\n");
		printf("\n         ������������루����ʾ��:      \n");
		char ch;
		unsigned int i;
		for (i = 0; (ch = getch()) != 13; )
		{
			if (ch == 8)
			{
				if (i == 0)
					data->password[0] = '\0';
				else
				{
					data->password[i - 1] = '\0';
					i--;
				}
			}
			else
			{
				data->password[i] = ch;
				i++;
			}
		}
		data->password[i] = '\0';
		printf("\n");
		printf("\n----------------------------------------\n");
	} 
	data->acid='1';
	data->auto_login='0';
	if(state==-3)
	{//ACID������� 
		data->acid='2';
	}  
}

char * login_info_handle(struct login_info *data)
{//����������Ϣ���� 
	char *ptr=(char *)malloc(200);	
	char *back=ptr;
	if(!ptr)
	{
		printf("\n----------------------------------------\n");
		printf("\n          Ϊ��������ڴ�ʧ�ܣ�          \n"); 
		printf("\n----------------------------------------\n");
		system("pause"); 
		exit(-1);
	}
	strcpy(ptr,"&action=login&drop=0&pop=1&type=2&n=117&mbytes=0&minutes=0&mac=&ac_id=");
	while(*ptr!='\0')ptr++; 
	*ptr=data->acid;
	ptr++;
	for(char *username_Post="&username=%7BSRUN3%7D%0D%0A";*username_Post!='\0';username_Post++,ptr++)
	{
		*ptr=*username_Post;
	}
	char temp[50]="\0";//����������ʱ�洢���ܺ��û��������� 
	for (int i = 0; i<strlen(data->username); ++i)
	{//�û������� 
		temp[i] = data->username[i] + 4;
	}
	CURL *curl1 = curl_easy_init();
	CURL *curl2 = curl_easy_init();
	if(!(curl1&&curl2))
  	{//�����������ʼ�� 
	 	printf("\nLibcurl��ʼ��ʧ�ܣ������´򿪱���������!\n");
	 	system("pause"); 
	 	exit(-1); 
	}
	char *name_urlencode = curl_easy_escape(curl1,temp,0);
	while(*name_urlencode!='\0')
	{
		*ptr=*name_urlencode;
		ptr++;
		name_urlencode++;
	}
	memset(temp,0,sizeof(temp));
	char key[] = "1234567890";//����key
	int i;
	for (i = 0; i<strlen(data->password); ++i)
	{
		int ki = data->password[i] ^ key[strlen(key) - i%strlen(key) - 1];
		char _l[4] = { (char)((ki & 0x0f) + 0x36) };
		char _h[4] = { (char)((ki >> 4 & 0x0f) + 0x63) };
		if (i % 2 == 0)
		{
			strcat(_l, _h);
			strcat(temp, _l);
		}
		else
		{
			strcat(_h, _l);
			strcat(temp, _h);
		}
	}
	for(char *password_Post="&password=";*password_Post!='\0';password_Post++,ptr++)
	{
		*ptr=*password_Post;
	}
	char *password_urlencode = curl_easy_escape(curl2,temp,0);
	while(*password_urlencode !='\0')
	{
		*ptr=*password_urlencode ;
		ptr++;
		password_urlencode++;
	}
	*ptr='\0'; 
	curl_easy_cleanup(curl1);//����
	curl_easy_cleanup(curl2);//����
	return back;
}

void read_or_input(int state)
{//�ж����Զ���ȡ�����ļ���������Ҫ��������Ϣ���� 
	if(read_config_file()==-1)
	{//�Զ���ȡ�����ļ�������Ҫ������  
		file=0;
		input_info(state);
	}
	else
	{//�Զ���ȡ�����ļ�����
		file=1;
		if(data->auto_login=='1'&&login_num==0)
   			{
   				printf("\n----------------------------------------\n");
				printf("\n      ��⵽�����ļ����Զ���½��!       \n"); 
				printf("\n----------------------------------------\n");
			}
		if(data->auto_login=='0'&&login_num==0)
		{
			printf("\n----------------------------------------\n");
			printf("\n              ��⵽�����ļ���          \n");
			printf("\n        ���Ƿ����ڵ�½:<1.��/2.�˳�>    \n"); 
			printf("\n----------------------------------------\n");
			while(1)
			{
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
					printf("\n             �������������!            \n"); 
					printf("\n----------------------------------------\n");
				}
			}
		} 
	} 
} 

void save_or_exit(int state,int file)
{//�ж��Ƿ�Ҫ���������ļ����� 
	if(state==1&&file==0) 
	{
		printf("\n----------------------------------------\n");
		printf("\n      �Ƿ񱣴������Ϣ<1.��/2.��>       \n"); 
		printf("\n----------------------------------------\n");
		while(1)
		{
			char input=getch();
   			if(input=='1')
   			{ 
   				printf("\n----------------------------------------\n");
				printf("\n      �Ƿ��Զ���½����<1.��/2.��>       \n"); 
				printf("\n----------------------------------------\n");
				while(1)
				{
					char input=getch();
		   			if(input=='1')
		   			{ 
		   				data->auto_login='1';
		   				break;
		   			}
	   				if(input=='2')
	   				{
	   					data->auto_login='0';
	   					break; 
					} 
	   				else
					{
						printf("\n----------------------------------------\n");
						printf("\n             �������������!            \n"); 
						printf("\n----------------------------------------\n");
					}
				}
				save_config_file(data);
   				break;
   			}
			if(input=='2')
				exit (0);
			else
			{
				printf("\n----------------------------------------\n");
				printf("\n             �������������!            \n"); 
				printf("\n----------------------------------------\n");
			}
		}
	} 
} 

void PRINT_WELCOME_INFO(void)
{//�����ӭ��Ϣ
	printf("\n----------------------------------------\n");
	printf("      ��ӭʹ��У԰����½�� C ���԰�     \n");	
	printf("Made By CHN-STUDENT & MouYouLing V0.0.9\n");
	printf("\n----------------------------------------\n");
}

int HTTP_GET_INFO(void)
{//��ȡ��ǰ��½״̬����   
	CURL *curl= curl_easy_init();
	if(!(curl))
  	{//�����������ʼ�� 
	 	printf("\nLibcurl��ʼ��ʧ�ܣ������´򿪱���������!\n");
	 	system("pause"); 
	 	exit(-1); 
	}
	CURLcode res;
	printf("\n----------------------------------------\n");
	printf("\n-------���ڻ�ȡ������Ϣ�����Ժ�:--------\n"); 
	struct string s;
    init_string(&s);
	const char GET_INFO_URL[]="http://172.16.154.130/cgi-bin/rad_user_info";//�����ȡ��½״̬����
	curl_easy_setopt(curl,CURLOPT_URL,GET_INFO_URL);//���õõ��û���Ϣ��GET��ַ
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//��ʱ���ó�1s
    while(1)
    {
		res = curl_easy_perform(curl);//����libcurl 
		if (res != CURLE_OK)
		{
			printf("\n----------------------------------------\n");
			printf("\n              *** ����: ***             \n");
			printf("\n  �޷���ȡ������Ϣ,����������������!  \n");
			printf("\n----------------------------------------\n");
			char input;
			while(1)
			{
				printf("\n     ���Ƿ�Ҫ���ԣ�<1.����/2.�˳�>:     \n");
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
					printf("\n             �������������!            \n"); 
					printf("\n----------------------------------------\n");
				}
			}
		}
		if(res == CURLE_OK)
			break;
	}
	(char *)s.ptr;
	if(*s.ptr=='n')
	{//���δ��½ 
		printf("\n              ��������                  \n");
		printf("\n----------------------------------------\n");
		return 0;
	}
	else
		{//����ѵ�¼ 
			printf("\n       �������ߣ�������������Ϣ:        \n");
			printf("\n----------------------------------------\n");
			char *j=(char *)s.ptr;
			for(int flag=0;;s.ptr++)//��ʼѭ��ֱ������ĩβ 
				{
					char temp[20]; //����һ����ʱ����
					if(*s.ptr==',')//�����ǰλΪ,�ָ��� 
					{	
						flag++;//,�ָ������� 
						char *t=temp;//����һ����ʱָ��ָ��t
						for(;*j!=*s.ptr;j++,t++)
							*t=*j; 
						*t='\0';
						if(flag==1)
							printf("\n���ĵ����û�����: %s\n",temp);
						if(flag==7)
							printf("\n���������������: %f GB\n",1.0*atof(temp)/1073741824);//atof(string)ת���� 
						if(flag==8)
							printf("\n�����������ʱ��: %03dСʱ%02d����%02d��\n",atoi(temp)/3600,atoi(temp)/60%60,atoi(temp)%60); //atoi(string)ת���� 
						if(flag==9)
						{
							printf("\n�� �� �� ��  IP : %s\n",temp);
							break;
						}
						j++; 
						continue;
					}
				}
			printf("\n----------------------------------------\n");
			return 1;
		} 
	curl_easy_cleanup(curl);//����
}

int HTTP_POST(char *back,int mode) 
{//���͵�½����ע�������� modeģʽ��0.ע����1.��½ 
	CURL *curl= curl_easy_init();
	CURLcode res;
	if(!(curl))
  	{//�����������ʼ�� 
	 	printf("\nLibcurl��ʼ��ʧ�ܣ������´򿪱���������!\n");
	 	system("pause"); 
	 	exit(-1); 
	}
	struct string feedback;
    init_string(&feedback);
	curl_easy_setopt(curl, CURLOPT_URL, POSTURL); //����POST��ַ
	if(mode==0)
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS,LOGOUT);//����POST����
	if(mode==1)
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS,back);
	curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//��ʱ���ó�1s
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &feedback); 
	res = curl_easy_perform(curl);//����libcurl 
	if (res != CURLE_OK)
	{
		printf("\n----------------------------------------\n");
		printf("\n              *** ����: ***             \n");
		printf("\n  �޷���ȡ������Ϣ,����������������!  \n");
		printf("\n----------------------------------------\n");
		char input;
		while(1)
		{
			printf("\n     ���Ƿ�Ҫ���ԣ�<1.����/2.�˳�>:     \n");
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
				printf("\n             �������������!            \n"); 
				printf("\n----------------------------------------\n");
			}
		}
	}
	char *k=(char*)feedback.ptr;
	printf("\n----------------------------------------\n");
	printf("\n----------------------------------------\n");
	printf("\n           ������������Ϣ:              \n");
	printf("\n----------------------------------------\n");
	for(;*k!='\0';k++) 
	{
		if(*k=='Y'&&*(k+1)=='o')
		{//��������� 
			printf("\n         �������ߣ��ǳ�ʧ��         \n");
			printf("\n----------------------------------------\n");
			return 0; 
			break;
		} 
		else if(*k=='t'&&*(k+2)=='o')
		{//ע���ɹ����
			printf("\n              ע���ɹ�              \n");
			printf("\n----------------------------------------\n");
			return 0;
			break;
		}
		else if(*k=='n'&&*(k+2)=='o')
		{//��½�ɹ����
			printf("\n              ��½�ɹ�              \n");
			printf("\n----------------------------------------\n");
			return 1;
			break;
		} 
		else if(*k=='P'&&*(k+1)=='a') 
		{//����������
			printf("\n        ��������������롣      \n");
			printf("\n----------------------------------------\n");
			return -1;
			break;
		} 
		else if(*k=='U'&&*(k+1)=='s')
		{//�û����������
			printf("\n      �û��������ڣ��������롣    \n");
			printf("\n----------------------------------------\n");
			return -2;
			break;
		} 
		else if(*k=='I'&&*(k+1)=='N') 
		{//ACID�������
			printf("\n              ACID����              \n");
			printf("\n----------------------------------------\n");
			return -3;
			break;
		}
		else if(*k=='m')
		{//����������� 
			printf("\n              ȱ�ٲ���              \n");
			printf("\n----------------------------------------\n");
			break;
		} 
	}
	printf("\n----------------------------------------\n");	
}
 
void LOGIN(int state,int file)
{
	read_or_input(state);//�Զ���ȡ�����ļ�����Ҫ������ 
	while(state!=1)
	{
		state=HTTP_POST(login_info_handle(data),1);//ʵ�ּ��ܲ��ҷ�����½ 
		if(state==1)
			break;
		printf("\n----------------------------------------\n");
		printf("\n ��½ʧ�ܣ����Ƿ���������:<1.��/2.�˳�> \n"); 
		printf("\n----------------------------------------\n");
		while(1)
		{
			login_num++; 
			char input=getch();
   			if(input=='1')
   			{ 
   				input_info(state);//�����Ϣ����Ҫ����������
   				state=HTTP_POST(login_info_handle(data),1);
   				break;
   			}
			if(input=='2')
				exit (0);
			else
			{
				printf("\n----------------------------------------\n");
				printf("\n             �������������!            \n"); 
				printf("\n----------------------------------------\n");
			}
		} 
	}
	if(state==1)
	{//�����½�ɹ� 
		save_or_exit(state,file);
	}  
 } 
 
 
int main(void)
{
	system("mode con cols=40");//���ÿ��40
	system("color A");//��������Ϊ��ɫ 
	int state=0;
	PRINT_WELCOME_INFO();
	state=HTTP_GET_INFO();//HTTP-GET�õ��û���Ϣ
   	if(state==0)
   	{//δ��¼  
   		LOGIN(state,file);
   	} 
   	if(state==1)
   	{//�ѵ�¼ 
		printf("\n----------------------------------------\n");
		printf("\n      ���Ƿ�����ע��:<1.��/2.�˳�>      \n"); 
		printf("\n----------------------------------------\n");
		while(1)
		{
			char input=getch();
   			if(input=='1')
   			{ 
   				state=HTTP_POST(LOGOUT,0);
   				break;
   			}
			if(input=='2')
				exit (0);
			else
			{
				printf("\n----------------------------------------\n");
				printf("\n             �������������!            \n"); 
				printf("\n----------------------------------------\n");
			}
		}
   		if(state==0)
   		{
   			printf("\n----------------------------------------\n");
			printf("\n      ���Ƿ������ص�½:<1.��/2.�˳�>    \n"); 
			printf("\n----------------------------------------\n");
			while(1)
			{
				char input=getch();
	   			if(input=='1')
	   			{ 
	   				LOGIN(state,file);
	   				break;
	   			}
   				if(input=='2')
   					exit (0);
   				else
				{
					printf("\n----------------------------------------\n");
					printf("\n             �������������!            \n"); 
					printf("\n----------------------------------------\n");
				}
			}
		}
	} 
	system("pause"); 
	return 0;
}
