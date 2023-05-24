#pragma once
#include <service/boss_zay.hpp>
#include <huecoding.h>

class App
{
public:
    sint32 mStar {0};
    bool mStarVoted {false};
    sint32 mOrderLog {0};
    sint32 mOrderPhy {0};
    AppCB mCB {nullptr};
};

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
    sint32 mLeft {0};
    sint32 mTop {0};
    sint32 mRight {0};
    sint32 mBottom {0};
    Color mColor;
    TextCB mTextCB {nullptr};
    ClickCB mClickCB {nullptr};
};

class Graph
{
public:
    enum class Type {Null, Line, Rect, Circle, Image};
    Type mType {Type::Null};
    sint32 mXBegin {0};
    sint32 mYBegin {0};
    sint32 mXEnd {0};
    sint32 mYEnd {0};
    Color mColor;
    String mImage;
};

class hueconsoleData : public ZayObject
{
public:
    hueconsoleData();
    ~hueconsoleData();

public:
    void RenderImeDialog(ZayPanel& panel);

public:
    static void ClearScreen(sint32 w, sint32 h);
    static void SetColor(Color color);
    static void SetBGColor(Color color);
    static void Print(String text);
    static void Scan(sint32 w, TextCB cb);
    static void Button(sint32 w, sint32 h, ClickCB cb);
    static void GotoXY(sint32 x, sint32 y);
    static void GraphTo(Graph::Type type, sint32 x, sint32 y);
    static void ImageTo(chars name, sint32 x, sint32 y);
    static void SetLoader(chars name, sint32 recent, BinaryCB cb);
    static void Save(chars name, bytes data, int length);
    static void Repaint();

private:
    void ValidCells(sint32 count);
    void SortingApps(bool init);

public:
    void SendGetToken();
    void SendGetVisitor();
    void SendGetHeart(chars item);
    void SendTurnHeart(chars item);
    bool RecvOnce();
    void OnRecvMessage(chars message);
    void OnRecv_Token(const Context& json);
    void OnRecv_Visitor(const Context& json);
    void OnRecv_Heart(const Context& json);

public:
    String mLastApp;
    sint32 mCellWidth {0};
    sint32 mCellHeight {0};
    Array<Cell> mCells;
    sint32 mCellFocus {0};
    Color mClearColor {Color::White};
    Color mLastColor {Color::Black};
    Color mLastBGColor {Color::White};
    Array<Box> mBoxes;
    Array<Graph> mGraphs;
    bool mScrollLock {false};
    sint32 mScrollLog {0};
    sint32 mScrollPhy {0};
    uint64 mUpdateMsec {0};

public: // IME관련
    point64 mImePosLog {0, 0};
    point64 mImePosPhy {0, 0};
    bool mImeShifted {false};

public: // 웹소켓관련
    id_socket mSocket {nullptr};
    bool mHasConnected {false};
    uint08s mRecvQueue;
    String mToken;
    sint32 mInfo_Total {0};
    sint32 mInfo_Member {0};
    sint32 mInfo_RealTime {0};

public:
    static inline Map<App>& _AllApps()
    {static Map<App> _; return _;}
};
