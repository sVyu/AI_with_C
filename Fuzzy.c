#if 0
#include <stdio.h>
#include <Windows.h>

#define level_of_honesty 4		// light, normal, very, great
#define level_of_sincerity 4	// light, normal, very, great

#define light_r 50.0
#define light_q 60.0
#define normal_p 50.0
#define normal_r 65.0
#define normal_q 80.0
#define very_p 65.0
#define very_r 80.0
#define very_q 95.0
#define great_p 80.0
#define great_r 90.0


#define very_negative 0
#define negative 1
#define normal 2
#define a_little_positive 4
#define positive 6
#define very_positive 8
#define full_score 10

#define gap_y 6
#define gap_x 6

void cal_level_of_honesty(int score_of_honesty, double degree_of_honesty[level_of_honesty]);			// 계산 : 정직함 정도 퍼지화
void cal_level_of_sincerity(int score_of_sincerity, double degree_of_sincerity[level_of_sincerity]);	// 계산 : 성실함 정도 퍼지화
double cal_defuzzification(double degree_of_honesty[], double degree_of_sincerity[], int pt_rules[][level_of_sincerity]);		//역퍼지화
void color_value(double result_of_defuzzy);		// color value 설정용 함수

int main()
{
	system("mode con cols=80 lines=27");
	int score_of_honesty = 0, score_of_sincerity = 0;
	double degree_of_honesty[level_of_honesty] = { 0 }, degree_of_sincerity[level_of_sincerity] = { 0 };

	// 인성 검사 룰 !! pt : personality test
	int pt_rules[level_of_honesty][level_of_sincerity] = {
		{	 very_negative,				     negative,			       normal,		a_little_positive},	// 0,  1,  2,  4,
		{		  negative,					   normal,		a_little_positive,				 positive}, // 1,  2,  4,  6,
		{			normal,			a_little_positive,				 positive,			very_positive}, // 2,  4,  6,  8,
		{a_little_positive,					 positive,			very_positive,			   full_score}  // 4,  6,  8, 10
	};

// 출력용 코드 (sincerity#성실함)
#if 1
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 241);
	printf("\n\n\t\t\t\tSincerity＃성실함\n\n\t ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	for (score_of_sincerity = 40; score_of_sincerity <= 100; score_of_sincerity += gap_y)
		printf("%6d", score_of_sincerity);

	printf("\n\n");
#endif

	int check = 0;

	for (score_of_honesty = 40; score_of_honesty <= 100; score_of_honesty += gap_x)
	{
// 출력용 코드 (honesty#정직함)
#if 1 
		check += 1;
		printf("  ");
		if (check == 1) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 244); printf(" H"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); }
		else if (check == 2) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 244); printf(" o"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); }
		else if (check == 3) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 244); printf(" n"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); }
		else if (check == 4) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 244); printf(" e"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); }
		else if (check == 5) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 244); printf(" s"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); }
		else if (check == 6) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 244); printf(" t"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); }
		else if (check == 7) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 244); printf(" y"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); }
		else if (check == 8) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 244); printf(" #"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); }
		else if (check == 9) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 244); printf("정"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); }
		else if (check == 10) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 244); printf("직"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); }
		else if (check == 11) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 244); printf("함"); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); }
		else { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240); printf("  "); }
#endif
		printf(" %3d ", score_of_honesty);
		for (score_of_sincerity = 40; score_of_sincerity <= 100; score_of_sincerity += gap_y)
		{
			cal_level_of_honesty(score_of_honesty, degree_of_honesty);
			cal_level_of_sincerity(score_of_sincerity, degree_of_sincerity);
			double result_of_defuzzy = cal_defuzzification(degree_of_honesty, degree_of_sincerity, pt_rules);
			color_value(result_of_defuzzy);

			printf("%6.2f", result_of_defuzzy);
		}
		printf("\n");
	}

	printf("\n\n\t\t\t");
}

// 계산 : 정직함 정도 퍼지화 
void cal_level_of_honesty(int score_of_honesty, double degree_of_honesty[level_of_honesty])
{
	// light
	if (score_of_honesty <= light_r) degree_of_honesty[0] = 1;
	else if (score_of_honesty > light_q) degree_of_honesty[0] = 0;
	else degree_of_honesty[0] = ((-1) /(light_q - light_r))*((double)score_of_honesty - light_r) + 1;

	// normal
	if (score_of_honesty <= normal_p) degree_of_honesty[1] = 0;
	else if (score_of_honesty > normal_p && score_of_honesty <= normal_r)
		degree_of_honesty[1] = (   1/(normal_r - normal_p)) * ((double)score_of_honesty - normal_r) + 1;
	else if (score_of_honesty > normal_r && score_of_honesty <= normal_q)
		degree_of_honesty[1] = ((-1)/(normal_q - normal_r)) * ((double)score_of_honesty - normal_r) + 1;
	else degree_of_honesty[1] = 0;

	// very
	if (score_of_honesty <= very_p) degree_of_honesty[2] = 0;
	else if (score_of_honesty > very_p && score_of_honesty <= very_r)
		degree_of_honesty[2] = (   1/(very_r - very_p)) * ((double)score_of_honesty - very_r) + 1;
	else if (score_of_honesty > very_r && score_of_honesty <= very_q)
		degree_of_honesty[2] = ((-1)/(very_q - very_r)) * ((double)score_of_honesty - very_r) + 1;
	else degree_of_honesty[2] = 0;

	// great
	if (score_of_honesty < great_p) degree_of_honesty[3] = 0;
	else if (score_of_honesty > great_r) degree_of_honesty[3] = 1;
	else degree_of_honesty[3] = (  1/(great_r - great_p)) * ((double)score_of_honesty - great_r) + 1;

}

// 계산 : 성실함 정도 퍼지화 
void cal_level_of_sincerity(int score_of_sincerity, double degree_of_sincerity[level_of_sincerity])
{
	// light
	if (score_of_sincerity <= light_r) degree_of_sincerity[0] = 1;
	else if (score_of_sincerity > light_q) degree_of_sincerity[0] = 0;
	else degree_of_sincerity[0] = ((-1) / (light_q - light_r)) * ((double)score_of_sincerity - light_r) + 1;

	// normal
	if (score_of_sincerity <= normal_p) degree_of_sincerity[1] = 0;
	else if (score_of_sincerity > normal_p && score_of_sincerity <= normal_r)
		degree_of_sincerity[1] = (1 / (normal_r - normal_p)) * ((double)score_of_sincerity - normal_r) + 1;
	else if (score_of_sincerity > normal_r && score_of_sincerity <= normal_q)
		degree_of_sincerity[1] = ((-1) / (normal_q - normal_r)) * ((double)score_of_sincerity - normal_r) + 1;
	else degree_of_sincerity[1] = 0;

	// very
	if (score_of_sincerity <= very_p) degree_of_sincerity[2] = 0;
	else if (score_of_sincerity > very_p && score_of_sincerity <= very_r)
		degree_of_sincerity[2] = (1 / (very_r - very_p)) * ((double)score_of_sincerity - very_r) + 1;
	else if (score_of_sincerity > very_r && score_of_sincerity <= very_q)
		degree_of_sincerity[2] = ((-1) / (very_q - very_r)) * ((double)score_of_sincerity - very_r) + 1;
	else degree_of_sincerity[2] = 0;

	// great
	if (score_of_sincerity < great_p) degree_of_sincerity[3] = 0;
	else if (score_of_sincerity > great_r) degree_of_sincerity[3] = 1;
	else degree_of_sincerity[3] = (1 / (great_r - great_p)) * ((double)score_of_sincerity - great_r) + 1;
}

// 출력 조정용 color_value 함수
void color_value(double result_of_defuzzy)
{
	if (result_of_defuzzy <= 1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 248);
	else if (result_of_defuzzy <= 3) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 245);
	else if (result_of_defuzzy <= 5) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 242);
	else if (result_of_defuzzy <= 7) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 243);
	else if (result_of_defuzzy <= 9) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
	else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 246);
}

// 역퍼지화 !!
double cal_defuzzification(double degree_of_honesty[], double degree_of_sincerity[], int pt_rules[][level_of_sincerity])
{
	double numerator = 0, denominator = 0, min = 0;
	for(int i = 0; i < level_of_honesty; i++)
		for (int j = 0; j < level_of_sincerity; j++)
		{
			if (degree_of_honesty[i] == 0 || degree_of_sincerity[j] == 0) continue;
			else
			{
				min = (degree_of_honesty[i] < degree_of_sincerity[j]) ? degree_of_honesty[i] : degree_of_sincerity[j];	// and
				numerator += min * pt_rules[i][j];
				denominator += min;
			}
		}
	
	return (numerator/denominator);
} 
#endif