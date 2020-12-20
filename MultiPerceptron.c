#if 0
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define n_input	25				// �Է��� ��� ����
#define n_hidden 10				// ������ ��� ����
#define n_output 8				// ����� ��� ����
#define n2_output 8
#define lines 5					// �Լ� ���� ����� ���Ǹ� ���� ���Ƿ� ������ �� �� �Դϴ�
								// ex) 25 ��� = 5 ��� x 5 lines
#define margin_of_error 0.0012	// ���� ���� : 2 x margin_of_error ,,

void SetW_ih(double W_ih[][n_input]);				// ����ġ �ʱ�ȭ
void SetW_ho(double W_ho[][n_hidden]);				// ����ġ �ʱ�ȭ
void PrintNodeH(int N_input[], double W_ih[][n_input], double N_hidden[], int check);					// �Է��� -> ������ ��� ���
void PrintNodeO(double N_hidden[], double W_ho[][n_hidden], double N_output[], int check);				// ������ -> ����� ��� ���
void CalOutError(double E_output[], double N_output[], int n);											// ����� ���� ���
void CalHiddenError(double E_hidden[], double N_hidden[], double E_output[], double W_ho[][n_hidden]);	// ������ ���� ���
void RenewW_ho(double W_ho[][n_hidden], double eta, double E_output[], double N_hidden[]);				// ����ġ ���� (������ -> �����)
void RenewW_ih(double W_ih[][n_input], double eta, double E_hidden[], int N_input[]);					// ����ġ ���� (�Է��� -> ������)
int CheckEnd(double W_ih[][n_input], double W_ih_check[][n_input], double W_ho[][n_hidden], double W_ho_check[][n_hidden]);		// ���� ���� üũ
void CopyW(double W_ih[][n_input],double W_ih_check[][n_input], double W_ho[][n_hidden], double W_ho_check[][n_hidden]);		// ����ġ ���� 

double GaussianRandom(void);						// ����þ� ǥ�����Ժ��� �Լ�, ����ġ �ʱ�ȭ�� ��� !!
void PrintInputPattern(int P_input[][n_input]);		// �Է����� �ܼ� ����� ���� �Լ� (printf)

int main()
{
	// �Է��� (����): A, B, C, D, E, F, G, H
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
		// ��
		{
			 1,	 1,	 1,	 1,  1,	 1,	 1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1
		},

		#if 1
		// ��
		{
			 1,	 0,	 0,	 0,  0,	 0,	 0,
			 1,	 0,	 0,	 0,	 0,	 0,  0,
			 1,	 0,	 0,	 0,	 0,	 0,  0,
			 1,	 0,	 0,	 0,	 0,	 0,  0,
			 1,	 1,	 1,	 1,	 1,	 1,  1
		},
		#endif

		#if 1
		// ��
		{
			 1,	 1,	 1,	 1,  1,	 1,	 1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 1,	 1,	 1,	 1,	 1,	 1,  1,
			 1,	 0,	 0,	 0,	 0,	 0,  0,
			 1,	 1,	 1,	 1,	 1,	 1,  1
		},
		#endif

		#if 1
		// ��
		{
			 1,	 1,	 1,	 1,  1,	 1,	 1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 1,	 1,	 1,	 1,  1,	 1,	 1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1
		},
		#endif

		#if 1
		// ��

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
	
	// �Է����� ��� �ڵ�
	PrintInputPattern(P_input);

	double N_hidden[n_hidden] = { 0 };	// ������ ���
	double N_output[n_output] = { 0 };	// ����� ���

	double W_ih[n_hidden][n_input] = { 0 };		// �Է��� �� ������ ����ġ �迭
	SetW_ih(W_ih);		// �������� ����?		// �ʱ�ȭ

	double W_ho[n_output][n_hidden] = { 0 };	// ������ �� ����� ����ġ �迭
	SetW_ho(W_ho);		// �������� ����?		// �ʱ�ȭ

	double E_hidden[n_hidden] = { 0 };			// ������ ���� �迭
	double E_output[n_output] = { 0 };			// ����� ���� �迭

	double eta = 0.1;				// �н���	// �� : ����ġ ������ ���� ��

	int check = 0;								// ���� ���Ḧ ���� ����
	double W_ih_check[n_hidden][n_input] = { 0 };	// ���� ���Ḧ ���� ����ġ �迭 w(t-1)
	double W_ho_check[n_output][n_hidden] = { 0 };	// ���� ���Ḧ ���� ����ġ �迭 w(t-1)

	int epoch = 0;								// ���� �ݺ� Ƚ��

	srand((unsigned)time(NULL));
	clock_t start, end;
	float runtime;
	start = clock();



	while (!check)
	{
		// �Է������� ��(= ����� ��� ��) ��ŭ �ݺ� !
		for (int i = 0; i < n_output; i++)
		{
			// �Է��� -> ������ ��� ���
			PrintNodeH(P_input[i], W_ih, N_hidden, check);

			// ������ -> ����� ��� ���
			PrintNodeO(N_hidden, W_ho, N_output, check);

			// ����� ���� ���
			CalOutError(E_output, N_output, i);

			// ������ ���� ���
			CalHiddenError(E_hidden, N_hidden, E_output, W_ho);

			// ����ġ ���� ( ������ -> ����� )
			RenewW_ho(W_ho, eta, E_output, N_hidden);

			// ����ġ ���� ( �Է��� -> ������ )
			RenewW_ih(W_ih, eta, E_hidden, P_input[i]);
		}
		epoch++;

		// ���� ������ �Ǵ��� �˻� !
		check = CheckEnd(W_ih, W_ih_check, W_ho, W_ho_check);
	}
	end = clock();
	runtime = (float) (end - start)/CLOCKS_PER_SEC;

	printf(" epoch : %d, �ҿ�ð� : %.3f ��\n", epoch, runtime);
	printf(" 1 epoch�� �ҿ�ð� : %.7f ��\n\n", runtime/epoch);
	system("pause");

	// ���� ����� ���� �迭
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
			// [1] �� �� ����� ����
		{
			 1,	 1,	 1,	 0,  1,	 1,	 1,
			 1,	 0,	 0,	 0,	 0,	 1,  0,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1
		},

		// [2] �� �� ����� ����
		{
			 1,	 1,	 0,	 1,  1,	 1,	 1,
			 1,	 1,	 1,	 0,	 1,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1
		},

		// [3] �� �� ����� ����
		{
			 1,	 0,	 0,	 0,  0,	 0,	 0,
			 1,	 0,	 1,	 0,	 0,	 0,  0,
			 1,	 0,	 0,	 0,	 0,	 0,  0,
			 1,	 0,	 0,	 1,	 0,	 1,  0,
			 1,	 1,	 1,	 1,	 1,	 0,  1
		},

		// [4] �� �� ����� ����
		{
			 0,	 1,	 0,	 0,  0,  0,	 0,
			 1,	 0,	 0,	 0,	 0,	 0,  1,
			 1,	 1,	 0,	 0,	 0,	 0,  0,
			 1,	 0,	 1,	 1,	 0,	 0,  1,
			 1,	 1,	 1,	 1,	 1,	 1,  1
		},

		// [5] �� �� ����� ����
		{
			 1,	 1,	 0,	 1,  1,	 1,	 1,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 1,	 1,	 0,	 1,	 1,	 1,  1,
			 1,	 1,  0,	 0,	 0,	 0,  0,
			 1,	 1,	 1,	 1,	 1,	 0,  1
		},

		// [6] �� �� ����� ����
		{
			 1,	 1,	 0,	 1,  1,	 1,	 0,
			 1,	 0,	 0,	 0,	 0,	 1,  1,
			 0,	 0,	 1,	 1,	 1,	 1,  0,
			 0,	 1,	 0,	 0,	 0,	 0,  0,
			 0,	 0,	 1,	 1,	 1,	 1,  1
		},

		// [7] �� �� ����� ����
		{
			 1,	 1,	 1,	 1,  1,	 1,	 1,
			 0,	 0,	 0,	 0,	 1,	 0,  1,
			 1,	 1,	 1,	 1,	 1,	 1,  1,
			 1,	 1,	 0,	 1,	 0,	 1,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1
		},

		// [8] �� �� ����� ����
		{
			 1,	 1,	 1,	 1,  0,	 1,	 0,
			 0,	 0,	 0,	 0,	 0,	 0,  1,
			 1,	 1,	 0,	 1,	 1,	 1,  0,
			 0,	 1,	 1,	 0,	 0,	 1,  1,
			 0,	 0,	 0,	 0,	 0,	 0,  1
		},

		// [9] ���� ������ ����
		 {
				 0,  1,  1,  1,  1,  1, 1,
				 1,  1,  1,  0,  0,  0, 0,
				 1,  1,  1,  1,  1,  1, 1,
				 1,  1,  1,  1,  0,  0, 0,
				 1,  1,  1,  1,  1,  1, 1
		 },

		// [10] ���� ������ ����
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
	//����� ���� �Լ�
	for (int i = 0; i < n2_output; i++)
		{
			PrintNodeH(S_input[i], W_ih, N_hidden, check);
			PrintNodeO(N_hidden, W_ho, N_output, check);

			printf("\n\t\t\t\t\t [ %d��° output ] !!\n\n", i+1);
//			printf("\n\t\t\t\t [ %d��° output ] !!\n\n", i+1);
			for (int j = 0; j < n_output; j++)
			{
				if (N_output[MaxValueIndex] < N_output[j]) MaxValueIndex = j;
				printf("%11.6f", N_output[j]);
			}
			printf("\n");

			for (int j = 0; j < n_output; j++)
			{
				N_output[j] = (int)(N_output[j] / N_output[MaxValueIndex]);
				printf("%11.f", N_output[j]);		// ���� �ƿ�ǲ !
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
						printf("��");
					else
						printf("��");
					k++;
				} while (k % (n_input / lines) != 0);
				printf("\t\t");

				k = j * (n_input / lines);
				do
				{
					if (P_input[MaxValueIndex][k] == 0)
						printf("��");
					else
						printf("��");
					k++;
				} while (k % (n_input / lines) != 0);
				printf("\n");
			}
			MaxValueIndex = 0;
			printf("\n");
			system("pause");
		}
		
}

// ����ġ �ʱ�ȭ (�Է��� �� ������)
void SetW_ih(double W_ih[][n_input])
{
	for (int i = 0; i < n_hidden; i++)
		for (int j = 0; j < n_input; j++)
			W_ih[i][j] = GaussianRandom()/sqrt(n_input);

	// ��¿� �ڵ�
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
	printf("\n SetW_ih �Ϸ� : �Է��� ��� %d * ������ ��� %d = %d �� �ʱ�ȭ !\n\n", n_input, n_hidden, n_input * n_hidden);
}

// ����ġ �ʱ�ȭ (������ - �����)
void SetW_ho(double W_ho[][n_hidden])
{
	for (int i = 0; i < n_output; i++)
		for (int j = 0; j < n_hidden; j++)
			W_ho[i][j] = GaussianRandom()/sqrt(n_input);
	
	// ��¿� �ڵ�
	for (int i = 0; i < n_output; i++)
	{
		for (int j = 0; j < n_hidden; j++)
		{
			printf("  %f", W_ho[i][j]);
		}
		printf("\n");
	}
	printf("\n SetW_ho �Ϸ� : ������ ��� %d * ����� ��� %d = %d �� �ʱ�ȭ !\n\n", n_hidden, n_output, n_hidden * n_output);
}

// �Է��� -> ������ ��� ���
void PrintNodeH(int N_input[], double W_ih[][n_input], double N_hidden[], int check)
{
	if (check)										// ���� check�� 0�� �ƴϸ� ( ������ �������� )
		for (int i = 0; i < n_hidden; i++)
			N_hidden[i] = 0;						// ������ ������ 0���� �ʱ�ȭ

	for (int i = 0; i < n_hidden; i++)
	{
		for (int j = 0; j < n_input; j++)
			N_hidden[i] += N_input[j] * W_ih[i][j];

		N_hidden[i] = 1 / (1 + exp(-N_hidden[i]));		// �ñ׸��̵�
		if (check)
		{
//			printf("%d ��° hidden node : %lf\n", i, N_hidden[i]);
//			printf("\n\n");
		}
	}
}

// ������ -> ����� ��� ���
void PrintNodeO(double N_hidden[], double W_ho[][n_hidden], double N_output[], int check)
{
	if (check)										// ���� check�� 0�� �ƴϸ� ( ������ �������� )
		for(int i = 0; i < n_output; i++)			
			N_output[i] = 0;						// ����� ������ 0���� �ʱ�ȭ

	for (int i = 0; i < n_output; i++)
	{
		for (int j = 0; j < n_hidden; j++)
		{
			N_output[i] += N_hidden[j] * W_ho[i][j];
		}
		N_output[i] = 1 / (1 + exp(-N_output[i]));		// �ñ׸��̵�
//		if (N_hidden[i] < 0) N_hidden[i] = 0;			// ReLu
		if (check)
		{
//			printf("%d ��° outnode : %f\n", i, N_output[i]);
//			printf("\n");
		}
	}
}

// ����� ���� ���
void CalOutError(double E_output[], double N_output[], int n)
{
	int check = 0;
	for (int i = 0; i < n_output; i++)
	{
		if (n == i) check = 1;				// ����� ���� �����
		else check = 0;						// (d_j - y_j)�� ���� �ڵ�
		E_output[i] = N_output[i] * (1 - N_output[i]) * (check - N_output[i]);
	}
}

// ������ ���� ���
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

// ����ġ ���� (������ -> �����)
void RenewW_ho(double W_ho[][n_hidden], double eta, double E_output[], double N_hidden[])
{
	for (int i = 0; i < n_hidden; i++)
		for (int j = 0; j < n_output; j++)
			W_ho[j][i] += eta * E_output[j] * N_hidden[i];
}

// ����ġ ���� (�Է��� -> ������)
void RenewW_ih(double W_ih[][n_input], double eta, double E_hidden[], int N_input[])
{
	for (int i = 0; i < n_input; i++)
		for (int j = 0; j < n_hidden; j++)
			W_ih[j][i] += eta * E_hidden[j] * N_input[i];
}

// ���� ���� üũ
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

// ����ġ ����
void CopyW(double W_ih[][n_input], double W_ih_check[][n_input], double W_ho[][n_hidden], double W_ho_check[][n_hidden])
{
	for (int i = 0; i < n_hidden; i++)
		for (int j = 0; j < n_input; j++)
			W_ih_check[i][j] = W_ih[i][j];		// �Է��� - ������ ����ġ ����

	for (int i = 0; i < n_output; i++)
		for (int j = 0; j < n_hidden; j++)
			W_ho_check[i][j] = W_ho[i][j];		// ������ - ����� ����ġ ����
}

// ����þ� ǥ�����Ժ��� �Լ�, ����ġ �ʱ�ȭ�� ��� !!
double GaussianRandom(void)
{
	double value_1, value_2, s;

	do {
		value_1 = 2 * ((double)rand() / RAND_MAX) - 1;      // -1.0 ~ 1.0 ������ ��
		value_2 = 2 * ((double)rand() / RAND_MAX) - 1;      // -1.0 ~ 1.0 ������ ��
		s = (value_1 * value_1) + (value_2 * value_2);
	} while (s >= 1 || s == 0);

	s = sqrt((-2 * log(s)) / s);

	return value_1 * s;
}

// �Է����� ����� ���� �Լ�,,
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
					printf("��");
				else
					printf("��");
				k++;
			} while (k % (n_input / lines) != 0);
			printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}
#endif