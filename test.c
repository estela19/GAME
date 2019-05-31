/*
	����:

	- �� �ڵ� ������ �츮 ���� ǥ�� ���α׷��� ȯ�濡�� �� �۵��ϵ��� �Ǿ� �־��.
	  �ٸ� ȯ���� ����ϴ� ģ���鿡�Դ� ���� �̾��ؿ�.

	- �� ������ ������ '�츮 ������ ���� â'�� �ſ� Ưȭ�Ǿ� ������ ������ ���� �ʴ� �� �� ���ƿ�.
	  (�Ĵٺ��⵵ �ȵ��� �Ϻη� ������ �ڵ� ���� �����)
	  ��� console.h�� �ִ� �ּ� ������� �о� ���鼭 ���̺귯���� �ڽ��� �ǵ��� �°� �����ϵ��� ������ ������.
*/

#include "console.h"

#include <stdio.h>

void InputTest()
{
	int count = 9999;
	int row = 0;

	Start_Console();

	Stopwatch_Start();
	
	while ( count )
	{
		Accept_Inputs();

		while ( Can_Read() )
		{
			struct Info_Input info;

			Read_Input(&info);

			if ( info.type & TypeCode_Key )
			{
				if ( row == INIT_CONSOLE_HEIGHT )
				{
					Clear_Screen();
					row = 0;
				}

				Move_Cursor(0, row);

				if ( info.type == TypeCode_Key_Press )
					printf("Key press  : code - 0x%x(\'%s\') | flags -", info.code, Get_KeyName(info.code));
				else
					printf("Key release: code - 0x%x(\'%s\') | flags -", info.code, Get_KeyName(info.code));

				if ( info.ctrl == 0 )
					printf(" (none)");

				if ( info.ctrl & Code_Right_Alt_Pressed )
					printf(" RAlt");

				if ( info.ctrl & Code_Left_Alt_Pressed )
					printf(" LAlt");

				if ( info.ctrl & Code_Right_Ctrl_Pressed )
					printf(" RCtrl");

				if ( info.ctrl & Code_Left_Ctrl_Pressed )
					printf(" LCtrl");

				if ( info.ctrl & Code_Shift_Pressed )
					printf(" Shift");

				if ( info.ctrl & Code_NumLock_On )
					printf(" NumLock");

				if ( info.ctrl & Code_ScrollLock_On )
					printf(" ScrollLock");

				if ( info.ctrl & Code_CapsLock_On )
					printf(" CapsLock");

				if ( info.ctrl & Code_Enhanced_Key )
					printf(" Enhanced");
			}
			else
			{
				struct Point pos;

				pos = Get_Mouse_Pos();

				if ( row == INIT_CONSOLE_HEIGHT )
				{
					Clear_Screen();
					row = 0;
				}

				Move_Cursor(0, row);

				switch ( info.type )
				{
				case TypeCode_Mouse:
					printf("Mouse click: code - %d | pos - (%d, %d)", info.code, pos.x, pos.y);
					break;
				case TypeCode_Mouse_DoubleClick:
					printf("Mouse double click: code - %d | pos - (%d, %d)", info.code, pos.x, pos.y);
					Move_Cursor(0, pos.y);

					Set_Input_Mode(1);
					Show_Cursor();

					printf("������ �� ��ȣ�� �Է��ϼ���>");
					scanf("%d", &row);

					Set_Input_Mode(0);
					Hide_Cursor();
					break;
				case TypeCode_Mouse_Release:
					printf("Mouse release: code - %d | pos - (%d, %d)", info.code, pos.x, pos.y);
					break;
				case TypeCode_Mouse_Wheel:
					if ( info.code > 0 )
						printf("Mouse wheel: direction - up | pos - (%d, %d)", pos.x, pos.y);
					else
						printf("Mouse wheel: direction - down | pos - (%d, %d)", pos.x, pos.y);
					break;
				case TypeCode_Mouse_Move:
					printf("Mouse move: code - %d | pos - (%d, %d)", info.code, pos.x, pos.y);
					break;
				}
			}

			++row;

			--count;
		}

		Move_Cursor(0, INIT_CONSOLE_HEIGHT);
		printf("count: %-4d time: %.10f", count, Stopwatch_Get());

		Stopwatch_SpinLock(1.0 / 240);
	}

	return;
}