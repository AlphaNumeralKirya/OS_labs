#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	TCHAR lpszComLine[80];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hWritePipe, hReadPipe;
	HANDLE R = CreateEvent(NULL, FALSE, FALSE, L"R");
	HANDLE SR = CreateEvent(NULL, FALSE, FALSE, L"SR");
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
	{
		cout << "Create pipe failed.\n";
		cout << "Press any key to finish.\n";
		_getch();
		return GetLastError();
	}
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	wsprintf(lpszComLine, L"Simple.exe %d %d", (int)hWritePipe, (int)hReadPipe);
	if (!CreateProcess(NULL, lpszComLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		cout << "Create process failed.\n";
		cout << "Press any key to finish.\n";
		_getch();
		return GetLastError();
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	int n;
	cout << "Введите размер массива" << endl;
	cin >> n;
	DWORD dwBytesWritten;
	if (!WriteFile(hWritePipe, &n, sizeof(n), &dwBytesWritten, NULL))
	{
		cout << "Write to file failed.\n";
		cout << "Press any key to finish.\n";
		_getch();
		return GetLastError();
	}
	cout << "Server отправил размер массива: " << n << endl;
	SetEvent(R);
	int k = 0;
	WaitForSingleObject(SR, INFINITE);
	while (WaitForSingleObject(SR, 1000) == false) {
		k++;
		_int32 nData;
		DWORD dwBytesRead;
		if (!ReadFile(hReadPipe, &nData, sizeof(nData), &dwBytesRead, NULL))
		{
			cout << "Read from the pipe failed.\n";
			cout << "Press any key to finish.\n";
			_getch();
			return GetLastError();
		}
		cout << "Полученное число от Simple " << nData << endl;
		SetEvent(R);
	}
	if (k == 0) {
		cout << "Ничего не было получено" << endl;
	}
	else {
		cout << "Все числа были получены" << endl;
	}
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	cout << "Press any key to exit.\n";
	_getch();
	return 0;
}