#include "stdafx.h"
#include "CG.h"

#define OUT *log.stream

void CG::StartGeneration(LT::LexTable & lexTable, IT::IdTable & idTable, Log::LOG log)
{
	OUT << ".586\n.model FLAT, C , stdcall\nincludelib libucrt.lib \n";
	if (checkLib(idTable))
		OUT << "includelib StaticLib.lib\n";
	OUT << "includelib kernel32.lib \nExitProcess PROTO : DWORD \n.stack 4096\noutputStr PROTO : DWORD\noutputIs PROTO : DWORD\noutputInt PROTO : DWORD\nissubstr PROTO : DWORD, : DWORD\ngetlength PROTO : DWORD\ngetnod PROTO : DWORD, : DWORD\ngetnok PROTO : DWORD, : DWORD\n";
	addLiterals(idTable, log);
	addData(lexTable, idTable, log);
	protImplem(lexTable, idTable, log);
}

bool CG::isFunc(int tek, LT::LexTable& lexTable)
{
	while (lexTable.table[tek].lexema != '@')
	{
		if (lexTable.table[tek].lexema == LEX_TILDA || lexTable.table[tek].lexema == '#' || lexTable.table[tek].lexema == LEX_OPERATOR)
		{
			return false;
		}

		tek++;
	}
	return true;
}

bool CG::checkLib(IT::IdTable& idTable)
{
	for (int i = 0; i < idTable.size; i++)
	{
		if (idTable.table[i].idtype == IT::LIB)
			return true;
	}
	return false;
}

void CG::addLiterals(IT::IdTable& idTable, Log::LOG log)
{
	OUT << ".CONST\n";
	int numint = 0;
	int numstr = 0;
	int numfl = 0;
	int numbl = 0;
	for (int i = 0; i < idTable.size; ++i)
	{
		if (idTable.table[i].idtype == IT::IDTYPE::L)
		{
			switch (idTable.table[i].iddatatype)
			{
			case IT::INT:
				OUT << idTable.table[i].id << " DWORD " << idTable.table[i].value.vint << "\n";
				break;
			case IT::STR:
				OUT << idTable.table[i].id << " DB \"" << idTable.table[i].value.vstr.str << "\", 0\n";
				break;
			case IT::IS:
				OUT << idTable.table[i].id << " DWORD " << idTable.table[i].value.vis << "\n";
				break;
			}
		}
	}
}

void CG::addData(LT::LexTable& lexTable, IT::IdTable & idTable, Log::LOG log)
{
	OUT << ".data\n";
	for (int i = 0; i < idTable.size; ++i)
	{
		/*if (idTable.table[i].idtype == IT::IDTYPE::F)
		{
			OUT << idTable.table[i].id 
		}*/
		if (idTable.table[i].idtype == IT::IDTYPE::V)
		{
			switch (idTable.table[i].iddatatype)
			{
			case IT::INT:
				OUT << idTable.table[i].id << " DWORD 0\n";
				break;
			case IT::STR:
				OUT << idTable.table[i].id << " DB 255 dup(0)\n";
				break;
			case IT::IS:
				OUT << idTable.table[i].id << " DWORD 0\n";
				break;
			}
		}
	}
}

void CG::protImplem(LT::LexTable & lexTable, IT::IdTable & idTable, Log::LOG log)
{

	OUT << ".CODE\n";
	for (int i = 0; i < lexTable.size; i++)
	{
		if (lexTable.table[i].lexema == LEX_MAIN) {
			OUT << "main PROC\n";
			releaseFun(lexTable, idTable, log, i);
			OUT << "MAIN ENDP\n";
			OUT << "end main\n";
		}

		if (lexTable.table[i].lexema == LEX_FUNCTION) {
			if (!strcmp((char*)idTable.table[lexTable.table[i + 1].idxTI].id, "strlen") ||
				!strcmp((char*)idTable.table[lexTable.table[i + 1].idxTI].id, "substr") ||
				!strcmp((char*)idTable.table[lexTable.table[i + 1].idxTI].id, "getnod") ||
				!strcmp((char*)idTable.table[lexTable.table[i + 1].idxTI].id, "getnok")) continue;
			int posFun = i;
			OUT << idTable.table[lexTable.table[i + 1].idxTI].id << " PROC ";
			i = i + 2;
			while (lexTable.table[i].lexema != LEX_RIGHTTHESIS)
			{
				if ((lexTable.table[i].idxTI) != LT_TI_NULLIDX)
				{
					switch (idTable.table[lexTable.table[i].idxTI].iddatatype)
					{
					case IT::INT: OUT << idTable.table[lexTable.table[i].idxTI].id << " : DWORD";
						if (lexTable.table[i + 1].lexema == LEX_COMMA)
							OUT << ", ";
						break;
					case IT::STR: OUT << idTable.table[lexTable.table[i].idxTI].id << " : OFFSET DWORD";
						if (lexTable.table[i + 1].lexema == LEX_COMMA)
							OUT << ", ";
						break;
					case IT::IS: OUT << idTable.table[lexTable.table[i].idxTI].id << " : DWORD";
						if (lexTable.table[i + 1].lexema == LEX_COMMA)
						OUT << ", ";
					default:
						break;
					}
				}
				i++;
			}
			OUT << "\n";
			releaseFun(lexTable, idTable, log, i);
			OUT << idTable.table[lexTable.table[posFun+1].idxTI].id << " ENDP\n";
			//ÄÎÏÈÑÀÒÜ ÄËß ÑÒÀÍÄÀÐÒÍÛÕ ÔÓÍÊÖÈÉ
		}

	}
}

int CG::releaseFun(LT::LexTable & lexTable, IT::IdTable & idTable, Log::LOG log, int pos)
{
	int tek;
	char buffint[20];
	for (int i = pos; ; i++)
	{
		if (lexTable.table[i].lexema == LEX_BRACELET || i + 1 == lexTable.size)
		{
			if (i + 2 == lexTable.size)
			{
				OUT << "PUSH 0\nCALL ExitProcess\n";		//ÝÒÎ ÍÅÏÐÀÂÈËÜÍÎ
			}																					//ÄÎÏÈÑÀÒÜ ÇÀÂÅÐØÅÍÈÅ
			else
			{
				OUT << "ret\n";
			}
			return i - 1;
		}
		switch (lexTable.table[i].lexema)
		{
		case LEX_EQUAL:
		{
			//òóò äîïèñàòü äëÿ is åñëè õîòèì îïåðàöèè ñ íèìè
			tek = lexTable.table[i - 1].idxTI;
			i = doEqual(i + 1, lexTable, idTable, log);
			if(idTable.table[tek].iddatatype != IT::STR)
				OUT << "POP " << idTable.table[tek].id << "\n";
			break;
		}
		case LEX_OUT:
		{
			
			switch (idTable.table[lexTable.table[i+1].idxTI].iddatatype)
			{
			case IT::IDDATATYPE::INT:
				OUT << "PUSH " << idTable.table[lexTable.table[i + 1].idxTI].id << "\n";
				OUT << "CALL outputInt \n";
					break;
			case IT::IDDATATYPE::STR:
				OUT << "PUSH OFFSET " << idTable.table[lexTable.table[i + 1].idxTI].id << "\n";
				OUT << "CALL outputStr\n";
					break;
			case IT::IDDATATYPE::IS:
				OUT << "PUSH " << idTable.table[lexTable.table[i + 1].idxTI].id << "\n";
				OUT << "CALL outputIs\n";
					break;
			}
			break;
		}
		case LEX_EXIT:
		{
			//i = doEqual(++i, lexTable, idTable, log);
			OUT << "MOV EAX, " << idTable.table[lexTable.table[i + 1].idxTI].id  << "\n";
			break;
		}
		}
	}
	return 0;
}


int CG::doEqual(int tek, LT::LexTable& lexTable, IT::IdTable& idTable, Log::LOG log)
{
	int i = tek;
	if (idTable.table[lexTable.table[tek].idxTI].iddatatype != IT::STR)
	{
		while (lexTable.table[i].lexema != LEX_TILDA && lexTable.table[i].lexema != '#')
		{
			if (lexTable.table[i].lexema == '@')
			{
				int cparm = lexTable.table[i + 1].lexema - '0';
				int j = i - 1;
				for (int g = 0; g < cparm; g++)
				{
					if (lexTable.table[j].lexema == LEX_LITERAL || lexTable.table[j].lexema == LEX_ID)
					{
						if(idTable.table[lexTable.table[j].idxTI].iddatatype == IT::STR)
							OUT << "PUSH OFFSET " << idTable.table[lexTable.table[j].idxTI].id << "\n";
						else 
							OUT << "PUSH " << idTable.table[lexTable.table[j].idxTI].id << "\n";
						j--;
					}
				}
				int posFun = j;
				if (!strcmp((char*)idTable.table[lexTable.table[posFun].idxTI].id, "strlen"))
					OUT << "CALL getlength\n";
				else
				{
					if (!strcmp((char*)idTable.table[lexTable.table[posFun].idxTI].id, "substr"))
						OUT << "CALL issubstr\n";
					else
						OUT << "CALL " << idTable.table[lexTable.table[posFun].idxTI].id << "\n";
				}
				OUT << "PUSH EAX\n";
			}
			if ((lexTable.table[i].lexema == LEX_LITERAL || lexTable.table[i].lexema == LEX_ID) && !isFunc(i, lexTable))
			{
				OUT << "PUSH " << idTable.table[lexTable.table[i].idxTI].id << "\n";
			}
			if (lexTable.table[i].lexema == LEX_OPERATOR)
			{
				switch (idTable.table[lexTable.table[i].idxTI].id[0])
				{
				case '+':
				{
					OUT << "POP EDX\n";
					OUT << "POP EBX\n";
					OUT << "ADD EBX, EDX\n";
					OUT << "PUSH EBX\n";
					break;
				}
				case '-':
				{
					OUT << "POP EDX\n";
					OUT << "POP EBX\n";
					OUT << "SUB EBX, EDX\n";
					OUT << "PUSH EBX\n";
					break;
				}
				case '*':
				{
					OUT << "POP EBX\n";
					OUT << "POP EAX\n";
					OUT << "MOV EDX, 0\n";
					OUT << "MUL EBX\n";
					OUT << "MOV EBX, EAX\n";
					OUT << "PUSH EBX\n";
					break;
				}
				case '/':
				{
					OUT << "POP EBX\n";
					OUT << "POP EAX\n";
					OUT << "MOV EDX, 0\n";
					OUT << "DIV EBX\n";
					OUT << "MOV EBX, EAX\n";
					OUT << "PUSH EBX\n";
					break;
				}
				}
			}
			i++;
		}
	}
	else
	{
		//âûðàæåíèå äëÿ ñòðîê
		
		int numID = i-2;//idTable.table[lexTable.table[i - 2].idxTI].idxfirstLE;
		int countSizeOfStr;
		char buffint[20];
		for (;; i++)
		{
			if (lexTable.table[i].lexema == LEX_TILDA) break;
			if (lexTable.table[i].lexema == LEX_ID || lexTable.table[i].lexema == LEX_LITERAL)
			{

				if (idTable.table[lexTable.table[i].idxTI].idtype == IT::IDTYPE::P)
				{
					OUT << "MOV ESI, " << idTable.table[lexTable.table[i].idxTI].id << "\n";
				}
				else OUT << "MOV ESI, OFFSET " << idTable.table[lexTable.table[i].idxTI].id << "\n";
				OUT << "MOV EDI, OFFSET " << idTable.table[lexTable.table[numID].idxTI].id  << "\n";
				countSizeOfStr = idTable.table[lexTable.table[i].idxTI].value.vstr.len;
				_itoa(idTable.table[lexTable.table[i].idxTI].value.vstr.len + 1, buffint, 10);
				OUT << "MOV ECX, " << buffint << "\n";
				OUT <<  "REP MOVSB\n";
				idTable.table[lexTable.table[numID].idxTI].value.vstr.len = countSizeOfStr;
			}
			/*if (lexTable.table[i].lexema == LEX_LITERAL)
			{
				IT::retIDlit(GETIDFROMLT(i), idname1);
				OUT << MOV(EMPTY, "ESI", "OFFSET ", idname1) << ENDL
					OUT << PMOV("EDI", "OFFSET ", idname2, countSizeOfStr) << ENDL;
				countSizeOfStr += GETIDFROMLT(i).value.vstr.len;
				_itoa(GETIDFROMLT(i).value.vstr.len + 1, buffInt, 10);
				OUT << MOV(EMPTY, "ECX", EMPTY, buffInt) << ENDL
					OUT << REPMOVSB << ENDL
					GETIDFROMLT(numID).value.vstr.len = countSizeOfStr;
			}*/
			
		}
	}
	return i;
}