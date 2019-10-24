#include <iostream>
#include <thread>
#include <ctime>
//#include <mutex>
#include <windows.h>

using namespace std;

//mutex res;

const int sizeX = 10000;
const int sizeY = 10000;
const int numThreads = 4;
int matrix[sizeX][sizeY];

void initMatrix();
int sum_nonparallel();
int sum_parallel(int **matr);
void sum(int **matr, int start, int finish, long& result);
void printMatrix();

int main()
{
	srand((unsigned int)time(nullptr));
	int *matrixPointer = matrix[0];
	initMatrix();
	printMatrix();

	cout << endl << "--------------------" << endl;
	cout << "Sum Non parallel = " << sum_nonparallel() << endl;
	cout << "Sum Parallel = " << sum_parallel(&matrixPointer) << endl;
	cout << endl << "--------------------" << endl;
	cout << "Program has finished. Press any key" << endl;
	cin.ignore();
	return 0;
}

void initMatrix()
{
	for (int i = 0; i < sizeX; i++)
		for (int j = 0; j < sizeY; j++)
			matrix[i][j] = rand() % 10;
}

void printMatrix()
{
	cout << "Matrix : " << endl;
	if ((sizeX > 25) && (sizeY > 25))
	{
		cout << "Matrix is too large" << endl;
		return;
	}
	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeY; j++)
			cout << "\t" << matrix[i][j];
		cout << endl;
	}
}

int sum_nonparallel()
{
	int result = 0;

	for (int i = 0; i < sizeX; i++)
		for (int j = 0; j < sizeY; j++)
			result += matrix[i][j];

	return result;
}

int sum_parallel(int **matr)
{
	thread t[numThreads];
	long s = 0;
	//int s[numThreads];
	int num = (sizeX*sizeY) / numThreads;
	for (int i = 0; i < numThreads; i++)
	{
		if (i == numThreads - 1)
			t[i] = thread(sum, matr, num*i, sizeX*sizeY, ref(s));
		else
			t[i] = thread(sum, matr, num*i, num*(i + 1), ref(s));
	}
	for (int i = 0; i < numThreads; i++)
		t[i].join();
	//int res = 0;
	//for (int i = 0; i < numThreads; i++)
	//	res += s[i];
	return s;
}

void sum(int **matr, int start, int finish, long& result)
{
	//res.lock();
	for (int i = start; i < finish; i++)
		//res += *(*matr + i);
		InterlockedAdd((volatile long *)&result, *(*matr + i));
	//res.unlock();
}