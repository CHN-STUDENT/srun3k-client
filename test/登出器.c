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
			printf("\n�������ߣ��ǳ�ʧ�ܡ�\n");
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
	char LOGOUT[50] = "username=0&mac=&type=2&action=logout";//ѧ��ץ�������õ���ͷ�ļ�
 	char acid[8]="&ac_id=";
 	ACID_INPUT(acid);//ACID����
	strcat(LOGOUT,acid);
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
	curl_easy_setopt(curl_post, CURLOPT_POSTFIELDS,LOGOUT);//����POST����
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
	return 0;
}
