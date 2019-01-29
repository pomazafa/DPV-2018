#include "stdafx.h"

namespace In
{
	IN getin(wchar_t infile[])
	{
		IN in;
		std::ifstream file;
		unsigned char c;
		int i = 0, col = -1;
		in.size = 0;
		in.ignor = 0;
		in.lines = 1;
		in.text = new unsigned char[IN_MAX_LEN_TEXT];
		int code[256] = IN_CODE_TABLE;
		for (int k = 0; k < 256; k++)
			in.code[k] = code[k];
		file.open(infile);
		if (file.fail())
			throw ERROR_THROW(40);
		while (true) {
			c = file.get();
			if (file.eof()) break;
			in.size++; col++;
			if (in.code[(int)c] == in.F) {
				throw ERROR_THROW_IN(41, in.lines - 1, col);
			}
			else if (in.code[(int)c] == in.T) {
				in.text[i] = c;
				i++;
			}
			else if (in.code[(int)c] == in.I) {
				in.ignor++;
				in.size--;
			}
			else {
				in.text[i] = in.code[(int)c];
				i++;
			}
			if (c == IN_CODE_ENDL) {
				in.lines++;
				col = -1;
			}
		}
		in.text[i] = '\0';
		return in;
	}
}