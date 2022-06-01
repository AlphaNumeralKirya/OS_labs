#include <windows.h>
#include <process.h>
#include <iostream>

using namespace std;

struct Data {
	int size;
	int* mas;
	int x;
};

UINT WINAPI worker(LPVOID par)
{
	Sleep(200);
	Data* param = (Data*)par;
	int n = param->size;
	int* mas = param->mas;
	int x = param->x;
	int k = 0;
	for (int i = 0; i < n; i++) {
		if (mas[i] > x) {
			k++;
		}
	}
	cout << "Количество элементов больших X = " << k << endl;
	return 0;
}

DWORD WINAPI Count()
{
	int* mass = new int[20];
	mass[0] = 0;
	cout << "Числа Фибоначчи" << endl;
	cout << mass[0] << " ";
	mass[1] = 1;
	cout << mass[1] << " ";
	for (int i = 2; i < 20; i++)
	{
		mass[i] = mass[i - 1] + mass[i - 2];
		cout << mass[i] << " ";
	}
	cout << "\n";

	return 0;
}

DWORD WINAPI threadMain(LPVOID par)
{
	int n;
	int x;
	int* mas;
	HANDLE hThread2;
	HANDLE hThread1;
	DWORD IDThread1;
	UINT IDThread2;
	n = (int)par;
	mas = new int[n];
	cout << "Сгенерированный массив" << endl;
	srand(time(0));
	for (int i = 0; i < n; i++) {
		mas[i] = -100 + rand() % 200;
		cout << mas[i] << " ";
	}
	cout << endl;
	cout << "Введите элемент Х" << endl;
	cin >> x;
	Data m;
	m.size = n;
	m.mas = mas;
	m.x = x;
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, worker, (void*)&m, 0, &IDThread2);
	hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Count, 0, CREATE_SUSPENDED, &IDThread1);
	SuspendThread(worker);
	ResumeThread(worker);
	WaitForSingleObject(hThread2, INFINITE);
	cout << endl;
	ResumeThread(hThread1);
	WaitForSingleObject(hThread1, INFINITE);
	CloseHandle(hThread2);
	CloseHandle(hThread1);



	return 0;
}


int main()
{
	setlocale(LC_ALL, "rus");
	HANDLE hThread;
	DWORD IDThread;
	int n;
	cout << "Введите размерность массива" << endl;
	cin >> n;
	hThread = CreateThread(NULL, 0, threadMain, (void*)n, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}