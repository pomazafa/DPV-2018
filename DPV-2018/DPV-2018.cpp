#include "stdafx.h"
#include <locale>
#include <cwchar>
#include "Div.h"
#include "LexAnaliz.h"
#include "PolishNotation.h"
#include "SemAnaliz.h"
#include "CG.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		In::IN in = In::getin(parm.in);
		Lex::LEX lex = Lex::lexAnaliz(log, in);

		

		MFST_TRACE_START(log)
			unsigned int start_time = clock();
		MFST::Mfst mfst(lex, GRB::getGreibach());
		mfst.start(log);
		unsigned int end_time = clock();
		unsigned int search_time = end_time - start_time;
		*(log.stream) << search_time << endl;
		mfst.savededucation();
		mfst.printrules(log);
		if (SemanticAnalyze(lex.lextable, in, lex.idtable, log))
			cout << "\nСемантический анализ завершен без ошибок.\n";
		bool rc = startPolishNotation(lex);
		if (rc)
			cout << "Польская запись построена" << endl;
		else
			cout << "Польская запись не построена" << endl;
		IT::showTable(lex.idtable);
		LT::showTable(lex.lextable, log);
		log = Log::getlog(parm.out);
		CG::StartGeneration(lex.lextable, lex.idtable, log);
		LT::Delete(lex.lextable);
		IT::Delete(lex.idtable);
		Log::Close(log);

	}
	catch (Error::ERROR e)
	{
		cout << "Ошибка " <<  e.id << " " << e.message << "\n";
	};
	system("notepad in.txt.log");
	system("pause");
	return 0;
};
