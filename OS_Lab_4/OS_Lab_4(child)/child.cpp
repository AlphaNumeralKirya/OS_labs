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
	HANDLE semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPSTR)"S");
	HANDLE C = OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPSTR)"C");
	HANDLE D = OpenEvent(EVENT_ALL_ACCESS, FALSE, (LPSTR)"D");
	cout << "Process is waiting" << endl;
	WaitForSingleObject(semaphore, INFINITE);
	system("cls");
	cout << "Process is working" << endl;
	for (int i = 0; i < ms; i++) {
		cout << "C or D\n";
		char input = ' ';
		while (input != 'C' && input != 'D') {
			cin >> input;
			if (input == 'C') {
				SetEvent(C);
				break;
			}
			if (input == 'D') {
				SetEvent(D);
				break;
			}
			if (input != 'C' && input != 'D')
				cout << "Wrong input" << endl;
		}
	}
	_cputs("\nPress any key to finish.\n");
	_getch();
	ReleaseSemaphore(semaphore, 1, NULL);
	ExitProcess(1);
	return 0;
}