#pragma once
#include "LT.h"
#include "IT.h"
#include "Log.h"
#include <fstream>
#include "Error.h"
#include <vector>
namespace CG
{
	enum typeBlock
	{
		w = 1,
		ut = 2,
		f = 3,
		i = 4,
		ul = 5,
		a = 6
	};
	struct LabelStruct
	{
		std::string _namestruct;
		int _posstruct;
		typeBlock _tbstruct;

		LabelStruct() {};

		LabelStruct(std::string name, int pos, typeBlock tb)
		{
			_namestruct = name;
			_posstruct = pos;
			_tbstruct = tb;
		}
	};
	bool isFunc(int tek, LT::LexTable& lexTable);
	bool checkLib(IT::IdTable& idTable);
	int doEqual(int tek, LT::LexTable& lexTable, IT::IdTable& idTable, Log::LOG log);
	void StartGeneration(LT::LexTable& lexTable, IT::IdTable& idTable, Log::LOG log);
	void addLiterals(IT::IdTable& idTable, Log::LOG log);
	void addData(LT::LexTable& lexTable, IT::IdTable& idTable, Log::LOG log);
	void protImplem(LT::LexTable& lexTable, IT::IdTable& idTable, Log::LOG log);
	int  releaseFun(LT::LexTable& lexTable, IT::IdTable& idTable, Log::LOG log, int pos);

}