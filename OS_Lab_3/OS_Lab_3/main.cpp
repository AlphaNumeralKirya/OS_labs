#include <Windows.h>
#include <iostream>
#include <ctime>
#include <process.h>
using namespace std;

CRITICAL_SECTION cs;
CRITICAL_SECTION cs2;
HANDLE sumElement;
HANDLE hOutEvent;
int k;
int sum;
int ssize;
int* mas;
int ttime;

DWORD WINAPI Work(LPVOID par)
{
	EnterCriticalSection(&cs);
	cout << "Введите временной интервал, требуемый для отдых" << endl;
	cin >> ttime;
	bool key = false;
	int rs = 0;
	bool r = true;
	for (int i = 0; i < ssize; i++)
	{
		Sleep(ttime);
		key = false;
		for (int j = 2; j <= sqrt((double)mas[i]); j++)
		{
			if (mas[i] % j == 0 && mas[i] != j)
			{
				key = true;
				break;
			}
		}
		if (mas[i] == 1)
			key = true;
		if (key == false) {
			if (rs != i)
			{
				int t = mas[rs];
				mas[rs] = mas[i];
				mas[i] = t;
				rs++;
			}
			else
				rs++;
		}
		if (rs == k && r) {
			r = false;
			LeaveCriticalSection(&cs);
			Sleep(100);
			EnterCriticalSection(&cs);
		}
	}
	LeaveCriticalSection(&cs);
	return 0;
}

DWORD WINAPI SumElement(LPVOID par) {
	EnterCriticalSection(&cs2);
	WaitForSingleObject(hOutEvent, INFINITE);
	for (int i = 0; i < k; i++) {
		Sleep(ttime);
		sum += mas[i];
	}
	LeaveCriticalSection(&cs2);
	return 0;
}

int main()
{
	HANDLE work, sumElement;
	DWORD IDwork, IDs;
	HANDLE hOutEvent;
	setlocale(LC_ALL, "RUS");
	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&cs2);
	cout << "Введите размер массива" << endl;
	cin >> ssize;
	mas = new int[ssize];
	cout << "Введите массив размерности " << ssize << endl;
	for (int i = 0; i < ssize; i++) {
		cin >> mas[i];
	}
	cout << "Исходный массив размерности " << ssize << endl;
	for (int i = 0; i < ssize; i++) {
		cout << mas[i] << " ";
	}


	work = CreateThread(NULL, 0, Work, NULL, CREATE_SUSPENDED, &IDwork);
	if (work == NULL) return GetLastError();
	hOutEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hOutEvent == NULL)
		return GetLastError();
	sumElement = CreateThread(NULL, 0, SumElement, NULL, CREATE_SUSPENDED, &IDs);
	if (sumElement == NULL) return GetLastError();
	cout << endl;
	cout << "Bведите k" << endl;
	cin >> k;
	ResumeThread(work);
	Sleep(5);
	EnterCriticalSection(&cs);
	ResumeThread(sumElement);
	SetEvent(hOutEvent);
	cout << "Промежуточный вывод " << endl;
	for (int i = 0; i < ssize; i++) {
		cout << mas[i] << " ";
	}
	cout << endl;
	LeaveCriticalSection(&cs);
	EnterCriticalSection(&cs2);
	cout << "результат работы потока SumElement: sum = " << sum << endl;
	LeaveCriticalSection(&cs2);
	DeleteCriticalSection(&cs2);
	EnterCriticalSection(&cs);
	cout << "Итоговый массив" << endl;
	for (int i = 0; i < ssize; i++) {
		cout << mas[i] << " ";
	}
	cout << endl;
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
	return 0;
}