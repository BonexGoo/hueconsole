#include <huecoding.h>

int score1 = 0;
void on_text1(const char* text)
{
    int len = boss_strlen(text);
    for(int i = 0; i < len; i++)
    {
        score1 += text[i] + 128;
    }
}

int score2 = 0;
void on_text2(const char* text)
{
    int len = boss_strlen(text);
    for(int i = 0; i < len; i++)
    {
        score2 += text[i] + 128;
    }
}

void oncheck(int x, int y)
{
    clrscr(50, 25);
    int v = (score1+score2)%100;
    print("\n결과는? %d점", v);
}

HUE_DECLARE_APP("이름으로 보는 연애확률/조민솔", lovepercent)
void lovepercent()
{
    gotoxy(14, 11); setcolor("#0000ff"); print("남자");scan(5, on_text1); gotoxy(23, 11); print("은/는\n\n");

    gotoxy(14, 13); setcolor("#ff0000"); print("여자");scan(5, on_text2); gotoxy(24, 13); print("을/를 얼만큼 좋아할까?\n");

    setcolor("#000000");gotoxy(18,18); button(12,1, oncheck); print("결과 확인!!!");
}
