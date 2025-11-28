#pragma once
#include <chrono>

class PerformanceTest
{
private:
    using stlclock = std::chrono::steady_clock;

    stlclock::time_point   m_start;
    std::optional<int>   m_count;

public:
    PerformanceTest()
    {
        Restart();
    }

    void EnableAverageMode()
    {
        m_count = 0;
    }

    void Restart()
    {
        m_start = stlclock::now();
    }

    int GetElapseTime() const
    {
        auto   elapsed = stlclock::now() - m_start;
        return (int)std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    }

    void DebugOut(PCWSTR prefix_txt = L"oxo -- ")
    {
        int   curr = GetElapseTime();
        if (m_count)
        {
            (*m_count)++;
            curr = curr / *m_count;
        }

        CString   s;
        s.Format(L"%s%d\n", prefix_txt, curr);
        ::OutputDebugString(s);
    }
};
