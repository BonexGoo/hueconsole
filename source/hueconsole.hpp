#pragma once
#include <service/boss_zay.hpp>
#include <huecoding.h>

class Cell
{
public:
    String mLetter;
    Color mColor;
    Color mBGColor;
    uint64 mWrittenMsec {0};
};

class Box
{
public:
    Color mColor;
    sint32 mLeft {0};
    sint32 mTop {0};
    sint32 mRight {0};
    sint32 mBottom {0};
    ScanCB mScanCB {nullptr};
    ClickCB mClickCB {nullptr};
};

class hueconsoleData : public ZayObject
{
public:
    hueconsoleData();
    ~hueconsoleData();

public:
    void RenderImeDialog(ZayPanel& panel);

public:
    static void ClearScreen(sint32 w, sint32 h, Color bgcolor);
    static void GotoXY(sint32 x, sint32 y);
    static void SetColor(Color color);
    static void SetBGColor(Color bgcolor);
    static void TextPrint(String text);
    static void TextScan(sint32 w, ScanCB cb);
    static void ClickBox(sint32 w, sint32 h, ClickCB cb);
    static void Repaint();

private:
    void ValidCells(sint32 count);

public:
    String mLastApp;
    sint32 mCellWidth {0};
    sint32 mCellHeight {0};
    Array<Cell> mCells;
    sint32 mCellFocus {0};
    Color mLastColor {Color::Black};
    Color mLastBGColor {Color::White};
    Color mClearBGColor {Color::White};
    Array<Box> mBoxes;
    sint32 mScrollLog {0};
    sint32 mScrollPhy {0};
    uint64 mUpdateMsec {0};

public:
    point64 mImePosLog {0, 0};
    point64 mImePosPhy {0, 0};
    bool mImeShifted {false};

public:
    static inline Map<AppCB>& _AllApps()
    {static Map<AppCB> _; return _;}
};
