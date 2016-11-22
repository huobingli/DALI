#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <algorithm>
using namespace std;


//这里面写你原来的代码

#ifndef FUNCTION_H
#define FUNCTION_H

//字符串反转
void strrev1(char *s)
{
	for (char *end = s + strlen(s) - 1; end > s; --end, ++s)
	{
		*s ^= *end ^= *s ^= *end;
	}
}

//int to binary
//for port 16bit
//整型转16位二进制
void IntToBin(unsigned int dec, char ch[]) {
	int i = 16;
	while (i >= 0)
	{
		int temp = dec;
		temp = temp >> i;
		ch[17 - i - 1] = (temp & 1) + '0';
		i--;
	}
}

//字符转二进制
void CharToBin(char c) {
	unsigned char k = 0x80;
	for (int i = 0; i<8; i++, k >>= 1)
	{
		if (c & k)
			cout << "1";
		else
			cout << "0";
	}
	cout << " ";
}

void IntToChar(char *changeBuffer, int changeInt) {
	sprintf(changeBuffer, "%d", changeInt);
	strcat(changeBuffer, "\0");
}


void IntToChar(char *changeBuffer, int changeInt, int changeLength) {
	sprintf(changeBuffer, "%d", changeInt);

	strrev1(changeBuffer);
	for (int i = 0; i < changeLength - 1; i++) {
		strcat(changeBuffer, "0");
	}
	strrev1(changeBuffer);


}

void divide(char * dest, char * src, int start, int n)
{
	src = src + start - 1;
	while ((*dest++ = *src++) && n - 1)
		n--;
	*dest = '\0';
}

void intToChar(unsigned int dec, char ch[]) {
	int i = 3;
	while (i >= 0)
	{
		int temp = dec;
		temp = temp >> i;
		ch[4 - i - 1] = (temp & 1) + '0';
		i--;
	}
}

int charToInt(char *changeBuffer) {
	int length = strlen(changeBuffer);
	int sum = 0;
	for (int i = 0; i < length; i++) {
		sum = sum + pow(10, length - i - 1) * (int)(changeBuffer[i] - '0');
	}
	return sum;
}

//十进制转十六进制
void DecToHex(int n, char *buffer) {
	_itoa_s(n, buffer, sizeof(buffer), 16);//转换为是小写16进制，
	transform(buffer, buffer + strlen(buffer), buffer, toupper);//转大写
}

//十六进制转十进制
int hex_char_value(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	//assert(0);
	return 0;
}

int hex_to_decimal(const char* szHex, int len)
{
	int result = 0;
	for (int i = 0; i < len; i++)
	{
		result += (int)pow((float)16, (int)len - i - 1) * hex_char_value(szHex[i]);
	}
	return result;
}


unsigned char HexToAsc(unsigned char aChar){
	if ((aChar >= 0x30) && (aChar <= 0x39))
		aChar -= 0x30;
	else if ((aChar >= 0x41) && (aChar <= 0x46))//大写字母
		aChar -= 0x37;
	else if ((aChar >= 0x61) && (aChar <= 0x66))//小写字母
		aChar -= 0x57;
	else aChar = 0xff;
	return aChar;
}

#endif