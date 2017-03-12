#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>

const char POSTURL[]="http://172.16.154.130:69/cgi-bin/srun_portal";  //����POST��ַ

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
{
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

int HTTP_LOGIN(CURL *curl,CURLcode res) 
{
	CURL *curl1 = curl_easy_init();
	if(!(curl&&curl1))
  	{//�����������ʼ�� 
	 	printf("\nLibcurl��ʼ��ʧ�ܣ������´򿪱���������!\n");
	 	system("pause"); 
	 	exit(-1); 
	}
	if(curl&&curl1)
	{ 
		char username[20];
		char username_Post[50]="{SRUN3}\r\n"; 
		char LOGIN[200]="&ac_id=1&action=login&drop=0&pop=1&type=2&n=117&mbytes=0&minutes=0&mac=&username=";
		char *ptr=LOGIN;
		while(*ptr!='\0')
 		  	ptr++;  
 		printf("\n----------------------------------------\n");
		printf("\n            ����������û���:           \n");
		gets(username); 
		for (int i = 0; i<strlen(username); ++i)
		{//�û���������� 
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
		printf("\n----------------------------------------\n");
		printf("\n         ������������루����ʾ��:      \n");
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
		char key[] = "1234567890";//����key
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
		curl_easy_cleanup(curl);//����
		curl_easy_cleanup(curl1);//����
		struct string in;
    	init_string(&in);
    	CURL *curl_post;
    	curl_post = curl_easy_init();
    	if(!(curl_post))
	  	{//�����������ʼ�� 
		 	printf("\nLibcurl��ʼ��ʧ�ܣ������´򿪱���������!\n");
		 	system("pause"); 
		 	exit(-1); 
		}
		curl_easy_setopt(curl_post, CURLOPT_URL, POSTURL); //����POST��ַ
		curl_easy_setopt(curl_post, CURLOPT_POSTFIELDS,LOGIN);//����POST����
		curl_easy_setopt(curl_post,CURLOPT_TIMEOUT,1L);//��ʱ���ó�1s
		curl_easy_setopt(curl_post, CURLOPT_WRITEFUNCTION, writefunc);
	    curl_easy_setopt(curl_post, CURLOPT_WRITEDATA, &in);
		res = curl_easy_perform(curl_post);//����libcurl 
		if (res != CURLE_OK)
		{
			printf("\n              *** ����: ***             \n");
			printf("\n  �޷���ȡ������Ϣ,����������������!  \n");
			printf("\n----------------------------------------\n");
			curl_easy_cleanup(curl);//����
			system("pause"); 
			exit(-1);
		}
		char *k=(char*)in.ptr;
		printf("\n----------------------------------------\n");
		for(;*k!='\0';k++) 
		{
			if(*k=='m')
			{//����������� 
				printf("\n              ȱ�ٲ���              \n");
				break;
			} 
			else if(*k=='n'&&*(k+2)=='o')//
			{//��½�ɹ����
				printf("\n              ��½�ɹ�              \n");
				break;
			} 
			else if(*k=='P'&&*(k+1)=='a') 
			{//����������
				printf("\n        ��������������롣      \n");
				break;
			} 
			else if(*k=='U'&&*(k+1)=='s')//
			{//�û����������
				printf("\n      �û��������ڣ��������롣    \n");
				break;
			} 
			else if(*k=='I'&&*(k+1)=='N') 
			{//ACID�������
				printf("\n              ACID����              \n");
				break;
			}
		}
		curl_easy_cleanup(curl_post);//����
		printf("\n----------------------------------------\n");
	} 
	
}; 

int HTTP_LOGOUT(CURL *curl,CURLcode res) 
{
	char LOGOUT[50] = "username=0&mac=&type=2&action=logout&ac_id=1";//ѧ��ץ�������õ���ͷ�ļ�
	struct string out;
    init_string(&out);
	curl_easy_setopt(curl, CURLOPT_URL, POSTURL); //����POST��ַ
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS,LOGOUT);//����POST����
	curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//��ʱ���ó�1s
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
	res = curl_easy_perform(curl);//����libcurl 
	if (res != CURLE_OK)
	{
		printf("\n              *** ����: ***             \n");
		printf("\n  �޷���ȡ������Ϣ,����������������!  \n");
		printf("\n----------------------------------------\n");
		curl_easy_cleanup(curl);//����
		system("pause"); 
		exit(-1);
	}
	printf("\n----------------------------------------\n");
	char *k=(char *)out.ptr;
	for(;;k++) 
	{
		if(*k=='m')
		{//����������� 
			printf("\n              ȱ�ٲ���              \n");
			break;
		} 
		else if(*k=='t'&&*(k+2)=='o')
		{//ע���ɹ����
			printf("\n              ע���ɹ�              \n");
			return 0;
			break;
		}
		else if(*k=='I'&&*(k+1)=='N') 
		{//ACID�������
			printf("\n              ACID����              \n");
			break;
		}
		else if(*k=='Y'&&*(k+1)=='o')
		{//��������� 
			printf("\n         �������ߣ��ǳ�ʧ��         \n");
			return 0; 
			break;
		} 
	}
	curl_easy_cleanup(curl);//����
}; 

int HTTP_GET_INFO(CURL *curl,CURLcode res)
{
	printf("\n-------���ڻ�ȡ������Ϣ�����Ժ�:--------\n"); 
	struct string s;
    init_string(&s);
	const char GET_INFO_URL[]="http://172.16.154.130/cgi-bin/rad_user_info";//�����ȡ��½״̬����
	curl_easy_setopt(curl,CURLOPT_URL,GET_INFO_URL);//���õõ��û���Ϣ��GET��ַ
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//��ʱ���ó�1s
	res = curl_easy_perform(curl);//����libcurl 
	if (res != CURLE_OK)
	{
		printf("\n              *** ����: ***             \n");
		printf("\n  �޷���ȡ������Ϣ,����������������!  \n");
		printf("\n----------------------------------------\n");
		curl_easy_cleanup(curl);//����
		system("pause"); 
		exit(-1);
	}
	(char *)s.ptr;
	if(*s.ptr=='n')
	{//���δ��½ 
		printf("\n       �������ߣ�����������Ϣ��½:        \n");
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

void PRINT_WELCOME_INFO(void)
{//�����ӭ��Ϣ
	printf("\n----------------------------------------\n");
	printf("      ��ӭʹ��У԰����½�� C ���԰�     \n");	
	printf("Made By CHN-STUDENT && MouYouLing V0.0.7\n");
	printf("\n----------------------------------------\n");
}

int main()
{ 
    
	system("mode con cols=40");//���ÿ��40
	system("color A");//��������Ϊ��ɫ 
	PRINT_WELCOME_INFO();
	int state=0;//����״̬��0����δ��¼��1�����Ѿ���½��Ĭ��δ��¼ 
  	CURL *curl;//����CURL�ľ�� 
  	CURLcode res;//����CURL�������
  	curl_global_init(CURL_GLOBAL_ALL);//ȫ�ֳ�ʼ�� 
  	curl = curl_easy_init(); //CURL�����ʼ�� 
  	if(!(curl))
  	{//�����������ʼ�� 
	 	printf("\nLibcurl��ʼ��ʧ�ܣ������´򿪱���������!\n");
	 	system("pause"); 
	 	exit(-1); 
	}
	if(curl)
	{//���������ʼ��
		state=HTTP_GET_INFO(curl,res);//HTTP-GET�õ��û���Ϣ
	   	if(state==0)
	   	{//δ��¼ 
		    HTTP_LOGIN(curl,res); 
	   	} 
	   	if(state==1)
	   	{
	   		while(1)
	   		{
			   	printf("\n     ���Ƿ�Ҫע����<1.ע��/2.�˳�>:     \n");
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
					printf("\n             �������������!            \n"); 
					printf("\n----------------------------------------\n");
				}
			}
			if(state==0)
			{
				printf("\n     ���Ƿ�Ҫ�صǣ�<1.�ص�/2.�˳�>:     \n");
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
						printf("\n             �������������!            \n"); 
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
