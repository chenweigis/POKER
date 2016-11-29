#include "stdafx.h"
#include "windows.h"
#include "string.h"
//#include "malloc.h"
/*
斗地主游戏规则:
1.一个player，两个AI；
2.一副poker总共39张={3,4,5,6,7,8,9,10,J,Q,K,A,2}*3
3.一次只能出一张card,比大小，有比desk上大的必须出
*/

#define Max_card 39
#define Max_player 3
#define Max_playercard 13
#define error -1 //表示异常返回值

int i_poker[Max_card]= {
	3,4,5,6,7,8,9,10,11,12,13,14,15,//12
	3,4,5,6,7,8,9,10,11,12,13,14,15,//25
	3,4,5,6,7,8,9,10,11,12,13,14,15//38
};
struct s_player {
	int cardnum;//手上剩余的牌个数
	int i_player_poker[Max_playercard];//手上所有牌
};
s_player player[Max_player];//定义三个玩家，下标0表示player
int deskcard=0;//存放桌面上的牌
int handindex=0;//存放出牌的索引
int loop=0;//记录要不起的次数，两次要不起时不再比大小，而是直接出牌

char *c_card[Max_playercard];

void washcard();//洗牌函数

void dealcard();//发牌函数

void handcard();//出牌函数

int playerhand();//玩家出牌

int aihand(int ai);//AI出牌

int judge(int card);//判断出的牌是不是合法的

char* exchange(int card);//输出牌ＪＱＫＡ２

int _tmain(int argc, _TCHAR* argv[])
{

	player[0].cardnum=Max_playercard;
	player[1].cardnum=Max_playercard;
	player[2].cardnum=Max_playercard;
	//洗牌
	washcard();
	//显示洗好的牌
	printf("洗牌中。。。。\n");
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

	//发牌
	printf("开始发牌。。。。\n");
	Sleep(1000);
	//printf("你手上的牌是。。。。\n");
	dealcard();
	//从大到小排序并显示玩家手里的牌

	for (int i=0; i<Max_playercard-1; i++) {
		for (int j=i; j<Max_playercard; j++) {
			if(player[0].i_player_poker[i]<player[0].i_player_poker[j]) {
				int temp=player[0].i_player_poker[i];
				player[0].i_player_poker[i]=player[0].i_player_poker[j];
				player[0].i_player_poker[j]=temp;
			}
		}
	}
//出牌	//比大小
	handcard();

	system("pause");
	return 0;

}
void washcard()
{

	for (int i=0; i<Max_card-1; i++) { //当i的值为i<Max_card时，内存溢出
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
	int handindex = rand()%Max_player;//获取首次出牌的人的索引【0，1，2】
	//0->1->2->0
	while(player[0].cardnum!=0 && player[1].cardnum!=0 && player[2].cardnum!=0) {
		if (handindex==0) {
			char yn;
			printf("是否出牌Y/N:");
			scanf("%c",&yn);
			getchar();
			printf("\n");
			if (yn=='Y') {
				int result =playerhand();
				if (result!=error) {
					deskcard=result;
					Sleep(1000);
					printf("你出的牌是：%s\n",exchange(result));
					/*if (result==10)
					{
					printf("你出的牌是：%d\n",result);
					}
					else
					{
					printf("你出的牌是：%c\n",exchange(result));
					}*/
					loop=0;
					Sleep(1000);
				} else if (result==error) {
					printf("要不起\n");
					loop=loop+1;
					Sleep(1000);
				}


				//处理索引指向下一个出牌人
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
				printf("AI[%d]要不起\n",handindex);
				loop=loop+1;
				Sleep(1000);
			}

			else if (result!=error) {
				printf("AI[%d]出的牌是：%s\n",handindex,exchange(result));
				/*if (result==10)
				{
				printf("AI[%d]出的牌是：%d\n",handindex,result);
				}
				else
				{
				printf("AI[%d]出的牌是：%c\n",handindex,exchange(result));
				}
				*/
				deskcard=result;
				loop=0;
				Sleep(1000);
			}

			//处理索引指向下一个出牌人
			handindex=(handindex+4)%3;
			continue;
		}
	}
	if (player[0].cardnum==0)
		printf("玩家获胜\n");
	else if (player[1].cardnum==0)
		printf("AI[1]获胜\n");
	else if (player[2].cardnum==0)
		printf("AI[2]获胜\n");
	else
		printf("error!\n");
};

int playerhand()//返回值等于-1为要不起,或者返回打出的牌
{


	char c_poutcard[3];
	memset(c_poutcard,0,3);
	//char* str1;
	//char* str2;


	//int i_poutcard=0;
	int mark=0;
	printf("你手上的牌是:\n");
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
			printf("请出牌：");

			//gets(c_poutcard);
			scanf("%s",c_poutcard);
			getchar();//循环输入字符后回车会被记录

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
						printf("你的出牌不符合规则，请重新出牌!\n");
						memset(c_poutcard,0,3);
						scanf("%s",c_poutcard);
						getchar();//循环输入字符后回车会被记录
						jj=0;
						continue;

					} else
						//i_poutcard=jj;
						break;
				} else if (jj==Max_playercard-1) {
					printf("你的出牌不符合规则，请重新出牌!\n");
					memset(c_poutcard,0,3);
					scanf("%s",c_poutcard);
					getchar();//循环输入字符后回车会被记录
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
			//		printf("你的出牌不符合规则，请重新出牌!\n");
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
			//判断输入的值是不是目前手上牌里面已有的,不正确时反复提示，直到出了合法的牌
			//mark=judge(i_poutcard);
			//更新玩家手上的牌
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
			memset(player[0].i_player_poker,0,sizeof(player[0].i_player_poker));//清空原有的牌，重新赋值
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
			//更新AI手上的牌
			int temp[Max_playercard]= {0};
			int j=0;
			for(j=0; j<player[ai].cardnum; j++) {
				if(j<i)
					temp[j]=player[ai].i_player_poker[j];
				else if(j>i)
					temp[j-1]=player[ai].i_player_poker[j];
			}
			memset(player[ai].i_player_poker,0,sizeof(player[ai].i_player_poker));//清空原有的牌，重新赋值
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

int judge(int card)//返回合法的牌
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
		/*printf("你出的牌不符合规则!请重新出牌：");
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