#include <stdio.h>
#include "stdafx.h"
#include <curl\curl.h>
#include <string.h>
#include <conio.h>
#define POSTURL "http://172.16.154.130:69/cgi-bin/srun_portal"  //����POST��ַ  


int main()
{
	char username[20];
	char post[200] = "action:login&drop:0&pop:1&type:2&n:117&acid:1&username:{SRUN3}\r\n";//ѧ��ץ�������õ���ͷ�ļ�
	char post2[200] = "action:login&drop:0&pop:1&type:2&n:117&acid:2&username:{SRUN3}\r\n";//ѧ��ץ�������õ���ͷ�ļ�
	char password[20];
	char key[] = "1234567890";//����key
	char password_encrypt[50] = "";
	unsigned int i;
	printf("+++++++��ӭʹ��У԰����½��C���԰�+++++++\n");
	printf("+++++++Made By CHN-STUDENT V0.0.1+++++++\n");
	printf("��������Ҫ��¼���û���:");
	gets_s(username);
	/*�û������ܲ��ֿ�ʼ*/
	for (i = 0; i<strlen(username); ++i)
	{
		username[i] = username[i] + 4;
	}
	strcat_s(post, username);//���Ƽ��ܺ��û�����post�����ַ���
	strcat_s(post, "&password:");
	/*�û������ܲ��ֽ���*/
	printf("��������Ҫ��¼�����루����ʾ��:");
	/*�������벿�ֿ�ʼ*/
	char ch;
	for (i = 0; (ch = _getch()) != 13; )
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
	/*������ܲ��ֿ�ʼ*/
	for (i = 0; i<strlen(password); ++i)
	{
		int ki = password[i] ^ key[strlen(key) - i%strlen(key) - 1];
		char _l[4] = { (char)((ki & 0x0f) + 0x36) };
		char _h[4] = { (char)((ki >> 4 & 0x0f) + 0x63) };
		if (i % 2 == 0)
		{
			strcat_s(_l, 4, _h);
			strcat_s(password_encrypt, 50, _l);
		}
		else
		{
			strcat_s(_h, 4, _l);
			strcat_s(password_encrypt, 50, _h);
		}
	}
	/*������ܲ��ֽ���*/
	strcat_s(post, password_encrypt);//�������뵽post�����ַ���
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();//��ʼ��libcurl
	if (!curl)
	{
		fprintf(stderr, "libcurl��ʼ��ʧ�ܣ�\n");
		return -1;
	}
	curl_easy_setopt(curl, CURLOPT_URL, POSTURL); //����POST��ַ
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);//����POST����
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		printf("POST����!\n");
		return -1;
	}
	curl_easy_cleanup(curl);//����
	return 0;
}

