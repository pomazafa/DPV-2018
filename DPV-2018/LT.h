#pragma once
#define LEXEMA_FIXSIZE	1			// фиксированный размер лексемы
#define LT_MAXSIZE		4096		// максимальное количество строк в таблице лексем
#define LT_TI_NULLIDX	0xfffffff	// нет элемента таблицы идентификаторов
#define LEX_INTEGER		't'	
#define LEX_STRING		't'	
#define LEX_IS			't' 
#define LEX_ID			'i'	
#define LEX_LITERAL		'l'	
#define LEX_FUNCTION	'f'	
#define LEX_DECL		'!'	
#define LEX_EXIT		'e'	
#define LEX_OUT			'o'	
#define LEX_MAIN		'm'	
#define LEX_TILDA		'~'	
#define LEX_COMMA		','	
#define LEX_LEFTBRACE	'['	
#define LEX_BRACELET	']'	
#define LEX_LEFTTHESIS	'('	
#define LEX_RIGHTTHESIS	')'	
#define LEX_PLUS		'v'	
#define LEX_MINUS		'v'	
#define LEX_STAR		'v'	
#define LEX_DIRSLASH	'v'	
#define LEX_OPERATOR	'v'	
#define LEX_EQUAL		'='
#define LEX_LIB			'L'

namespace LT		// таблица лексем
{
	struct Entry	// строка таблицы лексем
	{
		unsigned char lexema;	// лексема
		int sn;							// номер строки в исходном тексте
		int idxTI;						// индекс в таблице идентификаторов или LT_TI_NULLIDX
		int priority;					// приоритет
	};

	struct LexTable						
	{
		int maxsize;					
		int size;						
		Entry* table;					// массив строк таблицы лексем
	};

	LexTable Create(	
		int size		
	);

	void Add(				
		LexTable& lextable,	// экземпляр таблицы лексем
		Entry entry			// строка таблицы лексем
	);

	Entry GetEntry(			// получить строку таблицы лексем
		LexTable& lextable,
		int n				
	);

	void Delete(LexTable& lextable);	// удалить таблицу лексем 

	Entry writeEntry(					// заполнить строку таблицы лексем
		Entry &entry,
		unsigned char lexema,
		int indx,
		int line
	);

	void showTable(LexTable lextable, Log::LOG &log);	// вывод таблицы лексем
};