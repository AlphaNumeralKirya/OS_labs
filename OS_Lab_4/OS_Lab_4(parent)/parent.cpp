#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	int ms = atoi(argv[1]);
	cout << "Process is waiting" << endl;
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, (LPSTR)"hM");
	HANDLE A = OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPSTR)"A");
	HANDLE B = OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPSTR)"B");
	Sleep(10);
	WaitForSingleObject(hMutex, INFINITE);
	system("cls");
	cout << "Process is working" << endl;
	for (int i = 0; i < ms; i++) {
		cout << "A or B\n";
		char input = ' ';
		while (input != 'A' && input != 'B') {
			cin >> input;
			if (input == 'A') {
				SetEvent(A);
				break;
			}
			if (input == 'B') {
				SetEvent(B);
				break;
			}
			if (input != 'A' && input != 'B')
				cout << "Wrong input" << endl;
		}
	}
	_cputs("\nPress any key to finish.\n");
	_getch();
	ReleaseMutex(hMutex);
	ExitProcess(1);
	return 0;
}