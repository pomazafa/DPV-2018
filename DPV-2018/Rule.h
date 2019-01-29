#pragma once
#include "Greibach.h"
#define GRB_ERROR_SERIES 600
#define NS(n)	GRB::Rule::Chain::N(n)
#define TS(n)	GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)

namespace GRB
{
	Greibach greibach(
		NS('S'), TS('$'),                     // стартовый символ, дно стека
		6,									  // количество правил
		Rule(
			NS('S'), GRB_ERROR_SERIES + 0,    // неверная структура программы
			6,                                // кол-во
			Rule::Chain(10, TS('L'), TS('~'), TS('m'), TS('['), NS('F'), TS('e'), NS('E'), TS('~'), TS(']'), TS('~')),
			Rule::Chain(16, TS('L'), TS('~'), TS('t'), TS('f'), TS('i'), TS('('), NS('P'), TS(')'), TS('['), NS('F'), TS('e'), NS('E'), TS('~'), TS(']'), TS('~'), NS('S')),
			Rule::Chain(15, TS('L'), TS('~'), TS('t'), TS('f'), TS('i'), TS('('), NS('P'), TS(')'), TS('['), NS('F'), TS('e'), NS('E'), TS('~'), TS(']'), TS('~')),
			Rule::Chain(8, TS('m'), TS('['), NS('F'), TS('e'), NS('E'), TS('~'), TS(']'), TS('~')),
			Rule::Chain(14, TS('t'), TS('f'), TS('i'), TS('('), NS('P'), TS(')'), TS('['), NS('F'), TS('e'), NS('E'), TS('~'), TS(']'), TS('~'), NS('S')),
			Rule::Chain(13, TS('t'), TS('f'), TS('i'), TS('('), NS('P'), TS(')'), TS('['), NS('F'), TS('e'), NS('E'), TS('~'), TS(']'), TS('~'))
		),
		Rule(
			NS('F'), GRB_ERROR_SERIES + 1,    // конструкции в функциях
			12,                               // кол-во
			Rule::Chain(5, TS('!'), TS('t'), TS('i'), TS('~'), NS('F')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS('~')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS('~'), NS('F')),
			Rule::Chain(9, TS('!'), TS('t'), TS('f'), TS('i'), TS('('), NS('P'), TS(')'), TS('~'), NS('F')),
			Rule::Chain(4, TS('o'), TS('i'),  TS('~'), NS('F')),
			Rule::Chain(4, TS('o'), TS('l'),  TS('~'), NS('F')),				//F на P
			Rule::Chain(4, TS('o'), NS('E'),  TS('~'), NS('F')),				//N на F
			Rule::Chain(3, TS('o'), NS('E'),  TS('~')),							//M на О
			Rule::Chain(4, TS('!'), TS('t'), TS('i'), TS('~')),
			Rule::Chain(8, TS('!'), TS('t'), TS('f'), TS('i'), TS('('), NS('P'), TS(')'), TS('~')),
			Rule::Chain(3, TS('o'), TS('i'), TS('~')),
			Rule::Chain(3, TS('o'), TS('l'), TS('~'))
		),
		Rule(
			NS('E'), GRB_ERROR_SERIES + 2,    // ошибка в выражении
			8,                                // 
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('i'), NS('O')),
			Rule::Chain(2, TS('l'), NS('O')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('O')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('O'))
		),
		Rule(
			NS('P'), GRB_ERROR_SERIES + 3,    // ошибка в параметрах функции
			2,                                // 
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('P'))
		),
		Rule(
			NS('W'), GRB_ERROR_SERIES + 4,    // ошибка в параметрах вызываемой функции 
			4,                                // 
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		),
		Rule(
			NS('O'), GRB_ERROR_SERIES + 5,    // оператор
			2,								  //
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(3, TS('v'), NS('E'), NS('O'))
		)
	);
}