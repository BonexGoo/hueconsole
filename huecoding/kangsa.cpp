#include <huecoding.h>

static void SetTile()
{
    gotoxy(0, 0);
    for(int y = 0; y < 16; y++)
    for(int x = 0; x < 16; x++)
    {
        if(x % 2 == y % 2) // X축-짝홀여부가 Y축-짝홀여부와 같으면
            setbgcolor("#f0fff0");
        else setbgcolor("#ffffff");
        print(" ");
    }
}

static void OnClick(int x, int y)
{
    char Dot[2];
    Dot[0] = x;
    Dot[1] = y;
    send("dot.kangsa", Dot, 2);
}

static void OnListen(const void* data, int length)
{
    auto Dot = (char*) data;
    int X = Dot[0];
    int Y = Dot[1];
    if(X == 0 && Y == 0) // [0, 0]블럭을 눌렀다면
        SetTile();
    else
    {
        gotoxy(X, Y);
        setbgcolor("#000000");
        print(" ");
    }
}

//HUE_DECLARE_APP("도트편집기/강사", doteditor)
void doteditor()
{
    clrscr(16, 16);
    button(16, 16, OnClick);

    SetTile();
    listen("dot.kangsa", 500, OnListen);
}
