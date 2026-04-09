#pragma once
#include "widget_item.h"

using CWidgetItemPtr = std::unique_ptr<CWidgetItem>;

class CWidgetLayout
{
private:
    bool   m_modifying_scrollbar_visible = false;

public:
    virtual ~CWidgetLayout() = default;
    virtual void LayoutWidget(CWnd& wnd, std::deque<CWidgetItemPtr>& child_items) = 0;

    /// Set window's vertical scrollbar range.
    void SetVScrollRange(CWnd& wnd, int canvas_height)
    {
        CRect   rc = FCWnd::GetWindowRect(wnd);
        SetScrollbarRange(wnd, canvas_height, rc.Height(), SB_VERT);
    }

    void SetHScrollRange(CWnd& wnd, int canvas_width)
    {
        CRect   rc = FCWnd::GetWindowRect(wnd);
        SetScrollbarRange(wnd, canvas_width, rc.Width(), SB_HORZ);
    }

private:
    void SetScrollbarRange(CWnd& wnd, int canvas_size, int wnd_size, int bar)
    {
        // 显示/隐藏滚动条ShowScrollBar里面会send WM_SIZE再一次layout（其实不加保护也没问题，多一次设置info，两次参数一样）
        if (m_modifying_scrollbar_visible)
            return;

        m_modifying_scrollbar_visible = true;
        bool   visible = IsScrollbarVisible(wnd, bar);
        if (canvas_size > wnd_size)
        {
            if (!visible)
            {
                wnd.SetScrollPos(bar, 0, FALSE);
                wnd.ShowScrollBar(bar, TRUE);
            }
            CallSetScrollInfo(wnd, canvas_size, wnd_size, bar);
        }
        else
        {
            if (visible)
            {
                wnd.SetScrollPos(bar, 0, FALSE);
                wnd.ShowScrollBar(bar, FALSE);
            }
        }
        m_modifying_scrollbar_visible = false;
    }

    static void CallSetScrollInfo(CWnd& wnd, int canvas_size, int wnd_size, int bar)
    {
        SCROLLINFO   si = { .fMask = SIF_RANGE | SIF_PAGE };
        si.nMax = canvas_size - 1; // note -1
        si.nPage = wnd_size;
        wnd.SetScrollInfo(bar, &si, TRUE);
    }

    static bool IsScrollbarVisible(const CWnd& wnd, int bar)
    {
        DWORD   t = wnd.GetStyle(); // 目前的窗口都只显示一个H or V滚动条
        if (bar == SB_VERT)
            return (t & WS_VSCROLL) != 0;
        else
            return (t & WS_HSCROLL) != 0;
    }
};
//-------------------------------------------------------------------------------------
class CVerticalWidgetLayout : public CWidgetLayout
{
private:
    int   m_item_height;

public:
    CVerticalWidgetLayout(int item_height) : m_item_height(item_height) {}

private:
    void LayoutWidget(CWnd& wnd, std::deque<CWidgetItemPtr>& child_items) override
    {
        CRect   rc = FCWnd::GetClientRect(wnd);
        rc.bottom = rc.top + m_item_height;
        for (auto& iter : child_items)
        {
            iter->SetRectOnCanvas(rc);
            rc.OffsetRect(0, m_item_height);
        }
    }
};
//-------------------------------------------------------------------------------------
class CSingleWidgetLayout : public CWidgetLayout
{
    void LayoutWidget(CWnd& wnd, std::deque<CWidgetItemPtr>& child_items) override
    {
        for (auto& iter : child_items)
        {
            iter->SetRectOnCanvas(FCWnd::GetClientRect(wnd));
        }
    }
};
