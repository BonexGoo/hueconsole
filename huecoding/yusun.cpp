#include <huecoding.h>
void no(int x, int y)
{
	clrscr(50, 25);

	gotoxy(3, 3);
	print("그래도 사랑해요.\n");
}

void next(int x, int y)
{
	clrscr(50, 25);

	gotoxy(3, 3);
	setcolor("#FF8080");
	print("사랑해요\n");
	
}

HUE_DECLARE_APP("코딩엽서/남유선", postcard)
void postcard()
{
	gotoxy(20, 3);
	print("엄마아빠 사랑해요\n");
	//강사가 주석처리함 - setcolor("#ff");

	gotoxy(20, 4);
	//강사가 주석처리함 - setcolor("#ff");
	print("저 사랑해요?~\n");

	gotoxy(20, 6);
	button(13, 1, next);
	print("그럼 당연하지\n");

	gotoxy(20, 8);
	button(4, 1, no);
	print("아니\n");
}
