#pragma once
#include <stdint.h>

/// Размер блока
#define SIZE_BLOCK  16

/// Размер К
#define SIZE_KEY    SIZE_BLOCK

/// Таблица подстановок
/// Table S substitute
#define SIZE_TAB_S   256

/// Таблица перестановок
/// Table P permutation
#define SIZE_TAB_P   SIZE_BLOCK

/// Sussecs
#define ALG_SUCCESS      0x00
/// Sussecs, but used P default
#define ALG_SUCCESS_P    0x01
/// Sussecs, but used S default
#define ALG_SUCCESS_S    0x02
/// Sussecs, but used S and P default
#define ALG_SUCCESS_SP   ALG_SUCCESS_S ^ ALG_SUCCESS_P

#define ERR_NOT_KEY      0xE0
#define ERR_NOT_DATA     0xE1
#define ERR_TEST_VERIFY  0xE2


/// Класс криптографический
/// Crypto class
class Crypto
{
public:
	Crypto();

	~Crypto();

	/// Функция инициализации
        /// Initialization
	int Init(
		uint8_t *pucKey,
		uint8_t *pucTabP = nullptr,
		uint8_t *pucTabS = nullptr);

	/// Функция зашифрования
	/// Encode function
	int Encode(
		const uint8_t  *pucDataIn,
		const uint64_t ullLeingth,
		uint8_t        *pucDataOut);

	/// Функция расшифрования
	/// Decode function
	int Decode(
		const uint8_t  *pucDataIn,
		const uint64_t ullLeingth,
		uint8_t        *pucDataOut);


	/// Функция зашифрования
	/// Encode function
	int EncodeAlpha(
		const uint8_t  *pucDataIn,
		const uint64_t ullLeingth,
		uint8_t        *pucDataOut);

	/// Функция расшифрования
	/// Decode function
	int DecodeAlpha(
		const uint8_t  *pucDataIn,
		const uint64_t ullLeingth,
		uint8_t        *pucDataOut);

	/// Функция контрольного примера
	/// Control check algorithm
	int Test();
private:
	/// Такт
	/// One tact cypher
	void Tack();
	
	/// Операция подстановоки
	/// Substitute operation
	void Substitute();

	/// Операция перестановки
        /// Permute operation
	void Permute();

private:
	/// Таблица подстановок
    /// Tab Substitute
	uint8_t m_aucSbox[SIZE_TAB_S];

	/// Таблица перестановок
        /// Tab Permutetation
	uint8_t m_aucPbox[SIZE_TAB_P];

	/// Таблица подстановок
    /// Tab Substitute
    uint8_t m_aucSboxInv[SIZE_TAB_S];

    /// Таблица перестановок
    /// Tab Permutetation
    uint8_t m_aucPboxInv[SIZE_TAB_P];

	/// К
    /// Key
	uint8_t m_aucKey[SIZE_KEY];

	/// Регистр
        /// Registr work gamma
	uint8_t m_aucReg[SIZE_KEY];

        /// Flag key avialable
	bool isKey = false;
};

