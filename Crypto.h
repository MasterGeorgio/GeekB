#pragma once
#include <stdint.h>

/// Размер блока
#define SIZE_BLOCK  16

/// Размер К
#define SIZE_KEY    SIZE_BLOCK

/// Таблица подстановок
#define SIZE_TAB_S   256

/// Таблица перестановок
#define SIZE_TAB_P   SIZE_BLOCK

/// Успешно
#define ALG_SUSSECS      0x00
#define ALG_SUSSECS_P    0x01
#define ALG_SUSSECS_S    0x02
#define ALG_SUSSECS_SP   ALG_SUSSECS_S ^ ALG_SUSSECS_P

#define ERR_NOT_KEY      0xE0
#define ERR_NOT_DATA     0xE1
#define ERR_TEST_VERIFY  0xE2


/// Класс криптографический
class Crypto
{
public:
	Crypto();

	~Crypto();

	/// Функция инициализации
	int Init(
		uint8_t *pucKey,
		uint8_t *pucTabP = nullptr,
		uint8_t *pucTabS = nullptr);

	/// Функция зашифрования
	int Encode(
		uint8_t  *pucDataIn,
		uint64_t ullLeingth,
		uint8_t  *pucDataOut);

	/// Функция расшифрования
	int Decode(
		uint8_t  *pucDataIn,
		uint64_t ullLeingth,
		uint8_t  *pucDataOut);


	/// Функция контрольного примера
	int Test();
private:
	/// Такт
	void Tack();

	/// Операция подстановоки
	void Substitute();

	/// Операция перестановки
	void Permute();

private:
	/// Таблица подстановок
	uint8_t m_aucSbox[SIZE_TAB_S];

	/// Таблица перестановок
	uint8_t m_aucPbox[SIZE_TAB_P];

	/// К
	uint8_t m_aucKey[SIZE_KEY];

	/// Регистр
	uint8_t m_aucReg[SIZE_KEY];

	bool isKey = false;
};

