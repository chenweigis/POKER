#include "stdafx.h"
#include "windows.h"
#include "string.h"
//#include "malloc.h"
/*
��������Ϸ����:
1.һ��player������AI��
2.һ��poker�ܹ�39��={3,4,5,6,7,8,9,10,J,Q,K,A,2}*3
3.һ��ֻ�ܳ�һ��card,�ȴ�С���б�desk�ϴ�ı����
*/

#define Max_card 39
#define Max_player 3
#define Max_playercard 13
#define error -1 //��ʾ�쳣����ֵ

int i_poker[Max_card]= {
	3,4,5,6,7,8,9,10,11,12,13,14,15,//12
	3,4,5,6,7,8,9,10,11,12,13,14,15,//25
	3,4,5,6,7,8,9,10,11,12,13,14,15//38
};
struct s_player {
	int cardnum;//����ʣ����Ƹ���
	int i_player_poker[Max_playercard];//����������
};
s_player player[Max_player];//����������ң��±�0��ʾplayer
int deskcard=0;//��������ϵ���
int handindex=0;//��ų��Ƶ�����
int loop=0;//��¼Ҫ����Ĵ���������Ҫ����ʱ���ٱȴ�С������ֱ�ӳ���

char *c_card[Max_playercard];

void washcard();//ϴ�ƺ���

void dealcard();//���ƺ���

void handcard();//���ƺ���

int playerhand();//��ҳ���

int aihand(int ai);//AI����

int judge(int card);//�жϳ������ǲ��ǺϷ���

char* exchange(int card);//����ƣʣѣˣ���

int _tmain(int argc, _TCHAR* argv[])
{

	player[0].cardnum=Max_playercard;
	player[1].cardnum=Max_playercard;
	player[2].cardnum=Max_playercard;
	//ϴ��
	washcard();
	//��ʾϴ�õ���
	printf("ϴ���С�������\n");
	Sleep(1000);

	for(int i=0; i<Max_card; i++) {

		/*if (i_poker[i]==10)
		printf("%d ",i_poker[i]);
		else */
		if ((i+1)%13==0)
			printf("%s\n",exchange(i_poker[i]));
		else
			printf("%s ",exchange(i_poker[i]));
	}

	//����
	printf("��ʼ���ơ�������\n");
	Sleep(1000);
	//printf("�����ϵ����ǡ�������\n");
	dealcard();
	//�Ӵ�С������ʾ����������

	for (int i=0; i<Max_playercard-1; i++) {
		for (int j=i; j<Max_playercard; j++) {
			if(player[0].i_player_poker[i]<player[0].i_player_poker[j]) {
				int temp=player[0].i_player_poker[i];
				player[0].i_player_poker[i]=player[0].i_player_poker[j];
				player[0].i_player_poker[j]=temp;
			}
		}
	}
//����	//�ȴ�С
	handcard();

	system("pause");
	return 0;

}
void washcard()
{

	for (int i=0; i<Max_card-1; i++) { //��i��ֵΪi<Max_cardʱ���ڴ����
		int numbehind = Max_card - (i+1);
		srand(GetTickCount());
		int randindex = rand()%numbehind+(i+1);
		int temp = i_poker[i];
		i_poker[i]=i_poker[randindex];
		i_poker[randindex]=temp;
	}

};
void dealcard()
{

	for (int j=0; j<3; j++) {
		for (int i=0; i<Max_playercard; i++) {
			player[j].i_player_poker[i]=i_poker[j*Max_playercard+i];
		}

	}

};
void handcard()
{

	srand(GetTickCount());
	int handindex = rand()%Max_player;//��ȡ�״γ��Ƶ��˵�������0��1��2��
	//0->1->2->0
	while(player[0].cardnum!=0 && player[1].cardnum!=0 && player[2].cardnum!=0) {
		if (handindex==0) {
			char yn;
			printf("�Ƿ����Y/N:");
			scanf("%c",&yn);
			getchar();
			printf("\n");
			if (yn=='Y') {
				int result =playerhand();
				if (result!=error) {
					deskcard=result;
					Sleep(1000);
					printf("��������ǣ�%s\n",exchange(result));
					/*if (result==10)
					{
					printf("��������ǣ�%d\n",result);
					}
					else
					{
					printf("��������ǣ�%c\n",exchange(result));
					}*/
					loop=0;
					Sleep(1000);
				} else if (result==error) {
					printf("Ҫ����\n");
					loop=loop+1;
					Sleep(1000);
				}


				//��������ָ����һ��������
				handindex=(handindex+4)%3;
				continue;

			} else if(yn=='N') {
				handindex=(handindex+4)%3;
				loop=loop+1;
				continue;
			} else
				continue;


		} else if (handindex!=0) {
			int result=aihand(handindex);
			if (result==error) {
				printf("AI[%d]Ҫ����\n",handindex);
				loop=loop+1;
				Sleep(1000);
			}

			else if (result!=error) {
				printf("AI[%d]�������ǣ�%s\n",handindex,exchange(result));
				/*if (result==10)
				{
				printf("AI[%d]�������ǣ�%d\n",handindex,result);
				}
				else
				{
				printf("AI[%d]�������ǣ�%c\n",handindex,exchange(result));
				}
				*/
				deskcard=result;
				loop=0;
				Sleep(1000);
			}

			//��������ָ����һ��������
			handindex=(handindex+4)%3;
			continue;
		}
	}
	if (player[0].cardnum==0)
		printf("��һ�ʤ\n");
	else if (player[1].cardnum==0)
		printf("AI[1]��ʤ\n");
	else if (player[2].cardnum==0)
		printf("AI[2]��ʤ\n");
	else
		printf("error!\n");
};

int playerhand()//����ֵ����-1ΪҪ����,���߷��ش������
{


	char c_poutcard[3];
	memset(c_poutcard,0,3);
	//char* str1;
	//char* str2;


	//int i_poutcard=0;
	int mark=0;
	printf("�����ϵ�����:\n");
	for (int ii=0; ii<player[0].cardnum; ii++) {
		printf("%s ",exchange(player[0].i_player_poker[ii]));
		/*if (player[0].i_player_poker[ii]==10)
		{
			printf("%d ",player[0].i_player_poker[ii]);
		}
		else
		printf("%c ",exchange(player[0].i_player_poker[ii]));*/

	}
	printf("\n");

	for (int i=0; i<player[0].cardnum; i++) {
		if(deskcard<player[0].i_player_poker[i] || loop==2) {
			printf("����ƣ�");

			//gets(c_poutcard);
			scanf("%s",c_poutcard);
			getchar();//ѭ�������ַ���س��ᱻ��¼

			for (int jj=0; jj<Max_playercard; jj++) {
				//str1=c_poutcard;
				//str2=c_card[jj];
				/*while(*str1==*str2&&str1!='\0'&&str2!='\0'){
					str1+=1;
					str2+=1;

				}*/


				if (strcmp(c_poutcard,c_card[jj])==0) {
					mark=judge(jj+3);
					if (mark==error) {
						printf("��ĳ��Ʋ����Ϲ��������³���!\n");
						memset(c_poutcard,0,3);
						scanf("%s",c_poutcard);
						getchar();//ѭ�������ַ���س��ᱻ��¼
						jj=0;
						continue;

					} else
						//i_poutcard=jj;
						break;
				} else if (jj==Max_playercard-1) {
					printf("��ĳ��Ʋ����Ϲ��������³���!\n");
					memset(c_poutcard,0,3);
					scanf("%s",c_poutcard);
					getchar();//ѭ�������ַ���س��ᱻ��¼
					jj=0;
					continue;
				} else
					mark=0;
				//continue;
			}


			//int len;
			//len=strlen(c_poutcard);
			//if (len!=1)
			//{
			//	if(c_poutcard[0]=='1' && c_poutcard[1]=='0')
			//	{
			//		mark=judge(10);
			//        //break;
			//	}
			//	else
			//	{
			//		printf("��ĳ��Ʋ����Ϲ��������³���!\n");
			//		playerhand();
			//	}
			//	//break;
			//}
			//
			//else if (len==1)
			//{
			//	for (int jj=0;jj<Max_card;jj++)
			//	{
			//		if (c_poutcard[0]==exchange(jj))
			//		{
			//			mark=jj;
			//			//i_poutcard=jj;
			//			break;
			//		}
			//		else
			//			mark=0;
			//		continue;
			//	}
			//	mark=judge(mark);
			//	//break;
			//}
			//�ж������ֵ�ǲ���Ŀǰ�������������е�,����ȷʱ������ʾ��ֱ�����˺Ϸ�����
			//mark=judge(i_poutcard);
			//����������ϵ���
			for (int search=0; search<player[0].cardnum; search++) {
				if (mark==player[0].i_player_poker[search]) {
					i=search;
					break;
				}
			}
			int temp[Max_playercard]= {0};
			int j=0;
			for(j=0; j<player[0].cardnum; j++) {
				if(j<i)
					temp[j]=player[0].i_player_poker[j];
				else if(j>i)
					temp[j-1]=player[0].i_player_poker[j];
				else
					continue;
			}
			memset(player[0].i_player_poker,0,sizeof(player[0].i_player_poker));//���ԭ�е��ƣ����¸�ֵ
			for(i=0; i<j; i++) {
				player[0].i_player_poker[i]=temp[i];
			}

			player[0].cardnum=player[0].cardnum-1;
			break;
		} else {
			mark=error;
			//continue;
		}
	}
	if (mark==error)
		return mark;
	else
		return mark;
};

int aihand(int ai)
{
	int  i_aoutcard=0;
	int mark=0;
	for (int i=0; i<player[ai].cardnum; i++) {
		if(deskcard<player[ai].i_player_poker[i] || loop==2) {
			i_aoutcard=player[ai].i_player_poker[i];
			mark=0;
			//����AI���ϵ���
			int temp[Max_playercard]= {0};
			int j=0;
			for(j=0; j<player[ai].cardnum; j++) {
				if(j<i)
					temp[j]=player[ai].i_player_poker[j];
				else if(j>i)
					temp[j-1]=player[ai].i_player_poker[j];
			}
			memset(player[ai].i_player_poker,0,sizeof(player[ai].i_player_poker));//���ԭ�е��ƣ����¸�ֵ
			for(i=0; i<j; i++) {
				player[ai].i_player_poker[i]=temp[i];
			}

			player[ai].cardnum=player[ai].cardnum-1;
			break;
		} else {
			mark=error;
			//continue;
		}
	}
	if(mark==error) {
		return mark;
	} else
		return i_aoutcard;
};

int judge(int card)//���غϷ�����
{
	int mark=0;
	for (int j=0; j<player[0].cardnum; j++) {

		if (card == player[0].i_player_poker[j] ) {
			if (card > deskcard || loop==2) {
				mark=0;
				break;
			}
		} else {
			mark=error;
			continue;
		}
	}
	if(mark==error) {
		/*printf("������Ʋ����Ϲ���!�����³��ƣ�");
		playerhand();*/
		return mark;

	} else //if (mark==1)
		return card;
}
char* exchange(int card)
{
	//char *c_card[Max_playercard];
	char c_str[Max_playercard][3]= {"3","4","5","6","7","8","9","10","J","Q","K","A","2"};
	for(int i=0; i<Max_playercard; i++) {
		c_card[i]=(char *)malloc(sizeof(char)*3);
		strcpy(c_card[i],c_str[i]);
		//free(c_card[i]);
	}
	return c_card[card-3];
}