#pragma once

class LanguageTextSplitter
{
private:
    const CString   m_text;
    int   m_pos = 0;

public:
    explicit LanguageTextSplitter(const CString& text) : m_text(text)
    {
        assert(!text.IsEmpty());
    }

    LanguageTextSplitter(PCWSTR section, PCWSTR key) : LanguageTextSplitter(LanguageText::Get(section, key))
    {
    }

    LanguageTextSplitter(PCWSTR section, int key) : LanguageTextSplitter(LanguageText::Get(section, key))
    {
    }

    // Return the next token, empty string indicates end
    CString Next(PCWSTR tokens = L"|")
    {
        if (m_pos == -1)
            return L"";
        return m_text.Tokenize(tokens, m_pos);
    }
};
