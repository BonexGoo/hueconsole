#include <huecoding.h>

void OnRefresh(int x, int y)
{
    repaint();
}

void OnDan(const char* text)
{
    int dan = boss_atoi(text);

    print("\n [%i DAN]\n", dan);
    for(int i = 1; i <= 9; ++i)
        print(" %i x %i = %2i\n", dan, i, dan * i);

    print("\n ");
    button(7, 1, OnRefresh);
    print(" Retry \n");
}

HUE_DECLARE_APP("GooGooDan", googoodan)
void googoodan()
{
    clrscr(60, 20);
    print("\n Please type the DAN(2~9) you want : ");

    setcolor("#ff0000");
    scan(4, OnDan);
    setcolor("#000000");
}

////////////////////////////////////////////////////////////////////////////////

void OnTalk(const char* text)
{
    send("postal.talk", text, boss_strlen(text));
}

void OnBinary(const void* data, int length)
{
    print("\n >> %.*s", length, data);
}

HUE_DECLARE_APP("FreeTalk", freetalk)
void freetalk()
{
    setcolor("#808080");
    gotoxy(11, 1);
    image(14, 4, "ok_over");

    gotoxy(2, 2);
    scan(8, OnTalk);
    
    listen("postal.talk", 10, OnBinary);
}
