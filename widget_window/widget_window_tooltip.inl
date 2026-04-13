
inline void CWidgetWindow::ClearToolTips()
{
    constexpr UINT   FirstTipId = 100;

    if (m_tip_ctrl)
    {
        for (UINT i = FirstTipId; i < m_next_tool_id; i++)
        {
            m_tip_ctrl.DelTool(this, i);
        }
    }
    m_next_tool_id = FirstTipId;
}

inline void CWidgetWindow::EnsureToolTip()
{
    if (!m_tip_ctrl)
    {
        m_tip_ctrl.Create(this, TTS_ALWAYSTIP);
        m_tip_ctrl.Activate(TRUE);
    }
}

inline void CWidgetWindow::RegisterToolTip()
{
    if (!m_hWnd) { ASSERT(false); return; }

    EnsureToolTip();
    ClearToolTips();

    const CRect   wnd_rect = FCWnd::GetClientRect(*this);
    const CSize   sbpos = m_scrollbar.GetPos();

    for (auto& item : m_child_widget)
    {
        if (item->IsVisible() && !item->m_tip.IsEmpty())
        {
            CRect   rect_on_wnd = item->GetRectOnCanvas() - sbpos;
            if (CRect rc; rc.IntersectRect(rect_on_wnd, wnd_rect))
            {
                item->OnRegisterTip(m_tip_ctrl, this, m_next_tool_id++, rc);
            }
        }
    }
}
