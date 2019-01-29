#include "stdafx.h"
#include "LexAnaliz.h"
#include <stack>
#include <queue>

using namespace std;

bool PolishNotation(int i, Lex::LEX& lex)
{
	stack<LT::Entry> stack;		// стек для операций
	queue<LT::Entry> queue;		// очередь для операндов
	LT::Entry temp;		temp.idxTI = -1;	temp.lexema = '#';	temp.sn = -1;	// запрещенная лексема, все лишние элементы будут заменяться на нее
	LT::Entry func;		func.idxTI = -1;	func.lexema = '@';	func.sn = -1;	// лексема для вызова функции
	int countLex = 0;			// количество преобразованных лексем
	int countParm = 0;			// количество параметров функции
	int posLex = i;				// запоминаем номер лексемы перед преобразованием
	bool findFunc = false;		// флаг для функции
	bool findComma = false;		// флаг для запятой (кол-во параметров +2 сразу)
	char* buf = new char[1];	// буфер для countParm в строковом представлении

	for (i; lex.lextable.table[i].lexema != LEX_TILDA; i++, countLex++)
	{
		switch (lex.lextable.table[i].lexema)
		{
		case LEX_ID:	// если идентификатор
		{
			if (lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::F)
				findFunc = true;
			queue.push(lex.lextable.table[i]);
			continue;
		}
		case LEX_LITERAL:		// если литерал
		{
			queue.push(lex.lextable.table[i]);	// помещаем в очередь
			continue;
		}
		case LEX_LEFTTHESIS:		// если (
		{
			stack.push(lex.lextable.table[i]); // помещаем ее в стек
			continue;
		}
		case LEX_RIGHTTHESIS:	// если )
		{
			if (findFunc)		// если это вызов функции, то лексемы () заменяются на @ и кол-во параметров
			{
				if (findComma)
					countParm++;
				else
				{
					if (lex.lextable.table[i - 1].lexema != LEX_LEFTTHESIS)
						countParm++;
				}
				lex.lextable.table[i] = func;
				queue.push(lex.lextable.table[i]);		// добавляем в очередь лексему вызова функции
				_itoa_s(countParm, buf, 2, 10);			// преобразование числа countParm в строку
				stack.top().lexema = buf[0];	stack.top().idxTI = -1; stack.top().sn = -1;	// заполняем лексему, указывающую количество параметров функции
				queue.push(stack.top());				// добавляем в очередь эту лексему
				findFunc = false;
			}
			else {
				while (stack.top().lexema != LEX_LEFTTHESIS)	// пока не встретим (
				{
					queue.push(stack.top());	// выталкиваем со стека в очередь
					stack.pop();

					if (stack.empty())
						return false;
				}
			}
			stack.pop();	// уничтожаем ( или лексему, указывающую количество параметров функции
			continue;
		}
		case LEX_OPERATOR:	// если знак оператора
		{
			while (!stack.empty() && lex.lextable.table[i].priority <= stack.top().priority)	// пока приоритет текущего оператора 
																								// меньше или равен приоритету оператора в вершине стека
			{
				queue.push(stack.top());	// выталкиваем со стека в выходную строку
				stack.pop();
			}
			stack.push(lex.lextable.table[i]);
			continue;
		}
		case LEX_COMMA:	// если запятая
		{
			findComma = true;
			countParm++;
		}
		}
	}
	while (!stack.empty())	// если стек не пустой
	{
		if (stack.top().lexema == LEX_LEFTTHESIS || stack.top().lexema == LEX_RIGHTTHESIS)
			return false;
		queue.push(stack.top());	// выталкиваем все в очередь
		stack.pop();
	}
	while (countLex != 0)		// замена текущего выражения в таблице лексем на выражение в ПОЛИЗ
	{
		if (!queue.empty()) {
			lex.lextable.table[posLex++] = queue.front();
			//cout << lex.idtable.table[queue.front().idxTI].id;	// вывод в консоль
			queue.pop();
		}
		else
		{
			lex.lextable.table[posLex++] = temp;			// может лучше перезаписать таблицу?
		}
		countLex--;
	}

	for (int i = 0; i < posLex; i++)		// восстановление индексов первого вхождения в таблицу лексем у операторов из таблицы идентификаторов
	{
		if (lex.lextable.table[i].lexema == LEX_OPERATOR || lex.lextable.table[i].lexema == LEX_LITERAL)
			lex.idtable.table[lex.lextable.table[i].idxTI].idxfirstLE = i;
	}
	return true;
}

bool startPolishNotation(Lex::LEX& lex)
{
	bool rc = false;
	for (int i = 0; i < lex.lextable.size; i++)
	{
		if (lex.lextable.table[i].lexema == LEX_EQUAL)		// если нашли оператор присваивания
		{
			rc = PolishNotation(i + 1, lex);		// передаем номер следующей за ним лексемы
			if (!rc)
				return rc;
		}
	}
	return rc;
}