// Special Chalange2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // для exit()

// Отладка
#define DBG_TEST

using namespace std;

/// Получение параметров
void GetInfo(
	string strInfo,
	string &strData)
{
	cout << strInfo;
	getline(cin, strData);
}

/// Получение параметров
void GetInfo(
	string strInfo,
	int &nNum)
{
	cout << strInfo;
	cin >> nNum;
}

#define COLOR_BYTE_BLACK 0x00

#define COLOR_BYTE_WHITE 0xFF


#pragma pack(push, 1)
/// Структура заголовка файла BMP
struct BMP_HEAD_FILE
{
	// Заголовок файла BMP
	/// Head file BMP
	uint16_t m_wFile_head{ 0x4d42 };
	/// Size of the file, bytes
	uint32_t m_dwFile_size{ 0 };
	/// Field of the reserved, 0
	uint16_t m_wReserved1{ 0 };
	/// Field of the reserved, 0
	uint16_t m_wReserved2{ 0 };
	/// Start position of the data
	uint32_t m_dwOffset{ 0 };
};

//Структура заголовка изображения BitMapInfoHeader
struct BMP_HEAD_INFO
{
	/// Size of this header
	uint32_t m_dwSize{ 0 };
	/// Width of bitmap in pixeles
	int32_t  m_nWidth{ 0 };
	int32_t  m_nHeight{ 0 };
	uint16_t m_wPlanes{ 1 };
	/// Число бит на пиксель 24 or 32 bit
	uint16_t m_wCount{ 0 };
	uint32_t m_dwComprWes{ 0 };
	uint32_t m_dwSizeImage{ 0 };
	int32_t m_nXmetr{ 0 };
	int32_t m_nYmetr{ 0 };
	/// Count color using
	uint32_t m_dwUsed{ 0 };
	/// Count color important
	uint32_t m_dwImportant{ 0 };
};

struct BMP_HEAD_COLOR
{
	uint8_t m_ucBlue { 0 };
	uint8_t m_ucGreen{ 0 };
	uint8_t m_ucRed  { 0 };
	uint8_t m_ucAlpha{ 0 };
};
#pragma pack(pop)

//---------------------------------
/// To read bmp image  and write his in the console, if this black-white image
void BMP_Read(string strPath);

/**/
//---------------------------------
int main()
{
	// Testing


	while (1)
	{
		cout << "\n_____________________________________________________\n";

		// Ввод пути к файлу
		// Enter path file
		string strPath;
#ifdef DBG_TEST
		GetInfo(
			"Enter the full path to your file:\n",
			strPath);
#else
		
		//strPath = "C:\\Users\\GANSOR-PC\\Desktop\\test.txt";
		strPath = "D:\\Projects\\GeekBrains\\Learn Cpp\\SpecChallenge3\\Swim.bmp";
#endif

		BMP_Read(strPath);
	}

	system("pause");
	return 0;
}

//---------------------------------
/// To read bmp image  and write his in the console, if this black-white image
void BMP_Read(string strPath)
{
	// Открываем файл
	// Open file for read data
	ifstream inf(strPath);
	// Проверка наличия файла
	// Check path file
	if (!inf)
	{
		cout << "Warning.\n" << strPath << "\ncould not be opened for reading!\n";
		//continue;
	}

	// Считываем содержимиое файла
	// Read file contain
	string strInput;
	string strOutput;

	BMP_HEAD_FILE bmpHead_file;
	BMP_HEAD_INFO bmpHead_info;

	inf.read((char*)&bmpHead_file, sizeof(bmpHead_file));

	inf.seekg(sizeof(bmpHead_file), inf.beg);

	inf.read((char*)&bmpHead_info, sizeof(bmpHead_info));

	// Read data bmp
	// default: the heigth is negative value
	inf.seekg(sizeof(bmpHead_file) + sizeof(bmpHead_info), inf.beg);

	/// Define bit count 24 or 32
	int nBitCount = bmpHead_info.m_wCount >> 3;

	// Считываем
	int nCount = 0;
	int nOffset = 1;
	inf.seekg(bmpHead_file.m_dwFile_size - bmpHead_info.m_nWidth * nBitCount, inf.beg);

	/// Image size
	int64_t dwImageSize = bmpHead_info.m_nWidth * bmpHead_info.m_nHeight;
	if (bmpHead_info.m_nHeight < 0)
		dwImageSize *= -1;

	/// Image data
	BMP_HEAD_COLOR colorData;
	while (dwImageSize)
	{
		inf.read((char*)&colorData, nBitCount);

		if ((colorData.m_ucBlue == COLOR_BYTE_BLACK) &&
			(colorData.m_ucGreen == COLOR_BYTE_BLACK) &&
			(colorData.m_ucRed == COLOR_BYTE_BLACK))
			cout << "#";
		else
			cout << " ";

		nCount++;

		// next row
		if (nCount % bmpHead_info.m_nWidth == 0)
		{
			cout << "\n";

			// if heigth was positive value
			if (bmpHead_info.m_nHeight > 0)
			{
				nOffset++;
				inf.seekg(bmpHead_file.m_dwFile_size - bmpHead_info.m_nWidth * nOffset * nBitCount, inf.beg);
			}
		}

		dwImageSize--;
	}

	inf.close();
}

/**/