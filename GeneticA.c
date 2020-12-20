#if 1
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

#define Chromosome 20					// ����ü ����, 20��
#define CardCount 5						// ī�� �� ���� : 5��
#define BinSpace 6						// ī�� �� ���� ǥ���ϱ� ���� 0,1 �ڸ� �� : 6 ( 52 < 64 )
#define TotalCount CardCount*BinSpace	// 5 * 6 = 30
#define ParentsNum 8					// �θ�� ���õ� ����ü�� ��, 8��

#define SpadeRoyalFlushScore 5000
#define RoyalFlushScore 2000
#define StraightFlushScore 1000
#define FourCardScore 130
#define FullHouseScore 100
#define FlushScore 160
#define StraightScore 80
#define TripleScore 40
#define TwoPairScore 20
#define OnePairScore 10

void BinCardInit(int Bin_card[]);						// 0,1 2�������� ī�� �ʱ�ȭ
void CardSort(int Bin_card[]);							// Bin_card[]�迭�� ����
void BinToDec(int Local_Bin_cardset[], int Dec_card[]);	// 2���� -> 10����
void SelectionSort(int Dec_card2[][CardCount]);			// WorstCase�� ����ؼ� MergeSort�� ������ٰ� Overhead�� ������ SelectionSort ���
void ViewCard(int Dec_card2[][CardCount]);				// Card x 5(CardCount), ����� ���� �ڵ�
int FitnessFunction(int Dec_card[], int Score[][Chromosome], int k);	// ���յ� �Լ�

int main(void)
{
	system("mode con cols=150 lines=40");
	int Bin_card[Chromosome][TotalCount] = { 0 };		// 2��������(0,1) ǥ���� ī�� �迭
	int Bin_card_next[Chromosome][TotalCount] = { 0 };	// ���� ���븦 ���� �ӽ� �迭
	int Dec_card[CardCount] = { 0 };					// 10�������� ǥ���� ī�� �迭

	int Score[2][Chromosome] = { 0 };		// ���յ��� ���� ���� �迭
	int AccScore[Chromosome] = { 0 };		// ���� ������ �迭
	int ScoreSum = 0;
	double RanPro[Chromosome] = { 0 };		// ���� Ȯ�� -> �귿 �ٿ� ���
	double AccPro[Chromosome] = { 0 };		// ���� Ȯ�� -> �귿 �ٿ� ���

	srand((unsigned)time(NULL));

	// Bin_card �迭 �ʱ�ȭ
	for (int i = 0; i < Chromosome; i++)
		BinCardInit(Bin_card[i]);

	int Circle = 0;										// n���� üũ�� ���� ����
	while(1)
	{
		Circle++;
		for (int i = 0; i < Chromosome; i++)			// Chromosome ������ŭ��
			CardSort(Bin_card[i]);						// �� �迭�� 0,1�� �ʱ�ȭ
	
		// Score �ʱ�ȭ : 5��, �귿���� ���� RanPro �ʱ�ȭ
		for (int i = 0; i < Chromosome; i++)
		{
			Score[0][i] = i;
			Score[1][i] = 5;		// Score �⺻����
			RanPro[i] = (double)rand() / RAND_MAX;
		}

		// ��¿� �ڵ�
		{
			printf("\n   ��");
			for (int i = 0; i < 29; i++)
				printf("��");
			printf("%6d ����     ", Circle);
			for (int i = 0; i < 31; i++)
				printf("��");
			printf("��\n\n");
		}

		// ��� & FitnessFunction
		for (int i = 0; i < Chromosome; i++)
		{
			// Chromosome : 0,1 ��¿� �ڵ�
			{
				printf("\t %2d��° : ", i + 1);
//				printf("\t %2d��° : ", i + 1);
				for (int j = 0; j < TotalCount; j++)
				{
					printf("%d", Bin_card[i][j]);
					if ((j + 1) % BinSpace == 0)
						printf(" �� ");
				}
			}
			// 2���� -> 10���� �� ��¿� �ڵ�
			{
				BinToDec(Bin_card[i], Dec_card);
				for (int j = 0; j < CardCount; j++)
					printf("%3d", Dec_card[j]);
			}

			// ���յ� ��� !
			FitnessFunction(Dec_card, Score, i);
			printf("\n");
		}

		// ScoreSum, AccScore ���
		for (int j = 0; j < Chromosome; j++)
		{
			ScoreSum += Score[1][j];
			AccScore[j] = ScoreSum;
		}

		// AccPro ���
		for (int j = 0; j < Chromosome; j++)
		{
			AccPro[j] = (double)AccScore[j] / ScoreSum;
		}

		// ����� ��� ������,
	#if 0
		for (int j = 0; j < Chromosome; j++)
		{
			printf("  RanPro[%2d] : %f, \tAccPro[%2d] : %f,  \n", j, RanPro[j], j, AccPro[j]);
		}
	#endif

		// �귿 �� ����
		for (int j = 0; j < ParentsNum; j++)
		{
			for (int k = 0; k < Chromosome; k++)
			{
				if (RanPro[j] < AccPro[k])
				{
					for (int m = 0; m < TotalCount; m++)
						Bin_card_next[j][m] = Bin_card[k][m];

					break;
				}
			}
		}


		// CrossOver ����
		int CrossIndex = (rand() % (CardCount - 1)) + 1;
		for (int j = ParentsNum, Switch = 0; j < 2 * ParentsNum; j++)
		{
			if (j % 2 == 0) Switch = 1;
			else Switch = -1;
			int ParentsIndex = j - ParentsNum;

			for (int k = 0; k < TotalCount; k++)
			{
				if (k < CrossIndex * BinSpace)
					Bin_card_next[j][k] = Bin_card_next[ParentsIndex][k];
				else
					Bin_card_next[j][k] = Bin_card_next[ParentsIndex + Switch][k];
			}
		}

		// �������� �����ϰ� ���� ����ϴ�
		for (int j = 2 * ParentsNum; j < Chromosome; j++)
		{
			BinCardInit(Bin_card_next[j]);
		}

		// Bin_card_next ���, �����, ������, mutation ������
	#if 0
		for (int i = 0; i < Chromosome; i++)
		{
			printf(" %d��° : ", i+1);
			for (int j = 0; j < TotalCount; j++)
			{
				printf("%d", Bin_card_next[i][j]);
				if ((j + 1) % BinSpace == 0)
					printf(" �� ");
			}
			printf("\n");
		}
	#endif

		// mutation ����, 1%
		for (int j = 0, Rand = 1; j < Chromosome; j++)
		{
			for (int k = 0; k < TotalCount; k++)
			{
				if ((rand() % 100) < Rand)
				{
					if (Bin_card_next[j][k] == 0)
						Bin_card_next[j][k] = 1;
					else
						Bin_card_next[j][k] = 0;
				}
			}
		}

		// �����, ������,
		/*
		printf("CroSSIndex : %d\n", CrossIndex);
		for (int i = 0; i < Chromosome; i++)
		{
			for (int j = 0; j < TotalCount; j++)
			{
				printf("%d", Bin_card_next[i][j]);
				if ((j + 1) % BinSpace == 0)
					printf(" �� ");
			}
			printf("\n");
		}
		*/

		// ���� ����
		for (int i = 0; i < Chromosome; i++)
			for (int j = 0; j < TotalCount; j++)
				Bin_card[i][j] = Bin_card_next[i][j];

		// �ܼ� ��¿� �ڵ�
		{
			printf("\n   ��");
			for (int i = 0; i < 68; i++)
				printf("��");
			printf("��\n");
		printf("\n\t\t\t\t\t\t\t     Score ���� : %d\n\n\t\t\t\t\t\t    ", ScoreSum);
		}

		ScoreSum = 0;
		system("pause");
		system("cls");
		}

	//	for (int i = 0; i < Chromosome; i++)
	//		printf(" %d,%d/", Score[0][i], Score[1][i]);
}

void BinCardInit(int Bin_card[])
{
	int Dec_cardset[CardCount] = { 0 };
	for (int i = 0; i < CardCount; i++)
	{
		Dec_cardset[i] = (int)rand() % 52;
	}
	int index = 0;
	for (int i = 0; i < CardCount; i++)
	{
		index = BinSpace * i;
		for (int j = index + BinSpace - 1; j >= index; j--)
		{
			Bin_card[j] = Dec_cardset[i] % 2;
			Dec_cardset[i] /= 2;
		}
	}
}

void CardSort(int Bin_card[])
{

	int Dec_cardset[CardCount] = { 0 };
	BinToDec(Bin_card, Dec_cardset);

	int Dec_card2[2][CardCount] = { 0 };
	for (int i = 0, j = 0; i < CardCount; i++)
	{
		Dec_card2[0][i] = Dec_cardset[i] / 13;
		Dec_card2[1][i] = (Dec_cardset[i] % 13);
	}

	SelectionSort(Dec_card2);
	for (int i = 0, j = 0; i < CardCount; i++)
	{
		Dec_cardset[i] = (Dec_card2[0][i]*13) + Dec_card2[1][i];
	}

	int index = 0;
	for (int i = 0; i < CardCount; i++)
	{
		index = BinSpace * i;
		for (int j = index + BinSpace - 1; j >= index; j--)
		{
			Bin_card[j] = Dec_cardset[i] % 2;
			Dec_cardset[i] /= 2;
		}
	}
}

void BinToDec(int Local_Bin_cardset[], int Dec_card[])
{
	int index = 0, sum = 0, value = 1;
	for (int i = 0; i < CardCount; i++)
	{
		index = i * BinSpace;
		sum = 0, value = 1;
		for (int j = BinSpace - 1; j >= 0; j--)
		{
			sum += value * Local_Bin_cardset[index+j];
			value *= 2;
		}
		Dec_card[i] = sum;
	}
}

void SelectionSort(int Dec_card2[][CardCount])
{
	{
		int i, j, low, temp[2] = { 0 };
		for (i = 0; i < CardCount - 1; i++) {
			low = i;
			for (j = i + 1; j < CardCount; j++)
				if (Dec_card2[1][low] > Dec_card2[1][j])
					low = j;

			temp[0] = Dec_card2[0][i];
			temp[1] = Dec_card2[1][i];

			Dec_card2[0][i] = Dec_card2[0][low];
			Dec_card2[1][i] = Dec_card2[1][low];

			Dec_card2[0][low] = temp[0];
			Dec_card2[1][low] = temp[1];
		}
	}
}

void ViewCard(int Dec_card2[][CardCount])
{
	printf("   ");
	for (int i = 0; i < CardCount; i++)
	{
		if (Dec_card2[0][i] == 0)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 242);
			printf("��");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
		}
		else if (Dec_card2[0][i] == 1)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 252);
			printf("��");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
		}
		else if (Dec_card2[0][i] == 2)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 251);
			printf("��");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
		}
		else if (Dec_card2[0][i] == 3)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 246);
			printf("��");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 241);
			printf(" X   ");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
			continue;
		}
		if (Dec_card2[1][i] <= 10) printf("%-2d ", Dec_card2[1][i]);
		else if (Dec_card2[1][i] == 11) printf("J  ");
		else if (Dec_card2[1][i] == 12) printf("Q  ");
		else printf("K  ");
	}
}

int FitnessFunction(int Dec_card[], int Score[][Chromosome],int k)
{
	int cheet_check = 0;
	int Dec_card2[2][CardCount] = { 0 };
	for (int i = 0, j = 0; i < CardCount; i++)
	{
		if (Dec_card[i] > 51)
		{
			Dec_card2[0][i] = -1;
			Dec_card2[1][i] = --j;
			Score[1][k] = 0;
			cheet_check = 1;
			continue;
		}
		Dec_card2[0][i] = Dec_card[i] / 13;
		Dec_card2[1][i] = (Dec_card[i] % 13)+1;
	}
	
	// ���� ī��
	for (int i = 0, check = 0; i < CardCount - 1; i++)
	{
		for (int j = i+1; j < CardCount; j++)
			if (Dec_card[i] == Dec_card[j])
			{
			//	printf("���� ī�� %d %d", Dec_card[i], Dec_card[j]);
				check = 1;
				cheet_check = 2;
				Score[1][k] = 0;
				break;
			}
		if (check == 1)
			break;
	}

	ViewCard(Dec_card2);

	int SD_c2[2][CardCount] = { 0 };
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < CardCount; j++)
			SD_c2[i][j] = Dec_card2[i][j];


	// �ξ� ��Ʈ����Ʈ �÷��� ~~ ���� ~~ ������ ~~
#if 0
	int type = 3;
	int number = 9;
	for (int i = 0; i < CardCount; i++)
	{
		SD_c2[0][i] = type;
		SD_c2[1][i] = number++;
	}
	SD_c2[1][0] = 1;
//	Score[1][k] = 50;
#endif

	// �÷��� ~~ ���� ~~ ������ ~
#if 0
	int type = 3;
	int number = 8;
	for (int i = 0; i < CardCount; i++)
	{
		SD_c2[0][i] = type;
		SD_c2[1][i] = number++;
	}
//	SD_c2[1][0] = 7;
//	Score[1][k] = 50;
#endif

	SelectionSort(SD_c2);

	// ���� �Ǻ�
	if (cheet_check > 0)
	{
		printf("\tScore : %-4d", Score[1][k]);
		{
			if (cheet_check == 1)
				printf("OverValue");
			else if (cheet_check == 2)
				printf("Same Card");
			return 0;
		}
	}

	int check = 0;
	// (Royal)(Straight) Flush
	if (SD_c2[0][0] == SD_c2[0][1] && SD_c2[0][0] == SD_c2[0][2] && SD_c2[0][0] == SD_c2[0][3] && SD_c2[0][0] == SD_c2[0][4])
	{
		if (SD_c2[1][0] == 1 && SD_c2[1][1] == 10)
		{
			SD_c2[1][0] = 14;
			if (SD_c2[0][0] == 3)
			{
				check = 10; //printf("Spade Royal Flush !!");
				Score[1][k] += SpadeRoyalFlushScore + 80 * (SD_c2[1][0]+SD_c2[1][1]+SD_c2[1][2]+ SD_c2[1][3]+ SD_c2[1][4]);
			}
			else
			{
				check = 9; //printf("Royal Flush !!");
				Score[1][k] += RoyalFlushScore + 70 * (SD_c2[1][0] + SD_c2[1][1]+SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
			}
			SelectionSort(SD_c2);
			SD_c2[1][4] = 1;
		}
		else
		{
			int value = SD_c2[1][0];
			for (int i = 1; i < CardCount; i++)
			{
				if (SD_c2[1][i] == ++value) check++;
				else break;
			}
			if (check == CardCount - 1)
			{
				check = 8; // printf("Straight Flush !!");
				Score[1][k] += StraightFlushScore + 40 * (SD_c2[1][0] + SD_c2[1][1] + SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
			}
			else
			{
				check = 5; // printf("Flush!!");
				Score[1][k] += FlushScore + 20 * (SD_c2[1][0] + SD_c2[1][1] + SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
			}
		}
	}
	// Four Card�� Triple
	else if (SD_c2[1][1] == SD_c2[1][2] && SD_c2[1][1] == SD_c2[1][3])
	{
		if (SD_c2[1][0] == SD_c2[1][1] || SD_c2[1][4] == SD_c2[1][1])
		{
			check = 7; //printf("Four Card !!");
			Score[1][k] += FourCardScore + 7 * (SD_c2[1][0] + SD_c2[1][1] + SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
		}
		else
		{
			check = 3; //printf("Triple !!");
			Score[1][k] += TripleScore + 3 * (SD_c2[1][0] + SD_c2[1][1] + SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
		}
	}
	// Full House�� Triple
	else if (SD_c2[1][0] == SD_c2[1][1] && SD_c2[1][0] == SD_c2[1][2])
	{
		if (SD_c2[1][3] == SD_c2[1][4])
		{
			check = 6; //printf("Full House !!");
			Score[1][k] += FullHouseScore + 6 * (SD_c2[1][0] + SD_c2[1][1] + SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
		}
		else
		{
			check = 3; //printf("Triple !!");
			Score[1][k] += TripleScore + 3 * (SD_c2[1][0] + SD_c2[1][1] + SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
		}
	}
	else if (SD_c2[1][2] == SD_c2[1][3] && SD_c2[1][2] == SD_c2[1][4])
	{
		if (SD_c2[1][0] == SD_c2[1][1])
		{
			check = 6; // printf("Full House !!");
			Score[1][k] += FullHouseScore + 6 * (SD_c2[1][0] + SD_c2[1][1] + SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
		}
		else
		{
			check = 3; // printf("Triple !!");
			Score[1][k] += TripleScore + 3 * (SD_c2[1][0] + SD_c2[1][1] + SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
		}
	}
	// Straight ( ���� ) ( ���� )
	else if ((SD_c2[1][0] == SD_c2[1][1] - 1) && (SD_c2[1][1] == SD_c2[1][2] - 1) && (SD_c2[1][2] == SD_c2[1][3] - 1) && (SD_c2[1][3] == SD_c2[1][4] - 1))
	{
		check = 4; //printf("Straight !!");
		Score[1][k] += StraightScore + 4 * (SD_c2[1][0] + SD_c2[1][1] + SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
	}

	// Two Pair�� One Pair
	else if (SD_c2[1][0] == SD_c2[1][1])
	{
		if ((SD_c2[1][2] == SD_c2[1][3]) || (SD_c2[1][3] == SD_c2[1][4]))
//		if ((SD_c2[1][2] == SD_c2[1][3]) || (SD_c2[1][2] == SD_c2[1][4]) || (SD_c2[1][3] == SD_c2[1][4]))
		{
			check = 2; //printf("Two Pair");
			Score[1][k] += TwoPairScore + 2 * (SD_c2[1][0] + SD_c2[1][1] + SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
		}
		else
		{
			check = 1; //printf("One Pair");
			Score[1][k] += OnePairScore + 1 * (SD_c2[1][0] + SD_c2[1][1] + SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
		}
	}
	else if (SD_c2[1][1] == SD_c2[1][2])
	{
		if (SD_c2[1][3] == SD_c2[1][4])
		{
			check = 2; //printf("Two Pair");
			Score[1][k] += TwoPairScore + 2 * (SD_c2[1][0] + SD_c2[1][1] + SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
		}
		else
		{
			check = 1; //printf("One Pair");
			Score[1][k] += OnePairScore + 1 * (SD_c2[1][0] + SD_c2[1][1] + SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
		}
	}
	else if (SD_c2[1][2] == SD_c2[1][3] || SD_c2[1][3] == SD_c2[1][4])
	{
		check = 1;// printf("One Pair");
		Score[1][k] += OnePairScore + 1 * (SD_c2[1][0] + SD_c2[1][1] + SD_c2[1][2] + SD_c2[1][3] + SD_c2[1][4]);
    }

	// No Pair (high card)
	else
	{
		check = 0; //printf("No Pair");
//		Score[1][k] += (5 * SD_c2[0][0]) + SD_c2[1][4];
		Score[1][k] += 5;
	 }

	printf("\tScore : %-5d", Score[1][k]);
	switch (check)
	{
	case 0: printf("No Pair"); break;
	case 1: printf("One Pair"); break;
	case 2: printf("Two Pair"); break;
	case 3: printf("Triple"); break;
	case 4: printf("Straight"); break;
	case 5: printf("Flush"); break;
	case 6: printf("Full House"); break;
	case 7: printf("Four Card"); break;
	case 8: printf("Straight Flush"); break;
	case 9: printf("Royal Flush"); break;
	case 10: printf("Spade Royal Flush !!"); break;
	}
	return 0;
}
#endif