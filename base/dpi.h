#pragma once

class DPICalculator
{
private:
    static inline int   g_current_dpi = 0;

public:
    static void Update(HWND wnd)
    {
        g_current_dpi = ::GetDpiForWindow(wnd);
        assert(g_current_dpi);
    }

    static int Current()
    {
        if (g_current_dpi)
            return g_current_dpi;

        assert(false);
        return ::GetDpiForSystem();
    }

    static float GetDPIScaleFactor()
    {
        return Current() / (float)USER_DEFAULT_SCREEN_DPI;
    }

    static int Cast(int v, int v_designed_for_dpi = USER_DEFAULT_SCREEN_DPI)
    {
        if (!v_designed_for_dpi)
        {
            assert(false);
            return v;
        }

        return v * Current() / v_designed_for_dpi;
    }

    static CSize Cast(SIZE sz)
    {
        return CSize{ Cast(sz.cx), Cast(sz.cy) };
    }
};
