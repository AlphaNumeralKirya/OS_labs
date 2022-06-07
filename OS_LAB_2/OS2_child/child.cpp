#include <windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

bool isPrime(long num) {
	for (int i = 2; i < num / 2; i++)
		if (num % i == 0)
			return false;
	if (num == 1)
		return false;
	return true;
}

int main(int argc, char* argv[])
{
	int i;

	cout << "I am created" << endl;
	int size = atoi(argv[1]);
	cout << "Size of array is " << size << endl;

	long* massive = new long[size];
	int counter = 0;
	for (i = 2; i <argc; i++) {
		massive[counter] = atol(argv[i]);
		counter += 1;
	}
	for (int i = 0; i < size; i++) {
		cout << massive[i] << " ";
	}

	int swapCounter = 0;
	for (int i = 0; i < size; i++) {
		if (isPrime(massive[i])) {
			long temp = massive[i];
			massive[i] = massive[swapCounter];
			massive[swapCounter] = temp;
			swapCounter += 1;
		}
	}

	for (int i = 0; i < size; i++) {
		cout << massive[i] << " ";
	}
	cout << endl;

	system("pause");
	delete[]massive;
	return 0;
}