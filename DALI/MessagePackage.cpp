#include "stdafx.h"
#include "MessagePackage.h"


CMessagePackage::CMessagePackage()
{
	memset(m_MessageBuffer, 0, sizeof(m_MessageBuffer));
	m_MessageLength = 0;
}


CMessagePackage::~CMessagePackage()
{

		
}

//void CMessagePackage::insertToBuffer(BYTE *buffer) {

//}

void CMessagePackage::getCRCCheckBuffer() {
	
}

void CMessagePackage::toString() {
	strcat((char*)m_MessageBuffer, MESSAGEHEAD);

	strcat((char*)m_MessageBuffer, (char*)m_CRCCheckBuffer);
	strcat((char*)m_MessageBuffer, MESSAGEEND);
}

//CRC校验函数
uint16_t CMessagePackage::GetCrcValue(uint8_t *pSrc, uint16_t Length)
{
	uint16_t CrcValue = PRESET_VALUE;//0xFFFF
	for (uint16_t LengthIndex = 0; LengthIndex<Length; LengthIndex++)
	{
		CrcValue ^= pSrc[LengthIndex];
		for (uint8_t CheckIndex = 0; CheckIndex<8; CheckIndex++)
		{
			if ((CrcValue & 0x0001) != 0)
			{
				CrcValue >>= 1;
				CrcValue ^= PLOYNOMIAL;//0x8408
			}
			else
			{
				CrcValue >>= 1;
			}
		}
	}
	return(CrcValue);
}

//组成数据包
/*参数
* sendCache 数据包
* ParaNum 参数个数
* Command 命令
*
*/
int CMessagePackage::App_WifiSendFormat(uint8_t *SendCache, uint8_t ParaNum, uint8_t Command, ...)
{

	SendCache[0] = '<';
	SendCache[3] = ParaNum;
	SendCache[4] = 1;
	SendCache[5] = Command;

	uint8_t ParaLength = 6;
	va_list ap;
	va_start(ap, Command);
	while (--ParaNum)
	{
		uint8_t Length = va_arg(ap, uint8_t);
		uint8_t* ParaContent = va_arg(ap, uint8_t*);
		SendCache[ParaLength++] = Length;

		memcpy(SendCache + ParaLength, ParaContent, Length);

		ParaLength += Length;
		ParaContent += sizeof(ParaContent);
	}

	va_end(ap);
	SendCache[1] = (ParaLength + 1) & 0xFF;
	SendCache[2] = 0;

	uint16_t CrcValue;
	CrcValue = GetCrcValue(SendCache + 1, ParaLength - 1);
	memcpy(SendCache + ParaLength, &CrcValue, sizeof(uint16_t));

	SendCache[ParaLength + 2] = '>';

	ParaLength += 3;
	return ParaLength;
}
