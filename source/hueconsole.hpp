﻿#pragma once
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
    float mXBegin {0};
    float mYBegin {0};
    float mXEnd {0};
    float mYEnd {0};
    Color mColor;
    String mImage;
};

class hueconsoleData : public ZayObject
{
public:
    hueconsoleData();
    ~hueconsoleData();

public:
    void RenderApp(ZayPanel& panel);
    void RenderLobby(ZayPanel& panel);
    void RenderImeDialog(ZayPanel& panel);

public:
    static void ClearScreen(sint32 w, sint32 h);
    static void SetColor(Color color);
    static void SetBGColor(Color color);
    static void Print(String text);
    static void Scan(sint32 w, TextCB cb);
    static void Button(sint32 w, sint32 h, ClickCB cb);
    static void GotoXY(sint32 x, sint32 y);
    static void GraphTo(Graph::Type type, float x, float y);
    static void ImageTo(chars name, float x, float y);
    static void Listen(chars key, sint32 recent, BinaryCB cb);
    static void Send(chars key, bytes data, sint32 length);
    static void Repaint();

private:
    void ValidCells(sint32 count);
    void SortingApps(bool init);

public:
    void Send_GetToken();
    void Send_GetVisitor();
    void Send_GetHeart(chars item);
    void Send_TurnHeart(chars item);
    void Send_SendData(chars key, chars base64);
    void Send_ListenData(chars key, sint32 recent);
    bool RecvOnce();
    void OnRecvMessage(chars message);
    void OnRecv_Token(const Context& json);
    void OnRecv_Visitor(const Context& json);
    void OnRecv_Heart(const Context& json);
    void OnRecv_DataUpdated(const Context& json);

public:
    String mLastApp;
    sint32 mCellWidth {0};
    sint32 mCellHeight {0};
    Array<Cell> mCells;
    sint32 mCellFocus {0};
    float mLastGraphX {0};
    float mLastGraphY {0};
    Color mLastColor {Color::Black};
    Color mLastBGColor {Color::White};
    Color mClearColor {Color::White};
    Array<Box> mBoxes;
    Array<Graph> mGraphs;
    bool mScrollLock {false};
    sint32 mScrollLog {0};
    sint32 mScrollPhy {0};
    uint64 mUpdateMsec {0};
    String mSystemFont;

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
    Map<BinaryCB> mLoaders;

public:
    static inline Map<App>& _AllApps()
    {static Map<App> _; return _;}
};
