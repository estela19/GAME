/*
	주의:
	
	- 이 헤더 파일은 우리 수업 표준 프로그래밍 환경에서 잘 작동하도록 되어 있어요.
	  다른 환경을 사용하는 친구들에게는 조금 미안해요.

	- 이 파일 윗 부분에 보이는 각종 #define 부분들을 조절해서 라이브러리의 동작을 내 입맛에 맞게 커스터마이즈할 수 있어요.
*/

#pragma once

/* ------------------------------------------------------------------------------
	글자색 / 배경색을 다루기 위해 사용하는 친구들
*/

#define COLOR_BLACK     0x00
#define COLOR_BLUE      0x01
#define COLOR_GREEN     0x02
#define COLOR_CYAN      (COLOR_BLUE | COLOR_GREEN)
#define COLOR_RED       0x04
#define COLOR_MAGENTA   (COLOR_BLUE | COLOR_RED)
#define COLOR_YELLOW    (COLOR_GREEN | COLOR_RED)
#define COLOR_WHITE     (COLOR_BLUE | COLOR_GREEN | COLOR_RED)
#define COLOR_INTENSITY 0x08

/*
	이 매크로 함수를 써서 원하는 (글자색 / 배경색) 조합을 의미하는 숫자 표현을 얻을 수 있어요.
	예: 검은 글자색 / 밝은 흰 바탕색을 쓰려면 COLOR(COLOR_BLACK, COLOR_INTENSITY | COLOR_WHITE) 라고 적으면 돼요

	뭔가 복잡해 보여도 어차피 상수 수식이라서 compile time에 다 계산 끝나니 걱정 안 해도 돼요.
*/
#define COLOR(fore, back) ((fore) | (back) << 4)



/* ------------------------------------------------------------------------------
	라이브러리의 동작을 어떻게 시작할 것인지 정하는 친구들 (실행 도중 변경 가능)
*/

// 시작할 때 커서를 표시할 것인지 여부를 정합니다.
// 표시하려면 1, 그렇지 않으면 0으로 설정해 주세요. (기본값은 0)
// -> Show_Cursor() / Hide_Cursor()를 호출해서 커서 표시 유무를 그 때 그 때 전환할 수도 있어요.
#define INIT_SHOW_CURSOR 0

// 시작할 때의 검은 창의 가로 길이(영어 몇 글자를 적을 수 있나)를 정합니다. (기본값은 64)
// 너무 크거나 작게 정하면 실패할 수 있어요(모니터 해상도 등에 따라 달라짐).
// -> Resize_Console()을 호출해서 창의 크기 + 내용을 초기화할 수도 있어요.
#define INIT_CONSOLE_WIDTH 64

// 시작할 때의 검은 창의 세로 길이(줄 수)를 정합니다. (기본값은 32)
// 너무 크거나 작게 정하면 실패할 수 있어요(모니터 해상도 등에 따라 달라짐).
// -> 사용상의 편의를 위해, 실제 검은 창은 여기 적힌 숫자보다 한 줄 더 크게 설정돼요. 왜 그런지 궁금하면 오픈톡방 ㄱㄱ
// -> Resize_Console()을 호출해서 창의 크기 + 내용을 초기화할 수도 있어요.
#define INIT_CONSOLE_HEIGHT 32

// 시작할 때의 검은 창의 색상(글자색 / 배경색)을 정합니다. (기본값은 COLOR(COLOR_WHITE, COLOR_BLACK))
#define INIT_DEFAULT_COLOR COLOR(COLOR_WHITE, COLOR_BLACK)



/* ------------------------------------------------------------------------------
	라이브러리의 동작을 구체적으로 조율하는 친구들 (얘네는 실행 도중 변경 불가)
*/

// 마우스 버튼 클릭 및 포인터 이동을 입력으로 간주할 것인지 정합니다.
// 간주하려면 1, 무시하려면 0으로 설정해 주세요. (기본값은 1)
#define ACCEPT_MOUSE_AS_INPUT 1

// 키 또는 마우스 버튼을 '떼는 것'를 입력으로 간주할 것인지 정합니다.
// 간주하려면 1, 무시하려면 0으로 설정해 주세요. (기본값은 1)
#define ACCEPT_RELEASE_AS_INPUT 1

// ClearScreen()을 호출했을 때 전체 화면을 INIT_DEFAULT_COLOR로 칠할 지 현재 배경색으로 칠할 지 정합니다.
// 0: 최근에 설정한 배경색으로 칠함 (기본값은 0)
// 1: INIT_DEFAULT_COLOR로 칠함
#define CLEAR_WITH_DEFAULT_COLOR 0

// Stopwatch_Spinlock()을 호출했을 때 실제로는 Stopwatch_Sleep()과 동일한 동작을 수행할 것인지 정합니다.
// (노트북을 쓰는 경우 spinlock은 열이 꽤 많이 날 수 있어요. 일단 만드는 동안에는 sleep을 쓰고 마지막에 바꿔도 좋을 듯)
// 0: 원래대로
// 1: spinlock도 sleep처럼 동작하도록 변경 (기본값은 1)
#define USE_SLEEP_AS_SPINLOCK 1



/* ----------------------------------------------------------------------------------------
	검은 창 자체를 조작하거나 입/출력 제어를 보조하기 위한 Code들(출력 자체는 printf() 써도 돼요)
*/

/*
	이 라이브러리를 사용하려면 꼭 미리 호출해 주어야 하는 함수입니다.
	가급적이면 main()의 '내 문장들'이 실행되기 전에 미리 호출해 주세요.
*/
void Start_Console(void);

/*
	검은 창의 크기를 변경하고 싶을 때 호출하는 함수입니다.
	크기를 변경하면 화면 전체 내용들 또한 함께 지워집니다.
*/
void Resize_Console(int width, int height);

/*
	검은 창의 색상을 변경하고 싶을 때 호출하는 함수입니다.
	변경된 색은 기본적으로는 '이후에 print할 때' 적용됩니다.
	인수 trigger_recolor_all의 값이 0이 아니면 화면 전체를 해당 색상으로 다시 칠해 줍니다.
	- console.h 위에 있는 COLOR() 매크로 함수를 사용하면 색상 정하기 편할 거예요.
*/
void Set_Color(int default_color, int trigger_recolor_all);

/*
	검은 창에 표시된 전체 내용들을 지우고 커서를 (0, 0) 위치로 이동시킵니다.
*/
void Clear_Screen();

/*
	깜빡이는 커서를 표시하기 시작하고 싶을 때 호출하는 함수입니다.
	사용자 입력을 기다리고 싶을 때(특히, scanf() 등의 고급 함수들을 사용하려 할 때) 미리 호출해 주세요.
	그렇지 않을 때는 다시 커서를 가려 두는 것이 좀 더 나을 거예요.
*/ 
void Show_Cursor(void);

/*
	깜빡이는 커서를 가리기 시작하고 싶을 때 호출하는 함수입니다.
*/ 
void Hide_Cursor(void);

/*
	커서의 위치를 주어진 좌표로 이동시킵니다.
	성공한 경우 0을, 실패한 경우 0이 아닌 값을 return합니다.
	좌표가 현재 화면 영역을 벗어나는 경우 실패합니다. 실패한 경우 커서의 위치는 변경되지 않습니다.
*/
int Move_Cursor(int x, int y);

/*
	입력 방식을 변경하고 싶을 때 호출하는 함수입니다.
	인수 use_stdio의 값이 0이 아니면 표준 C 프로그래밍에서 쓰는 scanf() 등을 쓸 수 있습니다.
	(엔터 키 칠 때까지 기다렸다 한꺼번에 입력받음)
	인수 use_stdio의 값이 0이면 기존의 키 / 마우스 입력으로 돌아올 수 있습니다.
	-> 모드가 바뀌면 원래 받아 둔(읽진 않은) 사용자 입력들이 모두 증발하도록 만들어 놓았어요.
*/
void Set_Input_Mode(int use_stdio);


/* ----------------------------------------------------------------------------------------
	현실 시간과의 조율을 위한 Code들
*/

/*
	'경과된 시간'을 알기 위해 스톱워치를 켜거나 리셋시킵니다.
	이 함수를 일단 호출한 다음 나중에 Stopwatch_Get()을 호출해 보세요.
*/
void Stopwatch_Start();

/*
	마지막 Stopwatch_Start() 호출 이후로 얼마나 시간이 흘렀는지를 초 단위로 재어 return합니다.
	-> 아직 Stopwatch_Start()를 단 한 번도 호출하지 않았다면 -1을 return해요.
*/
double Stopwatch_Get();

/*
	주어진 시간만큼 프로그램을 재우는 함수입니다.
	Python에서처럼 초 단위로 쓸 수 있도록 구성해 놓았어요.
	-> 실제로는 밀리초 단위로 동작해요.
*/
void Stopwatch_Sleep(double interval);

/*
	주어진 시간만큼 '잠 없이' 프로그램의 실행을 지연시키는 함수입니다.
	Python에서처럼 초 단위로 쓸 수 있도록 구성해 놓았어요.
	-> 얘는 조금 더 정밀하게 동작해요.
	-> 얘는 배터리를 좀 더 많이 잡아먹어요. 그래서 USE_SLEEP_AS_SPINLOCK 옵션을 만들어 두었어요. console.h에 있는 설명을 확인해 주세요.
*/
void Stopwatch_SpinLock(double interval);


/* ----------------------------------------------------------------------------------------
	사용자 입력을 다루기 위한 Data 및 Code들
*/

// 사용자 입력 정보 하나의 의미를 나타내기 위한 열거자입니다.
// -> 열거자는 아직 익숙치 않을 수 있어서 각 항목 이름에 접두사를 좀 붙여 놨어요.
enum Input_TypeCode
{
	// 키보드 입력입니다. 단순히 '키 입력이냐 마우스 입력이냐'를 구분하고 싶을 때 사용합니다.
	TypeCode_Key = 0x01,

	// 키를 방금 눌렀습니다.
	// code 값은 키의 번호를 의미합니다. Get_KeyName()을 통해 번호별 키 이름을 확인해 볼 수 있습니다.
	TypeCode_Key_Press = 0x11,

	// 키를 방금 뗐습니다.
	// code 값은 키의 번호를 의미합니다. Get_KeyName()을 통해 번호별 키 이름을 확인해 볼 수 있습니다.
	TypeCode_Key_Release = 0x21,

	// 마우스 입력입니다. 단순히 '마우스 입력이냐 키 입력이냐'를 구분하고 싶을 때 사용합니다.
	// 이 입력이 발생한 시점의 마우스 포인터 좌표를 얻으려면 Get_Mouse_Pos()를 호출해 주세요.
	TypeCode_Mouse = 0x02,

	// 마우스 버튼을 방금 눌렀습니다.
	// code 값은 '방금 누른' 마우스 버튼의 번호(보통은 1: 왼/ 2: 오 / 4: 휠버튼 순서를 써요)를 의미합니다.
	// 이 입력이 발생한 시점의 마우스 포인터 좌표를 얻으려면 Get_Mouse_Pos()를 호출해 주세요.
	TypeCode_Mouse_Click = 0x12,

	// 마우스 버튼을 방금 뗐습니다.
	// code 값은 마우스 버튼의 번호(보통은 1: 왼/ 2: 오 / 4: 휠버튼 순서를 써요)를 의미합니다.
	// 이 입력이 발생한 시점의 마우스 포인터 좌표를 얻으려면 Get_Mouse_Pos()를 호출해 주세요.
	TypeCode_Mouse_Release = 0x22,

	// 마우스 버튼을 방금 '빠른 시간 안에 두번째로' 눌렀습니다.
	// 주의: 첫 클릭은 정상적인 TypeCode_Mouse_Click으로 간주돼요.
	// code 값은 '방금 두번째로 누른' 마우스 버튼의 번호(보통은 1: 왼/ 2: 오 / 4: 휠버튼 순서를 써요)를 의미합니다.
	// 이 입력이 발생한 시점의 마우스 포인터 좌표를 얻으려면 Get_Mouse_Pos()를 호출해 주세요.
	TypeCode_Mouse_DoubleClick = 0x42,

	// 마우스 휠을 방금 굴렸습니다.
	// code 값은 휠 굴린 방향(양수면 밀어내는 방향, 음수면 당겨오는 방향)을 의미합니다.
	// 이 입력이 발생한 시점의 마우스 포인터 좌표를 얻으려면 Get_Mouse_Pos()를 호출해 주세요.
	TypeCode_Mouse_Wheel = 0x82,

	// 마우스 포인터가 방금 다른 칸을 가리키기 시작했습니다.
	// code 값은 '현재 눌려 있는' 마우스 버튼의 번호(보통은 1: 왼/ 2: 오 / 4: 휠버튼 순서를 써요)를 의미합니다.
	// 실제 마우스 포인터 좌표를 얻으려면 Get_Mouse_Pos()를 호출해 주세요.
	TypeCode_Mouse_Move = 0x102
};

// 시스템(컴퓨터, 운영체제 등)마다 다른, 각종 키 입력 상태들을 축약하여 나타내기 위한 열거자입니다.
// -> 열거자는 아직 익숙치 않을 수 있어서 각 항목 이름에 접두사를 좀 붙여 놨어요.
enum Ctrl_Code
{
	// 오른쪽 Alt 키가 눌려 있습니다.
	Code_Right_Alt_Pressed = 0x001,

	// 왼쪽 Alt 키가 눌려 있습니다.
	Code_Left_Alt_Pressed = 0x002,

	// 오른쪽 Ctrl 키가 눌려 있습니다.
	Code_Right_Ctrl_Pressed = 0x004,

	// 왼쪽 Ctrl 키가 눌려 있습니다.
	Code_Left_Ctrl_Pressed = 0x008,

	// Shift 키가 눌려 있습니다.
	Code_Shift_Pressed = 0x010,

	// Num Lock 버튼에 불이 들어와 있습니다.
	Code_NumLock_On = 0x020,

	// Scroll Lock 버튼에 불이 들어와 있습니다.
	Code_ScrollLock_On = 0x040,

	// Caps Lock 버튼에 불이 들어와 있습니다.
	Code_CapsLock_On = 0x080,

	// 화살표 키와 같이 당시 기준으로 '기본 자판'에 포함되진 않은 키를 enhanced key라 불렀는데, 아무튼 그런 부류임을 나타냅니다.
	// 어떤 키가 enhanced 키로 간주되는 지 확인해 보는 것도 나름 재미있겠군용
	Code_Enhanced_Key = 0x100
};

// 사용자 입력 하나에 대한 정보를 모아 놓는 구조체입니다.
struct Info_Input
{
	// 이 입력이 '어떤 입력'인지를 나타내기 위한 값입니다.
	// 담긴 숫자의 의미는 console.h의 enum Input_TypeCode 부분을 확인해 주세요.
	int type;

	// type 값에 따라 서로 다른 의미를 갖는 숫자 값입니다.
	// 누른 키가 무엇인지, 누른 버튼이 무엇인지, 휠을 어느 방향으로 굴렸는지 등이 여기에 기록됩니다.
	int code;

	// 시스템(컴퓨터, 운영체제 등)마다 다른, 위 입력이 발생한 시기의 각종 키 입력 상태들을 추가로 확인할 수 있는 정보입니다.
	// 담긴 숫자의 의미는 console.h의 enum Ctrl_Code 부분을 확인해 주세요.
	int ctrl;
};

// 2차원 좌표를 나타내기 위한 구조체입니다.
struct Point
{
	int x;
	int y;
};

/*
	현재 시점까지의 사용자 입력을 내 프로그램으로 몽땅 가져옵니다.
	컴퓨터에 비해 사람은 매우 느린 편이므로 우리 프로그램은 보통 반복문을 써서 전체 실행 흐름을 구성하게 될 거예요.
	이 함수는 그 큰 반복문의 맨 윗 자리에 딱 한 번 적어 두면 돼요.
*/
void Accept_Inputs();

/*
	Accept_Inputs()가 호출된 이래 아직 읽지 않은 사용자 입력이 존재하는지 여부를 return합니다.
	정확히는 '읽지 않은 사용자 입력 수'를 return해요(다만 이건 일반적인 runtime에서는 그러지 않는 편이긴 함).
*/
int Can_Read();

/*
	주어진 포인터 값이 가리키는 칸에, Accept_Inputs()가 호출된 이래 아직 읽지 않은 가장 첫 사용자 입력을 담습니다.
	성공한 경우 0을, 실패한 경우 0이 아닌 값을 return합니다. 실패한 경우 원래 칸에 담겨 있던 값은 변경되지 않습니다.
*/
int Read_Input(struct Info_Input *out_input);

/*
	Accept_Inputs() 이후 Read_Input()이 호출된 이래 가장 마지막에 관찰된 마우스 포인터 좌표를 return합니다.
	일단 Read_Input()을 호출해서 다음 입력을 받고, 그 친구가 '마우스 입력'일 때 이 함수를 호출해서 좌표 값을 얻으면 돼요.
	-> 마우스 입력을 안 쓰도록 지정한 경우나 아직 이번 반복에서 유효한 마우스 입력을 읽은 적이 없는 경우 (-1, -1)을 return해요.
*/
struct Point Get_Mouse_Pos();

/*
	주어진 키 code 값에 해당하는 키 이름이 무엇인지에 대한 정보를 return합니다.
	printf() 등을 써서 출력해 볼 수 있어요! (%s 써서 출력해요)
*/
const char *Get_KeyName(int code);

/*
	내 main() 정의 중괄호 안에서 바로 그냥 호출하면 간단한 입력 테스트를 해 볼 수 있도록 만들어 둔 함수입니다.
	-> 창 너비가 너무 좁으면 정보가 한 줄에 다 안 담길 수 있어요. 입력 테스트를 할 때는 일단 적절한 크기로 늘린 다음 진행해 주세요
*/
void InputTest();