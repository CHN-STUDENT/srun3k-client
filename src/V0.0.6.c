#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

const char POSTURL[]="http://172.16.154.130:69/cgi-bin/srun_portal";  //����POST��ַ

size_t POST_FEEDBACK(void *ptr, size_t size, size_t nmemb, void *stream)
{//����õ��ķ������ش� 
	char *k=(char*)ptr;//��void *��ָ��ǿ��תΪchar *ָ��
	for(;*k!='\0';k++) 
	{
		if(*k=='m')
		{//����������� 
			printf("\nȱ�ٲ������������롣\n");
			break;
		} 
		else if(*k=='t'&&*(k+2)=='o')
		{//ע���ɹ����
			printf("\nע���ɹ�����л����ʹ�á�\n");
			break;
		}
		else if(*k=='n'&&*(k+2)=='o')//
		{//��½�ɹ����
			printf("\n��½�ɹ�����л����ʹ�á�\n");
			break;
		} 
		else if(*k=='P'&&*(k+1)=='a') 
		{//����������
			printf("\n��������������롣\n");
			break;
		} 
		else if(*k=='U'&&*(k+1)=='s')//
		{//�û����������
			printf("\n�û��������ڣ��������롣\n");
			break;
		} 
		else if(*k=='I'&&*(k+1)=='N') 
		{//�û����������
			printf("\nACID�����������롣\n");
			break;
		}
		else if(*k=='Y'&&*(k+1)=='o')
		{//��������� 
			printf("\n�������ߡ�\n");
			break;
		} 
	}
}

void ACID_INPUT(char *acid) 
{//ACID�����뺯�� 
	char ac_id[2]= {0};
    do
    {
        if (ac_id[0]!= '\0')
            printf("\n�Բ����������������!\n");
        printf("\n����������ACID<1/2>:");
        scanf("%1s",&ac_id[0]);
    } 
    while (ac_id[0]!= '1'&&ac_id[0]!= '2');
    strcat(acid,ac_id);
} 

int HTTP_POST_LOGIN(void)
{//���뺯��
	CURL *curl = curl_easy_init();
	CURL *curl1 = curl_easy_init();//����CURL�������ʼ��Ϊurlencode��׼��
	if(!(curl&&curl1))
	{//�����������ʼ�� 
	 	printf("\nLibcurl��ʼ��ʧ�ܣ������´򿪱��������ԣ�������ʧ�ܣ������ҷ�����\n");
		exit(-1); 
	} 
	if(curl&&curl1)
	{//���������ʼ��
		char acid[8]="&acid:";
		char username[20];
		char username_Post[50]="{SRUN3}\r\n"; 
		char LOGIN[200]="action=login&drop=0&pop=1&type=2&n=117&mbytes=0&minutes=0&mac=&username=";
		char *ptr=LOGIN;
		while(*ptr!='\0')
 		  	ptr++;  
		printf("����������û���:");
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
		printf("\n������������루����ʾ��:");
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
		ACID_INPUT(acid);//ACID����
		strcat(LOGIN,acid);
		CURL *curl_post;
		CURLcode res;
		curl_post = curl_easy_init();//��ʼ��libcurl
		if (!curl_post)
		{
			printf("\nLibcurl��ʼ��ʧ�ܣ������´򿪱��������ԣ�������ʧ�ܣ������ҷ�����\n");
		 	exit(-1); 
		}
		/*���������ò���*/ 
		curl_easy_setopt(curl_post, CURLOPT_URL, POSTURL); //����POST��ַ
		curl_easy_setopt(curl_post, CURLOPT_POSTFIELDS,LOGIN);//����POST����
		//curl_easy_setopt(curl_post,CURLOPT_TIMEOUT,1L);//��ʱ���ó�1s
		//curl_easy_setopt(curl_post,CURLOPT_VERBOSE,1); //����ģʽ
		curl_easy_setopt(curl_post, CURLOPT_WRITEFUNCTION,POST_FEEDBACK);//���ܻش� 
		/*���ò�������*/ 
		res = curl_easy_perform(curl_post);//����libcurl 
		if (res != CURLE_OK)
		{
			printf("\nPOST����!\n");
			curl_easy_cleanup(curl_post);//����
			exit(-1); 
		}
		curl_easy_cleanup(curl_post);//����
	}
	exit(0); 
}

int HTTP_POST_LOGOUT(void)
{//�ǳ�����
 	char LOGOUT[50] = "username=0&mac=&type=2&action=logout";//ѧ��ץ�������õ���ͷ�ļ�
 	char acid[8]="&acid:";
 	ACID_INPUT(acid);//ACID����
	strcat(LOGOUT,acid);
	CURL *curl_post;
	CURLcode res;
	curl_post = curl_easy_init();//��ʼ��libcurl
	if (!curl_post)
	{
			printf("\nLibcurl��ʼ��ʧ�ܣ������´򿪱��������ԣ�������ʧ�ܣ������ҷ�����\n");
		 	exit(-1); 
	}
	/*���������ò���*/ 
	curl_easy_setopt(curl_post, CURLOPT_URL, POSTURL); //����POST��ַ
	curl_easy_setopt(curl_post, CURLOPT_POSTFIELDS,LOGOUT);//����POST����
	//curl_easy_setopt(curl_post,CURLOPT_TIMEOUT,1L);//��ʱ���ó�1s
	//curl_easy_setopt(curl_post,CURLOPT_VERBOSE,1); //����ģʽ
	curl_easy_setopt(curl_post, CURLOPT_WRITEFUNCTION,POST_FEEDBACK);//���ܻش� 
	/*���ò�������*/ 
	res = curl_easy_perform(curl_post);//����libcurl 
	if (res != CURLE_OK)
		{
			printf("\nPOST����!\n");
			curl_easy_cleanup(curl_post);//����
			exit(-1); 
		}
	curl_easy_cleanup(curl_post);//����
	exit(0); 
}

void ASK_LOGIN_OR_EXIT(void)
{//ѯ���Ƿ��������˳����� 
	char login_or_exit = '\0';
    do
    {
        if (login_or_exit!= '\0')
            printf("\n�Բ����������������!\n");
        printf("\n�Ƿ��������˳���\n����<1.����/2.�˳�>:\n");
        scanf("%1s",&login_or_exit);
    } 
    while (login_or_exit!= '1'&&login_or_exit!= '2');
	if(login_or_exit==1)
		//���Ҫ���� 
		HTTP_POST_LOGIN();//ִ�е���  
	if(login_or_exit==2)
		//���Ҫ�˳�
		exit(0); 
}

void ASK_LOGOUT_OR_EXIT(void)
{//ѯ���Ƿ�ǳ������˳����� 
	char logout_or_exit = '\0';
    do
    {
        if (logout_or_exit!= '\0')
            printf("\n�Բ����������������!\n");
        printf("\n�Ƿ�ǳ������˳���\n����<1.�ǳ�/2.�˳�>:\n");
        scanf("%1s",&logout_or_exit);
    } 
    while (logout_or_exit!= '1'&&logout_or_exit!= '2');
	if(logout_or_exit==1)
	{//���Ҫ�ǳ�
		HTTP_POST_LOGOUT();//ִ�еǳ� 
		ASK_LOGIN_OR_EXIT();//ѯ���Ƿ��������˳� 
	}
	if(logout_or_exit==2)
	//���Ҫ�˳�
		exit(0); 	 	 
}

size_t GET_INFO(void *ptr, size_t size, size_t nmemb, void *stream)
{//����õ��û���Ϣ����
	char *k=(char*)ptr;//��void *��ָ��ǿ��תΪchar *ָ�� 
		  	if(*k=='n')
			{ //���δ��½ 
				HTTP_POST_LOGIN();//ִ�е�½;
			}
			else
			{ //����ѵ�¼ 
				printf("\n+++++++++++++�������ߣ�������������Ϣ:+++++++++++++\n");
				char *j=(char*)ptr;//����ָ��,�������� 
				for(int flag=0;k!='\0';k++)//��ʼѭ��ֱ������ĩβ 
				{
					char temp[20]; //����һ����ʱ����
					if(*k==',')//�����ǰλΪ,�ָ��� 
					{	
						flag++;//,�ָ������� 
						char *t=temp;//����һ����ʱָ��ָ��t
						for(;*j!=*k;j++,t++)
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
				ASK_LOGOUT_OR_EXIT();//ѯ���Ƿ�ǳ������˳�
			} 		
}

int HTTP_GET_INFO(void)
{//��ȡ��ǰ��½״̬����
	printf("+++++++++++++���ڻ�ȡ������Ϣ,���Ժ�:+++++++++++++\n"); 
	const char GET_INFO_URL[]="http://172.16.154.130/cgi-bin/rad_user_info";//�����ȡ��½״̬����
	CURL *curl;//����CURL�ľ�� 
  	CURLcode res;//����CURL�������
  	curl = curl_easy_init(); //CURL�����ʼ�� 
  	if(!(curl))
  	{//�����������ʼ�� 
	 	printf("\nLibcurl��ʼ��ʧ�ܣ������´򿪱��������ԣ�������ʧ�ܣ������ҷ�����\n");
	 	exit(-1); 
	}
	if(curl)
	{//���������ʼ��
		curl_easy_setopt(curl,CURLOPT_URL,GET_INFO_URL);//���õõ��û���Ϣ��GET��ַ
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,GET_INFO);//���ô���õ��û���Ϣ����
		//curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//��ʱ���ó�1s
		//curl_easy_setopt(curl,CURLOPT_VERBOSE,1); //����ģʽ
		res = curl_easy_perform(curl);//����libcurl 
		if (res != CURLE_OK)
		{
			printf("\nPOST����!\n");
			curl_easy_cleanup(curl);//����
			exit(-1); 
		}
		curl_easy_cleanup(curl);//����
	}
}
		
int main()
{ 
	printf("+++++++++++++��ӭʹ��У԰����½��C���԰�+++++++++++++\n");
	printf("+++++++Made By CHN-STUDENT && MouYouLing V0.0.6+++++++\n"); 
	HTTP_GET_INFO();//��ȡ��ǰ��½״̬ 
	system("pause"); 
	return 0;
}
