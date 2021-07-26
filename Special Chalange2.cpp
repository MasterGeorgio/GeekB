// Special Chalange2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Crypto.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // для exit()

// Отладка
//#define DBG_TEST


using namespace std;


/// Тестирование
void TestCrypto()
{
	uint8_t pucKey[SIZE_KEY] = {
		0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
		0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
	};

	uint8_t aucDataIn[SIZE_BLOCK] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};
	uint8_t aucDataOut[SIZE_BLOCK] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};

	Crypto CAlgCrypt;

	int nRes = CAlgCrypt.Init(pucKey);

	nRes = CAlgCrypt.Test();
}

/// Получение параметров
void GetInfo(
	string strInfo,
	string &strData)
{
	cout << strInfo;
	cin  >> strData;
}

/// Получение параметров
void GetInfo(
	string strInfo,
	int &nNum)
{
	cout << strInfo;
	cin >> nNum;
}

int main()
{
	/// Тестирование
	TestCrypto();

	while (1)
	{
		cout << "\n\n_____________________________________________________\n";
		// Ввод ключа
		string strKey;
		GetInfo(
			"Enter your key (" + to_string(SIZE_KEY) + " characters):\n",
			strKey);
#ifdef DBG_TEST
		strKey = "1234567890123456";
#endif

		// Проверка необходимой длины
		if (strKey.length() < SIZE_KEY)
		{
			cout << "Warning. Your key less " << SIZE_KEY << " characters!" << endl;
			continue;
		}

		// Ввод пути к файлу
		string strPath;
		GetInfo(
			"Enter the full path to your file:\n",
			strPath);
#ifdef DBG_TEST
		strPath = "C:\\Users\\GANSOR-PC\\Desktop\\test.txt";
#endif

		// Открываем файл
		ifstream inf(strPath);
		// Проверка наличия файла
		if (!inf)
		{
			cout << "Warning.\n" << strPath << "\ncould not be opened for reading!\n";
			continue;
		}

		// Тип работы
		int nWorkType;
		GetInfo(
			"Selected operation:\n1. Encode:\n2. Decode:\n",
			nWorkType);

		// Считываем содержимиое файла
		string strInput;
		// Считываем
		while (inf)
		{
			string strRead;
			getline(inf, strRead);
			strInput.append(strRead);
			//cout << strInput << endl;
		}

		// Содержимое файла должно быть кратно блоку
		if (strInput.length() % SIZE_BLOCK != 0)

		{
			for (int i = 0; i < SIZE_BLOCK; i++)
			{
				if (strInput.length() % SIZE_BLOCK != 0)
					strInput += " ";
			}
		}


		/// Подготовка к обработке данных
		Crypto CAlgCrypt;

		int nRes = CAlgCrypt.Init((uint8_t*)strKey.data());

		if (nRes != (ALG_SUSSECS_SP))
		{
			// ..
		}

		/// Выходные данные
		//uint8_t *pucDataOut = new uint8_t[strInput.length()];
		string strOutput;
		strOutput.resize(strInput.length());

		switch (nWorkType)
		{
		case 1:
		{
			cout << "Start encode...\n";
			nRes = CAlgCrypt.Encode(
				(uint8_t*)strInput.data(),
				strInput.length(),
				(uint8_t*)strOutput.data());

			if (!nRes)
				cout << "Sussecs encode.\n";

			cout << "End encode.\n";
			strPath += ".enc";
		}break;

		case 2:
		{
			cout << "Start decode...\n";
			nRes = CAlgCrypt.Decode(
				(uint8_t*)strInput.data(),
				strInput.length(),
				(uint8_t*)strOutput.data());

			if (!nRes)
				cout << "Sussecs decode.\n";

			cout << "End decode.\n";
			strPath += ".dec";
		}break;
		default:
		{
			cout << "Warning. Undefined operation.\n";
			continue;
		}break;
		}

		// Открываем файл
		ofstream outf(strPath);
		// Проверка наличия файла
		if (!outf)
		{
			cout << "Warning.\n" << strPath << "\ncould not be opened for writing!\n";
			continue;
		}
		
		outf << strOutput;
		//for (uint64_t i = 0; i < strInput.length(); i++)
		//	outf << pucDataOut[i];
		//delete []pucDataOut;

		outf.close();
		inf.close();
	}

    return 0;
}
