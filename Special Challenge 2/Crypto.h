#pragma once
#include <stdint.h>

/// ������ �����
#define SIZE_BLOCK  16

/// ������ �
#define SIZE_KEY    SIZE_BLOCK

/// ������� �����������
/// Table S substitute
#define SIZE_TAB_S   256

/// ������� ������������
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


/// ����� �����������������
/// Crypto class
class Crypto
{
public:
	Crypto();

	~Crypto();

	/// ������� �������������
        /// Initialization
	int Init(
		uint8_t *pucKey,
		uint8_t *pucTabP = nullptr,
		uint8_t *pucTabS = nullptr);

	/// ������� ������������
	/// Encode function
	int Encode(
		const uint8_t  *pucDataIn,
		const uint64_t ullLeingth,
		uint8_t        *pucDataOut);

	/// ������� �������������
	/// Decode function
	int Decode(
		const uint8_t  *pucDataIn,
		const uint64_t ullLeingth,
		uint8_t        *pucDataOut);


	/// ������� ������������
	/// Encode function
	int EncodeAlpha(
		const uint8_t  *pucDataIn,
		const uint64_t ullLeingth,
		uint8_t        *pucDataOut);

	/// ������� �������������
	/// Decode function
	int DecodeAlpha(
		const uint8_t  *pucDataIn,
		const uint64_t ullLeingth,
		uint8_t        *pucDataOut);

	/// ������� ������������ �������
	/// Control check algorithm
	int Test();
private:
	/// ����
	/// One tact cypher
	void Tack();
	
	/// �������� ������������
	/// Substitute operation
	void Substitute();

	/// �������� ������������
        /// Permute operation
	void Permute();

private:
	/// ������� �����������
    /// Tab Substitute
	uint8_t m_aucSbox[SIZE_TAB_S];

	/// ������� ������������
        /// Tab Permutetation
	uint8_t m_aucPbox[SIZE_TAB_P];

	/// ������� �����������
    /// Tab Substitute
    uint8_t m_aucSboxInv[SIZE_TAB_S];

    /// ������� ������������
    /// Tab Permutetation
    uint8_t m_aucPboxInv[SIZE_TAB_P];

	/// �
    /// Key
	uint8_t m_aucKey[SIZE_KEY];

	/// �������
        /// Registr work gamma
	uint8_t m_aucReg[SIZE_KEY];

        /// Flag key avialable
	bool isKey = false;
};

