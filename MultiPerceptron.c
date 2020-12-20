#if 0
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define n_input	25				// 입력층 노드 개수
#define n_hidden 10				// 은닉층 노드 개수
#define n_output 8				// 출력층 노드 개수
#define n2_output 8
#define lines 5					// 함수 사용과 출력의 편의를 위해 임의로 설정한 줄 수 입니다
								// ex) 25 노드 = 5 노드 x 5 lines
#define margin_of_error 0.0012	// 오차 범위 : 2 x margin_of_error ,,

void SetW_ih(double W_ih[][n_input]);				// 가중치 초기화
void SetW_ho(double W_ho[][n_hidden]);				// 가중치 초기화
void PrintNodeH(int N_input[], double W_ih[][n_input], double N_hidden[], int check);					// 입력층 -> 은닉층 노드 출력
void PrintNodeO(double N_hidden[], double W_ho[][n_hidden], double N_output[], int check);				// 은닉층 -> 출력층 노드 출력
void CalOutError(double E_output[], double N_output[], int n);											// 출력층 오차 계산
void CalHiddenError(double E_hidden[], double N_hidden[], double E_output[], double W_ho[][n_hidden]);	// 은닉층 오차 계산
void RenewW_ho(double W_ho[][n_hidden], double eta, double E_output[], double N_hidden[]);				// 가중치 수정 (은닉층 -> 출력층)
void RenewW_ih(double W_ih[][n_input], double eta, double E_hidden[], int N_input[]);					// 가중치 수정 (입력층 -> 은닉층)
int CheckEnd(double W_ih[][n_input], double W_ih_check[][n_input], double W_ho[][n_hidden], double W_ho_check[][n_hidden]);		// 루프 종료 체크
void CopyW(double W_ih[][n_input],double W_ih_check[][n_input], double W_ho[][n_hidden], double W_ho_check[][n_hidden]);		// 가중치 복사 

double GaussianRandom(void);						// 가우시안 표준정규분포 함수, 가중치 초기화에 사용 !!
void PrintInputPattern(int P_input[][n_input]);		// 입력패턴 단순 출력을 위한 함수 (printf)

int main()
{
	// 입력층 (패턴): A, B, C, D, E, F, G, H
	int P_input[n_output][n_input] =
	{

// 5x5
#if 1
		// A
		{
			0, 1, 1, 1, 0,
			1, 0, 0, 0, 1,
			1, 1, 1, 1, 1,
			1, 0, 0, 0, 1,
			1, 0, 0, 0, 1
		},

		// B
		{
			1, 1, 1, 1, 0,
			1, 0, 0, 0, 1,
			1, 1, 1, 1, 0,
			1, 0, 0, 0, 1,
			1, 1, 1, 1, 0
		},

		// C
		{
			0, 1, 1, 1, 0,
			1, 0, 0, 0, 1,
			1, 0, 0, 0, 0,
			1, 0, 0, 0, 1,
			0, 1, 1, 1, 0
		},

		// D
		{
			1, 1, 1, 1, 0,
			1, 0, 0, 0, 1,
			1, 0, 0, 0, 1,
			1, 0, 0, 0, 1,
			1, 1, 1, 1, 0
		},


		// E
		{
			1, 1, 1, 1, 1,
			1, 0, 0, 0, 0,
			1, 1, 1, 1, 1,
			1, 0, 0, 0, 0,
			1, 1, 1, 1, 1
		},

		// F
		{
			1, 1, 1, 1, 1,
			1, 0, 0, 0, 0,
			1, 1, 1, 1, 1,
			1, 0, 0, 0, 0,
			1, 0, 0, 0, 0
		},

		// G
		{ 
			0, 1, 1, 1, 0,
			1, 0, 0, 0, 0,
			1, 0, 1, 1, 1,
			1, 0, 0, 0, 1,
			0, 1, 1, 1, 0
		},

		// H
		{
			1, 0, 0, 0, 1,
			1, 0, 0, 0, 1,
			1, 1, 1, 1, 1,
			1, 0, 0, 0, 1,
			1, 0, 0, 0, 1
		},

#if 0
		// I
		{
			1, 1, 1, 1, 1,
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0,
			1, 1, 1, 1, 1,
		},
#endif
#if 0
		// J
		{
			1, 1, 1, 1, 1,
			0, 0, 1, 0, 0,
			0, 0, 1, 0, 0,
			1, 0, 1, 0, 0,
			0, 1, 0, 0, 0,
		},
#endif
#if 0
		// K
		{
			1, 0, 0, 0, 1,
			1, 0, 0, 1, 0,
			1, 1, 1, 0, 0,
			1, 0, 0, 1, 0,
			1, 0, 0, 0, 1,
		},
#endif

#endif
		/*
		// ㄱ
		{
			 1,	 1,	 1,	 1,  1,	 1,	 1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1
		},

		#if 1
		// ㄴ
		{
			 1,	 0,	 0,	 0,  0,	 0,	 0,
			 1,	 0,	 0,	 0,	 0,	 0,  0,
			 1,	 0,	 0,	 0,	 0,	 0,  0,
			 1,	 0,	 0,	 0,	 0,	 0,  0,
			 1,	 1,	 1,	 1,	 1,	 1,  1
		},
		#endif

		#if 1
		// ㄹ
		{
			 1,	 1,	 1,	 1,  1,	 1,	 1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 1,	 1,	 1,	 1,	 1,	 1,  1,
			 1,	 0,	 0,	 0,	 0,	 0,  0,
			 1,	 1,	 1,	 1,	 1,	 1,  1
		},
		#endif

		#if 1
		// ㅋ
		{
			 1,	 1,	 1,	 1,  1,	 1,	 1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 1,	 1,	 1,	 1,  1,	 1,	 1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1
		},
		#endif

		#if 1
		// ㅌ

		{
			 1,	 1,	 1,	 1,  1,	 1,	 1,
			 1,	 0,	 0,	 0,	 0,	 0,  0,
			 1,	 1,	 1,	 1,	 1,	 1,  1,
			 1,	 0,	 0,	 0,	 0,	 0,  0,
			 1,	 1,	 1,	 1,	 1,	 1,  1
		},
		#endif
		*/

// 6x6
#if 0
		//A
		{
			0, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1
		},

		//B
		{
			1, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 0
		},
	
		//C
		{
			0, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1
		},

		//D
		{
			1, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 0
		},

		//E
		{
			1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 1, 1
		},

		//F
		{
			1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0,
		},

		//G
		{
			0, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0,
			1, 0, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			0, 1, 1, 1, 1, 0,
		},

		//H
		{
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1
		},
#endif
		
// 7x7
#if 0

		// A
		{
			0, 1, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
		},

		// B
		{
			1, 1, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 1, 0,
		},

		// C
		{
			0, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1,
		},

		// D
		{
			1, 1, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 1, 0,
		},

		// E
		{
			1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 1, 1, 1,
		},

		// F
		{
			1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
		},
		// G
		{
			0, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			0, 1, 1, 1, 1, 1, 0,
		},
		// H
		{
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
		},
#endif
	};
	
	// 입력패턴 출력 코드
	PrintInputPattern(P_input);

	double N_hidden[n_hidden] = { 0 };	// 은닉층 노드
	double N_output[n_output] = { 0 };	// 출력층 노드

	double W_ih[n_hidden][n_input] = { 0 };		// 입력층 ㅡ 은닉층 가중치 배열
	SetW_ih(W_ih);		// 랜덤으로 설정?		// 초기화

	double W_ho[n_output][n_hidden] = { 0 };	// 은닉층 ㅡ 출력층 가중치 배열
	SetW_ho(W_ho);		// 랜덤으로 설정?		// 초기화

	double E_hidden[n_hidden] = { 0 };			// 은닉층 오차 배열
	double E_output[n_output] = { 0 };			// 출력층 오차 배열

	double eta = 0.1;				// 학습율	// η : 가중치 수정을 위한 값

	int check = 0;								// 루프 종료를 위한 변수
	double W_ih_check[n_hidden][n_input] = { 0 };	// 루프 종료를 위한 가중치 배열 w(t-1)
	double W_ho_check[n_output][n_hidden] = { 0 };	// 루프 종료를 위한 가중치 배열 w(t-1)

	int epoch = 0;								// 루프 반복 횟수

	srand((unsigned)time(NULL));
	clock_t start, end;
	float runtime;
	start = clock();



	while (!check)
	{
		// 입력패턴의 수(= 출력층 노드 수) 만큼 반복 !
		for (int i = 0; i < n_output; i++)
		{
			// 입력층 -> 은닉층 노드 출력
			PrintNodeH(P_input[i], W_ih, N_hidden, check);

			// 은닉층 -> 출력층 노드 출력
			PrintNodeO(N_hidden, W_ho, N_output, check);

			// 출력층 오차 계산
			CalOutError(E_output, N_output, i);

			// 은닉층 오차 계산
			CalHiddenError(E_hidden, N_hidden, E_output, W_ho);

			// 가중치 수정 ( 은닉층 -> 출력층 )
			RenewW_ho(W_ho, eta, E_output, N_hidden);

			// 가중치 수정 ( 입력층 -> 은닉층 )
			RenewW_ih(W_ih, eta, E_hidden, P_input[i]);
		}
		epoch++;

		// 종료 조건이 되는지 검사 !
		check = CheckEnd(W_ih, W_ih_check, W_ho, W_ho_check);
	}
	end = clock();
	runtime = (float) (end - start)/CLOCKS_PER_SEC;

	printf(" epoch : %d, 소요시간 : %.3f 초\n", epoch, runtime);
	printf(" 1 epoch당 소요시간 : %.7f 초\n\n", runtime/epoch);
	system("pause");

	// 최종 출력을 위한 배열
	int S_input[n2_output][n_input] =
	{
// 5x5
#if 1
					// A
		{
			0, 1, 1, 0, 0,
			1, 0, 0, 0, 1,
			1, 1, 1, 1, 1,
			1, 0, 0, 0, 1,
			1, 0, 0, 0, 1
		},

		// B
		{
			1, 1, 1, 1, 0,
			1, 0, 0, 0, 1,
			1, 1, 1, 1, 0,
			1, 0, 0, 0, 1,
			1, 1, 1, 1, 0
		},

		// C
		{
			0, 1, 1, 1, 0,
			1, 0, 0, 0, 1,
			1, 0, 0, 0, 0,
			1, 0, 0, 0, 1,
			0, 1, 1, 1, 0
		},

		// D
		{
			1, 1, 1, 1, 0,
			1, 0, 0, 0, 1,
			1, 0, 0, 0, 1,
			1, 0, 0, 0, 1,
			1, 1, 1, 1, 0
		},


		// E
		{
			1, 1, 1, 1, 1,
			1, 0, 0, 0, 0,
			1, 1, 1, 1, 1,
			1, 0, 0, 0, 0,
			1, 1, 1, 1, 1
		},

		// F
		{
			1, 1, 1, 1, 1,
			1, 0, 0, 0, 0,
			1, 1, 1, 1, 1,
			1, 0, 0, 0, 0,
			1, 0, 0, 0, 0
		},

		// G
		{
			0, 1, 1, 1, 0,
			1, 0, 0, 0, 0,
			1, 0, 1, 1, 0,
			0, 0, 0, 0, 1,
			0, 1, 1, 1, 0
		},

		// H
		{
			1, 0, 0, 0, 1,
			1, 0, 0, 0, 1,
			1, 1, 1, 1, 1,
			1, 0, 0, 0, 1,
			1, 0, 0, 0, 1
		},

#if 0
			// I
			{
				1, 1, 0, 1, 1,
				0, 0, 1, 0, 0,
				0, 0, 1, 0, 0,
				0, 0, 1, 0, 0,
				1, 1, 1, 1, 1,
			},
#endif

#if 0
			// J
			{
				1, 1, 0, 1, 1,
				0, 0, 1, 0, 0,
				0, 0, 1, 0, 0,
				1, 0, 1, 0, 0,
				0, 1, 0, 0, 0,
			},
#endif

#if 0
			// K
			{
				1, 0, 0, 0, 1,
				1, 0, 0, 1, 0,
				0, 1, 1, 0, 0,
				1, 0, 0, 1, 0,
				1, 0, 0, 0, 1,
			},
#endif
#endif
		/*
			// [1] ㄱ 과 흡사한 패턴
		{
			 1,	 1,	 1,	 0,  1,	 1,	 1,
			 1,	 0,	 0,	 0,	 0,	 1,  0,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1
		},

		// [2] ㄱ 과 흡사한 패턴
		{
			 1,	 1,	 0,	 1,  1,	 1,	 1,
			 1,	 1,	 1,	 0,	 1,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1
		},

		// [3] ㄴ 과 흡사한 패턴
		{
			 1,	 0,	 0,	 0,  0,	 0,	 0,
			 1,	 0,	 1,	 0,	 0,	 0,  0,
			 1,	 0,	 0,	 0,	 0,	 0,  0,
			 1,	 0,	 0,	 1,	 0,	 1,  0,
			 1,	 1,	 1,	 1,	 1,	 0,  1
		},

		// [4] ㄴ 과 흡사한 패턴
		{
			 0,	 1,	 0,	 0,  0,  0,	 0,
			 1,	 0,	 0,	 0,	 0,	 0,  1,
			 1,	 1,	 0,	 0,	 0,	 0,  0,
			 1,	 0,	 1,	 1,	 0,	 0,  1,
			 1,	 1,	 1,	 1,	 1,	 1,  1
		},

		// [5] ㄹ 과 흡사한 패턴
		{
			 1,	 1,	 0,	 1,  1,	 1,	 1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 1,	 1,	 0,	 1,	 1,	 1,  1,
			 1,	 1,  0,	 0,	 0,	 0,  0,
			 1,	 1,	 1,	 1,	 1,	 0,  1
		},

		// [6] ㄹ 과 흡사한 패턴
		{
			 1,	 1,	 0,	 1,  1,	 1,	 0,
			 1,	 0,	 0,	 0,	 0,	 1,  1,
			 0,	 0,	 1,	 1,	 1,	 1,  0,
			 0,	 1,	 0,	 0,	 0,	 0,  0,
			 0,	 0,	 1,	 1,	 1,	 1,  1
		},

		// [7] ㅋ 과 흡사한 패턴
		{
			 1,	 1,	 1,	 1,  1,	 1,	 1,
			 0,	 0,	 0,	 0,	 1,	 0,  1,
			 1,	 1,	 1,	 1,	 1,	 1,  1,
			 1,	 1,	 0,	 1,	 0,	 1,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1
		},

		// [8] ㅋ 과 흡사한 패턴
		{
			 1,	 1,	 1,	 1,  0,	 1,	 0,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 1,	 1,	 0,	 1,	 1,	 1,  0,
			 0,	 1,	 1,	 0,	 0,	 1,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1
		},

		// [9] ㅌ과 유사한 패턴
		 {
				 0,  1,  1,  1,  1,  1, 1,
				 1,  1,  1,  0,  0,  0, 0,
				 1,  1,  1,  1,  1,  1, 1,
				 1,  1,  1,  1,  0,  0, 0,
				 1,  1,  1,  1,  1,  1, 1
		 },

		// [10] ㅌ과 유사한 패턴
		{
			 0,	 1,	 1,	 1,  1,	 1,	 0,
			 1,	 0,	 0,	 0,	 0,	 0,  0,
			 1,	 1,	 1,	 1,	 1,	 1,  1,
			 1,	 0,	 0,	 0,	 0,	 0,  0,
			 1,	 1,	 0,	 1,	 1,	 0,  1
		}
		*/
		
// 6x6
#if 0
		//A
		{
			0, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 1,
			0, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1
		},

		//B
		{
			1, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 1,
			0, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 0
		},

		//C
		{
			0, 1, 0, 1, 1, 1,
			1, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1
		},

		//D
		{
			1, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 1, 1, 1, 0
		},

		//E
		{
			1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0,
			1, 1, 0, 1, 1, 1,
			1, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 1, 1
		},

		//F
		{
			1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 0, 1,
			1, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0,
		},

		//G
		{
			0, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0,
			1, 0, 1, 0, 1, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			0, 1, 1, 1, 1, 0,
		},

		//H
		{
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			1, 1, 1, 0, 1, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 1
		},
#endif

// 7x7
#if 0
			// A
		{
			0, 1, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			0, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
		},

			// B
		{
			1, 1, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			0, 1, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 1, 0,
		},

		// C
		{
			0, 1, 1, 0, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1,
		},

		// D
		{
			1, 1, 1, 1, 1, 1, 0,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 1, 0, 1, 1, 1, 0,
		},

		// E
		{
			1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 1, 0, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 1, 1, 1,
		},

		// F
		{
			1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			0, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
		},
		// G
		{
			0, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 0, 0, 0, 0, 0,
			1, 0, 1, 1, 0, 1, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			0, 1, 1, 1, 1, 1, 0,
		},
		// H
		{
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 0, 1, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1,
		},
#endif
	};

	int MaxValueIndex = 0;
	//출력을 위한 함수
	for (int i = 0; i < n2_output; i++)
		{
			PrintNodeH(S_input[i], W_ih, N_hidden, check);
			PrintNodeO(N_hidden, W_ho, N_output, check);

			printf("\n\t\t\t\t\t [ %d번째 output ] !!\n\n", i+1);
//			printf("\n\t\t\t\t [ %d번째 output ] !!\n\n", i+1);
			for (int j = 0; j < n_output; j++)
			{
				if (N_output[MaxValueIndex] < N_output[j]) MaxValueIndex = j;
				printf("%11.6f", N_output[j]);
			}
			printf("\n");

			for (int j = 0; j < n_output; j++)
			{
				N_output[j] = (int)(N_output[j] / N_output[MaxValueIndex]);
				printf("%11.f", N_output[j]);		// 최종 아웃풋 !
			}
			printf("\n");
			
			int k = 0;
			printf("\n\t\t\t\t  INPUT \t\t  OUTPUT\n");
			for (int j = 0; j < lines; j++)
			{
				printf("\t\t\t\t");
				k = j * (n_input / lines);
				do
				{
					if (S_input[i][k] == 0)
						printf("□");
					else
						printf("■");
					k++;
				} while (k % (n_input / lines) != 0);
				printf("\t\t");

				k = j * (n_input / lines);
				do
				{
					if (P_input[MaxValueIndex][k] == 0)
						printf("□");
					else
						printf("■");
					k++;
				} while (k % (n_input / lines) != 0);
				printf("\n");
			}
			MaxValueIndex = 0;
			printf("\n");
			system("pause");
		}
		
}

// 가중치 초기화 (입력층 ㅡ 은닉층)
void SetW_ih(double W_ih[][n_input])
{
	for (int i = 0; i < n_hidden; i++)
		for (int j = 0; j < n_input; j++)
			W_ih[i][j] = GaussianRandom()/sqrt(n_input);

	// 출력용 코드
	for (int i = 0; i < n_hidden; i++)
	{
		int j = 0;
		do
		{
			printf("%10f", W_ih[i][j]);
			j++;
			if (j % 10 == 0) printf("\n");
		} while (j < n_input);
		printf("\n");
	}
	printf("\n SetW_ih 완료 : 입력층 노드 %d * 은닉층 노드 %d = %d 개 초기화 !\n\n", n_input, n_hidden, n_input * n_hidden);
}

// 가중치 초기화 (은닉층 - 출력층)
void SetW_ho(double W_ho[][n_hidden])
{
	for (int i = 0; i < n_output; i++)
		for (int j = 0; j < n_hidden; j++)
			W_ho[i][j] = GaussianRandom()/sqrt(n_input);
	
	// 출력용 코드
	for (int i = 0; i < n_output; i++)
	{
		for (int j = 0; j < n_hidden; j++)
		{
			printf("  %f", W_ho[i][j]);
		}
		printf("\n");
	}
	printf("\n SetW_ho 완료 : 은닉층 노드 %d * 출력층 노드 %d = %d 개 초기화 !\n\n", n_hidden, n_output, n_hidden * n_output);
}

// 입력층 -> 은닉층 노드 출력
void PrintNodeH(int N_input[], double W_ih[][n_input], double N_hidden[], int check)
{
	if (check)										// 만약 check가 0이 아니면 ( 루프가 끝났으면 )
		for (int i = 0; i < n_hidden; i++)
			N_hidden[i] = 0;						// 은닉층 노드들을 0으로 초기화

	for (int i = 0; i < n_hidden; i++)
	{
		for (int j = 0; j < n_input; j++)
			N_hidden[i] += N_input[j] * W_ih[i][j];

		N_hidden[i] = 1 / (1 + exp(-N_hidden[i]));		// 시그모이드
		if (check)
		{
//			printf("%d 번째 hidden node : %lf\n", i, N_hidden[i]);
//			printf("\n\n");
		}
	}
}

// 은닉층 -> 출력층 노드 출력
void PrintNodeO(double N_hidden[], double W_ho[][n_hidden], double N_output[], int check)
{
	if (check)										// 만약 check가 0이 아니면 ( 루프가 끝났으면 )
		for(int i = 0; i < n_output; i++)			
			N_output[i] = 0;						// 출력층 노드들을 0으로 초기화

	for (int i = 0; i < n_output; i++)
	{
		for (int j = 0; j < n_hidden; j++)
		{
			N_output[i] += N_hidden[j] * W_ho[i][j];
		}
		N_output[i] = 1 / (1 + exp(-N_output[i]));		// 시그모이드
//		if (N_hidden[i] < 0) N_hidden[i] = 0;			// ReLu
		if (check)
		{
//			printf("%d 번째 outnode : %f\n", i, N_output[i]);
//			printf("\n");
		}
	}
}

// 출력층 오차 계산
void CalOutError(double E_output[], double N_output[], int n)
{
	int check = 0;
	for (int i = 0; i < n_output; i++)
	{
		if (n == i) check = 1;				// 출력층 오류 계산의
		else check = 0;						// (d_j - y_j)를 위한 코드
		E_output[i] = N_output[i] * (1 - N_output[i]) * (check - N_output[i]);
	}
}

// 은닉층 오차 계산
void CalHiddenError(double E_hidden[], double N_hidden[], double E_output[], double W_ho[][n_hidden])
{
	double ErrorSum = 0;
	for (int i = 0; i < n_hidden; i++)
	{
		ErrorSum = 0;
		for (int j = 0; j < n_output; j++)
			ErrorSum += E_output[j] * W_ho[j][i];

		E_hidden[i] = N_hidden[i] * (1 - N_hidden[i]) * ErrorSum;
	}
}

// 가중치 수정 (은닉층 -> 출력층)
void RenewW_ho(double W_ho[][n_hidden], double eta, double E_output[], double N_hidden[])
{
	for (int i = 0; i < n_hidden; i++)
		for (int j = 0; j < n_output; j++)
			W_ho[j][i] += eta * E_output[j] * N_hidden[i];
}

// 가중치 수정 (입력층 -> 은닉층)
void RenewW_ih(double W_ih[][n_input], double eta, double E_hidden[], int N_input[])
{
	for (int i = 0; i < n_input; i++)
		for (int j = 0; j < n_hidden; j++)
			W_ih[j][i] += eta * E_hidden[j] * N_input[i];
}

// 루프 종료 체크
int CheckEnd(double W_ih[][n_input], double W_ih_check[][n_input], double W_ho[][n_hidden], double W_ho_check[][n_hidden])
{
	double CheckVal = 0;
	for (int i = 0; i < n_hidden; i++)
	{
		for (int j = 0; j < n_input; j++)
		{
			CheckVal = W_ih[i][j] - W_ih_check[i][j];
			if ( CheckVal > margin_of_error || CheckVal < (- margin_of_error))
			{
				CopyW(W_ih, W_ih_check, W_ho, W_ho_check);
				return 0;
			}
		}
	}
	for (int i = 0; i < n_output; i++)
	{
		for (int j = 0; j < n_hidden; j++)
		{
			CheckVal = W_ho[i][j] - W_ho_check[i][j];
			if (CheckVal > margin_of_error || CheckVal < (-margin_of_error))
			{
				CopyW(W_ih, W_ih_check, W_ho, W_ho_check);
				return 0;
			}
		}
	}
	return 1;
}

// 가중치 복사
void CopyW(double W_ih[][n_input], double W_ih_check[][n_input], double W_ho[][n_hidden], double W_ho_check[][n_hidden])
{
	for (int i = 0; i < n_hidden; i++)
		for (int j = 0; j < n_input; j++)
			W_ih_check[i][j] = W_ih[i][j];		// 입력층 - 은닉층 가중치 복사

	for (int i = 0; i < n_output; i++)
		for (int j = 0; j < n_hidden; j++)
			W_ho_check[i][j] = W_ho[i][j];		// 은닉층 - 출력층 가중치 복사
}

// 가우시안 표준정규분포 함수, 가중치 초기화에 사용 !!
double GaussianRandom(void)
{
	double value_1, value_2, s;

	do {
		value_1 = 2 * ((double)rand() / RAND_MAX) - 1;      // -1.0 ~ 1.0 까지의 값
		value_2 = 2 * ((double)rand() / RAND_MAX) - 1;      // -1.0 ~ 1.0 까지의 값
		s = (value_1 * value_1) + (value_2 * value_2);
	} while (s >= 1 || s == 0);

	s = sqrt((-2 * log(s)) / s);

	return value_1 * s;
}

// 입력패턴 출력을 위한 함수,,
void PrintInputPattern(int P_input[][n_input])
{
	printf("\n");
	for (int i = 0; i < lines; i++)
	{
		printf("  ");
		for (int j = 0; j < n_output; j++)
		{
			printf(" ");
			int k = i * (n_input / lines);
			do
			{
				if (P_input[j][k] == 1)
					printf("■");
				else
					printf("□");
				k++;
			} while (k % (n_input / lines) != 0);
			printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}
#endif