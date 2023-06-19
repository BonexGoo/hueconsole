#include <huecoding.h>

void OnRefresh(int x, int y)
{
    repaint();
}

void OnDan(const char* text)
{
    int dan = boss_atoi(text);

    print("\n [%i 단]\n", dan);
    for(int i = 1; i <= 9; ++i)
        print(" %i x %i = %2i\n", dan, i, dan * i);

    print("\n ");
    button(7, 1, OnRefresh);
    print(" Retry \n");
}

//HUE_DECLARE_APP("구구단", googoodan)
void googoodan()
{
    clrscr(60, 20);
    print("\n 원하는 구구단을 적어봐요(2~9) : ");

    setcolor("#ff0000");
    scan(4, OnDan);
    setcolor("#000000");
}

////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <math.h>
#include <time.h>
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)
#define M_PI   3.14159265358979323846264338327950288

char TimeText[20];
int Angle[3] = {0, 0, 0};
float Dist[3] = {7, 6, 4};
void OnUpdate(int x, int y)
{
    time_t t = time(NULL);
    struct tm* tt = localtime(&t);
    int Hour = tt->tm_hour % 12;
    sprintf(TimeText, "%s %02d:%02d:%02d", (tt->tm_hour < 12)? "AM" : "PM",
        (0 < Hour)? Hour : 12, tt->tm_min, tt->tm_sec);
    Angle[0] = tt->tm_sec * 6;
    Angle[1] = tt->tm_min * 6;
    Angle[2] = Hour * 30;
    repaint();
}

//HUE_DECLARE_APP("미니시계", miniclock)
void miniclock()
{
    clrscr(30, 30);

    gotoxy(15, 15);
    for(int a = 0; a < 3; a++)
    {
        float XYs[3][2] = {
            {-0.5, -Dist[a]},
            {0, -Dist[a] - 3},
            {0.5, -Dist[a]}};
        for(int i = 0; i < 3; i++)
        {
            float Rad = degToRad(Angle[a]);
            float X = XYs[i][0] * cos(Rad) - XYs[i][1] * sin(Rad);
            float Y = XYs[i][0] * sin(Rad) + XYs[i][1] * cos(Rad);
            line(15 + X, 15 + Y);
        }
        line(15, 15);
    }

    gotoxy(14, 14);
    circle(16, 16);

    gotoxy(1, 1);
    button(8, 1, OnUpdate);
    print("새로고침");
    print(" %s", TimeText);
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

//HUE_DECLARE_APP("우리톡톡", freetalk)
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
    if(length < 2) return;
    auto Pixel = (char*) data;
    int X = Pixel[0];
    int Y = Pixel[1];
    if(X == -1 && Y == -1)
    {
        setcolor("#ffffff");
        gotoxy(1, 1);
        rect(51, 51);
    }
    else if(length == 8)
    {
        const char Color[8] = {'#', Pixel[2], Pixel[3], Pixel[4], Pixel[5], Pixel[6], Pixel[7], '\0'};
        setcolor(Color);
        gotoxy(X, Y);
        circle(X + 1, Y + 1);
    }
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

void OnRemove(int x, int y)
{
    char Pixel[2];
    Pixel[0] = -1;
    Pixel[1] = -1;
    send("iconeditor.pixel", Pixel, 2);
}

//HUE_DECLARE_APP("아이콘에디터", iconeditor)
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

    // 지우개
    setcolor("#808080");
    gotoxy(45, 53);
    button(5, 5, OnRemove);
    rect(50, 58);
    setcolor("#ff0000");
    gotoxy(45, 53);
    line(50, 58);

    listen("iconeditor.pixel", 500, RecvClick);
}
