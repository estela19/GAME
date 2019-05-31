/*
	주의:

	- 이 코드 파일은 우리 수업 표준 프로그래밍 환경에서 잘 작동하도록 되어 있어요.
	  다른 환경을 사용하는 친구들에게는 조금 미안해요.

	- 이 파일의 내용은 '우리 수업의 검은 창'에 매우 특화되어 있으니 지금은 읽지 않는 게 더 좋아요.
	  (쳐다보기도 싫도록 일부러 더럽게 코드 적어 놨어요)
	  대신 console.h에 있는 주석 설명들을 읽어 보면서 라이브러리가 자신의 의도에 맞게 동작하도록 구성해 보세요.
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

					printf("점프할 줄 번호를 입력하세요>");
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