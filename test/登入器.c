#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>

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
	printf("\n");
	system("pause"); 
	exit(0);
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

int main(void)
{//���뺯��
	CURL *curl = curl_easy_init();
	CURL *curl1 = curl_easy_init();//����CURL�������ʼ��Ϊurlencode��׼��
	if(!(curl&&curl1))
	{//�����������ʼ�� 
	 	printf("\nLibcurl��ʼ��ʧ�ܣ������´򿪱��������ԣ�������ʧ�ܣ������ҷ�����\n");
	 	system("pause"); 
		exit(-1); 
	} 
	if(curl&&curl1)
	{//���������ʼ��
		char acid[8]="&ac_id=";
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
			system("pause"); 
		 	exit(-1); 
		}
		/*���������ò���*/ 
		curl_easy_setopt(curl_post, CURLOPT_URL, POSTURL); //����POST��ַ
		curl_easy_setopt(curl_post, CURLOPT_POSTFIELDS,LOGIN);//����POST����
		curl_easy_setopt(curl_post,CURLOPT_TIMEOUT,1L);//��ʱ���ó�1s
		//curl_easy_setopt(curl_post,CURLOPT_VERBOSE,1); //����ģʽ
		curl_easy_setopt(curl_post, CURLOPT_WRITEFUNCTION,POST_FEEDBACK);//���ܻش� 
		/*���ò�������*/ 
		res = curl_easy_perform(curl_post);//����libcurl 
		if (res != CURLE_OK)
		{
			printf("\nPOST����!���������������!\n");
			curl_easy_cleanup(curl_post);//����
			printf("\n");
			system("pause"); 
			exit(-1); 
		}
		curl_easy_cleanup(curl_post);//����
	}
	return 0;
}
