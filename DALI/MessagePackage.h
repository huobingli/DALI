#pragma once
#include <string>
//#include "Function.h"
#include "MessageCommand.h"

#define MESSAGEHEAD "<"
#define MESSAGEEND ">"
#define PRESET_VALUE 0xFFFF
#define PLOYNOMIAL 0x8408

//���Խṹ��
/*
struct messageBuffer{
	char head[1];
	char MessageLength[2];
	char MessageParams[1];
	char ControlLength[1];
	char ControlCommand[1];
	char HostIDLength[1];
	char HostID[4];
	char HostPwdLength[1];
	char HostPwd[16];
	char CRCCheck[2];
	char end[1];
};


void makeBuffer(messageBuffer *Buffer) {
	memcpy(Buffer->head, MESSAGEHEAD, sizeof(1));
	IntToChar(Buffer->MessageLength, sizeof(Buffer));
	Buffer->MessageParams

	memcpy(Buffer->end, MESSAGEEND, sizeof(1));
}*/

/*
static uint16_t GetCrcValue(uint8_t *pSrc, uint16_t Length)
{
	uint16_t CrcValue = PRESET_VALUE;
	for (uint16_t LengthIndex = 0; LengthIndex < Length; LengthIndex++)
	{
		CrcValue ^= pSrc[LengthIndex];
		for (uint8_t CheckIndex = 0; CheckIndex < 8; CheckIndex++)
		{
			if ((CrcValue & 0x0001) != 0)
			{
				CrcValue >>= 1;
				CrcValue ^= PLOYNOMIAL;
			}
			else
			{
				CrcValue >>= 1;
			}
		}
	}
	return CrcValue;
}
*/

//typedef char BYTE;

class CMessagePackage
{
private:
	//��buffer
	BYTE m_MessageBuffer[256];
	//����2BYTE
	int m_MessageLength;
	BYTE m_LengthBuffer[3];
	//����ID����Ϊ4BYTE
	int m_HostIDLength;
	BYTE m_HostIDBuffer[5];
	//���볤��Ϊ16BYTE
	int m_HostPwdLength;
	BYTE m_HostPwdBuffer[17];
	//�ݶ�50BYTE
	int m_CRCCheckLength;
	BYTE m_CRCCheckBuffer[50];
	
public:
	CMessagePackage();
	~CMessagePackage();

	//���뵽����buffer��
	void insertToBuffer(char *buffer) {

	}

	//�õ�CRCУ��ֵ
	uint16_t GetCrcValue(uint8_t *pSrc, uint16_t Length);

	//��װ��
	int App_WifiSendFormat(uint8_t *SendCache, uint8_t ParamNum, uint8_t Command, ...);

	//�õ�CRCbuffer
	void getCRCCheckBuffer();

	//��������תΪһ���ַ���
	void toString();
};

