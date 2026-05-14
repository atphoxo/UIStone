#pragma once

class FCString
{
public:
    static std::deque<CString> SplitTextByToken(const CString& txt, PCWSTR tokens)
    {
        std::deque<CString>   vec;
        for (int curr = 0;;)
        {
            CString   tmp = txt.Tokenize(tokens, curr);
            if (curr == -1)
                break;
            vec.push_back(tmp);
        }
        return vec;
    }

    template<class Container>
    static int FindNoCase(const Container& vec, const CString& target)
    {
        for (int idx = 0; const auto& it : vec)
        {
            if (_wcsicmp(it, target) == 0)
                return idx;
            idx++;
        }
        return -1;
    }

    static CString Make(const std::wstring_view& sv)
    {
        return CString(sv.data(), (int)sv.size());
    }

    static CString From(int n)
    {
        CString   t;
        t.Format(L"%d", n);
        return t;
    }

    static CString FormatByteSize(LONGLONG bytes)
    {
        WCHAR   buf[32] = {};
        ::StrFormatByteSize(bytes, buf, 32);
        return buf;
    }
};
