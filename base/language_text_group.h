#pragma once

class LanguageTextGroup
{
private:
    std::deque<CString>   m_text_list;

public:
    explicit LanguageTextGroup(const CString& str) : m_text_list{ FCString::SplitTextByToken(str, L"|") }
    {
    }

    LanguageTextGroup(PCWSTR section, PCWSTR key) : LanguageTextGroup{ LanguageText::Get(section, key) }
    {
    }

    LanguageTextGroup(PCWSTR section, int key) : LanguageTextGroup{ LanguageText::Get(section, key) }
    {
    }

    size_t size() const
    {
        return m_text_list.size();
    }

    CString PopFront()
    {
        if (m_text_list.empty())
        {
            assert(false);
            return L"";
        }

        CString   txt = m_text_list.front();
        m_text_list.pop_front();
        return txt;
    }

    CString operator[](int index) const
    {
        try
        {
            return m_text_list.at(index);
        }
        catch (const std::out_of_range&) { assert(false); }
        return L"";
    }
};
