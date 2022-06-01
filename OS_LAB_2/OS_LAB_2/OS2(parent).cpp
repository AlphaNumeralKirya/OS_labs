#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <tchar.h>
#include <stdio.h>

using namespace std;


int main() {
	int size;
	cout << "Input size of array: ";
	cin >> size;

	unsigned long* array = new unsigned long[size];
	for (int i = 0; i < size; i++) {
		cin >> array[i];
	}

	unsigned int x;
	cout << "Input X: ";
	cin >> x;

	string cmd_args;
	cmd_args += to_string(size);
	cmd_args += " ";
	for (int i = 0; i < size; i++) {
		cmd_args += to_string(array[i]);
		if (i != size - 1) {
			cmd_args += " ";
		}
	}

	wstring stemp = wstring(cmd_args.begin(), cmd_args.end());

	LPCTSTR lpszAppName = "child.exe";
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USEPOSITION;
	si.dwX = x;

	if (!CreateProcess(lpszAppName, (LPSTR)&stemp[0], NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
	{
		cout << "New process is not created" << endl;
		return 0;
	}

	cout << "New process is created" << endl;
	WaitForSingleObject(piApp.hProcess, INFINITE);

	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	delete[]array;
	return 0;
}