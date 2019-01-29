#pragma once
#include "stdafx.h"
#include "LT.h"
#include "IT.h"
#include "Log.h"
#include "In.h"
#include "Error.h"


bool SemanticAnalyze(LT::LexTable &Lextable, In::IN &InStruct, IT::IdTable &idtable, Log::LOG &log);

struct func
{
	int number_of_params;
	int* arr_of_types_of_params;
	unsigned char* name;
	func()
	{
		number_of_params = -1;
	}
};

func* CreateF(int number, int* arr, unsigned char* name);

struct ListId
{
	unsigned char* x;
	ListId *Next, *Head;
};

struct ListFunc
{
	func* x;
	ListFunc *Next, *Head;
};

bool  SemanticAnalyze(LT::LexTable &Lextable, In::IN &in, IT::IdTable &idtable, Log::LOG &log);
void Add(func* x, ListFunc *&MyList);
void ClearList(ListFunc *MyList);
void Add(unsigned char* x, ListId *&MyList);
void ClearList(ListId *MyList);
bool checkId(ListId *MyList, unsigned char* f);
bool Compare(func* f1, func* f2);
bool checkFunc(ListFunc *MyList, func* f);
func* CreateF(int number, int* arr, unsigned char* name);
bool checkLib(unsigned char* t);
