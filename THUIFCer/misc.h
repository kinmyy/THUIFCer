#pragma once

#include <string>
using namespace std;

/************************************************************************/
/* 16进制字符转换为对应数字                                             */
/************************************************************************/
unsigned char CharToHex(char ch) 
{ 
	//0-9 
	if (ch >= '0' && ch <= '9') 
		return (ch - '0');         
	//9-15 
	if (ch >= 'A' && ch <= 'F') 
		return (ch - 'A' + 0xA); 
	//9-15
	if (ch >= 'a' && ch <= 'f') 
		return (ch - 'a' + 0xA);

	return(255); 
}

/************************************************************************/
/* 解析ifc文件中被编码的字符串                                          */
/************************************************************************/
string strParser(const char *str)
{
	if (!str)
		return "<null>";

	string tmpstr(str);

	/* \S\ */
	int pos;
	pos=tmpstr.find("\\S\\"); /* 匹配\S\ */
	while(pos!=string::npos)
	{
		tmpstr.erase(pos,3);  //删除控制字符
		tmpstr[pos]=tmpstr[pos]+128;
		pos=tmpstr.find("\\S\\");
	}

	/* \X\ */
	char tmp;
	char chr=0;
	pos=tmpstr.find("\\X\\"); /* 匹配\X\ */
	while(pos!=string::npos)
	{
		tmpstr.erase(pos,3);       //删除控制字符
		tmp=tmpstr[pos];
		chr=chr+CharToHex(tmp)*16; //高位
		tmp=tmpstr[pos+1];
		chr=chr+CharToHex(tmp);    //低位
		tmpstr.erase(pos,2);       //删除Hex字符
		tmpstr.insert(pos,1,chr);  //插入解析后的字符
		chr=0;
		pos=tmpstr.find("\\X\\");
	}

	return tmpstr;
}

/************************************************************************/
/* 多字节到Unicode的转换                                                */
/* 注意：lpwszStr在使用前必须分配足够的空间                             */
/************************************************************************/
int AnsiToUnicode(LPCSTR lpszStr,LPWSTR lpwszStr)
{
	if (NULL==lpszStr)
	{
		return -1;
	}

	if (NULL==lpwszStr)
	{
		return -1;
	}

	int size=strlen(lpszStr);
	if (0==MultiByteToWideChar(CP_ACP,0,lpszStr,-1,lpwszStr,size))
	{
		return -1;
	}

	lpwszStr[size]='\0';
	return size;
}

/************************************************************************/
/* Unicode到UTF-8的转换                                                 */
/* 注意：lpszStr在使用前必须分配足够的空间                              */
/************************************************************************/
int UnicodeToUTF8(LPCWSTR lpcwszStr, LPSTR lpszStr)
{
	if (NULL==lpcwszStr)
	{
		return -1;
	}

	int wlen=wcslen(lpcwszStr);
	if (wlen<=0)
	{
		return -1;
	}

	if (NULL==lpszStr)
		return -1;

	int len=0;
	WCHAR wchr;
	for(int i=0;i<wlen;++i)
	{
		wchr=lpcwszStr[i];
		if (wchr < 0x80) //ascii
		{
			lpszStr[len++] = (char)wchr;
		}
		else if(wchr < 0x800) //U-00000080 – U-000007FF
		{
			lpszStr[len++] = 0xc0 | ( wchr >> 6 );
			lpszStr[len++] = 0x80 | ( wchr & 0x3f );
		}
		else if(wchr < 0x10000 ) //U-00000800 – U-0000FFFF
		{
			lpszStr[len++] = 0xe0 | ( wchr >> 12 );
			lpszStr[len++] = 0x80 | ( (wchr >> 6) & 0x3f );
			lpszStr[len++] = 0x80 | ( wchr & 0x3f );
		}
		/*
		else if( wchr < 0x200000 ) //U-00010000 – U-001FFFFF
		{
			lpszStr[len++] = 0xf0 | ( (int)wchr >> 18 );
			lpszStr[len++] = 0x80 | ( (wchr >> 12) & 0x3f );
			lpszStr[len++] = 0x80 | ( (wchr >> 6) & 0x3f );
			lpszStr[len++] = 0x80 | ( wchr & 0x3f );
		}
		*/
	}
	lpszStr[len]='\0';
	return len;
}

/************************************************************************/
/* UTF-8到Unicode的转换                                                 */
/************************************************************************/
int UTF8ToUnicode(const char* src, std::wstring &t)
{
	if (src == NULL)
	{
		return -1;
	}

	int size_s = strlen(src);
	int size_d = size_s + 10;          //?

	wchar_t *des = new wchar_t[size_d];
	memset(des, 0, size_d * sizeof(wchar_t));

	int s = 0, d = 0;
	bool toomuchbyte = true; //set true to skip error prefix.

	while (s < size_s && d < size_d)
	{
		unsigned char c = src[s];
		if ((c & 0x80) == 0)
		{
			des[d++] += src[s++];
		}
		else if((c & 0xE0) == 0xC0)  ///< 110x-xxxx 10xx-xxxx
		{
			WCHAR &wideChar = des[d++];
			wideChar  = (src[s + 0] & 0x3F) << 6;
			wideChar |= (src[s + 1] & 0x3F);

			s += 2;
		}
		else if((c & 0xF0) == 0xE0)  ///< 1110-xxxx 10xx-xxxx 10xx-xxxx
		{
			WCHAR &wideChar = des[d++];

			wideChar  = (src[s + 0] & 0x1F) << 12;
			wideChar |= (src[s + 1] & 0x3F) << 6;
			wideChar |= (src[s + 2] & 0x3F);

			s += 3;
		}
		else if((c & 0xF8) == 0xF0)  ///< 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx
		{
			WCHAR &wideChar = des[d++];

			wideChar  = (src[s + 0] & 0x0F) << 18;
			wideChar  = (src[s + 1] & 0x3F) << 12;
			wideChar |= (src[s + 2] & 0x3F) << 6;
			wideChar |= (src[s + 3] & 0x3F);

			s += 4;
		}
		else
		{
			WCHAR &wideChar = des[d++]; ///< 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx

			wideChar  = (src[s + 0] & 0x07) << 24;
			wideChar  = (src[s + 1] & 0x3F) << 18;
			wideChar  = (src[s + 2] & 0x3F) << 12;
			wideChar |= (src[s + 3] & 0x3F) << 6;
			wideChar |= (src[s + 4] & 0x3F);

			s += 5;
		}
	}

	t = des;
	delete[] des;
	des = NULL;

	return 0;
}
