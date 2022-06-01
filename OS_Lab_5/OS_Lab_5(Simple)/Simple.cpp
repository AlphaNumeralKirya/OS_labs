#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	HANDLE writePipe = (HANDLE)atoi(argv[1]);
	HANDLE readPipe = (HANDLE)atoi(argv[2]);
	HANDLE R = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"R");
	HANDLE SR = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"SR");
	cout << "В ожидание получения размерности массива" << endl;
	WaitForSingleObject(R, INFINITE);
	int n;
	DWORD dwBytesRead;
	if (!ReadFile(readPipe, &n, sizeof(n), &dwBytesRead, NULL))
	{
		cout << "Read from the pipe failed.\n";
		cout << "Press any key to finish.\n";
		_getch();
		return 0;
	}
	system("cls");
	cout << "Simple получил размер массива: " << n << endl;
	_int32* mas = new _int32[n];
	cout << "Сгенерированный массив" << endl;
	srand(time(0));
	for (int i = 0; i < n; i++) {
		mas[i] = -100 + rand() % 200;
		cout << mas[i] << " ";
	}
	bool key;
	bool keyl;
	int rs = 0;
	SetEvent(SR);
	for (int i = 0; i < n; i++)
	{
		key = false;
		keyl = false;
		if (mas[i] < 0) {
			mas[i] = mas[i] * (-1);
			keyl = true;
		}
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
			if (keyl == true) {
				mas[i] = mas[i] * (-1);
			}
			cout << mas[i] << endl;
			DWORD dwBytesWritten;
			if (!WriteFile(writePipe, &mas[i], sizeof(mas[i]), &dwBytesWritten, NULL))
			{
				cout << "Write to file failed.\n";
				cout << "Press any key to finish.\n";
				_getch();
				return GetLastError();
			}
			SetEvent(SR);
			WaitForSingleObject(R, INFINITE);
		}
	}
	CloseHandle(writePipe);
	CloseHandle(readPipe);
	cout << "\nВсе найденные числа были отправлены\n";
	cout << "Press any key to exit.\n";
	_getch();
	return 0;
}