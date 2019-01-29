#include "stdafx.h"
#include <cwchar>

namespace Parm
{
	PARM getparm(int argc, _TCHAR* argv[])
	{
		PARM Pr;
		wchar_t in[PARM_MAX_SIZE], out[PARM_MAX_SIZE], log[PARM_MAX_SIZE];
		if (argc == 1)
			throw ERROR_THROW(20);
		wchar_t *parIn = wcsstr(argv[1], PARM_IN);	// parIn - указатель на первое вхождение строки PARM_IN в строку argv[1], или пустой указатель
		if (!parIn)
			throw ERROR_THROW(20);
		if (wcslen(argv[1]) > PARM_MAX_SIZE)
			throw ERROR_THROW(21);
		wcscpy_s(in, parIn + wcslen(PARM_IN));	// сдвиг указателя на wcslen(PARM_IN) кол-во символов и копирование полученной строки в массив in
		wcscpy_s(Pr.in, in);
		if (argc < 3) {
			wcscpy_s(out, in);
			wcscat_s(out, PARM_MAX_SIZE, PARM_OUT_DEFAULT_EXT);
			wcscpy_s(Pr.out, out);
		}
		else {
			if (wcslen(argv[2]) > PARM_MAX_SIZE)
				throw ERROR_THROW(21);
			wcscpy_s(out, argv[2] + wcslen(PARM_OUT));
			wcscpy_s(Pr.out, out);
		}
		if (argc < 4) {
			wcscpy_s(log, in);
			wcscat_s(log, PARM_MAX_SIZE, PARM_LOG_DEFAULT_EXT);
			wcscpy_s(Pr.log, log);
		}
		else {
			if (wcslen(argv[3]) > PARM_MAX_SIZE)
				throw ERROR_THROW(21);
			wcscpy_s(log, argv[3] + wcslen(PARM_LOG));
			wcscpy_s(Pr.log, log);
		}
		return Pr;
	}
}