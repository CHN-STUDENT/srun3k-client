#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>
const char POSTURL[]="http://172.16.154.130:69/cgi-bin/srun_portal";  //����POST��ַ  

void username_pt(char *username) //�û������ܺ��� 
{
	int i;
	/*�û������ܲ��ֿ�ʼ*/
	for (i = 0; i<strlen(username); ++i)
	{
		username[i] = username[i] + 4;
	}
}
void password_input(char *password) //�������뺯�� 
{
	/*�������벿�ֿ�ʼ*/
	char ch;
	int i;
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
	/*�������벿�ֽ���*/
}
void passwd_pt(char *password) //������ܺ��� 
{
	unsigned int i;
	char key[] = "1234567890";//����key
	char password_encrypt[50] = "";
	/*������ܲ��ֿ�ʼ*/
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
	/*������ܲ��ֽ���*/
	strcpy(password,password_encrypt);
}
void acid_input(char *acid) //acid���뺯�� 
{
	char ac_id[2]= {0};
    do
    {
        if (ac_id[0]!= '\0')
            printf("\n�Բ����������������!\n");
        printf("\n����������ACID<1/2>:");
        scanf("%1s",&ac_id[0]);
    } 
    while ( ac_id[0]!= '1'&&ac_id[0]!= '2');
    strcat(acid,ac_id);
} 
int libcurl(const char *POSTURL,char *post) //libcurl
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();//��ʼ��libcurl
	if (!curl)
	{
		fprintf(stderr, "\nlibcurl��ʼ��ʧ�ܣ�\n");
		return -1;
	}
	curl_easy_setopt(curl, CURLOPT_URL, POSTURL); //����POST��ַ
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);//����POST����
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		printf("\nPOST����!\n");
		return -1;
	}
	curl_easy_cleanup(curl);//����
	return 0;
}
int main()
{
	char username[20];
	char post[200] = "drop:0&pop:1&type:2&n:117&username:{SRUN3}\r\n";//ѧ��ץ�������õ���ͷ�ļ�
	char password[20];
	const char action_login[]="&action:login";
	const char action_logout[]="&action:logout";
	char acid[8]="&acid:";
	unsigned int i;
	printf("+++++++++++++��ӭʹ��У԰����½��C���԰�+++++++++++++\n");
	printf("+++++++Made By CHN-STUDENT && MouYouLing V0.0.3+++++++\n\n"); 
	printf("����������û���:");
	gets(username);
	username_pt(username);//�û������� 
	printf("\n������������루����ʾ��:");
	password_input(password);//�������� 
	passwd_pt(password);//�������
	strcat(post,username);//���Ƽ��ܺ��û�����post�����ַ���
	strcat(post, "&password:");
	strcat(post,password);//�������뵽post�����ַ���
	acid_input(acid);//acid���� 
	strcat(post,acid);//����acid��post 
	char login_or_logout = '\0';
    do
    {
        if (login_or_logout!= '\0')
            printf("\n�Բ����������������!\n");
        printf("\n������<1.��½/2.ע��>:");
        scanf("%1s",&login_or_logout);
    } 
    while (login_or_logout!= '1'&&login_or_logout!= '2');
    if(login_or_logout== '1')
    	strcat(post,action_login);
    if(login_or_logout== '2')
    	strcat(post,action_logout);
	libcurl(POSTURL,post);//libcurl
	return 0;
}


