﻿#include <huecoding.h>

void OnRefresh(int x, int y)
{
    repaint();
}

void OnDan(const char* text, int enter)
{
    if(enter)
    {
        int dan = boss_atoi(text);

        print("\n [%i DAN]\n", dan);
        for(int i = 1; i <= 9; ++i)
            print(" %i x %i = %2i\n", dan, i, dan * i);

        print("\n ");
        button(7, 1, OnRefresh);
        print(" Retry \n");
    }
}

HUE_DECLARE_APP("GooGooDan", googoodan)
void googoodan()
{
    clrscr(60, 20, "#ffffff");
    print("\n Please type the DAN(2~9) you want : ");

    setcolor("#ff0000");
    scan(4, OnDan);
    setcolor("#000000");
}

HUE_DECLARE_APP("Postal", postal)
void postal()
{
    setcolor("#ff0000");
    gotoxy(2, 2);
    circle(9, 5);

    setcolor("#ffff00");
    gotoxy(3, 3);
    line(8, 3);
    line(8, 4);
    line(3, 4);
    line(3, 3);

    setcolor("#000000");
    gotoxy(3, 3);
    print("HELLO");

    setcolor("#808080");
    gotoxy(10, 10);
    image(18, 13, "btn_save_p");
}
