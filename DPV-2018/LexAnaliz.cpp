#include "stdafx.h"
#include "Div.h"
#include "Graphs.h"
#include "LexAnaliz.h"

#define DIV '|'
#define SPACE ' '
#define PLUS '+'
#define MINUS '-'
#define STAR '*'
#define DIRSLASH '/'
#define EQUAL '='

using namespace std;

namespace Lex
{
	LEX lexAnaliz(Log::LOG log, In::IN in)
	{
		LEX lex;
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);
		clearSpace(in.text, in.size);

		unsigned char** word = new unsigned char*[max_world];
		for (int i = 0; i < max_world; i++)
			word[i] = new unsigned char[size_world] {NULL};

		do {
			word = divideText(in.text, in.size);
		} while (word == NULL);

		int indxLex = 0;		// индекс лексемы
		int indexID = 0;		// индекс идент.
		int clit = 1;			// счетчик литералов
		int line = 1;			// номер строки
		int position = 0;			// номер позиции в исходном файле
		int counteropen = 0;
		int counterclose = 0;

		unsigned char emptystr[] = "";	// пустая строка
		unsigned char* prefix = new unsigned char[10]{ "" };	// текущий префикс
		unsigned char* bufprefix = new unsigned char[10]{ "" };	// буфер для префикса
		unsigned char* oldprefix = new unsigned char[10]{ "" };	// предыдущий префикс
		unsigned char* L = new unsigned char[2]{ "L" };
		unsigned char* bufL = new unsigned char[TI_STR_MAXSIZE];
		unsigned char* nameLiteral = new unsigned char[10]{ "" };
		char* charclit = new char[10]{ "" };		// для строковой записи счетчика

		bool findFunc = false;
		bool findParm = false;
		bool findLib = false;
		IT::Entry entryIT;

		for (int i = 0; word[i] != NULL; i++, indxLex++)
		{
			if (word[i][0] ==  (unsigned char)'<')
			{
				i++;
				while (word[i][0] != (unsigned char)'>')
				{
					i++;
					if (word[i] == NULL)
						throw ERROR_THROW(63);
				}
				continue;
			}
			if (word[i + 1] != NULL && word[i + 1][0] == (unsigned char)'>')
			{
				throw ERROR_THROW(63);
			}

			bool findSameID = false;

			FST::FST fstDecl(word[i], FST_DECL);
			if (FST::execute(fstDecl))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_DECL, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstTypeInteger(word[i], FST_INTEGER);
			if (FST::execute(fstTypeInteger))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_INTEGER, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				entryIT.iddatatype = IT::INT;
				continue;
			}
			FST::FST fstTypeIs(word[i], FST_IS);
			if (FST::execute(fstTypeIs))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_IS, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				entryIT.iddatatype = IT::IS;
				continue;
			}
			FST::FST fstTypeString(word[i], FST_STRING);
			if (FST::execute(fstTypeString))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_STRING, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				entryIT.value.vstr.len = 0;
				entryIT.iddatatype = IT::STR;
				_mbscpy(entryIT.value.vstr.str, emptystr);
				continue;
			}
			FST::FST fstFunction(word[i], FST_FUNCTION);
			if (FST::execute(fstFunction))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_FUNCTION, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::F;
				findFunc = true;
				continue;
			}
			FST::FST fstLibrary(word[i], FST_LIBRARY);
			if (FST::execute(fstLibrary))
			{
				findLib = true;
				LT::Entry entryLT = writeEntry(entryLT, LEX_LIB, indexID++, line);
				entryIT.iddatatype = IT::INT;
				LT::Add(lextable, entryLT);
				_mbscpy(entryIT.id, word[i]);
				entryIT.idxfirstLE = indxLex;
				entryIT.idtype = IT::LIB;
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstExit(word[i], FST_EXIT);
			if (FST::execute(fstExit))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_EXIT, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstOut(word[i], FST_OUT);
			if (FST::execute(fstOut))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_OUT, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstMain(word[i], FST_MAIN);
			if (FST::execute(fstMain))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_MAIN, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				_mbscpy(oldprefix, prefix);
				_mbscpy(prefix, word[i]);
				_mbscpy(entryIT.visibility, emptystr);
				continue;
			}
			FST::FST fstLiteralIs(word[i], FST_ISLIT);					//is
			if (FST::execute(fstLiteralIs))
			{
				bool value = false;
				if (word[i][0] == 'y')
					value = true;
				// поиск такого же
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vis == value && idtable.table[k].idtype == IT::L && idtable.table[k].iddatatype == IT::IS)
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						findSameID = true;
						break;
					}
				}
				if (findSameID)	// если был найден такой же
					continue;
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::IS;
				entryIT.value.vis = value;
				entryIT.idxfirstLE = indxLex;						// дальше формирование имени литерала
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// преобразуем значение счетчика в строку(charclit)
				_mbscpy(bufL, L);									// помещаем в буфер "L"
				word[i] = _mbscat(bufL, (unsigned char*)charclit);	// формируем имя для литерала
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstIdentif(word[i], FST_ID);
			if (FST::execute(fstIdentif))
			{
				// поиск такого же
				if (findFunc)	// если функция
				{
					int idx = IT::IsId(idtable, word[i]);	// ищем без префикса
					if (idx != TI_NULLIDX)		// если такой идентификатор уже есть
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						findFunc = false;
						continue;
					}
				}
				else
				{
					int idx = IT::IsId(idtable, word[i]);	// ищем без префикса, а потом с префиксом
					if (idx != TI_NULLIDX)		// если такой идентификатор уже есть
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						findFunc = false;
						continue;
					}

					_mbscpy(bufprefix, prefix);
					word[i] = _mbscat(bufprefix, word[i]);
					idx = IT::IsId(idtable, word[i]);
					if (idx != TI_NULLIDX)		// если такой идентификатор уже есть
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						continue;
					}
				}
				LT::Entry entryLT = writeEntry(entryLT, LEX_ID, indexID++, line);
				LT::Add(lextable, entryLT);
				if (findParm) {		// если параметр
					entryIT.idtype = IT::P;
					_mbscpy(entryIT.visibility, prefix);
				}
				else if (!findFunc) {	// если переменная
					entryIT.idtype = IT::V;
					_mbscpy(entryIT.visibility, prefix);
					if (entryIT.iddatatype == IT::INT)
						entryIT.value.vint = TI_INT_DEFAULT;
					if (entryIT.iddatatype == IT::IS)
						entryIT.value.vis = TI_IS_DEFAULT;
					if (entryIT.iddatatype == IT::STR) {
						entryIT.value.vstr.len = 0;
						memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
					}
				}
				else {		// если функция
					_mbscpy(oldprefix, prefix);
					_mbscpy(prefix, word[i]);
					_mbscpy(entryIT.visibility, emptystr);
				}

				entryIT.idxfirstLE = indxLex;
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				findFunc = false;
				continue;
			}
			FST::FST fstLiteralInt(word[i], FST_INTLIT);
			if (FST::execute(fstLiteralInt))
			{
				int value = atoi((char*)word[i]);	// значение литерала
													// поиск такого же
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vint == value && idtable.table[k].idtype == IT::L && idtable.table[k].iddatatype == IT::INT)
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						findSameID = true;
						break;
					}
				}
				if (findSameID)	// если был найден такой же
					continue;
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::INT;
				entryIT.value.vint = value;
				entryIT.idxfirstLE = indxLex;
				// формирование имени литерала
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// преобразуем значение счетчика в строку(charclit)
				_mbscpy(bufL, L);	// помещаем в буфер "L"
				word[i] = _mbscat(bufL, (unsigned char*)charclit);	// формируем имя для литерала
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstLiteralString(word[i], FST_STRLIT);
			if (FST::execute(fstLiteralString))
			{
				// запись значения
				int length = _mbslen(word[i]);
				for (int k = 0; k < length; k++)	// перезапись массива, убираем кавычки
					word[i][k] = word[i][k + 1];
				word[i][length - 2] = 0;
				// поиск такого же
				for (int k = 0; k < idtable.size; k++)
				{
					if (!(_mbscmp(idtable.table[k].value.vstr.str, word[i])))
					{
						findSameID = true;
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}
				if (findSameID)	// если был найден такой же
					continue;
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				_mbscpy(entryIT.value.vstr.str, word[i]);	// запись значиния строкового литерала
				entryIT.value.vstr.len = length - 2;		// запись длины строкового литерала
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::STR;
				entryIT.idxfirstLE = indxLex;
				// формирование имени литерала
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// преобразуем значение счетчика в строку(charclit)
				_mbscpy(bufL, L);	// помещаем в буфер "L"
				nameLiteral = _mbscat(bufL, (unsigned char*)charclit);	// формируем имя для литерала (L + charclit)
				_mbscpy(entryIT.id, nameLiteral);
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstOperator(word[i], FST_OPERATOR);
			if (FST::execute(fstOperator))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_OPERATOR, indexID++, line);
				switch (word[i][0])
				{
				case PLUS: case MINUS:
					entryLT.priority = 2;
					break;
				case DIRSLASH: case STAR:
					entryLT.priority = 3;
					break;
				}
				LT::Add(lextable, entryLT);
				_mbscpy(entryIT.id, word[i]);
				entryIT.idxfirstLE = indxLex;
				entryIT.idtype = IT::OP;
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstTilda(word[i], FST_TILDA);
			if (FST::execute(fstTilda))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_TILDA, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				if (counterclose != counteropen)
					throw ERROR_THROW_IN(62, line, position);
				continue;
			}
			FST::FST fstComma(word[i], FST_COMMA);
			if (FST::execute(fstComma))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_COMMA, LT_TI_NULLIDX, line);
				entryLT.priority = 1;
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstLeftBrace(word[i], FST_LEFTBRACE);
			if (FST::execute(fstLeftBrace))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTBRACE, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstRightBrace(word[i], FST_BRACELET);
			if (FST::execute(fstRightBrace))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_BRACELET, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstLeftThesis(word[i], FST_LEFTTHESIS);
			if (FST::execute(fstLeftThesis))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTTHESIS, LT_TI_NULLIDX, line);
				entryLT.priority = 0;
				LT::Add(lextable, entryLT);
				counteropen++;
				if (idtable.table[indexID - 1].idtype == IT::F)
					findParm = true;
				continue;
			}
			FST::FST fstRightThesis(word[i], FST_RIGHTTHESIS);
			if (FST::execute(fstRightThesis))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_RIGHTTHESIS, LT_TI_NULLIDX, line);
				entryLT.priority = 0;
				counterclose++;
				if (findParm && word[i + 1][0] != LEX_LEFTBRACE && word[i + 2][0] != LEX_LEFTBRACE)		// если после функции нет {
					_mbscpy(prefix, oldprefix);		// возвращаем предыдущую обл. видимости
				findParm = false;
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstEqual(word[i], FST_EQUAL);
			if (FST::execute(fstEqual))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_EQUAL, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			position += _mbslen(word[i]);
			if (word[i][0] == DIV) {
				line++;
				position = 0;
				indxLex--;
				continue;
			}
			throw ERROR_THROW_IN(62, line, position);
		}
		lex.idtable = idtable;
		lex.lextable = lextable;
		return lex;
	}
}