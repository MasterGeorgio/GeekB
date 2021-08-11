// Special Chalange2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Crypto.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // для exit()

#include <random>
#include <ctime>

// Отладка
//#define DBG_TEST


using namespace std;

#define SUCCESS 0

enum eTypeArray
{
	eKey,
	eSubmit,
	ePermut
};


//-------------------------------------------------------------
/// Тестирование
void TestCrypto();

//-------------------------------------------------------------
// Сгенерировать ключ
// Key generated
void DataGen(string &strData, eTypeArray eType);
//-------------------------------------------------------------
// Генерация таблицы без повторяющихся элементов
// Необходима для S и P
// Need that to generating table S and P
void TabGen(
	uint8_t      *pData,
	const size_t dwSize,
	uint32_t     MaxValue);

//-------------------------------------------------------------
/// Function input, get and check the key
int InputKey(string &strKey);
//-------------------------------------------------------------
/// Read data [strInput] from file [strPath]
int InputFile(string &strPath,
	          string &strInput);
//-------------------------------------------------------------
/// Write data [strOutput] into the path [strPath]
void OutputData(
	string strPath,
	string strOutput);
//-------------------------------------------------------------
/// The function cypher/decypher data [strInput] from of path [strOutput],
/// used the key [strKey] and register the type operation in name file [strPath]
int funcCrypto(
	string strKey,
	string &strPath,
	string strInput,
	string &strOutput);

//-------------------------------------------------------------
/// Получение параметров
/// Get paramets [string]
void GetInfo(
	string strInfo,
	string &strData)
{
	cout << strInfo;
	cin  >> strData;
}

//-------------------------------------------------------------
/// Получение параметров
/// Get paramets [int]
void GetInfo(
	string strInfo,
	int &nNum)
{
	cout << strInfo;
	cin >> nNum;
}


//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
int main()
{
	setlocale(LC_ALL, "Russian");

	// Тестирование
	// Check crypto lib
	TestCrypto();

	while (1)
	{
		cout << "\n_____________________________________________________\n";

		// Ключ
		// Кey
		string strKey;

		// Get Key
		// 0123456789ABCDEF
		if (InputKey(strKey) != SUCCESS)
			continue;

		// Ввод пути к файлу
		// Enter path file
		string strPath;
		// Read file contain
		string strInput;
		// Get Key
		if (InputFile(strPath, strInput) != SUCCESS)
			continue;

		// Выходные данные
		// Input data
		// Note: the size input data will equal the size output data
		//uint8_t *pucDataOut = new uint8_t[strInput.length()];
		string strOutput;
		strOutput.resize(strInput.length());

		/// Encoding/decoding data [strInput] into [strOutput]
		if (funcCrypto(strKey,
			           strPath,
			           strInput,
			           strOutput) != SUCCESS)
			continue;

		/// Write data [strOutput] into the path [strPath]
		OutputData(strPath, strOutput);
	}

    return 0;
}


//-------------------------------------------------------------
// Сгенерировать ключ
// Key generated
void DataGen(string &strData, eTypeArray eType)
{
	uint8_t aucTabPermute[SIZE_TAB_P] = { 0 };
	uint8_t aucTabSubstitute[SIZE_TAB_S] = { 0 };
	uint8_t aucKey[SIZE_KEY] = { 0 };

	// Generate tab P
	TabGen(
		aucTabPermute,
		SIZE_TAB_P,
		SIZE_TAB_P);

	// Generate tab S
	TabGen(
		aucTabSubstitute,
		SIZE_TAB_S,
		SIZE_TAB_S);

	// Generate Key


	std::mt19937 gen(time(0));
	std::uniform_int_distribution<> dist(0x30, 0xFF);

	for (size_t i = 0; i < sizeof(aucKey); i++)
		aucKey[i] = dist(gen);

	// ..
	// Groundwork for further work
	std::uniform_int_distribution<> distRand(0x00, 0xFF);
	for (size_t i = 0; i < sizeof(aucTabSubstitute); i++)
		aucTabSubstitute[i] = distRand(gen);
	for (size_t i = 0; i < sizeof(aucTabSubstitute); i++)
		aucTabSubstitute[i] = distRand(gen);

	if (eType == eTypeArray::eKey)
	{
		strData.clear();
		strData.resize(sizeof(aucKey));
		memcpy((char*)strData.data(), aucKey, sizeof(aucKey));
	}

	// ..
	// Groundwork for further work
	if (eType == eTypeArray::eSubmit)
	{
		strData.resize(sizeof(aucTabSubstitute));
		memcpy((char*)strData.data(), aucTabSubstitute, sizeof(aucTabSubstitute));
	}

	if (eType == eTypeArray::ePermut)
	{
		strData.resize(sizeof(aucTabPermute));
		memcpy((char*)strData.data(), aucTabPermute, sizeof(aucTabPermute));
	}
}

//-------------------------------------------------------------
/// Function input, get and check the key
int InputKey(string &strKey)
{
	int nRes = SUCCESS;

	// Тип ввода ключа
	// Type key input
	int nTypeKeyIn = 0;

#ifdef DBG_TEST
	nTypeKeyIn = 1;
#else
	GetInfo(
		"Need key, select:\n"
		"1. Key enter;\n"
		"2. Key generate;\n"
		"3. Exit.\n",
		nTypeKeyIn);
#endif
	// Type key input
	switch (nTypeKeyIn)
	{
		// Enter key
	case 1:
	{
#ifdef DBG_TEST
		strKey = "0123456789ABCDEF";
#else
		GetInfo(
			"Enter your key (" + to_string(SIZE_KEY) + " characters):\n",
			strKey);
#endif

	}break;
	// Generate key
	case 2:
	{
		DataGen(strKey, eKey);
		cout << "Your key is:\n" << strKey << endl;
	}break;
	// Exit
	case 3:
	{
		exit(0);
	}break;
	// Undefined operation
	default:
	{
		cout << "Warning. Undefined operation.\n";
		nRes = 1;
	}break;
	}


	// Проверка необходимой длины
	// Check must length key
	if ((nRes == SUCCESS ) && (strKey.length() < SIZE_KEY))
	{
		cout << "Warning. Your key less " << SIZE_KEY << " characters!" << endl;
		nRes = 2;
	}
	else
	{
		//system("cls");
	}

	return nRes;
}

//-------------------------------------------------------------
/// Read data [strInput] from file [strPath]
int InputFile(
	string &strPath,
	string &strInput)
{
	int nRes = SUCCESS;

	// Ввод пути к файлу
	// Enter path file
	GetInfo(
		"Enter the full path to your file:\n",
		strPath);
#ifdef DBG_TEST
	strPath = "C:\\Users\\GANSOR-PC\\Desktop\\test.txt";
#endif

	// Открываем файл
	// Open file for read data
	ifstream inf(strPath, ios::binary);
	// Проверка наличия файла
	// Check path file
	if (!inf)
	{
		cout << "Warning.\n" << strPath << "\ncould not be opened for reading!\n";
		nRes = 1;
	}
	else
	{
		cout << "Data red success from the path:\n" << strPath << endl;
	}

	// Считываем содержимиое файла
	// Full read file contain. Existing 0x0D and 0x2A
	inf.seekg(0, std::ios::end);
	size_t size = inf.tellg();
	strInput.resize(size);
	inf.seekg(0);
	inf.read(&strInput[0], size);

	/*/
	// Считываем содержимиое файла
	// Read file contain
	while (inf)
	{
		string strRead;
		getline(inf, strRead);
		strInput.append(strRead);
		//cout << strInput << endl;
	}
	*/

	// Содержимое файла должно быть кратно блоку
	// Check must be divisible for block size (see Crypto.h)
	if (strInput.length() % SIZE_BLOCK != 0)
	{
		for (int i = 0; i < SIZE_BLOCK; i++)
		{
			if (strInput.length() % SIZE_BLOCK != 0)
				strInput += " ";
		}
	}

	inf.close();
	return nRes;
}


//-------------------------------------------------------------
/// Write data [strOutput] into the path [strPath]
void OutputData(
	string strPath,
	string strOutput)
{
	// Открываем файл
	// Open file for write data
	ofstream outf(strPath, ios::binary);
	// Проверка наличия файла
	if (!outf)
	{
		cout << "Warning.\n" << strPath << "\ncould not be opened for writing!\n";
	}
	else
	{
		outf << strOutput;
		cout << "Data saved success into the path:\n" << strPath << endl;
		//for (uint64_t i = 0; i < strInput.length(); i++)
		//	outf << pucDataOut[i];
		//delete []pucDataOut;

		outf.close();
	}
}

//-------------------------------------------------------------
/// The function cypher/decypher data [strInput] from of path [strOutput],
/// used the key [strKey] and register the type operation in name file [strPath]
int funcCrypto(
	string strKey,
	string &strPath,
	string strInput,
	string &strOutput)
{
	// Тип работы
	int nWorkType = 0;
	GetInfo(
		"Selected operation:\n"
		"1. Encode:\n"
		"2. Decode:\n",
		nWorkType);


	/// Подготовка к обработке данных
	// Init crypto algorithm
	Crypto CAlgCrypt;

	int nRes = CAlgCrypt.Init((uint8_t*)strKey.data());

	// Check init alg
	if (nRes != ALG_SUCCESS_SP)
	{
		// ..
		return nRes;
	}

	// Type work
	switch (nWorkType)
	{
		// Cypher file selected
	case 1:
	{
		cout << "Start encode...\n";
		nRes = CAlgCrypt.Encode(
			(uint8_t*)strInput.data(),
			strInput.length(),
			(uint8_t*)strOutput.data());

		if (nRes == ALG_SUCCESS)
			cout << "Sussecs encode.\n";

		cout << "End encode.\n";
		strPath += ".enc";
	}break;
	// Decypher file selected
	case 2:
	{
		cout << "Start decode...\n";
		nRes = CAlgCrypt.Decode(
			(uint8_t*)strInput.data(),
			strInput.length(),
			(uint8_t*)strOutput.data());

		if (nRes == ALG_SUCCESS)
			cout << "Sussecs decode.\n";

		cout << "End decode.\n";
		strPath += ".dec";
	}break;
	// Undefined operation
	default:
	{
		cout << "Warning. Undefined operation.\n";
		nRes = 1;
	}break;
	}

	return nRes;
}


//-------------------------------------------------------------
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

	for (int i = 0; i < sizeof(aucDataIn); i++)
		aucDataIn[i] = i;

	Crypto CAlgCrypt;

	int nRes = CAlgCrypt.Init(pucKey);

	nRes = CAlgCrypt.Test();

	nRes = CAlgCrypt.EncodeAlpha(
		aucDataIn,
		sizeof(aucDataIn),
		aucDataOut);

	nRes = CAlgCrypt.DecodeAlpha(
		aucDataOut,
		sizeof(aucDataOut),
		aucDataOut);
}

// ..
// Groundwork for further work
//-------------------------------------------------------------
// Генерация таблицы без повторяющихся элементов
// Необходима для S и P
// Need that to generating the tables S and P
void TabGen(
	uint8_t *pData,
	const size_t dwSize,
	uint32_t MaxValue)
{
	mt19937 gen(time(0));
	uniform_int_distribution<> dist(0, MaxValue - 1);

	//bool *pTab = new bool[dwSize];

	bool pTab[256];

	for (size_t i = 0; i < dwSize; i++)
		pTab[i] = false;


	for (size_t i = 0; i < dwSize; i++)
	{
		uint8_t ucValue = dist(gen);

		if (pTab[ucValue])
		{
			i--;
		}
		else
		{
			pData[i] = ucValue;
			pTab[ucValue] = true;
		}
	}

	//delete[]pTab;
}
/**/