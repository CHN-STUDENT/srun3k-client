#include <stdio.h>
#include <curl\curl.h>
#include <string.h>
#include <conio.h>

size_t GET_INFO(void *ptr)
{//����õ��û���Ϣ����
	char *k=(char*)ptr;//��void *��ָ��ǿ��תΪchar *ָ�� 
		  	if(*k=='n')
			{ //���δ��½ 
				printf("\n��������\n"); 
				printf("\n");
				system("pause");
				exit(0);
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
				printf("\n");
				system("pause");
				exit(0);
			} 		
}

int main(void)
{//��ȡ��ǰ��½״̬����
	printf("+++++++++++++���ڻ�ȡ������Ϣ,���Ժ�:+++++++++++++\n"); 
	const char GET_INFO_URL[]="http://172.16.154.130/cgi-bin/rad_user_info";//�����ȡ��½״̬����
	CURL *curl;//����CURL�ľ�� 
  	CURLcode res;//����CURL�������
  	curl = curl_easy_init(); //CURL�����ʼ�� 
  	if(!(curl))
  	{//�����������ʼ�� 
	 	printf("\nLibcurl��ʼ��ʧ�ܣ������´򿪱��������ԣ�������ʧ�ܣ������ҷ�����\n");
	 	system("pause");
	 	exit(-1); 
	}
	if(curl)
	{//���������ʼ��

		curl_easy_setopt(curl,CURLOPT_URL,GET_INFO_URL);//���õõ��û���Ϣ��GET��ַ
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,GET_INFO);//���ô���õ��û���Ϣ����
		curl_easy_setopt(curl,CURLOPT_TIMEOUT,1L);//��ʱ���ó�1s
		//curl_easy_setopt(curl,CURLOPT_VERBOSE,1); //����ģʽ
		res = curl_easy_perform(curl);//����libcurl 
		if (res != CURLE_OK)
		{
			printf("\nGET����!���������������!\n");
			curl_easy_cleanup(curl);//����
			printf("\n");
			system("pause");
			exit(-1); 
		}
		curl_easy_cleanup(curl);//����
	}
}
