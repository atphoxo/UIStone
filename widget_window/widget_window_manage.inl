
inline void CWidgetWindow::AddWidget(CWidgetItem* item_src, int add_index)
{
    if (!item_src) return;

    std::unique_ptr<CWidgetItem>   item(item_src);
    if (add_index == -1)
    {
        m_child_widget.push_back(std::move(item));
    }
    else
    {
        if ((add_index >= 0) && (add_index <= (int)m_child_widget.size()))
        {
            m_child_widget.insert(m_child_widget.begin() + add_index, std::move(item));
        }
        else { ASSERT(false); }
    }
}

inline int CWidgetWindow::FindWidgetIndex(int id) const
{
    for (size_t i = 0; i < m_child_widget.size(); i++)
    {
        if (m_child_widget[i]->GetID() == id)
            return (int)i;
    }
    return -1;
}

inline CWidgetItem* CWidgetWindow::FindWidgetByID(int id) const
{
    int   pos = FindWidgetIndex(id);
    return (pos != -1) ? m_child_widget[pos].get() : nullptr;
}

inline CWidgetItem* CWidgetWindow::GetWidgetByIndex(int index) const
{
    bool   ok = FCHelper::IsValidIndex(m_child_widget, index); ASSERT(ok);
    return ok ? m_child_widget[index].get() : nullptr;
}

inline void CWidgetWindow::DeleteAllWidget()
{
    m_child_widget.clear();
    ClearToolTips();
    m_highlight = nullptr;
}

inline void CWidgetWindow::DeleteWidgetByID(int id)
{
    DeleteWidgetByIndex(FindWidgetIndex(id));
}

inline void CWidgetWindow::DeleteWidgetByIndex(int index)
{
    if (FCHelper::IsValidIndex(m_child_widget, index))
    {
        m_child_widget.erase(m_child_widget.begin() + index);
    }
}

inline CWidgetItem* CWidgetWindow::ReleaseWidgetOwnership(int id)
{
    if (int pos = FindWidgetIndex(id); pos != -1)
    {
        auto   t = m_child_widget[pos].release();
        DeleteWidgetByIndex(pos);
        return t;
    }
    return nullptr;
}

inline CWidgetItem* CWidgetWindow::ClickHitTest(CPoint pt_on_window, bool include_hidden) const
{
    CPoint   pt = pt_on_window + m_scrollbar.GetPos(); // -> canvas coordinate
    for (const auto& iter : m_child_widget)
    {
        if (iter->GetRectOnCanvas().PtInRect(pt) && iter->IsEnable())
        {
            if (include_hidden || iter->IsVisible())
                return iter.get();
        }
    }
    return nullptr;
}
