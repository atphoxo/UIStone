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

    LanguageTextSplitter(PCWSTR section, PCWSTR key, LanguageText::TextStyle style = LanguageText::TextStyle::None)
        : LanguageTextSplitter(LanguageText::Get(section, key, style))
    {
    }

    LanguageTextSplitter(PCWSTR section, int key, LanguageText::TextStyle style = LanguageText::TextStyle::None)
        : LanguageTextSplitter(LanguageText::Get(section, key, style))
    {
    }

    // Return the next token, empty string indicates end
    CString Next(PCWSTR tokens = L"|")
    {
        if (m_pos == -1)
            return L"";
        return m_text.Tokenize(tokens, m_pos);
    }

    // Fill span, return true if all elements are not empty
    bool Fill(std::span<CString> out, PCWSTR tokens = L"|")
    {
        return std::ranges::all_of(out, [this, tokens](CString& it) {
            it = Next(tokens);
            return !it.IsEmpty();
        });
    }
};
