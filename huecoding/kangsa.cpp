#include <huecoding.h>

static void next(int x, int y)
{
	clrscr(40, 30);

	gotoxy(20, 3);
	print("뻥이예여\n");
}

//HUE_DECLARE_APP("그림엽서/제임스", postcard)
void postcard()
{
	clrscr(40, 30);

	setcolor("#ffff00");
	gotoxy(20, 3);
	rect(28, 4);

	setcolor("#000000");
	gotoxy(20, 3);
	print("엄마에게\n");

	gotoxy(12, 5);
	setcolor("#0000ff");
	print("엄마 항상 절 최고로\n");

	gotoxy(12, 6);
	print("잘해주셔서 감사해요~\n");

	setcolor("#000000");
	gotoxy(25, 10);
	print("-아무개가\n");

	gotoxy(20, 12);
	button(8, 1, next);
	print("다음으로");

	setcolor("#808080");
	gotoxy(1, 1);
	image(5, 5, "character_a");
}
