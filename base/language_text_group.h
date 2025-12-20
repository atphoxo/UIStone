#pragma once

class LanguageTextGroup
{
private:
    std::deque<CString>   m_text_list;

public:
    LanguageTextGroup(PCWSTR section, PCWSTR key)
    {
        CString   txt = LanguageText::Get(section, key);
        m_text_list = FCString::SplitTextByToken(txt, L"|");
    }

    LanguageTextGroup(PCWSTR section, int key) : LanguageTextGroup(section, FCString::From(key))
    {
    }

    size_t size() const
    {
        return m_text_list.size();
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
