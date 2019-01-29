#pragma once
#define LEXEMA_FIXSIZE	1			// ������������� ������ �������
#define LT_MAXSIZE		4096		// ������������ ���������� ����� � ������� ������
#define LT_TI_NULLIDX	0xfffffff	// ��� �������� ������� ���������������
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

namespace LT		// ������� ������
{
	struct Entry	// ������ ������� ������
	{
		unsigned char lexema;	// �������
		int sn;							// ����� ������ � �������� ������
		int idxTI;						// ������ � ������� ��������������� ��� LT_TI_NULLIDX
		int priority;					// ���������
	};

	struct LexTable						
	{
		int maxsize;					
		int size;						
		Entry* table;					// ������ ����� ������� ������
	};

	LexTable Create(	
		int size		
	);

	void Add(				
		LexTable& lextable,	// ��������� ������� ������
		Entry entry			// ������ ������� ������
	);

	Entry GetEntry(			// �������� ������ ������� ������
		LexTable& lextable,
		int n				
	);

	void Delete(LexTable& lextable);	// ������� ������� ������ 

	Entry writeEntry(					// ��������� ������ ������� ������
		Entry &entry,
		unsigned char lexema,
		int indx,
		int line
	);

	void showTable(LexTable lextable, Log::LOG &log);	// ����� ������� ������
};