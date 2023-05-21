#pragma once
#include <boss.h>

#ifdef __cplusplus
    extern "C" {
#endif

    // 비공개부
    typedef void (*AppCB)();
    typedef void (*ScanCB)(const char* text, int enter);
    typedef void (*ButtonCB)(int x, int y);
    int _declareapp(const char* app, AppCB cb);

    // 공개부
    void clrscr(int w, int h, const char* bgcolor);
    void setcolor(const char* color);
    void setbgcolor(const char* bgcolor);
    void print(const char* format, ...);
    void scan(int w, ScanCB cb);
    void button(int w, int h, ButtonCB cb);
    void gotoxy(int x, int y);
    void line(int x, int y);
    void rect(int x, int y);
    void circle(int x, int y);
    void image(int x, int y, const char* name);
    void repaint();

#ifdef __cplusplus
    }
#endif

// 앱등록
#define HUE_DECLARE_APP(NAME, FUNC) \
    static void FUNC(); \
    static int _##FUNC = _declareapp(NAME, FUNC);
