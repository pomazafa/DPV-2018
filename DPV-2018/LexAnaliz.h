#pragma once
#include "stdafx.h"

namespace Lex
{
	struct LEX
	{
		IT::IdTable idtable;;
		LT::LexTable lextable;
	};

	LEX lexAnaliz(Log::LOG log, In::IN in);
}