#include "stdafx.h"
#include <cstring>
namespace Error
{

	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"),		// код ошибки вне диапазона 0 - ERROR_MAX_ENTRY
		ERROR_ENTRY(1, "Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10),
		ERROR_ENTRY(20, "Параметр -in должен быть задан"),
		ERROR_ENTRY(21, "Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(22), ERROR_ENTRY_NODEF(23), ERROR_ENTRY_NODEF(24), ERROR_ENTRY_NODEF(25),
		ERROR_ENTRY_NODEF(26), ERROR_ENTRY_NODEF(27), ERROR_ENTRY_NODEF(28),  ERROR_ENTRY_NODEF(29),
		ERROR_ENTRY_NODEF10(30),
		ERROR_ENTRY(40, "Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(41, "Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(42, "Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY_NODEF(43), ERROR_ENTRY_NODEF(44), ERROR_ENTRY_NODEF(45),
		ERROR_ENTRY_NODEF(46), ERROR_ENTRY_NODEF(47), ERROR_ENTRY_NODEF(48),  ERROR_ENTRY_NODEF(49),
		ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY(60, "Превышен максимальный размер таблицы лексем"),
		ERROR_ENTRY(61, "Таблица лексем переполнена"),
		ERROR_ENTRY(62, "Ошибка лексического анализа"),
		ERROR_ENTRY(63, "Ошибка с комментарием"),
		ERROR_ENTRY(64, "Превышен максимльный размер таблицы идентификаторов"),
		ERROR_ENTRY(65, "Таблица идентификаторов переполнена"),
		ERROR_ENTRY_NODEF(66), ERROR_ENTRY_NODEF(67), ERROR_ENTRY_NODEF(68),  ERROR_ENTRY_NODEF(69),
		ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY_NODEF10(100), ERROR_ENTRY_NODEF10(110), ERROR_ENTRY_NODEF10(120),
		ERROR_ENTRY(130, "Ошибка: переменная неизвестного типа"),
		ERROR_ENTRY(131, "Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY(132, "Ошибка формата, присвоение переменной другого типа данных"),
		ERROR_ENTRY(133, ""),
		ERROR_ENTRY(134, "Тип возвращаемого параметра функции не совпадает с параметром при объявлении"),
		ERROR_ENTRY(135, "Ошибка в функции стандартной библиотеки"),
		ERROR_ENTRY(136, "Использование необъявленной переменной"),
		ERROR_ENTRY(137, "Излишнее подключение стандартной библиотеки"),
		ERROR_ENTRY(138, "Объявлены функции с одинаковым именем"),
		ERROR_ENTRY(139, "Деление на 0"),
		ERROR_ENTRY(140, "Операции с недопустимым типом"),
		ERROR_ENTRY(141, "Использование функций вывода из стандартной библиотеки без её подключения"),
		ERROR_ENTRY_NODEF(142),
		ERROR_ENTRY_NODEF(143), ERROR_ENTRY_NODEF(144), ERROR_ENTRY_NODEF(145),
		ERROR_ENTRY_NODEF(146), ERROR_ENTRY_NODEF(147), ERROR_ENTRY_NODEF(148),  ERROR_ENTRY_NODEF(149),
		ERROR_ENTRY_NODEF10(150), ERROR_ENTRY_NODEF10(160),
		ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "Неверная структура программы"),
		ERROR_ENTRY(601, "Ошибочный оператор"),
		ERROR_ENTRY(602, "Ошибка в выражении"),
		ERROR_ENTRY(603, "Ошибка в параметрах функции"),
		ERROR_ENTRY(604, "Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY_NODEF(605),
		ERROR_ENTRY_NODEF(606), ERROR_ENTRY_NODEF(607), ERROR_ENTRY_NODEF(607), ERROR_ENTRY_NODEF(609),
		ERROR_ENTRY_NODEF10(610), ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640),
		ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800),
		ERROR_ENTRY_NODEF100(900)
	};

	ERROR geterror(int id)
	{
		ERROR er;
		if (id < 0 || id > ERROR_MAX_ENTRY)
			return errors[0];
		er.id = id;
		strcpy_s(er.message, errors[id].message);
		return er;
	}

	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		ERROR er;
		if (id < 0 || id > ERROR_MAX_ENTRY)
			return errors[0];
		er.id = id;
		er.inext.line = line;
		er.inext.col = col;
		strcpy_s(er.message, errors[id].message);
		return er;
	}
};