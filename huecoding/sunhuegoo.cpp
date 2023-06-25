#include <huecoding.h>

void onclick(int x, int y)
{
	gotoxy(x, y);
	setbgcolor("#000000");
	print(" ");

	char dot[2];
	dot[0] = x;
	dot[1] = y;
	send("구선휴", dot, 2);
}

void OnListen(const void* data, int len)
{
	auto Dot = (char*)data;
	int X = Dot[0];
	int Y = Dot[1];
	if (X == 0 && Y == 0)
	{
		setbgcolor("#ffffff");
		clrscr(60, 60);
		button(60, 60, onclick);
	}
	else
	{
		gotoxy(X, Y);
		setbgcolor("#000000");
		print(" ");
	}
}

HUE_DECLARE_APP("도트편집기/선휴", doteditor)
void doteditor()
{
	listen("구선휴", 500, OnListen);
	clrscr(60, 60);
	button(60, 60, onclick);

	for (int y = 0; y < 60; y++)
	for (int x = 0; x < 60; x++)
	{
		if (x % 2 == 1)
			setbgcolor("#f0fff0");
		else
			setbgcolor("#ffffff");
		print(" ");
	}
}

////////////////////////////////////////////////////////////////

int score = 0;

void OnTest5_1(int x, int y)
{
}

void OnTest5_2(int x, int y)
{
}

void OnTest5_3(int x, int y)
{
	
}

void OnTest4_1(int x, int y)
{
	score += 2;
	print("\n\n클럽같은 곳에 가면 시선이 집중됩니까?\n\n  ");

	button(6, 1, OnTest5_1);
	print("  예    ");

	button(6, 1, OnTest5_2);
	print(" 몰라   ");

	button(6, 1, OnTest5_3);
	print("아니오");
}

void OnTest4_2(int x, int y)
{
	score += 1;
	print("\n\n클럽같은 곳에 가면 시선이 집중됩니까?\n\n  ");

	button(6, 1, OnTest5_1);
	print("  예    ");

	button(6, 1, OnTest5_2);
	print(" 몰라   ");

	button(6, 1, OnTest5_3);
	print("아니오");
}

void OnTest4_3(int x, int y)
{
	score += 0;
	print("\n\n클럽같은 곳에 가면 시선이 집중됩니까?\n\n  ");

	button(6, 1, OnTest5_1);
	print("  예    ");

	button(6, 1, OnTest5_2);
	print(" 몰라   ");

	button(6, 1, OnTest5_3);
	print("아니오");
}

void OnTest3_1(int x, int y)
{
	score += 2;
	print("\n\n연애횟수가 많습니까?\n\n  ");

	button(6, 1, OnTest4_1);
	print("  예    ");

	button(6, 1, OnTest4_2);
	print(" 몰라   ");

	button(6, 1, OnTest4_3);
	print("아니오");
}

void OnTest3_2(int x, int y)
{
	score += 1;
	print("\n\n연애횟수가 많습니까?\n\n  ");

	button(6, 1, OnTest4_1);
	print("  예    ");

	button(6, 1, OnTest4_2);
	print(" 몰라   ");

	button(6, 1, OnTest4_3);
	print("아니오");
}

void OnTest3_3(int x, int y)
{
	score += 0;
	print("\n\n연애횟수가 많습니까?\n\n  ");

	button(6, 1, OnTest4_1);
	print("  예    ");

	button(6, 1, OnTest4_2);
	print(" 몰라   ");

	button(6, 1, OnTest4_3);
	print("아니오");
}

void OnTest2_1(int x, int y)
{
	score += 2;
	print("\n\n친구들에게 잘생겼다는 말을 자주 듣습니까?\n\n  ");

	button(6, 1, OnTest3_1);
	print("  예    ");

	button(6, 1, OnTest3_2);
	print(" 몰라   ");

	button(6, 1, OnTest3_3);
	print("아니오");
}

void OnTest2_2(int x, int y)
{
		score += 1;
		print("\n\n친구들에게 잘생겼다는 말을 자주 듣습니까?\n\n  ");

		button(6, 1, OnTest3_1);
		print("  예    ");

		button(6, 1, OnTest3_2);
		print(" 몰라   ");

		button(6, 1, OnTest3_3);
		print("아니오");
}

void OnTest2_3(int x, int y)
{
	score += 0;
	print("\n\n친구들에게 잘생겼다는 말을 자주 듣습니까?\n\n  ");

	button(6, 1, OnTest3_1);
	print("  예    ");

	button(6, 1, OnTest3_2);
	print(" 몰라   ");

	button(6, 1, OnTest3_3);
	print("아니오");
}

void OnTest1_1(int x, int y)
{
	print("\n\n당신은 잘생겼다고 생각합니까?\n\n  ");

	button(6, 1, OnTest2_1);
	print("  예    ");

	button(6, 1, OnTest2_2);
	print(" 몰라   ");

	button(6, 1, OnTest2_3);
	print("아니오");
}

void OnTest1_2(int x, int y)
{
	print("ㅗ");
}

void OnTest1_3(int x, int y)
{
	print("ㅗ");
}

HUE_DECLARE_APP("당신의 외모/선휴", yourface)
void yourface()
{
	print("테스트를 시작하시겠습니까?\n\n  ");
	
	button(6, 1, OnTest1_1);
	print("  예    ");

	button(6, 1, OnTest1_2);
	print(" 몰라   ");

	button(6, 1, OnTest1_3);
	print("아니오");
}
