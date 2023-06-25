#include <huecoding.h>

static void next(int x, int y)
{
    clrscr(50, 25);

    gotoxy(20, 3);
    print("항상 감사합니다\n");
}


HUE_DECLARE_APP("코딩엽서/고은빈", postcard)
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

///////////////////////////////////////

void On1(int x, int y)
{
    gotoxy(0, 5);
    print("   정답 : 흰색\n\n");
}

void On2(int x, int y)
{
    gotoxy(0, 10);
    print("   정답 : 강아지\n");
}

void On3(int x, int y)
{
    gotoxy(0, 15);
    print("   정답 : 아이폰\n\n");
}

void On4(int x, int y)
{
    gotoxy(0, 20);
    print("   정답 : 사리울중\n\n");
}


HUE_DECLARE_APP("나를 잘 아는 사람/고은빈", main33)
void main33()
{
    print("    <나를 가장 잘 아는 사람은??>\n\n");



    print("   어떤색을 가장 좋아할까?\n   ");

    button(8, 1, On1);
    print("1.노란색   ");

    button(8, 1, On1);
    print("2. 흰색   ");

    button(8, 1, On1);
    print("3.하늘색\n");




    print("\n\n\n   내가 가장 좋아하는 동물은?\n   ");

    button(8, 1, On2);
    print("1.고양이     ");
    
    button(8, 1, On2);
    print("2.강아지     ");

    button(8, 1, On2);
    print("3.물고기\n");



    print("\n\n\n   내가 쓰는 핸드폰은??\n   ");

    button(8, 1, On3);
    print("1.삼성폰   ");

    button(8, 1, On3);
    print("2.아이폰\n");



    print("\n\n\n   내가 다니는 중학교는??\n   ");

    button(10, 1, On4);
    print("1.사리울중   ");

    button(8, 1, On4);
    print("2.신정중   ");

    button(8, 1, On4);
    print("2.신송중   ");

    button(8, 1, On4);
    print("2.예송중   ");
}
