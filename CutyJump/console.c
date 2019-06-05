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
#include <time.h>
#include <Windows.h>
#include <stdlib.h>

#define LENGTH_INPUT_BUFFER 256

/* --------------------------------------------------------------------------
	검은 창 하나를 다루기 위해 필요한 internal data들
	(여기 선언되어 있는 이름들은 여러분이 직접 부를 수 없어요)
*/

static int isAlreadyStarted;

static HWND handle_out;
static HWND handle_in;

static CONSOLE_SCREEN_BUFFER_INFO info_buffer;
static CONSOLE_CURSOR_INFO info_cursor;
static COORD pos_cursor;
static const COORD pos_zero = { 0, 0 };

static WORD color = INIT_DEFAULT_COLOR;

static int mode = -1;

static LARGE_INTEGER clock_startTime;
static LARGE_INTEGER freq;
static int isClockAlreadyCalled;

static DWORD numberOfInputs;
static INPUT_RECORD current_input_record;
static struct _KEY_EVENT_RECORD *const ptr_current_key_event = &current_input_record.Event.KeyEvent;
static struct _MOUSE_EVENT_RECORD *const ptr_current_mouse_event = &current_input_record.Event.MouseEvent;
static DWORD last_state_mouse_button;

struct Info_Input_Internal
{
	struct Info_Input info;
	struct Point pos;
};

static struct Info_Input_Internal input_buffer[LENGTH_INPUT_BUFFER];
static int count_accepted_inputs;
static int pos_accepted_inputs;
static struct Point last_mouse_pos = { -1, -1 };
static struct Point last_mouse_pos_toReturn = { -1, -1 };


/* --------------------------------------------------------------------------
	헤더 파일에 선언해 둔 각종 함수들에 대한 함수 정의들
	(더러우니 굳이 읽진 않는 게 좋겠어요)
*/

void Start_Console(void)
{
	if ( isAlreadyStarted == 1 )
		return;

	handle_in = GetStdHandle(STD_INPUT_HANDLE);
	handle_out = GetStdHandle(STD_OUTPUT_HANDLE);

	Resize_Console(INIT_CONSOLE_WIDTH, INIT_CONSOLE_HEIGHT);

	Set_Input_Mode(0);

#if INIT_SHOW_CURSOR == 1
	Show_Cursor();
#else
	Hide_Cursor();
#endif

	QueryPerformanceFrequency(&freq);

	isAlreadyStarted = 1;
}

void Resize_Console(int width, int height)
{
	info_buffer.dwSize.X = width;
	info_buffer.dwSize.Y = height + 1;

	info_buffer.srWindow.Left = 0;
	info_buffer.srWindow.Right = width - 1;
	info_buffer.srWindow.Top = 0;
	info_buffer.srWindow.Bottom = height;

	info_buffer.dwMaximumWindowSize.X = width;
	info_buffer.dwMaximumWindowSize.Y = height + 1;

	SetConsoleWindowInfo(handle_out, TRUE, &info_buffer.srWindow);
	SetConsoleScreenBufferSize(handle_out, info_buffer.dwSize);
	SetConsoleWindowInfo(handle_out, TRUE, &info_buffer.srWindow);

	Clear_Screen();
}

void Set_Color(int new_color, int trigger_recolor_all)
{
	color = (WORD)new_color;
	SetConsoleTextAttribute(handle_out, color);

	if ( trigger_recolor_all != 0 )
		FillConsoleOutputAttribute(handle_out, color, info_buffer.dwSize.X * info_buffer.dwSize.Y, pos_zero, &numberOfInputs);
}

void Clear_Screen()
{
	Move_Cursor(0, 0);
	FillConsoleOutputCharacterA(handle_out, ' ', info_buffer.dwSize.X * info_buffer.dwSize.Y, pos_zero, &numberOfInputs);

#if CLEAR_WITH_DEFAULT_COLOR == 1
	FillConsoleOutputAttribute(handle_out, 0x80, info_buffer.dwSize.X * info_buffer.dwSize.Y, pos_zero, &numberOfInputs);
#else
	FillConsoleOutputAttribute(handle_out, color, info_buffer.dwSize.X * info_buffer.dwSize.Y, pos_zero, &numberOfInputs);
#endif

}

void Show_Cursor(void)
{
	info_cursor.dwSize = 1;
	info_cursor.bVisible = TRUE;
	SetConsoleCursorInfo(handle_out, &info_cursor);
}

void Hide_Cursor(void)
{
	info_cursor.dwSize = 1;
	info_cursor.bVisible = FALSE;
	SetConsoleCursorInfo(handle_out, &info_cursor);
}

int Move_Cursor(int x, int y)
{
	if ( x < 0 || x > info_buffer.dwSize.X ||
		y < 0 || y > info_buffer.dwSize.Y + 1 )
		return -1;

	pos_cursor.X = x;
	pos_cursor.Y = y;

	SetConsoleCursorPosition(handle_out, pos_cursor);

	return 0;
}

void Set_Input_Mode(int use_stdio)
{
	if ( mode == 0 && use_stdio != 0 )
	{
		last_state_mouse_button = 0;
		FlushConsoleInputBuffer(handle_in);
		SetConsoleMode(handle_in, ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_INPUT);

		mode = 1;
	}
	else if ( mode != 0 && use_stdio == 0 )
	{
		last_state_mouse_button = 0;
		FlushConsoleInputBuffer(handle_in);
		SetConsoleMode(handle_in, ENABLE_MOUSE_INPUT);

		mode = 0;
	}

}

void Stopwatch_Start()
{
	if ( isClockAlreadyCalled == 0 )
	{
		isClockAlreadyCalled = 1;
		QueryPerformanceFrequency(&freq);
	}

	QueryPerformanceCounter(&clock_startTime);
}

double Stopwatch_Get()
{
	if ( isClockAlreadyCalled == 0 )
		return -1;
	else
	{
		LARGE_INTEGER now;
		double result;

		QueryPerformanceCounter(&now);

		result = (double)(now.QuadPart - clock_startTime.QuadPart);
		result /= freq.QuadPart;

		return result;
	}
}

void Stopwatch_Sleep(double interval)
{
	Sleep((DWORD)(interval * 1000));
}

void Stopwatch_SpinLock(double interval)
{
#if USE_SLEEP_AS_SPINLOCK != 0
	Stopwatch_Sleep(interval);
#else
	LARGE_INTEGER now, endTime;

	if ( isClockAlreadyCalled == 0 )
	{
		isClockAlreadyCalled = 1;
		QueryPerformanceFrequency(&freq);
	}

	QueryPerformanceCounter(&endTime);

	endTime.QuadPart += interval * freq.QuadPart;

	do
	{
		QueryPerformanceCounter(&now);
	}
	while ( now.QuadPart < endTime.QuadPart );
#endif
}

void Accept_Inputs()
{
	DWORD isReadSuccessful;

	struct Info_Input_Internal *ptr_input_buffer = input_buffer;
	count_accepted_inputs = 0;
	pos_accepted_inputs = 0;

	numberOfInputs = 0;
	GetNumberOfConsoleInputEvents(handle_in, &numberOfInputs);

	if ( numberOfInputs > LENGTH_INPUT_BUFFER )
		numberOfInputs = LENGTH_INPUT_BUFFER;

	while ( numberOfInputs > 0 )
	{
		ReadConsoleInputA(handle_in, &current_input_record, 1, &isReadSuccessful);

		if ( isReadSuccessful == 0 )
			break;

		switch ( current_input_record.EventType )
		{
		case KEY_EVENT:
			if ( ptr_current_key_event->bKeyDown )
				ptr_input_buffer->info.type = TypeCode_Key_Press;
			else
#if ACCEPT_RELEASE_AS_INPUT == 1
				ptr_input_buffer->info.type = TypeCode_Key_Release;
#else
			{
				isReadSuccessful = 0;
				break;
			}
#endif

			ptr_input_buffer->info.code = ptr_current_key_event->wVirtualKeyCode;
			ptr_input_buffer->info.ctrl = ptr_current_key_event->dwControlKeyState;
			break;
#if ACCEPT_MOUSE_AS_INPUT == 1
		case MOUSE_EVENT:
			ptr_input_buffer->pos.x = ptr_current_mouse_event->dwMousePosition.X;
			ptr_input_buffer->pos.y = ptr_current_mouse_event->dwMousePosition.Y;

#if ACCEPT_RELEASE_AS_INPUT == 1
			// release 먼저, click 등은 나중
			if ( last_state_mouse_button != 0 )
			{
				DWORD state_released = last_state_mouse_button ^ (last_state_mouse_button & ptr_current_mouse_event->dwButtonState);

				while ( state_released != 0 )
				{
					DWORD flag_button = 1;

					while ( state_released ^ flag_button )
						flag_button <<= 1;

					ptr_input_buffer->info.type = TypeCode_Mouse_Release;
					ptr_input_buffer->info.code = flag_button;
					ptr_input_buffer->info.ctrl = ptr_current_mouse_event->dwControlKeyState;

					ptr_input_buffer = &input_buffer[count_accepted_inputs];
					++count_accepted_inputs;

					last_state_mouse_button ^= flag_button;
					state_released ^= flag_button;

					isReadSuccessful = 0;
				}
			}
#endif

			switch ( ptr_current_mouse_event->dwEventFlags )
			{
			case 0:
			case DOUBLE_CLICK:
				if ( ptr_current_mouse_event->dwButtonState == 0 )
					isReadSuccessful = 0;
				else
				{
					DWORD state_pressed = ptr_current_mouse_event->dwButtonState ^ (last_state_mouse_button & ptr_current_mouse_event->dwButtonState);

					while ( state_pressed != 0 )
					{
						DWORD flag_button = 1;

						while ( state_pressed ^ flag_button )
							flag_button <<= 1;

						if ( ptr_current_mouse_event->dwEventFlags )
							ptr_input_buffer->info.type = TypeCode_Mouse_DoubleClick;
						else
							ptr_input_buffer->info.type = TypeCode_Mouse;

						ptr_input_buffer->info.code = flag_button;
						ptr_input_buffer->info.ctrl = ptr_current_mouse_event->dwControlKeyState;

						ptr_input_buffer = &input_buffer[count_accepted_inputs];
						++count_accepted_inputs;

						last_state_mouse_button ^= flag_button;
						state_pressed ^= flag_button;

						isReadSuccessful = 0;
					}
				}
#if ACCEPT_RELEASE_AS_INPUT == 0
				last_state_mouse_button = ptr_current_mouse_event->dwButtonState;
#endif
				break;
			case MOUSE_WHEELED:
				ptr_input_buffer->info.type = TypeCode_Mouse_Wheel;
				ptr_input_buffer->info.code = ptr_current_mouse_event->dwButtonState;
				ptr_input_buffer->info.ctrl = ptr_current_mouse_event->dwControlKeyState;
				break;
			case MOUSE_MOVED:
				if ( last_mouse_pos.x == ptr_input_buffer->pos.x &&
					last_mouse_pos.y == ptr_input_buffer->pos.y )
				{
					isReadSuccessful = 0;
					break;
				}

				last_mouse_pos = ptr_input_buffer->pos;

				ptr_input_buffer->info.type = TypeCode_Mouse_Move;
				ptr_input_buffer->info.code = ptr_current_mouse_event->dwButtonState;
				ptr_input_buffer->info.ctrl = ptr_current_mouse_event->dwControlKeyState;
				break;
			}

			break;
#endif
		default:
			isReadSuccessful = 0;
			break;
		}

		if ( isReadSuccessful != 0 )
		{
			ptr_input_buffer = &input_buffer[count_accepted_inputs];
			++count_accepted_inputs;
		}

		--numberOfInputs;
	}

	last_mouse_pos_toReturn.x = -1;
	last_mouse_pos_toReturn.y = -1;
}

int Can_Read()
{
	return count_accepted_inputs - pos_accepted_inputs;
}

int Read_Input(struct Info_Input * out_input)
{
	if ( Can_Read() == 0 )
		return -1;

	*out_input = input_buffer[pos_accepted_inputs].info;

	if ( input_buffer[pos_accepted_inputs].info.type & TypeCode_Mouse )
		last_mouse_pos_toReturn = input_buffer[pos_accepted_inputs].pos;

	++pos_accepted_inputs;

	return 0;
}

struct Point Get_Mouse_Pos()
{
	return last_mouse_pos_toReturn;
}


static const char *keyNames[] =
{
	// 0x00 ~ 0x0F
	"Not_Defined", 
	"Left Mouse Button",
	"Right Mouse Button",
	"Cancel",

	"Middle Mouse Button",
	"X1 Mouse Button",
	"X2 Mouse Button",
	"Reserved",

	"Backspace",
	"Tab",
	"Reserved",
	"Reserved",

	"Clear",
	"Enter",
	"Not_Defined",
	"Not_Defined",

	// 0x10 ~ 0x1F
	"Shift",
	"Control",
	"Alt",
	"Pause",

	"CapsLock",
	"Hangul",
	"Not_Defined",
	"Jeonja",

	"Final",
	"Hanja",
	"Not_Defined",
	"Escape",

	"Convert",
	"NonConvert",
	"Accept",
	"ModeChange",

	// 0x20 ~ 0x2F
	"Space",
	"Page Up",
	"Page Down",
	"End",

	"Home",
	"Left",
	"Up",
	"Right",

	"Down",
	"Select",
	"Print",
	"Execute",

	"Print Screen",
	"Insert",
	"Delete",
	"Help",

	// 0x30 ~ 0x3F
	"0",
	"1",
	"2",
	"3",

	"4",
	"5",
	"6",
	"7",

	"8",
	"9",
	"Not_Defined",
	"Not_Defined",

	"Not_Defined",
	"Not_Defined",
	"Not_Defined",
	"Not_Defined",

	// 0x40 ~ 0x4F
	"Not_Defined",
	"A",
	"B",
	"C",

	"D",
	"E",
	"F",
	"G",

	"H",
	"I",
	"J",
	"K",

	"L",
	"M",
	"N",
	"O",

	// 0x50 ~ 0x5F
	"P",
	"Q",
	"R",
	"S",

	"T",
	"U",
	"V",
	"W",

	"X",
	"Y",
	"Z",
	"Left Windows",

	"Right Windows",
	"Apps",
	"Reserved",
	"Sleep",

	// 0x60 ~ 0x6F
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",

	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",

	"Numpad 8",
	"Numpad 9",
	"Numpad *",
	"Numpad +",

	"Separator",
	"Numpad -",
	"Numpad .",
	"Numpad /",

	// 0x70 ~ 0x7F
	"F1",
	"F2",
	"F3",
	"F4",

	"F5",
	"F6",
	"F7",
	"F8",

	"F9",
	"F10",
	"F11",
	"F12",

	"F13",
	"F14",
	"F15",
	"F16",

	// 0x80 ~ 0x8F
	"F17",
	"F18",
	"F19",
	"F20",

	"F21",
	"F22",
	"F23",
	"F24",

	"Nav View",
	"Nav Menu",
	"Nav Up",
	"Nav Down",

	"Nav Left",
	"Nav Right",
	"Nav Accept",
	"Nav Cancel",

	// 0x90 ~ 0x9F
	"NumLock",
	"ScrollLock",
	"Keypad =",
	"Not_Defined",

	"Not_Defined",
	"Not_Defined",
	"Not_Defined",
	"Not_Defined",

	"Not_Defined",
	"Not_Defined",
	"Not_Defined",
	"Not_Defined",

	"Not_Defined",
	"Not_Defined",
	"Not_Defined",
	"Not_Defined",

	// 0xA0 ~ 0xAF
	"Left Shift",
	"Right Shift",
	"Left Control",
	"Right Control",

	"Left Alt",
	"Right Alt",
	"Browser Back",
	"Browser Forward",

	"Browser Refresh",
	"Browser Stop",
	"Browser Search",
	"Browser Favorites",

	"Browser Home",
	"Mute",
	"Volume Down",
	"Volume Up",

	// 0xB0 ~ 0xBF
	"Media Next",
	"Media Prev",
	"Media Stop",
	"Media Play",

	"Launch Mail",
	"Launch Media",
	"Launch App1",
	"Launch App2",

	"Reserved",
	"Reserved",
	";",
	"+",

	",",
	"-",
	".",
	"/",

	// 0xC0 ~ 0xCF
	"`",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	// 0xD0 ~ 0xDF
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"[",

	"\\",
	"]",
	"\'",
	"OEM 8",

	// 0xE0 ~ 0xEF
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	// 0xF0 ~ 0xFF
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
};

const char *Get_KeyName(int code)
{
	if ( code >= 0 && code < 0x100 )
		return keyNames[code];

	return "[invalid code]";
}


