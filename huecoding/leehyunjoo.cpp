#include <huecoding.h>

static void next(int x, int y)
{
    clrscr(50, 25);

    gotoxy(20, 3);
    print("사랑해여~~~!!!!!\n");
    setcolor("#ff0000");

}


HUE_DECLARE_APP("그림엽서/d('~')b", postcard)
void postcard()
{
    gotoxy(20, 3);
    print("엄마에게\n");
    setcolor("#ffff00");
    gotoxy(20, 4);
    line(28, 4);



    gotoxy(12, 5);
    setcolor("#0000ff");
    print("엄마 항상 저에게\n");

    gotoxy(12, 6);
    print("잘해주셔서 감사해요~\n");

    setcolor("#000000");
    gotoxy(25, 10);
    print("d('~')b가\n");

    gotoxy(20, 12);
    button(8, 1, next);
    print("다음으로");

    setcolor("#808080");
        gotoxy(1, 1);
        image(5, 5, "character_a");
}
