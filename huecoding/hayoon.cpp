#include <huecoding.h>

static void next(int x, int y)
{
	clrscr(50, 25);

	gotoxy(20, 3);
	setcolor("#FF40F9");
	print("사랑해요!\n");
}

HUE_DECLARE_APP("그림엽서/하윤", postcard)
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