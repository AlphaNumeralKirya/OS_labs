#include <iostream>
#include<stdio.h>
#include<string.h>
#include <windows.h>
#include <tchar.h>
#include <conio.h>
#pragma warning(disable : 4996)
using namespace std;
int main()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, L"hM");
	if (hMutex == NULL)
	{
		cout << "Create mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}
	HANDLE A = CreateEvent(NULL, FALSE, FALSE, L"A");
	HANDLE B = CreateEvent(NULL, FALSE, FALSE, L"B");
	HANDLE C = CreateEvent(NULL, FALSE, FALSE, L"C");
	HANDLE D = CreateEvent(NULL, FALSE, FALSE, L"D");
	setlocale(LC_ALL, "rus");
	TCHAR lpszCommandLine1[256] = L"parent.exe";
	TCHAR lpszCommandLine2[256] = L"child.exe";
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	int ps, cs, ms;
	cout << "Количество процессоров Parent" << endl;
	cin >> ps;
	cout << "Количество процессоров  Child" << endl;
	cin >> cs;
	cout << "Количество сообщений, принятых от Parent или Child" << endl;
	cin >> ms;
	HANDLE semaphore = CreateSemaphore(NULL, 2, 2, L"S");
	PROCESS_INFORMATION* piApp = new PROCESS_INFORMATION[ps + cs];
	HANDLE* piH = new HANDLE[ps + cs];
	int pi = 0;
	TCHAR buf[300];
	_stprintf(buf, TEXT(" %d"), ms);
	_tcscat(lpszCommandLine1, buf);
	_tcscat(lpszCommandLine2, buf);
	for (int i = 0; i < ps; i++) {
		if (!CreateProcess(NULL, lpszCommandLine1, NULL, NULL, FALSE,
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
		if (!CreateProcess(NULL, lpszCommandLine2, NULL, NULL, FALSE,
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