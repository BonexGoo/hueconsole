#pragma once
#include <boss.h>

#ifdef __cplusplus
    extern "C" {
#endif

    // 비공개부
    typedef void (*AppCB)();
    typedef void (*TextCB)(const char* text);
    typedef void (*ClickCB)(int x, int y);
    int _declareapp(const char* app, AppCB cb);

    // 공개부
    void clrscr(int w, int h);
    void setcolor(const char* name);
    void setbgcolor(const char* name);
    void print(const char* format, ...);
    void scan(int w, TextCB cb);
    void button(int w, int h, ClickCB cb);
    void gotoxy(int x, int y);
    void line(int x, int y);
    void rect(int x, int y);
    void circle(int x, int y);
    void image(int x, int y, const char* name);
    void push(const char* name, const char* text);
    void pop(const char* name, int count, TextCB cb);
    void repaint();

#ifdef __cplusplus
    }
#endif

// 앱등록
#define HUE_DECLARE_APP(NAME, FUNC) \
    static void FUNC(); \
    static int _##FUNC = _declareapp(NAME, FUNC);
