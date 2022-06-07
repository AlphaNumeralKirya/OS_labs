#include <iostream>
#include<stdio.h>
#include<string.h>
#include <string>
#include <windows.h>
#include <tchar.h>
#include <conio.h>
#pragma warning(disable : 4996)
using namespace std;
int main()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, (LPSTR)"hM");
	if (hMutex == NULL)
	{
		cout << "Create mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}
	HANDLE A = CreateEvent(NULL, FALSE, FALSE, (LPSTR)"A");
	HANDLE B = CreateEvent(NULL, FALSE, FALSE, (LPSTR)"B");
	HANDLE C = CreateEvent(NULL, FALSE, FALSE, (LPSTR)"C");
	HANDLE D = CreateEvent(NULL, FALSE, FALSE, (LPSTR)"D");
	setlocale(LC_ALL, "rus");
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	int ps, cs, ms;
	cout << "Количество процессоров Parent" << endl;
	cin >> ps;
	cout << "Количество процессоров  Child" << endl;
	cin >> cs;
	cout << "Количество сообщений, принятых от Parent или Child" << endl;
	cin >> ms;
	HANDLE semaphore = CreateSemaphore(NULL, 2, 2, "S");
	PROCESS_INFORMATION* piApp = new PROCESS_INFORMATION[ps + cs];
	HANDLE* piH = new HANDLE[ps + cs];
	int pi = 0;
	string cmd_args1 = "C:\\Users\\kirya\\source\\repos\\OS_Lab_4\\Debug\\OS_Lab_4(parent).exe";
	string cmd_args2 = "C:\\Users\\kirya\\source\\repos\\OS_Lab_4\\Debug\\OS_Lab_4(child).exe";
	cmd_args1 += " ";

	cmd_args1 += to_string(ms);
	cmd_args2 += " ";
	cmd_args2 += to_string(ms);
	char* lpszCommandLine1 = (char*)cmd_args1.c_str();
	char* lpszCommandLine2 = (char*)cmd_args2.c_str();
	//cout<<
	for (int i = 0; i < ps; i++) {
		if (!CreateProcess(NULL, (LPSTR)lpszCommandLine1, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp[pi]))
		{
			_cputs("The new process is not created.\n");
			_cputs("Check a name of the process.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return 0;
		}
		piH[pi] = piApp[pi].hProcess;
		pi++;
	}
	for (int i = 0; i < cs; i++) {
		if (!CreateProcess(NULL, (LPSTR)lpszCommandLine2, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp[pi]))
		{
			_cputs("The new process is not created.\n");
			_cputs("Check a name of the process.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return 0;
		}
		piH[pi] = piApp[pi].hProcess;
		pi++;
	}
	HANDLE* hMas = new HANDLE[4];
	for (int i = 0; i < (ps + cs) * ms; i++) {
		hMas[0] = A;
		hMas[1] = B;
		hMas[2] = C;
		hMas[3] = D;
		DWORD dw = WaitForMultipleObjects(4, hMas, FALSE, INFINITE);
		if (dw == WAIT_OBJECT_0 + 0) {
			cout << "Process Parent: A" << endl;
		}
		if (dw == WAIT_OBJECT_0 + 1) {
			cout << "Process Parent: B" << endl;
		}
		if (dw == WAIT_OBJECT_0 + 2) {
			cout << "Process Child: C" << endl;
		}
		if (dw == WAIT_OBJECT_0 + 3) {
			cout << "Process Child: D" << endl;
		}
	}
	WaitForMultipleObjects(ps + cs, piH, TRUE, INFINITE);
	for (int i = 0; i < ps + cs; i++) {
		CloseHandle(piApp[i].hThread);
		CloseHandle(piApp[i].hProcess);
	}
	CloseHandle(hMutex);
	CloseHandle(semaphore);
	return 0;
}