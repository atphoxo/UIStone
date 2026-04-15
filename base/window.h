#pragma once

/// Window helper.
class FCWnd
{
public:
    static CRect GetWindowRect(HWND wnd)
    {
        CRect   t;
        ::GetWindowRect(wnd, t);
        return t;
    }

    static CRect GetClientRect(HWND wnd)
    {
        CRect   t;
        ::GetClientRect(wnd, t);
        return t;
    }

    static CSize GetClientSize(HWND wnd)
    {
        CRect   t;
        ::GetClientRect(wnd, t);
        return t.Size();
    }

    static int GetScrollbarWidth()
    {
        return GetSystemMetricsForDpi(SM_CXVSCROLL, DPICalculator::Current());
    }

#ifdef _AFX
    static CRect GetChildRectOnParent(const CWnd& parent, int child_ctrl_id)
    {
        CRect   rc;
        ::GetWindowRect(::GetDlgItem(parent, child_ctrl_id), rc);
        parent.ScreenToClient(rc);
        return rc;
    }

    static void DDX_Text_NoTip(CDataExchange* pDX, int nIDC, int& t, int tDefault)
    {
        if (pDX->m_bSaveAndValidate)
        {
            CString   s;
            pDX->m_pDlgWnd->GetDlgItemText(nIDC, s);
            t = (s.GetLength() ? StrToInt(s) : tDefault);
        }
        else
        {
            DDX_Text(pDX, nIDC, t);
        }
    }

    static void AddComboString(CWnd* ctrl, const CString& text)
    {
        if (!ctrl)
            return;

        LanguageTextSplitter   parts(text);
        for (CString t = parts.Next(); !t.IsEmpty(); t = parts.Next())
        {
            ((CComboBox*)ctrl)->AddString(t);
        }
    }
#endif

    static void MoveRectInside(CRect& rc, CSize limit)
    {
        int   dx = OffsetInRange(rc.left, rc.right, limit.cx);
        int   dy = OffsetInRange(rc.top, rc.bottom, limit.cy);
        rc.OffsetRect(dx, dy);
    }

    static int WheelDeltaToStep(short zDelta)
    {
        if (int v = zDelta / WHEEL_DELTA)
            return v;
        return (zDelta >= 0) ? 1 : -1;
    }

private:
    static int OffsetInRange(int low, int high, int limit)
    {
        if (low < 0)
            return -low;
        else if (high > limit)
            return limit - high;
        return 0;
    }
};
