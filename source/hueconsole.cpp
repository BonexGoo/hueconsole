#include <boss.hpp>
#include "hueconsole.hpp"

#include <resource.hpp>
#include <service/boss_zaywidget.hpp>
#include <base64/base64.h>

ZAY_DECLARE_VIEW_CLASS("hueconsoleView", hueconsoleData)

extern h_view gView;
extern sint32 gViewWidth;
extern sint32 gViewHeight;
static hueconsoleData* gSelf = nullptr;

ZAY_VIEW_API OnCommand(CommandType type, id_share in, id_cloned_share* out)
{
    if(type == CT_Size)
    {
        sint32s WH(in);
        gViewWidth = WH[0];
        gViewHeight = WH[1];
        m->mImePosLog.x = Math::Clamp(m->mImePosLog.x,
            0 - gViewWidth / 2 + (30 * 10) / 2,
            gViewWidth - gViewWidth / 2 - (30 * 10) / 2);
    }
    else if(type == CT_Tick)
    {
        if(Platform::Utility::CurrentTimeMsec() <= m->mUpdateMsec)
            m->invalidate(2);

        // 웹소켓 연결직후
        if(!m->mHasConnected && Platform::Socket::IsConnected(m->mSocket))
        {
            m->mHasConnected = true;
            m->Send_GetToken();
            m->Send_GetVisitor();
        }

        // 앱리스트
        if(m->mLastApp.Length() == 0)
        {
            auto& AllApps = hueconsoleData::_AllApps();
            for(sint32 i = 0, iend = AllApps.Count(); i < iend; ++i)
            {
                if(auto OneApp = AllApps.AccessByOrder(i))
                {
                    if(OneApp->mOrderPhy < OneApp->mOrderLog)
                    {
                        OneApp->mOrderPhy = Math::Min((OneApp->mOrderPhy * 8 + OneApp->mOrderLog * 2) / 10 + 1, OneApp->mOrderLog);
                        m->invalidate(2);
                    }
                    else if(OneApp->mOrderLog < OneApp->mOrderPhy)
                    {
                        OneApp->mOrderPhy = Math::Max((OneApp->mOrderPhy * 8 + OneApp->mOrderLog * 2) / 10 - 1, OneApp->mOrderLog);
                        m->invalidate(2);
                    }
                }
            }
        }
        // 앱진입
        else
        {
            // 스크롤처리
            if(!m->mScrollLock)
            {
                const sint32 ScrollMin = 0 * 1000;
                const sint32 ScrollMax = Math::Max(0, (m->mCells.Count() / m->mCellWidth) - m->mCellHeight) * 1000;
                if(m->mScrollLog < ScrollMin)
                {
                    m->mScrollLog = Math::Min((m->mScrollLog * 7 + ScrollMin * 3) / 10 + 1, ScrollMin);
                    m->invalidate(2);
                }
                else if(ScrollMax < m->mScrollLog)
                {
                    m->mScrollLog = Math::Max((m->mScrollLog * 7 + ScrollMax * 3) / 10 - 1, ScrollMax);
                    m->invalidate(2);
                }
            }
            if(m->mScrollPhy < m->mScrollLog)
            {
                m->mScrollPhy = Math::Min((m->mScrollPhy * 8 + m->mScrollLog * 2) / 10 + 1, m->mScrollLog);
                m->invalidate(2);
            }
            else if(m->mScrollLog < m->mScrollPhy)
            {
                m->mScrollPhy = Math::Max((m->mScrollPhy * 8 + m->mScrollLog * 2) / 10 - 1, m->mScrollLog);
                m->invalidate(2);
            }

            // IME처리
            if(m->mImePosPhy.x < m->mImePosLog.x)
            {
                m->mImePosPhy.x = Math::Min((m->mImePosPhy.x * 7 + m->mImePosLog.x * 3) / 10 + 1, m->mImePosLog.x);
                m->invalidate(2);
            }
            else if(m->mImePosLog.x < m->mImePosPhy.x)
            {
                m->mImePosPhy.x = Math::Max((m->mImePosPhy.x * 7 + m->mImePosLog.x * 3) / 10 - 1, m->mImePosLog.x);
                m->invalidate(2);
            }
            if(m->mImePosLog.y == 1 && m->mImePosPhy.y < 30 * 4)
            {
                m->mImePosPhy.y = Math::Min((m->mImePosPhy.y * 8 + (30 * 4) * 2) / 10 + 1, 30 * 4);
                m->invalidate(2);
            }
            else if(m->mImePosLog.y == 0 && 0 < m->mImePosPhy.y)
            {
                m->mImePosPhy.y = Math::Max((m->mImePosPhy.y * 8 + (0) * 2) / 10 - 1, 0);
                m->invalidate(2);
            }
        }
    }
}

ZAY_VIEW_API OnNotify(NotifyType type, chars topic, id_share in, id_cloned_share* out)
{
    if(type == NT_ZayWidget)
    {
        if(!String::Compare(topic, "EnterPressing"))
        {
            const String DOMName(in);
            const sint32 BoxIdx = Parser::GetInt(DOMName.Offset(4)); // dom_0
            const String Text = ZayWidgetDOM::GetComment(DOMName);
            auto& CurBox = gSelf->mBoxes[BoxIdx];
            if(CurBox.mTextCB)
                CurBox.mTextCB(Text);
        }
    }
    else if(type == NT_SocketReceive)
    {
        if(m->RecvOnce())
            m->invalidate();
    }
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
    if(0 < m->mLastApp.Length())
    {
        static sint32 OldScrollLog;
        static sint32 OldY;

        if(type == GT_Pressed)
        {
            m->clearCapture();
            m->mScrollLock = true;
            OldScrollLog = m->mScrollLog;
            OldY = y;
        }
        else if(type == GT_InDragging || type == GT_OutDragging)
        {
            m->mScrollLog = OldScrollLog + m->mCellHeight * 1000 * (OldY - y) / Math::Max(1, gViewHeight);
            m->invalidate(2);
        }
        else if(type == GT_InReleased || type == GT_OutReleased || type == GT_CancelReleased)
        {
            m->mScrollLock = false;
        }
        else if(type == GT_WheelUp || type == GT_WheelDown)
        {
            m->mScrollLog = m->mScrollLog + ((type == GT_WheelUp)? -1000 : 1000) * 3;
            m->invalidate(2);
        }
    }
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 192, 192, 192)
        panel.fill();

    if(0 < m->mLastApp.Length())
    {
        const float FontSizeA = panel.w() / m->mCellWidth / 7;
        const float FontSizeB = panel.h() / m->mCellHeight / 15;
        ZAY_FONT(panel, Math::MinF(FontSizeA, FontSizeB), m->mSystemFont)
        ZAY_MOVE(panel, 0, -panel.h() * m->mScrollPhy / m->mCellHeight / 1000)
            m->RenderApp(panel);
        // 가상키보드
        ZAY_FONT(panel, 1.5, m->mSystemFont)
        ZAY_XYWH(panel, panel.w() / 2 - (30 * 10) / 2 + m->mImePosPhy.x, panel.h() - m->mImePosPhy.y, 30 * 10, 30 * 4)
            m->RenderImeDialog(panel);
    }
    else ZAY_FONT(panel, 1.0, m->mSystemFont)
        m->RenderLobby(panel);
}

hueconsoleData::hueconsoleData()
{
    gSelf = this;
    ClearScreen(50, 25);
    mSocket = Platform::Socket::OpenForWS(false);
    Platform::Socket::ConnectAsync(mSocket, "220.121.14.168", 7993);
    SortingApps(true);

    String DateText = __DATE__;
    DateText.Replace("Jan", "01"); DateText.Replace("Feb", "02"); DateText.Replace("Mar", "03");
    DateText.Replace("Apr", "04"); DateText.Replace("May", "05"); DateText.Replace("Jun", "06");
    DateText.Replace("Jul", "07"); DateText.Replace("Aug", "08"); DateText.Replace("Sep", "09");
    DateText.Replace("Oct", "10"); DateText.Replace("Nov", "11"); DateText.Replace("Dec", "12");
    const String Day = String::Format("%02d", Parser::GetInt(DateText.Middle(2, DateText.Length() - 6).Trim()));
    DateText = DateText.Right(4) + "/" + DateText.Left(2) + "/" + Day;
    ZayWidgetDOM::SetValue("update", "'" + DateText + "'");

    // 시스템폰트 등록
    buffer NewFontData = Asset::ToBuffer("font/daum_regular.ttf");
    mSystemFont = Platform::Utility::CreateSystemFont((bytes) NewFontData, Buffer::CountOf(NewFontData));
    Buffer::Free(NewFontData);
}

hueconsoleData::~hueconsoleData()
{
    gSelf = nullptr;
    Platform::Socket::Close(mSocket);
}

void hueconsoleData::RenderApp(ZayPanel& panel)
{
    // 셀-바탕
    for(sint32 y = 0, yend = mCells.Count() / mCellWidth; y < yend; ++y)
    {
        ZAY_LTRB_SCISSOR(panel, 0, sint32(panel.h() * y / mCellHeight),
            panel.w(), sint32(panel.h() * (y + 1) / mCellHeight))
        for(sint32 x = 0, xend = mCellWidth; x < xend; ++x)
        {
            const auto& CurCell = mCells[x + y * xend];
            const bool ExtendFont = (1 < CurCell.mLetter.Length());
            ZAY_LTRB(panel, sint32(panel.w() * x / mCellWidth), 0,
                sint32(panel.w() * (x + 1 + ExtendFont) / mCellWidth), panel.h())
            {
                ZAY_COLOR(panel, CurCell.mBGColor)
                {
                    ZAY_RGBA(panel, 128, 128, 128, 120)
                        panel.fill();
                    ZAY_LTRB(panel, 0, 0, panel.w() - 1, panel.h() - 1)
                        panel.fill();
                }
            }
            if(ExtendFont) x++;
        }
    }

    // 그래프
    for(sint32 i = 0, iend = mGraphs.Count(); i < iend; ++i)
    {
        auto& CurGraph = mGraphs[i];
        ZAY_COLOR(panel, CurGraph.mColor)
        switch(CurGraph.mType)
        {
        case Graph::Type::Line:
            panel.line(
                Point(sint32(panel.w() * CurGraph.mXBegin / mCellWidth),
                    sint32(panel.h() * CurGraph.mYBegin / mCellHeight)),
                Point(sint32(panel.w() * CurGraph.mXEnd / mCellWidth),
                    sint32(panel.h() * CurGraph.mYEnd / mCellHeight)), 2);
            break;
        case Graph::Type::Rect:
            ZAY_LTRB(panel,
                sint32(panel.w() * Math::Min(CurGraph.mXBegin, CurGraph.mXEnd) / mCellWidth),
                sint32(panel.h() * Math::Min(CurGraph.mYBegin, CurGraph.mYEnd) / mCellHeight),
                sint32(panel.w() * Math::Max(CurGraph.mXBegin, CurGraph.mXEnd) / mCellWidth),
                sint32(panel.h() * Math::Max(CurGraph.mYBegin, CurGraph.mYEnd) / mCellHeight))
                panel.fill();
            break;
        case Graph::Type::Circle:
            ZAY_LTRB(panel,
                sint32(panel.w() * Math::Min(CurGraph.mXBegin, CurGraph.mXEnd) / mCellWidth),
                sint32(panel.h() * Math::Min(CurGraph.mYBegin, CurGraph.mYEnd) / mCellHeight),
                sint32(panel.w() * Math::Max(CurGraph.mXBegin, CurGraph.mXEnd) / mCellWidth),
                sint32(panel.h() * Math::Max(CurGraph.mYBegin, CurGraph.mYEnd) / mCellHeight))
                panel.circle();
            break;
        case Graph::Type::Image:
            ZAY_RGBA(panel, 128, 128, 128, -128)
            ZAY_LTRB(panel,
                sint32(panel.w() * Math::Min(CurGraph.mXBegin, CurGraph.mXEnd) / mCellWidth),
                sint32(panel.h() * Math::Min(CurGraph.mYBegin, CurGraph.mYEnd) / mCellHeight),
                sint32(panel.w() * Math::Max(CurGraph.mXBegin, CurGraph.mXEnd) / mCellWidth),
                sint32(panel.h() * Math::Max(CurGraph.mYBegin, CurGraph.mYEnd) / mCellHeight))
                panel.ninepatch(R(CurGraph.mImage));
            break;
        }
    }

    // 셀-텍스트
    const uint64 CurMsec = Platform::Utility::CurrentTimeMsec();
    const sint32 FreeSpace = panel.h() / mCellHeight * 2;
    for(sint32 y = 0, yend = mCells.Count() / mCellWidth; y < yend; ++y)
    {
        ZAY_LTRB_SCISSOR(panel, 0, sint32(panel.h() * y / mCellHeight) - FreeSpace,
            panel.w(), sint32(panel.h() * (y + 1) / mCellHeight) + FreeSpace)
        for(sint32 x = 0, xend = mCellWidth; x < xend; ++x)
        {
            const auto& CurCell = mCells[x + y * xend];
            const bool ExtendFont = (1 < CurCell.mLetter.Length());
            ZAY_LTRB(panel, sint32(panel.w() * x / mCellWidth), FreeSpace,
                sint32(panel.w() * (x + 1 + ExtendFont) / mCellWidth), panel.h() - FreeSpace)
            {
                const float Opacity = (500 - Math::Max(0, CurCell.mWrittenMsec - CurMsec)) / 500.0f;
                ZAY_COLOR(panel, CurCell.mColor)
                ZAY_RGBA(panel, 128, 128, 128, 128 * Opacity)
                    panel.text(panel.w() / 2 - 1, panel.h() / 2 - 1, CurCell.mLetter, UIFA_CenterMiddle);
            }
            if(ExtendFont) x++;
        }
    }

    // 박스
    for(sint32 i = 0, iend = mBoxes.Count(); i < iend; ++i)
    {
        auto& CurBox = mBoxes[i];
        const String UIName = String::Format("ui_%d", i);
        ZAY_LTRB(panel,
            panel.w() * CurBox.mLeft / mCellWidth,
            panel.h() * CurBox.mTop / mCellHeight,
            panel.w() * CurBox.mRight / mCellWidth,
            panel.h() * CurBox.mBottom / mCellHeight)
        {
            const bool Focused = ((panel.state(UIName) & (PS_Focused | PS_Dropping)) == PS_Focused);
            ZAY_COLOR(panel, CurBox.mColor)
            for(sint32 j = 0, jend = (Focused)? 4 : 2; j < jend; ++j)
                ZAY_INNER(panel, -j)
                ZAY_RGBA(panel, 128, 128, 128, 128 - (128 / jend) * j)
                    panel.rect(1);

            // 스캔에디터
            if(CurBox.mTextCB)
            {
                const String DOMName = String::Format("dom_%d", i);
                ZAY_COLOR(panel, CurBox.mColor)
                ZAY_LTRB_SCISSOR(panel, 0, 0, panel.w(), panel.h())
                ZAY_LTRB(panel, 2, 0, (panel.w() - 4) - 2, panel.h())
                if(ZayControl::RenderEditBox(panel, UIName, DOMName, 1, true, false))
                    panel.repaint();
            }
            // 클릭박스
            else if(CurBox.mClickCB)
            {
                ZAY_INNER_UI(panel, 0, UIName,
                    ZAY_GESTURE_VNTXY(v, n, t, x, y, this, i)
                    {
                        if(t == GT_InReleased)
                        {
                            auto& CurBox = mBoxes[i];
                            if(CurBox.mClickCB)
                            {
                                auto OneRect = v->rect(n);
                                const sint32 X = CurBox.mLeft + (CurBox.mRight - CurBox.mLeft) * (x - OneRect.l) / (OneRect.r - OneRect.l);
                                const sint32 Y = CurBox.mTop + (CurBox.mBottom - CurBox.mTop) * (y - OneRect.t) / (OneRect.b - OneRect.t);
                                CurBox.mClickCB(X, Y);
                            }
                        }
                    });
            }
        }
    }
}

void hueconsoleData::RenderLobby(ZayPanel& panel)
{
    // 정보필드
    ZAY_FONT(panel, 1.2)
    ZAY_LTRB(panel, 0, 0, panel.w(), 40)
    {
        ZAY_RGB(panel, 220, 220, 255)
        {
            panel.fill();
            // 빌드날짜
            const String UpdateText = ZayWidgetDOM::GetValue("update").ToText();
            ZAY_RGB(panel, 96, 96, 96)
                panel.text(panel.w() - 10, panel.h() / 2, UpdateText, UIFA_RightMiddle);
            // 통계정보
            ZAY_RGB_IF(panel, 0, 0, 0, mHasConnected)
            ZAY_RGB_IF(panel, 96, 96, 96, !mHasConnected)
                panel.text(String::Format("  방문 %d회 / 실시간 %d명", mInfo_Total, mInfo_RealTime), UIFA_LeftMiddle, UIFE_Right);
        }
    }

    // 앱리스트
    auto& AllApps = hueconsoleData::_AllApps();
    ZAY_LTRB(panel, 10, 40, panel.w() - 10, panel.h() - 10)
    for(sint32 i = 0, iend = AllApps.Count(); i < iend; ++i)
    {
        chararray CurApp;
        if(auto OneApp = AllApps.AccessByOrder(i, &CurApp))
        ZAY_LTRB_SCISSOR(panel, 0, sint32(panel.h() * (OneApp->mOrderPhy * 0.001) / iend + 10),
            panel.w(), sint32(panel.h() * (OneApp->mOrderPhy * 0.001 + 1) / iend))
        {
            // 배경
            ZAY_RGB(panel, 255, 255, 255)
            {
                ZAY_RGBA(panel, 128, 128, 128, 120)
                    panel.fill();
                ZAY_LTRB(panel, 0, 0, panel.w() - 1, panel.h() - 1)
                    panel.fill();
            }

            // 앱버튼
            const String UIApp = String::Format("ui_app_%d", i);
            ZAY_LTRB_UI(panel, 0, 0, panel.w() - 150, panel.h(), UIApp,
                ZAY_GESTURE_T(t, this, CurApp)
                {
                    if(t == GT_InReleased)
                    {
                        auto& AllApps = hueconsoleData::_AllApps();
                        if(auto OneApp = AllApps.Access(&CurApp[0]))
                        {
                            mLastApp = &CurApp[0];
                            // 윈도우타이틀
                            Platform::SetWindowName(String::Format("%s - HueConsole[%dx%d]",
                                (chars) mLastApp, mCellWidth, mCellHeight));
                            OneApp->mCB();
                        }
                    }
                })
            {
                ZAY_FONT(panel, 2.0)
                ZAY_RGB(panel, 0, 0, 0)
                    panel.text(&CurApp[0], UIFA_CenterMiddle, UIFE_Right);
            }

            // 앱정보
            ZAY_LTRB(panel, panel.w() - 150, 0, panel.w(), panel.h())
            {
                // 별점
                ZAY_RGB(panel, 220, 220, 255)
                {
                    panel.fill();
                    ZAY_FONT(panel, 3.0)
                    ZAY_RGB(panel, 92, 92, 92)
                    ZAY_LTRB(panel, 0, 0, panel.w(), panel.h() * 0.3)
                        panel.text(panel.w() / 2, panel.h() / 2, "★");
                    ZAY_FONT(panel, 2.0)
                    ZAY_RGB(panel, 64, 64, 64)
                    ZAY_LTRB(panel, 0, panel.h() * 0.3, panel.w(), panel.h() * 0.5)
                        panel.text(panel.w() / 2, panel.h() / 2, String::Format("%04d", OneApp->mStar));
                }

                // 투표
                const String AppName(&CurApp[0]);
                const String UIVote = String::Format("ui_vote_%d", i);
                const bool Focused = ((panel.state(UIVote) & (PS_Focused | PS_Dropping)) == PS_Focused);
                const bool Pressed = ((panel.state(UIVote) & (PS_Pressed | PS_Dragging)) != 0);
                ZAY_LTRB_UI(panel, 0, panel.h() * 0.5, panel.w(), panel.h(), UIVote,
                    ZAY_GESTURE_T(t, this, AppName)
                    {
                        if(t == GT_InReleased)
                            Send_TurnHeart(AppName);
                    })
                ZAY_INNER_SCISSOR(panel, (Pressed)? 10 : 8)
                ZAY_RGB_IF(panel, 0, 0, 255, OneApp->mStarVoted)
                ZAY_RGB_IF(panel, 80, 80, 80, !OneApp->mStarVoted)
                {
                    ZAY_RGBA(panel, 128, 128, 128, (Focused)? 64 : 32)
                        panel.fill();
                    ZAY_FONT(panel, 1.5)
                        panel.text((OneApp->mStarVoted)? "VOTED" : "VOTE");
                    ZAY_INNER(panel, 2)
                        panel.rect(2);
                }
            }
        }
    }
}

void hueconsoleData::RenderImeDialog(ZayPanel& panel)
{
    static chars KeyCodes[4][10][2] = {
        {{"1", "."}, {"2", ","}, {"3", "?"}, {"4", "!"}, {"5", ":"}, {"6", "="}, {"7", "+"}, {"8", "-"}, {"9", "*"}, {"0", "/"}},
        {{"a", "A"}, {"b", "B"}, {"c", "C"}, {"d", "D"}, {"e", "E"}, {"f", "F"}, {"g", "G"}, {"h", "H"}, {"i", "I"}, {"j", "J"}},
        {{"k", "K"}, {"l", "L"}, {"m", "M"}, {"n", "N"}, {"o", "O"}, {"p", "P"}, {"q", "Q"}, {"r", "R"}, {" ", "_"}, {"↑", "↑"}},
        {{"s", "S"}, {"t", "T"}, {"u", "U"}, {"v", "V"}, {"w", "W"}, {"x", "X"}, {"y", "Y"}, {"z", "Z"}, {"←", "←"}, {"↙", "↙"}}};
    for(sint32 y = 0, yend = 4; y < yend; ++y)
    for(sint32 x = 0, xend = 10; x < xend; ++x)
    {
        const String UIName = String::Format("key_%d_%d", y, x);
        ZAY_LTRB_UI(panel, panel.w() * x / xend, panel.h() * y / yend, panel.w() * (x + 1) / xend, panel.h() * (y + 1) / yend, UIName,
            ZAY_GESTURE_T(t, y, x, this)
            {
                if(t == GT_InReleased)
                {
                    chars OneKey = KeyCodes[y][x][mImeShifted];
                    if(!String::Compare(OneKey, "↑"))
                        mImeShifted ^= true;
                    else
                    {
                        if(!String::Compare(OneKey, "←"))
                        {
                            Platform::SendKeyEvent(gView, 8, OneKey, true);
                            Platform::SendKeyEvent(gView, 8, OneKey, false);
                        }
                        else if(!String::Compare(OneKey, "↙"))
                        {
                            Platform::SendKeyEvent(gView, 13, OneKey, true);
                            Platform::SendKeyEvent(gView, 13, OneKey, false);
                        }
                        else
                        {
                            Platform::SendKeyEvent(gView, OneKey[0] & 0xFF, OneKey, true);
                            Platform::SendKeyEvent(gView, OneKey[0] & 0xFF, OneKey, false);
                        }
                        mImeShifted = false;
                    }
                }
            })
        ZAY_LTRB(panel, 2, 2, panel.w() - 3, panel.w() - 3)
        {
            const bool Focused = ((panel.state(UIName) & (PS_Focused | PS_Dropping)) == PS_Focused);
            const bool Pressed = ((panel.state(UIName) & PS_Pressed) != 0);
            const String CurText = KeyCodes[y][x][mImeShifted];
            // 바탕
            ZAY_INNER(panel, (Pressed)? 2 : 0)
            ZAY_RGB(panel, 220, 220, 255)
            ZAY_RGB_IF(panel, 255, 128, 128, mImeShifted && !CurText.Compare("↑"))
            {
                ZAY_RGBA(panel, 128, 128, 128, (Focused)? 96 : 64)
                    panel.fill();
                panel.rect(2);
            }
            // 텍스트
            ZAY_INNER_SCISSOR(panel, 0)
            ZAY_RGB(panel, 0, 0, 0)
                panel.text(panel.w() / 2, panel.h() / 2, CurText);
        }
    }

    // 타이틀
    ZAY_LTRB_UI(panel, 2, -18, panel.w() - 3, -3, "key_title",
        ZAY_GESTURE_TXY(t, x, y, this)
        {
            static bool HasDrag;
            static sint32 OldPosLogX;
            static sint32 OldX;

            if(t == GT_Pressed)
            {
                HasDrag = false;
                OldPosLogX = mImePosLog.x;
                OldX = x;
            }
            else if(t == GT_InDragging || t == GT_OutDragging)
            {
                if(mImePosLog.y == 1)
                {
                    HasDrag = true;
                    mImePosLog.x = OldPosLogX + x - OldX;
                    invalidate(2);
                }
            }
            else if(t == GT_InReleased || t == GT_OutReleased || t == GT_CancelReleased)
            {
                if(HasDrag)
                {
                    mImePosLog.x = Math::Clamp(mImePosLog.x,
                        0 - gViewWidth / 2 + (30 * 10) / 2,
                        gViewWidth - gViewWidth / 2 - (30 * 10) / 2);
                }
                else mImePosLog.y = 1 - mImePosLog.y;
            }
        })
    {
        const bool Focused = ((panel.state("key_title") & (PS_Focused | PS_Dropping)) == PS_Focused);
        const bool Pressed = ((panel.state("key_title") & (PS_Pressed | PS_Dragging)) != 0);
        ZAY_RGB(panel, 200, 200, 200)
        {
            ZAY_RGBA(panel, 128, 128, 128, (Focused) ? 96 : 64)
                panel.fill();
            ZAY_FONT(panel, 0.6)
                panel.text(" KEYBOARD", UIFA_LeftMiddle, UIFE_Right);
            panel.rect(2);
        }
    }
}

void hueconsoleData::ClearScreen(sint32 w, sint32 h)
{
    if(gSelf)
    {
        gSelf->mCellWidth = Math::Max(1, w);
        gSelf->mCellHeight = Math::Max(1, h);
        gSelf->mCells.Clear();
        gSelf->mCellFocus = 0;
        gSelf->mClearColor = gSelf->mLastBGColor;
        for(sint32 i = 0, iend = gSelf->mCellWidth * gSelf->mCellHeight; i < iend; ++i)
        {
            auto& NewCell = gSelf->mCells.AtAdding();
            NewCell.mColor = Color::Black;
            NewCell.mBGColor = gSelf->mClearColor;
            NewCell.mWrittenMsec = 0;
        }
        gSelf->mBoxes.Clear();
        gSelf->mGraphs.Clear();
        gSelf->mScrollLog = 0;
        gSelf->mScrollPhy = 0;
        // 윈도우타이틀
        if(0 < gSelf->mLastApp.Length())
            Platform::SetWindowName(String::Format("%s - HueConsole[%dx%d]",
                (chars) gSelf->mLastApp, gSelf->mCellWidth, gSelf->mCellHeight));
    }
}

void hueconsoleData::SetColor(Color color)
{
    if(gSelf)
        gSelf->mLastColor = color;
}

void hueconsoleData::SetBGColor(Color bgcolor)
{
    if(gSelf)
        gSelf->mLastBGColor = bgcolor;
}

void hueconsoleData::Print(String text)
{
    if(gSelf)
    {
        gSelf->ValidCells(gSelf->mCellFocus);
        const uint64 UpdateMsec = Platform::Utility::CurrentTimeMsec() + 500;
        sint32 LetterIndex = gSelf->mCellFocus;
        for(sint32 i = 0, iend = boss_strlen(text); i < iend;)
        {
            const sint32 LetterCount = String::GetLengthOfFirstLetter(((chars) text) + i);
            const String NewLetter = text.Middle(i, LetterCount);
            i += LetterCount;

            if(!NewLetter.Compare("\n"))
            {
                LetterIndex -= (LetterIndex % gSelf->mCellWidth); // 현재줄의 맨앞으로 이동
                LetterIndex += gSelf->mCellWidth; // 다음줄로 이동
            }
            else
            {
                gSelf->ValidCells(++LetterIndex);
                auto& CurCell = gSelf->mCells.At(LetterIndex - 1);
                CurCell.mLetter = NewLetter;
                CurCell.mColor = gSelf->mLastColor;
                CurCell.mBGColor = gSelf->mLastBGColor;
                CurCell.mWrittenMsec = UpdateMsec;
                if(1 < LetterCount) // 한글처럼 2칸을 쓰는 폰트에 대한 처리
                {
                    gSelf->ValidCells(++LetterIndex);
                    auto& NextCell = gSelf->mCells.At(LetterIndex - 1);
                    NextCell.mLetter.Empty();
                    NextCell.mColor = gSelf->mLastColor;
                    NextCell.mBGColor = gSelf->mLastBGColor;
                    NextCell.mWrittenMsec = UpdateMsec;
                }
            }
        }
        GotoXY(LetterIndex % gSelf->mCellWidth, LetterIndex / gSelf->mCellWidth);
        gSelf->mUpdateMsec = UpdateMsec;
        const sint32 YSize = (gSelf->mCellFocus + gSelf->mCellWidth - 1) / gSelf->mCellWidth;
        gSelf->ValidCells(gSelf->mCellWidth * YSize);
    }
}

void hueconsoleData::Scan(sint32 w, TextCB cb)
{
    if(gSelf)
    {
        sint32 X = gSelf->mCellFocus % gSelf->mCellWidth;
        sint32 Y = gSelf->mCellFocus / gSelf->mCellWidth;
        sint32 Width = w;
        if(gSelf->mCellWidth < X + Width)
        {
            X = 0;
            Y++;
            Width = Math::Min(Width, gSelf->mCellWidth);
        }
        GotoXY(0, Y + 1);

        auto& NewBox = gSelf->mBoxes.AtAdding();
        NewBox.mLeft = X;
        NewBox.mTop = Y;
        NewBox.mRight = X + Width;
        NewBox.mBottom = Y + 1;
        NewBox.mColor = gSelf->mLastColor;
        NewBox.mTextCB = cb;

        const sint32 BoxIdx = gSelf->mBoxes.Count() - 1;
        const String DOMName = String::Format("dom_%d", BoxIdx);
        ZayWidgetDOM::SetComment(DOMName, "");
    }
}

void hueconsoleData::Button(sint32 w, sint32 h, ClickCB cb)
{
    if(gSelf)
    {
        sint32 X = gSelf->mCellFocus % gSelf->mCellWidth;
        sint32 Y = gSelf->mCellFocus / gSelf->mCellWidth;
        sint32 Width = w;
        if(gSelf->mCellWidth < X + Width)
        {
            X = 0;
            Y++;
            Width = Math::Min(Width, gSelf->mCellWidth);
        }

        auto& NewBox = gSelf->mBoxes.AtAdding();
        NewBox.mLeft = X;
        NewBox.mTop = Y;
        NewBox.mRight = X + Width;
        NewBox.mBottom = Y + h;
        NewBox.mColor = gSelf->mLastColor;
        NewBox.mClickCB = cb;
    }
}

void hueconsoleData::GotoXY(sint32 x, sint32 y)
{
    if(gSelf)
    {
        gSelf->mCellFocus = x + y * gSelf->mCellWidth;
        if(!gSelf->mScrollLock)
        {
            if(gSelf->mScrollLog + (gSelf->mCellHeight - 1) * 1000 < y * 1000)
                gSelf->mScrollLog = y * 1000 - (gSelf->mCellHeight - 1) * 1000;
            else if(y * 1000 < gSelf->mScrollLog)
                gSelf->mScrollLog = y * 1000;
        }
    }
}

void hueconsoleData::GraphTo(Graph::Type type, sint32 x, sint32 y)
{
    if(gSelf)
    {
        auto& NewGraph = gSelf->mGraphs.AtAdding();
        NewGraph.mType = type;
        NewGraph.mXBegin = gSelf->mCellFocus % gSelf->mCellWidth;
        NewGraph.mYBegin = gSelf->mCellFocus / gSelf->mCellWidth;
        NewGraph.mXEnd = x;
        NewGraph.mYEnd = y;
        NewGraph.mColor = gSelf->mLastColor;
        GotoXY(x, y);
    }
}

void hueconsoleData::ImageTo(chars name, sint32 x, sint32 y)
{
    if(gSelf)
    {
        auto& NewGraph = gSelf->mGraphs.AtAdding();
        NewGraph.mType = Graph::Type::Image;
        NewGraph.mXBegin = gSelf->mCellFocus % gSelf->mCellWidth;
        NewGraph.mYBegin = gSelf->mCellFocus / gSelf->mCellWidth;
        NewGraph.mXEnd = x;
        NewGraph.mYEnd = y;
        NewGraph.mColor = gSelf->mLastColor;
        NewGraph.mImage = name;
        GotoXY(x, y);
    }
}

void hueconsoleData::Listen(chars key, sint32 recent, BinaryCB cb)
{
    if(gSelf)
    {
        if(0 < recent && cb)
        {
            gSelf->mLoaders(key) = cb;
            gSelf->Send_ListenData(key, recent);
        }
        else
        {
            gSelf->mLoaders.Remove(key);
            gSelf->Send_ListenData(key, 0);
        }
    }
}

void hueconsoleData::Send(chars key, bytes data, sint32 length)
{
    if(gSelf)
    {
        auto NewText = base64_encode(data, length);
        gSelf->Send_SendData(key, NewText.c_str());
    }
}

void hueconsoleData::Repaint()
{
    if(gSelf)
    {
        if(auto OneApp = hueconsoleData::_AllApps().Access(gSelf->mLastApp))
        {
            OneApp->mCB();
            gSelf->invalidate();
        }
    }
}

void hueconsoleData::ValidCells(sint32 count)
{
    for(sint32 i = mCells.Count(); i < count; ++i)
    {
        auto& NewCell = mCells.AtAdding();
        NewCell.mColor = Color::Black;
        NewCell.mBGColor = mClearColor;
        NewCell.mWrittenMsec = 0;
    }
}

void hueconsoleData::SortingApps(bool init)
{
    // 점수화
    sint32s ScoreCollector;
    sint32 LastScore = 0;
    auto& AllApps = hueconsoleData::_AllApps();
    for(sint32 i = 0, iend = AllApps.Count(); i < iend; ++i)
    {
        chararray CurApp;
        if(auto OneApp = AllApps.AccessByOrder(i, &CurApp))
        {
            const sint32 CurScore = OneApp->mStar * iend + (iend - i - 1) + 1;
            ScoreCollector.AtWherever(i) = CurScore;
            LastScore = Math::Max(LastScore, CurScore + 1);
        }
    }

    // 소팅
    for(sint32 i = 0, iend = ScoreCollector.Count(); i < iend; ++i)
    {
        sint32 BestScore = 0;
        sint32 BestJ = 0;
        for(sint32 j = 0, jend = ScoreCollector.Count(); j < jend; ++j)
        {
            if(ScoreCollector[j] < LastScore && BestScore < ScoreCollector[j])
            {
                BestScore = ScoreCollector[j];
                BestJ = j;
            }
        }
        LastScore = BestScore;
        if(auto OneApp = AllApps.AccessByOrder(BestJ))
        {
            OneApp->mOrderLog = i * 1000;
            if(init) OneApp->mOrderPhy = i * 1000;
        }
    }
}

void hueconsoleData::Send_GetToken()
{
    Context NewPacket;
    NewPacket.At("type").Set("GetToken");
    const String NewJson = NewPacket.SaveJson();
    Platform::Socket::Send(mSocket, (bytes)(chars) NewJson, NewJson.Length(), 3000, true);
}

void hueconsoleData::Send_GetVisitor()
{
    Context NewPacket;
    NewPacket.At("type").Set("GetVisitor");
    const String NewJson = NewPacket.SaveJson();
    Platform::Socket::Send(mSocket, (bytes)(chars) NewJson, NewJson.Length(), 3000, true);
}

void hueconsoleData::Send_GetHeart(chars item)
{
    Context NewPacket;
    NewPacket.At("type").Set("GetHeart");
    NewPacket.At("token").Set(mToken);
    NewPacket.At("item").Set(item);
    const String NewJson = NewPacket.SaveJson();
    Platform::Socket::Send(mSocket, (bytes)(chars) NewJson, NewJson.Length(), 3000, true);
}

void hueconsoleData::Send_TurnHeart(chars item)
{
    Context NewPacket;
    NewPacket.At("type").Set("TurnHeart");
    NewPacket.At("token").Set(mToken);
    NewPacket.At("item").Set(item);
    const String NewJson = NewPacket.SaveJson();
    Platform::Socket::Send(mSocket, (bytes)(chars) NewJson, NewJson.Length(), 3000, true);
}

void hueconsoleData::Send_SendData(chars key, chars base64)
{
    Context NewPacket;
    NewPacket.At("type").Set("SendData");
    NewPacket.At("token").Set(mToken);
    NewPacket.At("key").Set(key);
    NewPacket.At("base64").Set(base64);
    const String NewJson = NewPacket.SaveJson();
    Platform::Socket::Send(mSocket, (bytes)(chars) NewJson, NewJson.Length(), 3000, true);
}

void hueconsoleData::Send_ListenData(chars key, sint32 recent)
{
    Context NewPacket;
    NewPacket.At("type").Set("ListenData");
    NewPacket.At("token").Set(mToken);
    NewPacket.At("key").Set(key);
    NewPacket.At("recent").Set(String::FromInteger(recent));
    const String NewJson = NewPacket.SaveJson();
    Platform::Socket::Send(mSocket, (bytes)(chars) NewJson, NewJson.Length(), 3000, true);
}

bool hueconsoleData::RecvOnce()
{
    bool NeedUpdate = false;
    while(0 < Platform::Socket::RecvAvailable(mSocket))
    {
        uint08 RecvTemp[4096];
        sint32 RecvSize = Platform::Socket::Recv(mSocket, RecvTemp, 4096);
        if(0 < RecvSize)
            Memory::Copy(mRecvQueue.AtDumpingAdded(RecvSize), RecvTemp, RecvSize);
        else if(RecvSize < 0)
            return NeedUpdate;

        sint32 QueueEndPos = 0;
        for(sint32 iend = mRecvQueue.Count(), i = iend - RecvSize; i < iend; ++i)
        {
            if(mRecvQueue[i] == '\0')
            {
                // 스트링 읽기
                const String Message((chars) &mRecvQueue[QueueEndPos], i - QueueEndPos);
                QueueEndPos = i + 1;
                OnRecvMessage(Message);
                NeedUpdate = true;
            }
        }
        if(0 < QueueEndPos)
            mRecvQueue.SubtractionSection(0, QueueEndPos);
    }
    return NeedUpdate;
}

void hueconsoleData::OnRecvMessage(chars message)
{
    const Context RecvJson(ST_Json, SO_OnlyReference, message);
    const String Type = RecvJson("type").GetText();

    branch;
    jump(!Type.Compare("Token")) OnRecv_Token(RecvJson);
    jump(!Type.Compare("Visitor")) OnRecv_Visitor(RecvJson);
    jump(!Type.Compare("Heart")) OnRecv_Heart(RecvJson);
    jump(!Type.Compare("DataUpdated")) OnRecv_DataUpdated(RecvJson);
}

void hueconsoleData::OnRecv_Token(const Context& json)
{
    mToken = json("token").GetText();
    auto& AllApps = hueconsoleData::_AllApps();
    for(sint32 i = 0, iend = AllApps.Count(); i < iend; ++i)
    {
        chararray CurApp;
        if(auto OneApp = AllApps.AccessByOrder(i, &CurApp))
            Send_GetHeart(&CurApp[0]);
    }
}

void hueconsoleData::OnRecv_Visitor(const Context& json)
{
    mInfo_Total = json("total").GetInt();
    mInfo_Member = json("member").GetInt();
    mInfo_RealTime = json("realtime").GetInt();
}

void hueconsoleData::OnRecv_Heart(const Context& json)
{
    auto& AllApps = hueconsoleData::_AllApps();
    const String OneItem = json("item").GetText();
    if(auto OneApp = AllApps.Access(OneItem))
    {
        OneApp->mStar = json("score").GetInt();
        OneApp->mStarVoted = json("yours").GetInt();
        SortingApps(false);
    }
}

void hueconsoleData::OnRecv_DataUpdated(const Context& json)
{
    const String OneKey = json("key").GetText();
    if(auto OneBinaryCB = mLoaders.Access(OneKey))
    {
        const String OneBase64 = json("base64").GetText();
        auto NewBinary = base64_decode((chars) OneBase64);
        (*OneBinaryCB)((const void*) NewBinary.data(), NewBinary.length());
    }
}
