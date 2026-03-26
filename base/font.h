#pragma once

/// Font helper.
#ifdef _AFX
class FCFont : public CFont
{
public:
    /// Get default UI font (menu font). The returned font must not be destroyed.
    static FCFont& GetDefaultFont()
    {
        static FCFont   v;
        if (!v.m_hObject)
        {
            v.CreatePointFontWithDPI(9);
        }
        return v;
    }

    static CSize MeasureStringSize(PCWSTR str, HFONT font)
    {
        CDC   dc;
        dc.CreateCompatibleDC(NULL);
        auto   old_font = dc.SelectObject(font);
        CSize   sz = dc.GetTextExtent(str);
        dc.SelectObject(old_font);
        return sz;
    }

    static PCWSTR DefaultUIFacename()
    {
        return L"Segoe UI";
    }

    BOOL CreatePointFontWithDPI(int pointsize, PCWSTR facename = DefaultUIFacename(), int dpi = DPICalculator::Current())
    {
        DeleteObject();

        LOGFONT   lf = { .lfCharSet = DEFAULT_CHARSET };
        Checked::tcsncpy_s(lf.lfFaceName, _countof(lf.lfFaceName), facename, _TRUNCATE);
        lf.lfHeight = -MulDiv(pointsize, dpi, 72);
        return CreateFontIndirect(&lf);
    }

    static void RebuildDefaultFont()
    {
        GetDefaultFont().DeleteObject();
        GetDefaultFont(); // force rebuld font with new dpi
    }
};
#endif
