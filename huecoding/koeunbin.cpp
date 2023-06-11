#include <huecoding.h>

static void next(int x, int y)
{
    clrscr(50, 25);

    gotoxy(20, 3);
    print("항상 감사합니다\n");
}


HUE_DECLARE_APP("그림엽서/고은빈", postcard)
void postcard()
{
    gotoxy(20, 3);
    print("엄마에게\n");
    setcolor("#ffff00");
    gotoxy(20, 4);
    line(28, 4);


    gotoxy(12, 5);
    setcolor("#0000ff");
    print("엄마 항상 절\n");


    gotoxy(12, 6);
    print("챙겨주셔서 감사해요~\n");


  
    gotoxy(20, 12);
    button(8, 1, next);
    print("다음으로");

}
