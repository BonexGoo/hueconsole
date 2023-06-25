#include <huecoding.h>

static void next(int x, int y)
{
	clrscr(50, 25);

	gotoxy(20, 3);
	setcolor("#FF40F9");
	print("사랑해요!\n");
}

HUE_DECLARE_APP("코딩엽서/하윤", postcard)
void postcard()
{
	gotoxy(20, 3);
	print("부모님께\n");
	setcolor("#34CF72");
	gotoxy(20, 4);
	line(28, 4);

	gotoxy(12, 5);
	setcolor("#000000");
	print("엄마, 아빠! 항상 날");
		
	gotoxy(32, 5);
	setcolor("#FF1212");
	print("최고로");

	gotoxy(12, 6);
	setcolor("#000000");
	print("헹복하게 해줘서 고마워~\n");

	gotoxy(12, 7);
	setcolor("#7500EB");
	print("언제나");

	gotoxy(19, 7);
	setcolor("#1447FF");
	print("그 누구보다도");

	gotoxy(12, 8);
	setcolor("#FF40F9");
	print("사랑할게");





	setcolor("#000000");
	gotoxy(25, 10);
	print("-하윤\n");

	gotoxy(20, 12);
	button(8, 1, next);
	print("다음으로");
}

//////////////////////////////////////////////////////////////////////////

static void onclick(int x, int y)
{
	gotoxy(x, y);
	setbgcolor("#ff458c");
	print(" ");

	char dot[2];
	dot[0] = x;
	dot[1] = y;
	send("dot.hayoon", dot, 2);
}

static void OnListen(const void* data, int len)
{
	char* dot = (char*)data;
	int x = dot[0];
	int y = dot[1];
	gotoxy(x, y);
	setcolor("#000000");
}

HUE_DECLARE_APP("도트편집기/하윤", doteditor)
void doteditor()
{
	listen("dot.hayoon", 500, OnListen);
	clrscr(30, 30);
	button(30, 30, onclick);

	for (int y = 0; y < 30; y++)
	for (int x = 0; x < 30; x++)
	{
		{
			if (x % 2 == y % 2)
				setbgcolor("#f0fff0");
			else
				setbgcolor("#ffffff");
			print(" ");

		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

int left_sel = 0;
int right_sel = 1;
int next_sel = 2;
const char* names[8] = {
	"김민재", "안효섭", "김 범", "이동욱",
	"송중기", "박보검", "남주혁", "최현욱" };

void left(int x, int y)
{
	if (7 < next_sel)
	{
		gotoxy(4, 8);
		setcolor("#8000FF");
		print("!당신의 선택은 %s!\n", names[left_sel]);
		return;
	}

	right_sel = next_sel;
	gotoxy(18, 6);
	print("%s\n", names[next_sel]);
	next_sel++;
}

void right(int x, int y)
{
	if (7 < next_sel)
	{
		gotoxy(4, 8);
		setcolor("#8000FF");
		print("!당신의 선택은 %s!\n", names[right_sel]);
		return;
	}

	left_sel = next_sel;
	gotoxy(6, 6);
	print("%s\n", names[next_sel]);
	next_sel++;
}

HUE_DECLARE_APP("월드컵/하윤", worldcup)
void worldcup()
{
	clrscr(30, 10);

	gotoxy(3, 2);
	setcolor("#000000");
	print("<남자배우 이상형 월드컵>\n");

	gotoxy(6, 6);
	setcolor("#000000");
	button(6, 1, left);
	print("%s\n", names[0]);


	gotoxy(18, 6);
	setcolor("#000000");
	button(6, 1, right);
	print("%s\n", names[1]);

	gotoxy(14, 6);
	setcolor("#FFOOOO");
	print("V\n");

	gotoxy(15, 6);
	setcolor("#002BFF");
	print("S\n");
}
