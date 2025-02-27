#include "stdafx.h"
#include "Crypto.h"

#include <iostream>

// �������
//#define DBG_CRYPTO

//-------------------------------------------------------------
/// ������������� ���������
Crypto::Crypto()
{
#if (SIZE_BLOCK != 16)
#pragma warning Size_block_was_changing
#endif

#if (SIZE_KEY != SIZE_BLOCK)
#error Size key must equal size block (See Shannon's theorem)
#endif

#if (SIZE_TAB_P != SIZE_BLOCK)
#error Size Tab P must equal size block
#endif

#if (SIZE_TAB_S != 256)
#error Size Tab S must equal 256
#endif
}

//-------------------------------------------------------------
Crypto::~Crypto()
{
}

//-------------------------------------------------------------
/// ������������� ���������
/// ������� �������������
/// Initialization
int Crypto::Init(
	uint8_t *pucKey,
	uint8_t *pucTabP,
	uint8_t *pucTabS)
{

	int nRes = ALG_SUCCESS_SP;

	if (pucKey == nullptr)
	{
		nRes = ERR_NOT_KEY;
		return nRes;
	}
	else
	{
		for (int i = 0; i < SIZE_KEY; i++)
			m_aucKey[i] = pucKey[i];

		isKey = true;
	}

	// ���������� �����
	// Key complication
	for (size_t i = 0; i < 16; i++)
	{
		Tack();
		memcpy(m_aucKey, m_aucReg, sizeof(m_aucKey));
	}

	/// ������������� �������������� ����������
        /// Initialization long parametrs
        ///
        /// ������� �����������
        /// Tab Substitute
	uint8_t aucSbox[] = {
		0xFC, 0xEE, 0xDD, 0x11, 0xCF, 0x6E, 0x31, 0x16, 0xFB, 0xC4, 0xFA, 0xDA, 0x23, 0xC5, 0x04, 0x4D,
		0xE9, 0x77, 0xF0, 0xDB, 0x93, 0x2E, 0x99, 0xBA, 0x17, 0x36, 0xF1, 0xBB, 0x14, 0xCD, 0x5F, 0xC1,
		0xF9, 0x18, 0x65, 0x5A, 0xE2, 0x5C, 0xEF, 0x21, 0x81, 0x1C, 0x3C, 0x42, 0x8B, 0x01, 0x8E, 0x4F,
		0x05, 0x84, 0x02, 0xAE, 0xE3, 0x6A, 0x8F, 0xA0, 0x06, 0x0B, 0xED, 0x98, 0x7F, 0xD4, 0xD3, 0x1F,
		0xEB, 0x34, 0x2C, 0x51, 0xEA, 0xC8, 0x48, 0xAB, 0xF2, 0x2A, 0x68, 0xA2, 0xFD, 0x3A, 0xCE, 0xCC,
		0xB5, 0x70, 0x0E, 0x56, 0x08, 0x0C, 0x76, 0x12, 0xBF, 0x72, 0x13, 0x47, 0x9C, 0xB7, 0x5D, 0x87,
		0x15, 0xA1, 0x96, 0x29, 0x10, 0x7B, 0x9A, 0xC7, 0xF3, 0x91, 0x78, 0x6F, 0x9D, 0x9E, 0xB2, 0xB1,
		0x32, 0x75, 0x19, 0x3D, 0xFF, 0x35, 0x8A, 0x7E, 0x6D, 0x54, 0xC6, 0x80, 0xC3, 0xBD, 0x0D, 0x57,
		0xDF, 0xF5, 0x24, 0xA9, 0x3E, 0xA8, 0x43, 0xC9, 0xD7, 0x79, 0xD6, 0xF6, 0x7C, 0x22, 0xB9, 0x03,
		0xE0, 0x0F, 0xEC, 0xDE, 0x7A, 0x94, 0xB0, 0xBC, 0xDC, 0xE8, 0x28, 0x50, 0x4E, 0x33, 0x0A, 0x4A,
		0xA7, 0x97, 0x60, 0x73, 0x1E, 0x00, 0x62, 0x44, 0x1A, 0xB8, 0x38, 0x82, 0x64, 0x9F, 0x26, 0x41,
		0xAD, 0x45, 0x46, 0x92, 0x27, 0x5E, 0x55, 0x2F, 0x8C, 0xA3, 0xA5, 0x7D, 0x69, 0xD5, 0x95, 0x3B,
		0x07, 0x58, 0xB3, 0x40, 0x86, 0xAC, 0x1D, 0xF7, 0x30, 0x37, 0x6B, 0xE4, 0x88, 0xD9, 0xE7, 0x89,
		0xE1, 0x1B, 0x83, 0x49, 0x4C, 0x3F, 0xF8, 0xFE, 0x8D, 0x53, 0xAA, 0x90, 0xCA, 0xD8, 0x85, 0x61,
		0x20, 0x71, 0x67, 0xA4, 0x2D, 0x2B, 0x09, 0x5B, 0xCB, 0x9B, 0x25, 0xD0, 0xBE, 0xE5, 0x6C, 0x52,
		0x59, 0xA6, 0x74, 0xD2, 0xE6, 0xF4, 0xB4, 0xC0, 0xD1, 0x66, 0xAF, 0xC2, 0x39, 0x4B, 0x63, 0xB6
	};

	/// ������� ������������
        /// Tab Permutation
	uint8_t aucPbox[] = {
		0,  8,
		1,  9,
		2, 10,
		3, 11,
		4, 12,
		5, 13,
		6, 14,
		7, 15
	};

	for (int i = 0; i < SIZE_TAB_S; i++)
		m_aucSbox[i] = aucSbox[i];

	for (int i = 0; i < SIZE_TAB_P; i++)
		m_aucPbox[i] = aucPbox[i];

	if (pucTabP != nullptr)
	{
		for (int i = 0; i < SIZE_TAB_P; i++)
			m_aucPbox[i] = pucTabP[i];

		nRes ^= ALG_SUCCESS_P;
	}

	if (pucTabS != nullptr)
	{
		for (int i = 0; i < SIZE_TAB_S; i++)
			m_aucSbox[i] = pucTabS[i];

		nRes ^= ALG_SUCCESS_S;
	}
	
	// .. �������� ������� � SP
	// Inverse Tab Substitute
	for (int j = 0; j < sizeof(m_aucSboxInv); j++)
		m_aucSboxInv[m_aucSbox[j]] = j;

	return nRes;
}



//-------------------------------------------------------------
/// ������� ������������ �� ����� XPS ��� ���������
/// Encode function on the scheme XPS over Reg
int Crypto::Encode(
        const uint8_t  *pucDataIn,
        const uint64_t ullLeingth,
        uint8_t        *pucDataOut)
{
    int nRes = ALG_SUCCESS;

    // Key must be avialable
    if (!isKey)
        return ERR_NOT_KEY;

	// Data must be avialable
    if ((pucDataIn  == nullptr) ||
		(pucDataOut == nullptr))
            return ERR_NOT_DATA;

    /// ������������ �������� ����������
    /// Init work registr cypher
    for (size_t i = 0; i < SIZE_KEY; i++)
            m_aucReg[i] = m_aucKey[i];

#ifdef DBG_CRYPTO
    uint8_t aucIn[SIZE_BLOCK * 8];
    for (uint64_t i = 0; i < ullLeingth; i++)
            aucIn[i] = pucDataIn[i];
    uint8_t aucOut[SIZE_BLOCK * 8];
    for (uint64_t i = 0; i < ullLeingth; i++)
            aucOut[i] = pucDataOut[i];
#endif

#ifdef DBG_CRYPTO
	std::cout << "Gamma:\n";
#endif
    for (size_t i = 0; i < ullLeingth; i += SIZE_BLOCK)
    {
		/// ���� ������ ����� XPS �����
		/// One tack work XPS scheme
		for (size_t j = 0; j < 16; j++)
			Tack();

#ifdef DBG_CRYPTO
		std::cout << i << " gamma:\n";
#endif
		for (uint8_t j = 0; j < SIZE_BLOCK; j++)
		{
			*pucDataOut++ = m_aucReg[j] ^ *pucDataIn++;

#ifdef DBG_CRYPTO
			aucOut[i + j] = aucIn[i + j] ^ m_aucReg[j];
#endif
#ifndef DBG_CRYPTO
			std::cout << std::showbase << std::hex << std::uppercase << (int)m_aucReg[j] << "\t";
#endif
		}
#ifndef DBG_CRYPTO
		std::cout << "\n";
#endif
	}

    return nRes;
}

//-------------------------------------------------------------
/// ������� ������������� �� ����� XPS ��� ���������
/// Decode function on the scheme XPS over Reg
int Crypto::Decode(
        const uint8_t  *pucDataIn,
        const uint64_t ullLeingth,
        uint8_t        *pucDataOut)
{
	int nRes = Encode(
		pucDataIn,
		ullLeingth,
		pucDataOut);

	return nRes;
}

//-------------------------------------------------------------
/// ������� ������������ �� ����� XPS ��� �������
/// Encode function on the scheme XPS over data
int Crypto::EncodeAlpha(
        const uint8_t  *pucDataIn,
        const uint64_t ullLeingth,
        uint8_t        *pucDataOut)
{
    int nRes = ALG_SUCCESS;

	// Key must be avialable
    if (!isKey)
        return ERR_NOT_KEY;

	// Data must be avialable
    if ((pucDataIn  == nullptr) ||
       (pucDataOut == nullptr))
        return ERR_NOT_DATA;

#ifdef DBG_CRYPTO
    uint8_t aucIn[SIZE_BLOCK * 8];
    for (uint64_t i = 0; i < ullLeingth; i++)
            aucIn[i] = pucDataIn[i];
    uint8_t aucOut[SIZE_BLOCK * 8];
    for (uint64_t i = 0; i < ullLeingth; i++)
            aucOut[i] = pucDataOut[i];
#endif

    for (size_t i = 0; i < ullLeingth; i += SIZE_BLOCK)
    {
        //////////////////////////////
        // One tack work XPS scheme
        // Substitute operation
        /// Registr
        uint8_t aucReg[SIZE_BLOCK] = {0};
        for (size_t j = 0; j < sizeof (aucReg); j++)
            aucReg[j] = m_aucSbox[ pucDataIn[i + j] ];

        // Permute operation
        for (size_t j = 0; j < sizeof (aucReg); j++)
                pucDataOut[i + j] = aucReg[m_aucPbox[j]];

        // Xor operation
        for (size_t j = 0; j < SIZE_BLOCK; j++)
        {
            pucDataOut[i + j] ^= m_aucKey[j];

#ifdef DBG_CRYPTO
            aucOut[i + j] ^= aucReg[j];
#endif
        }
        //////////////////////////////
    }

    return nRes;
}

/// ������� ������������� �� ����� XPS ��� �������
/// Decode function on the scheme XPS over data
int Crypto::DecodeAlpha(
	const uint8_t  *pucDataIn,
	const uint64_t ullLeingth,
	uint8_t        *pucDataOut)
{
    int nRes = ALG_SUCCESS;

    // Key avialable
    if (!isKey)
        return ERR_NOT_KEY;

    // Data avialable
    if ((pucDataIn  == nullptr) ||
       (pucDataOut == nullptr))
        return ERR_NOT_DATA;

#ifdef DBG_CRYPTO
    uint8_t aucIn[SIZE_BLOCK * 8];
    for (uint64_t i = 0; i < ullLeingth; i++)
            aucIn[i] = pucDataIn[i];
    uint8_t aucOut[SIZE_BLOCK * 8];
    for (uint64_t i = 0; i < ullLeingth; i++)
            aucOut[i] = pucDataOut[i];
#endif

    for (size_t i = 0; i < ullLeingth; i += SIZE_BLOCK)
    {
        //////////////////////////////
        // One tack work XPS scheme inverse for alg Alpha

		// Xor operation
		for (size_t j = 0; j < SIZE_BLOCK; j++)
		{
			pucDataOut[i + j] = pucDataIn[i + j] ^ m_aucKey[j];

#ifdef DBG_CRYPTO
			aucOut[i + j] ^= aucReg[j];
#endif
		}

		/// Registr
		uint8_t aucReg[SIZE_BLOCK] = { 0 };
		// Permute operation
		for (size_t j = 0; j < sizeof(aucReg); j++)
			aucReg[m_aucPbox[ j ]] = pucDataOut[i + j];

        // Substitute operation
        for (size_t j = 0; j < sizeof (aucReg); j++)
			pucDataOut[i + j] = m_aucSboxInv[aucReg[ j ]];

        //////////////////////////////
    }

    return nRes;
}


//-------------------------------------------------------------
/// ����
/// One tact cypher
void Crypto::Tack()
{
	// Substitute operation
	Substitute();

	// Permute operation
	Permute();

	// Xor operation
	for (size_t i = 0; i < SIZE_KEY; i++)
		m_aucReg[i] ^= m_aucKey[i];
}

//-------------------------------------------------------------
/// �������� ������������
/// Substitute operation
void Crypto::Substitute()
{
	for (size_t i = 0; i < SIZE_KEY; i++)
		m_aucReg[i] = m_aucSbox[ m_aucReg[i] ];
}

//-------------------------------------------------------------
/// �������� ������������
/// Permute operation
void Crypto::Permute()
{
	uint8_t aucResult[SIZE_TAB_P];
	for (size_t i = 0; i < SIZE_TAB_P; i++)
		aucResult[i] = m_aucReg[m_aucPbox[i]];

	for (size_t i = 0; i < SIZE_KEY; i++)
		m_aucReg[i] = aucResult[i];
}

//-------------------------------------------------------------
/// ����
/// Control check algorithm
int Crypto::Test()
{
	int nRes = ALG_SUCCESS;

	/// �
        /// Key
	uint8_t aucKey[] = {
		0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
		0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
	};

	/// �������� ������
        /// Open text
	uint8_t aucDataIn[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};

	/// �������� ������
        /// Close text
	uint8_t aucDataOut[sizeof(aucDataIn)] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};

	nRes = Init(aucKey);

	uint8_t aucVerify[sizeof(aucDataIn)] = {
		0xFC, 0xC6, 0x55, 0xDB, 0x21, 0X6D, 0xC8, 0xA0,
		0x62, 0x11, 0xA6, 0x63, 0x56, 0xB1, 0x90, 0x49,
	};

	nRes = Encode(
		aucDataIn,
		sizeof(aucDataIn),
		aucDataOut);

	for (size_t i = 0; i < sizeof(aucDataIn); i++)
		if (aucVerify[i] != aucDataOut[i])
		{
			nRes = ERR_TEST_VERIFY;
			break;
		}

/**/
	nRes = Decode(
		aucDataOut,
		sizeof(aucDataOut),
		aucDataOut);
/**/

	return nRes;
}
