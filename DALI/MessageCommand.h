#pragma once

//#include "Function.h"

typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;

typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
class CMessageCommand
{
private:
	//���� + ��������λ + ����ID���� + ����ID + ���볤�� + ��������
	//�ܳ���24λ 1+1+1+4+1+16
	BYTE m_CommandBuffer[25];
	//��һλ���ȣ� ռһλ
	BYTE m_ControlCommandLength[2];
	//�ڶ�λ��������λ�� ռһλ
	BYTE m_ControlCommandBuffer[2];
	//����λ����ID���ȣ� ռһλ
	BYTE m_HostIDLength[2];
	//����λ����ID�� ռ��λ
	BYTE m_HostIDBuffer[5];
	//����λ���볤�ȣ�ռһλ
	BYTE m_HostPwdLength[2];
	//����λ�������ݣ� ռ16λ
	BYTE m_HostPwdBuffer[17];

public:
	CMessageCommand();
	~CMessageCommand();

	void setCommandBuffer();
	
};

