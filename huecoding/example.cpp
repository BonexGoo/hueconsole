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
    send("freetalk.text", text, boss_strlen(text));
}

void RecvTalk(const void* data, int length)
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
    
    listen("freetalk.text", 10, RecvTalk);
}

////////////////////////////////////////////////////////////////////////////////

const char* LastColor = "#000000";
void OnClick(int x, int y)
{
    setcolor(LastColor);
    gotoxy(x, y);
    circle(x + 1, y + 1);

    char Pixel[8];
    Pixel[0] = x;
    Pixel[1] = y;
    Pixel[2] = LastColor[1];
    Pixel[3] = LastColor[2];
    Pixel[4] = LastColor[3];
    Pixel[5] = LastColor[4];
    Pixel[6] = LastColor[5];
    Pixel[7] = LastColor[6];
    send("iconeditor.pixel", Pixel, 8);
}

void RecvClick(const void* data, int length)
{
    if(length != 8) return;
    auto Pixel = (char*) data;
    int X = Pixel[0];
    int Y = Pixel[1];
    const char Color[8] = {'#', Pixel[2], Pixel[3], Pixel[4], Pixel[5], Pixel[6], Pixel[7], '\0'};

    setcolor(Color);
    gotoxy(X, Y);
    circle(X + 1, Y + 1);
}

void OnRed(int x, int y)
{
    LastColor = "#ff0000";
}

void OnGreen(int x, int y)
{
    LastColor = "#00ff00";
}

void OnBlue(int x, int y)
{
    LastColor = "#0000ff";
}

void OnBlack(int x, int y)
{
    LastColor = "#000000";
}

void OnWhite(int x, int y)
{
    LastColor = "#ffffff";
}

HUE_DECLARE_APP("IconEditor", iconeditor)
void iconeditor()
{
    setbgcolor("#808080");
    clrscr(52, 63);

    setcolor("#ffffff");
    gotoxy(1, 1);
    button(50, 50, OnClick);
    rect(51, 51);

    setcolor("#404040");
    gotoxy(1, 52);
    rect(51, 59);

    // 빨강
    setcolor("#ff0000");
    gotoxy(2, 53);
    button(5, 5, OnRed);
    rect(7, 58);

    // 초록
    setcolor("#00ff00");
    gotoxy(9, 53);
    button(5, 5, OnGreen);
    rect(14, 58);

    // 파랑
    setcolor("#0000ff");
    gotoxy(16, 53);
    button(5, 5, OnBlue);
    rect(21, 58);

    // 검정
    setcolor("#000000");
    gotoxy(23, 53);
    button(5, 5, OnBlack);
    rect(28, 58);

    // 하양
    setcolor("#ffffff");
    gotoxy(30, 53);
    button(5, 5, OnWhite);
    rect(35, 58);

    listen("iconeditor.pixel", 500, RecvClick);
}
