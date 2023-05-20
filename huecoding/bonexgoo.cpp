#include <huecoding.h>

void OnDan(const char* text, int enter)
{
    if(enter)
    {
        int dan = boss_atoi(text);

        print("\n [%i DAN]\n", dan);
        for(int i = 1; i <= 9; ++i)
            print(" %i x %i = %2i\n", dan, i, dan * i);

        print("\n ");
        clickbox(7, 1, repaint);
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
