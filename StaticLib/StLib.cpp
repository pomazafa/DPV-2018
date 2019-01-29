#include "stdafx.h"
#include <iostream>

extern "C"
{
	void _stdcall outputStr(char * str)
	{
		setlocale(0, "");
		std::cout << str << std::endl;
	}

	void _stdcall outputIs(int is)
	{
		if(is == 0)
			std::cout << "false" << std::endl;
		else
			std::cout << "true" << std::endl;
	}

	void _stdcall outputInt(int intnum)
	{
		std::cout << intnum << std::endl;
	}

	int _stdcall getnod(int num1, int num2)
	{
		int min;
		if (num1 < num2)
			min = num1;
		else
			min = num2;
		int result = 1;
		for (int i = 2; i <= min; i++)
		{
			if (num1 % i == 0 && num2 % i == 0)
				result = i;
		}
		return result;
	}

	int _stdcall getnok(int num1, int num2)
	{
		int result = num1;
		for (int i = num1; i <= num1 * num2; i++)
		{
			if (i % num1 == 0 && i % num2 == 0)
			{
				result = i;
				break;
			}
		}
		return result;
	}

	bool _stdcall issubstr(char* str, char* sub)
	{
		char *ptrstrstr = strstr(str, sub);
		if (ptrstrstr == NULL) 
			return false;
		return true;
	}

	int _stdcall getlength(char* str)
	{
		return strlen(str);
	}
}