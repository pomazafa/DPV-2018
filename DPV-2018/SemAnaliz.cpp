#include "stdafx.h"
#include "SemAnaliz.h"


void Add(func* x, ListFunc *&MyList)                     
{
	ListFunc *temp = new ListFunc;                           
	temp->x = x;                                     
	temp->Next = MyList->Head;                       
	MyList->Head = temp;                             
}

void ClearList(ListFunc *MyList)
{
	while (MyList->Head != NULL)
	{
		ListFunc *temp = MyList->Head->Next;
		delete MyList->Head;
		MyList->Head = temp;
	}
}

void Add(unsigned char* x, ListId *&MyList)
{
	ListId *temp = new ListId;
	temp->x = x;
	temp->Next = MyList->Head;
	MyList->Head = temp;
}

void ClearList(ListId *MyList)
{
	while (MyList->Head != NULL)
	{
		ListId *temp = MyList->Head->Next;
		delete MyList->Head;
		MyList->Head = temp;
	}
}

bool checkId(ListId *MyList, unsigned char* f)
{
	ListId *temp = MyList->Head;
	bool find = false;
	int co = 0;
	while (temp != NULL)
	{
		int k = 0;
		co = 0;
		for (k = 0; *(temp->x + k) != '\0' && *(f + k) != '\0'; k++)
		{
			if (*(temp->x + k) == *(f + k))
			{
				co++;
			}
		}
		if (co == k)
		{
			find = true;
			break;
		}
		temp = temp->Next;
	}
	return find;
}

bool Compare(func* f1, func* f2)
{
	if (f1->name != f2->name)
		return false;
	if (f1->number_of_params != f2->number_of_params)
		return false;
	for (int i = 0; i < f1->number_of_params; i++)
	{
		if (f1->arr_of_types_of_params[i] != f2->arr_of_types_of_params[i])
			return false;
	}
	return true;
}

bool checkFunc(ListFunc *MyList, func* f)             
{
	ListFunc *temp = MyList->Head;                    
	bool find = false;								
	while (temp != NULL)                          
	{
		if (Compare(temp->x, f))
		{
			if (find)
			{
				throw ERROR_THROW(138);
			}
			find = true;
		}
		temp = temp->Next;
	}
	return find;
}

func* CreateF(int number, int* arr, unsigned char* name)
{
	func* f = new func();
	f->number_of_params = number;
	f->arr_of_types_of_params = new int[number];
	for (int i = 0; i < number; i++)
		f->arr_of_types_of_params[i] = arr[i];
	f->name = name;
	return f;
}

bool checkLib(unsigned char* t)
{
	bool z = false;
	unsigned char strlen[] = "strlen";
	unsigned char substr[] = "substr";
	unsigned char getnod[] = "getnod";
	unsigned char getnok[] = "getnok";
	unsigned char* libFunctions[] = { strlen, substr, getnod, getnok, NULL };

	for (int i = 0; libFunctions[i] != NULL; i++)
	{
		int count = 0;
		int k;
		for (k = 0; *((libFunctions[i]) + k) != '\0' && *(t + k) != '\0'; k++)
		{
			if (*((libFunctions[i]) + k) == *(t + k))
				count++;
		}

		if (count == k)
		{
			z = true;
			break;
		}
	}									
	return z;
}

bool  SemanticAnalyze(LT::LexTable &Lextable, In::IN &in, IT::IdTable &idtable, Log::LOG &log)
{
	bool stlib = false;
	int params[10];
	ListFunc *Functions = new ListFunc;
	ListId *Ids = new ListId;
	Ids->Head = NULL;
	Functions->Head = NULL;
	unsigned char* name;
	int counter = 0;
	int type = 0;


	for (int i = 0; i < idtable.size; i++)								//проверка на переменную неопознанного типа + на кол-во подключения библиотек
	{
		if (idtable.table[i].idtype != IT::V && idtable.table[i].idtype != IT::F
			&& idtable.table[i].idtype != IT::P && idtable.table[i].idtype != IT::L
			&& idtable.table[i].idtype != IT::OP && idtable.table[i].idtype != IT::LIB)
			throw ERROR_THROW(130);
		if (stlib && idtable.table[i].idtype == IT::LIB)
			throw ERROR_THROW(137);
		if (idtable.table[i].idtype == IT::LIB)
			stlib = true;
	}

	for (int i = 0; i < Lextable.size; i++)					//проверка на исп-ние out без подключения библиотеки
	{
		if (Lextable.table[i].lexema == LEX_OUT && !stlib)
		{
			throw ERROR_THROW(141);
		}
	}
	counter = 0;

	for (int i = 0; i < Lextable.size; i++)					//проверка типов данных при присваивании
	{
		if (Lextable.table[i].lexema == LEX_EQUAL)
		{
			type = idtable.table[Lextable.table[i - 1].idxTI].iddatatype;
			i++;
			while (Lextable.table[i].lexema != LEX_TILDA)
			{
				if (Lextable.table[i].lexema == LEX_ID || Lextable.table[i].lexema == LEX_LITERAL)
				{
					if (idtable.table[Lextable.table[i].idxTI].idtype == IT::F)
					{
						if (idtable.table[Lextable.table[i].idxTI].iddatatype != type)
						{
							throw ERROR_THROW(132);
							break;
						}
						while (Lextable.table[i].lexema != LEX_RIGHTTHESIS)
							i++;
						continue;
					}
					if (idtable.table[Lextable.table[i].idxTI].iddatatype != type)
					{
						throw ERROR_THROW(132);
						break;
					}
				}
				i++;
			}
		}
	}

	for (int i = 0; i < Lextable.size; i++)
	{
		if (Lextable.table[i].lexema == LEX_FUNCTION)						//ищет все прототипы функций
		{
			int index = Lextable.table[i + 1].idxTI;
			name = idtable.table[index].id;
			int k = i + 2;
			counter = 0;
			while (Lextable.table[k].lexema != LEX_RIGHTTHESIS)
			{
				if (Lextable.table[k].lexema != LEX_COMMA && Lextable.table[k].lexema != LEX_LEFTTHESIS)
				{
					index = Lextable.table[k + 1].idxTI;
					params[counter] = idtable.table[index].iddatatype;
					counter++;
					k++;
				}
				k++;
			}
			func* f = CreateF(counter, params, name);					//создаем структуру с описанием прототипа функции
			Add(f, Functions);
		}
																//проверка деления на 0 + проверка на операции с НЕ целочисл. переменными
		if (Lextable.table[i].lexema == LEX_OPERATOR)
		{
			if (!strcmp((char*)idtable.table[Lextable.table[i].idxTI].id, "/"))
			{
				if (Lextable.table[i + 1].lexema != LEX_LEFTTHESIS)
				{
					if (idtable.table[Lextable.table[i + 1].idxTI].value.vint == 0)
					{
						throw ERROR_THROW(139);
					}
				}
			}

			for (int j = i; Lextable.table[j].lexema != LEX_TILDA; j++)
			{
				if (Lextable.table[j].idxTI < idtable.size)
				{
					if(idtable.table[Lextable.table[j].idxTI].iddatatype != IT::INT && idtable.table[Lextable.table[j].idxTI].idtype != IT::P && idtable.table[Lextable.table[j].idxTI].idtype != IT::OP)
						throw ERROR_THROW(140);
				}
			}
		}
	}

	int typeInit;
	int typeExit;
	for (int i = 0; i < Lextable.size; i++)					//проверка возвращаемого значения функции
	{
		if (Lextable.table[i].lexema == LEX_FUNCTION && !(stlib && checkLib(idtable.table[Lextable.table[i + 1].idxTI].id)))
		{																		//проверка функций НЕ стандартной библиотеки
			if (Lextable.table[i].lexema == LEX_FUNCTION && checkLib(idtable.table[Lextable.table[i + 1].idxTI].id) && !stlib)
				throw ERROR_THROW(135);
			typeInit = idtable.table[Lextable.table[i + 1].idxTI].iddatatype;	//тип при инициализации
			int k = i;
			while (Lextable.table[k].lexema != LEX_EXIT)			
			{																		
				k++;
			}

			typeExit = idtable.table[Lextable.table[k + 1].idxTI].iddatatype;	//тип возвращаемого значения

			if (typeInit != typeExit)
			{
				throw ERROR_THROW(134);
				break;
			}
		}
		else
		{
			if (Lextable.table[i].lexema == LEX_FUNCTION && (stlib && checkLib(idtable.table[Lextable.table[i + 1].idxTI].id)))	//если полключена библиотека и 
			{																	//проверяем стандартную функцию
				unsigned char strlen[] = "strlen";
				unsigned char substr[] = "substr";
				unsigned char getnod[] = "getnod";
				unsigned char getnok[] = "getnok";
				unsigned char* libFunctions[] = { strlen, substr, getnod, getnok, NULL };
				bool find = false;
				int id = -1;
				for (int i3 = 0; libFunctions[i3] != NULL; i3++)
				{
					int count = 0;
					int k;
					for (k = 0; *((libFunctions[i3]) + k) != '\0' && *(idtable.table[Lextable.table[i + 1].idxTI].id + k) != '\0'; k++)
					{
						if (*((libFunctions[i3]) + k) == *(idtable.table[Lextable.table[i + 1].idxTI].id + k))
							count++;
					}
					if (count == k)
					{
						find = true;
						id = i3;
						break;
					}
				}
				if (!find)
				{
					throw ERROR_THROW(135);
				}
				else
				{
					switch (id)
					{
					case 0:								//strlen
						if (idtable.table[Lextable.table[i + 1].idxTI].iddatatype != IT::INT)
							throw ERROR_THROW(134);
						break;

					case 1:								//substr
						if (idtable.table[Lextable.table[i + 1].idxTI].iddatatype != IT::IS)
							throw ERROR_THROW(134);
						break;
					case 2:								//getnod
					case 3:								//getnok
						if (idtable.table[Lextable.table[i + 1].idxTI].iddatatype != IT::INT)
							throw ERROR_THROW(134);
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < Lextable.size; i++)					//проверка использования необъявленной переменной
	{
		if (Lextable.table[i].lexema == LEX_DECL)									//заполняем лист объявленными идентификаторами
		{
			if (Lextable.table[i + 2].lexema != LEX_FUNCTION)
			{
				Add(idtable.table[Lextable.table[i + 2].idxTI].id, Ids);
			}
		}

		if (Lextable.table[i].lexema == LEX_FUNCTION)
		{
			i++;			//сдвиг на название функции
			i++;			//сдвиг на (
			while (Lextable.table[i].lexema != LEX_RIGHTTHESIS)
			{
				if (Lextable.table[i].lexema == LEX_ID)
					Add(idtable.table[Lextable.table[i].idxTI].id, Ids);
				i++;
			}
		}

		int count;
		if (Lextable.table[i].idxTI < idtable.size)
		{
			if (idtable.table[Lextable.table[i].idxTI].idtype == IT::F)
			{
				if (Lextable.table[i - 1].lexema != LEX_FUNCTION)		//если это не объявление функции
				{
					name = idtable.table[Lextable.table[i].idxTI].id;
					i++;
					count = 0;
					while (Lextable.table[i].lexema != LEX_RIGHTTHESIS)
					{
						if (Lextable.table[i].idxTI < idtable.size)		//если идентификатор
						{

							
							params[count] = idtable.table[Lextable.table[i].idxTI].iddatatype;
							count++;

						}
						i++;
					}
					func* fcheck = CreateF(count, params, name);
					if (!checkFunc(Functions, fcheck))
					{
						throw ERROR_THROW(131);
					}
				}
			}
		}
	}

	for (int i = 0; i < idtable.size; i++)
	{
		if (idtable.table[i].idtype != IT::OP && idtable.table[i].idtype != IT::LIB && idtable.table[i].idtype != IT::F && idtable.table[i].idtype != IT::L)
		{
			if (!(checkId(Ids, idtable.table[i].id)))
			{
				throw ERROR_THROW(136);				
			}
		}
	}


	ClearList(Ids);
	ClearList(Functions);
	return true;
}