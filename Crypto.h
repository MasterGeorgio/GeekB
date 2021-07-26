#pragma once
#include <stdint.h>

/// ������ �����
#define SIZE_BLOCK  16

/// ������ �
#define SIZE_KEY    SIZE_BLOCK

/// ������� �����������
#define SIZE_TAB_S   256

/// ������� ������������
#define SIZE_TAB_P   SIZE_BLOCK

/// �������
#define ALG_SUSSECS      0x00
#define ALG_SUSSECS_P    0x01
#define ALG_SUSSECS_S    0x02
#define ALG_SUSSECS_SP   ALG_SUSSECS_S ^ ALG_SUSSECS_P

#define ERR_NOT_KEY      0xE0
#define ERR_NOT_DATA     0xE1
#define ERR_TEST_VERIFY  0xE2


/// ����� �����������������
class Crypto
{
public:
	Crypto();

	~Crypto();

	/// ������� �������������
	int Init(
		uint8_t *pucKey,
		uint8_t *pucTabP = nullptr,
		uint8_t *pucTabS = nullptr);

	/// ������� ������������
	int Encode(
		uint8_t  *pucDataIn,
		uint64_t ullLeingth,
		uint8_t  *pucDataOut);

	/// ������� �������������
	int Decode(
		uint8_t  *pucDataIn,
		uint64_t ullLeingth,
		uint8_t  *pucDataOut);


	/// ������� ������������ �������
	int Test();
private:
	/// ����
	void Tack();

	/// �������� ������������
	void Substitute();

	/// �������� ������������
	void Permute();

private:
	/// ������� �����������
	uint8_t m_aucSbox[SIZE_TAB_S];

	/// ������� ������������
	uint8_t m_aucPbox[SIZE_TAB_P];

	/// �
	uint8_t m_aucKey[SIZE_KEY];

	/// �������
	uint8_t m_aucReg[SIZE_KEY];

	bool isKey = false;
};

