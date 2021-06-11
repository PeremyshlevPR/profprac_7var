#include <iostream>
#include <vector>
#include <cmath>
#include "plant.h"


using namespace std;

void enter_data(vector<int>& M, int& H, int& P, double& Y, size_t& N, vector<double>& A, size_t k)
{
	cerr << "Номера опрашиваемых датчиков: ";
	for (size_t i = 1; i <= k; i++)
	{
		cerr << "M[" << i << "] = "; cin >> M[i];
	}

	cerr << "Номер датчика, измеряющего особо важную величину: "; cin >> H;

	
	cerr << "Номер канала, по которому на объект будет подаваться управляющее воздействие: "; cin >> P;

	
	cerr << "Номинальное значение отклика: "; cin >> Y;

	
	cerr << "Число тактов выработки управляющих воздействий: "; cin >> N;

	
	cerr << "Коэфиценты, определяющие закон управления: ";
	cerr << "A[0] = "; cin >> A[0];
	A[0] = 0;
	for (size_t i = 1; i <= k; i++)
	{
		cerr << "A[" << i << "] = "; cin >> A[i];
	}
}

double calc_of_C(double Yn, vector<double> Y)
{
	double C = 0;
	for (size_t i = 0; i < Y.size(); i++)
	{
		C += pow((Y[i] - Yn), 2);
	}
	return C;
}

int main()
{
	setlocale(LC_ALL, "rus");

	//Инициализация ОУ
	Plant plant;
	plant_init(plant);

    //Шаг 1 ввод исходных данных
	size_t k;
	cerr << "Количество оправшиваемых датчиков: "; cin >> k;
	vector<int> M(k+1);
	int H, P;
	double Yn;
	size_t N;
	vector<double> A(k+1);

    enter_data(M, H, P, Yn, N, A, k);
    
	//шаг 2 опрос каналов
	vector<double> Y(N);

	cout << "  №      ";
	for (size_t j = 1; j <= k; j++)
	{
		cout << "X[" << j << "]\t";
	}

	cout << "  U\t" << "   Y\t (Yi - Yn)^2" << endl;
	for (int i = 0; i < N; i++)
	{

		vector<double> X(k+1);
		for (size_t j = 1; j <= k; j++)
		{
			int in_channel = M[j];
			X[j] = plant_measure(in_channel, plant);
		}

		//шаг 3 расчет величины управляющего воздействия
		double U = A[0];
		for (size_t j = 1; j <= k; j++)
		{
			U += (A[j] * X[j]);
		}

		//шаг 4 применение коэффицента U на канал с номером Р
		int out_channel = P;
		double value = U;
		plant_control(out_channel, value, plant);

		//шаг 5 измерим значение отклика Y на канале Н
		int in_channel = H;
		 Y[i] = plant_measure(in_channel, plant);

		 //шаг 6 вывод результатов измерений и расчетов
		 cout << "  " << i + 1 << "\t";
		 for (int j = 1; j <= k; j++)
		 {
			 cout.precision(2);
			 cout << fixed << X[j] << "\t";
		 }
		 cout.precision(2);
		 cout << fixed << U << "\t " << fixed << Y[i] << "\t    " << fixed << pow((Y[i] - Yn), 2) << endl;
	}

	// Расчет и вывод суммы С
	double C = calc_of_C(Yn, Y);
	cout << "C = " << C << endl;
	cout << "C1 = " << C / N << endl;
}
