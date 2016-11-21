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
	//长度 + 控制命令位 + 主机ID长度 + 主机ID + 密码长度 + 密码内容
	//总长度24位 1+1+1+4+1+16
	BYTE m_CommandBuffer[25];
	//第一位长度， 占一位
	BYTE m_ControlCommandLength[2];
	//第二位控制命令位， 占一位
	BYTE m_ControlCommandBuffer[2];
	//第三位主机ID长度， 占一位
	BYTE m_HostIDLength[2];
	//第四位主机ID， 占四位
	BYTE m_HostIDBuffer[5];
	//第五位密码长度，占一位
	BYTE m_HostPwdLength[2];
	//第六位密码内容， 占16位
	BYTE m_HostPwdBuffer[17];

public:
	CMessageCommand();
	~CMessageCommand();

	void setCommandBuffer();
	
};

