#include <boss.hpp>
#include "huecoding.h"

#include <resource.hpp>
#include <hueconsole.hpp>
#include <stdarg.h>

int _declareapp(const char* app, AppCB cb)
{
    auto& NewApp = hueconsoleData::_AllApps()(app);
    NewApp.mCB = cb;
    return 0;
}

void clrscr(int w, int h)
{
    hueconsoleData::ClearScreen(w, h);
}

void setcolor(const char* name)
{
    hueconsoleData::SetColor(name);
}

void setbgcolor(const char* name)
{
    hueconsoleData::SetBGColor(name);
}

void print(const char* format, ...)
{
    va_list Args;
    va_start(Args, format);
    const sint32 Size = boss_vsnprintf(nullptr, 0, format, Args);
    va_end(Args);

    chararray Collector;
    if(0 <= Size)
    {
        Collector.AtWherever(Size) = '\0';
        va_start(Args, format);
        boss_vsnprintf(Collector.AtDumping(0, Size + 1), Size + 1, format, Args);
        va_end(Args);
        hueconsoleData::Print(&Collector[0]);
    }
    else
    {
        BOSS_ASSERT("vsnprintf에서 text의 길이를 추산하지 못함", false);
        hueconsoleData::Print("<error>");
    }
}

void scan(int w, TextCB cb)
{
    hueconsoleData::Scan(w, cb);
}

void button(int w, int h, ClickCB cb)
{
    hueconsoleData::Button(w, h, cb);
}

void gotoxy(int x, int y)
{
    hueconsoleData::GotoXY(x, y);
}

void line(int x, int y)
{
    hueconsoleData::GraphTo(Graph::Type::Line, x, y);
}

void rect(int x, int y)
{
    hueconsoleData::GraphTo(Graph::Type::Rect, x, y);
}

void circle(int x, int y)
{
    hueconsoleData::GraphTo(Graph::Type::Circle, x, y);
}

void image(int x, int y, const char* name)
{
    hueconsoleData::ImageTo(name, x, y);
}

void listen(const char* key, int recent, BinaryCB cb)
{
    hueconsoleData::Listen(key, recent, cb);
}

void send(const char* key, const void* data, int length)
{
    hueconsoleData::Send(key, (bytes) data, length);
}

void repaint()
{
    hueconsoleData::Repaint();
}
