#include "stdafx.h"
#include <cstring>
namespace Error
{

	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "������������ ��� ������"),		// ��� ������ ��� ��������� 0 - ERROR_MAX_ENTRY
		ERROR_ENTRY(1, "��������� ����"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10),
		ERROR_ENTRY(20, "�������� -in ������ ���� �����"),
		ERROR_ENTRY(21, "��������� ����� �������� ���������"),
		ERROR_ENTRY_NODEF(22), ERROR_ENTRY_NODEF(23), ERROR_ENTRY_NODEF(24), ERROR_ENTRY_NODEF(25),
		ERROR_ENTRY_NODEF(26), ERROR_ENTRY_NODEF(27), ERROR_ENTRY_NODEF(28),  ERROR_ENTRY_NODEF(29),
		ERROR_ENTRY_NODEF10(30),
		ERROR_ENTRY(40, "������ ��� �������� ����� � �������� ����� (-in)"),
		ERROR_ENTRY(41, "������������ ������ � �������� ����� (-in)"),
		ERROR_ENTRY(42, "������ ��� �������� ����� ��������� (-log)"),
		ERROR_ENTRY_NODEF(43), ERROR_ENTRY_NODEF(44), ERROR_ENTRY_NODEF(45),
		ERROR_ENTRY_NODEF(46), ERROR_ENTRY_NODEF(47), ERROR_ENTRY_NODEF(48),  ERROR_ENTRY_NODEF(49),
		ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY(60, "�������� ������������ ������ ������� ������"),
		ERROR_ENTRY(61, "������� ������ �����������"),
		ERROR_ENTRY(62, "������ ������������ �������"),
		ERROR_ENTRY(63, "������ � ������������"),
		ERROR_ENTRY(64, "�������� ����������� ������ ������� ���������������"),
		ERROR_ENTRY(65, "������� ��������������� �����������"),
		ERROR_ENTRY_NODEF(66), ERROR_ENTRY_NODEF(67), ERROR_ENTRY_NODEF(68),  ERROR_ENTRY_NODEF(69),
		ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY_NODEF10(100), ERROR_ENTRY_NODEF10(110), ERROR_ENTRY_NODEF10(120),
		ERROR_ENTRY(130, "������: ���������� ������������ ����"),
		ERROR_ENTRY(131, "������ � ���������� ���������� �������"),
		ERROR_ENTRY(132, "������ �������, ���������� ���������� ������� ���� ������"),
		ERROR_ENTRY(133, ""),
		ERROR_ENTRY(134, "��� ������������� ��������� ������� �� ��������� � ���������� ��� ����������"),
		ERROR_ENTRY(135, "������ � ������� ����������� ����������"),
		ERROR_ENTRY(136, "������������� ������������� ����������"),
		ERROR_ENTRY(137, "�������� ����������� ����������� ����������"),
		ERROR_ENTRY(138, "��������� ������� � ���������� ������"),
		ERROR_ENTRY(139, "������� �� 0"),
		ERROR_ENTRY(140, "�������� � ������������ �����"),
		ERROR_ENTRY(141, "������������� ������� ������ �� ����������� ���������� ��� � �����������"),
		ERROR_ENTRY_NODEF(142),
		ERROR_ENTRY_NODEF(143), ERROR_ENTRY_NODEF(144), ERROR_ENTRY_NODEF(145),
		ERROR_ENTRY_NODEF(146), ERROR_ENTRY_NODEF(147), ERROR_ENTRY_NODEF(148),  ERROR_ENTRY_NODEF(149),
		ERROR_ENTRY_NODEF10(150), ERROR_ENTRY_NODEF10(160),
		ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "�������� ��������� ���������"),
		ERROR_ENTRY(601, "��������� ��������"),
		ERROR_ENTRY(602, "������ � ���������"),
		ERROR_ENTRY(603, "������ � ���������� �������"),
		ERROR_ENTRY(604, "������ � ���������� ���������� �������"),
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