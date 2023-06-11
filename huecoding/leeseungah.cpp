#include <huecoding.h>

HUE_DECLARE_APP("코딩엽서/이승아", postcard)
void postcard()
{
    setcolor("#000000");
    gotoxy(18, 1);
    print("<칭찬편지>\n");

    setcolor("#000000");
    gotoxy(20, 3);
    print("to.나\n");

    setcolor("#ff0000");
    gotoxy(16, 7);
    print("못하는게 없고\n");

    setcolor("#00ff00");
    gotoxy(16, 9);
    print("드럽게 잘나서\n");

    setcolor("#0000ff");
    gotoxy(9, 11);
    print("모든 친구들의 부러움을 사는\n");

    setcolor("#00ff00");
    gotoxy(12, 13);
    print("해송중 1학년 이승아를\n");

    setcolor("#ff0000");
    gotoxy(17, 15);
    print("칭찬합니다\n");

    setcolor("#000000");
    gotoxy(16, 19);
    print("from.이승아");
    image(28, 20, "character_b");

}

   
    