#pragma once

class DllLoader
{
private:
    HMODULE   m_dll;

public:
    DllLoader(PCWSTR dll_path)
    {
        m_dll = ::LoadLibrary(dll_path);
    }

    ~DllLoader()
    {
        if (m_dll) { ::FreeLibrary(m_dll); }
    }

    template<typename T>
    T get(PCSTR name) const
    {
        return m_dll ? (T)::GetProcAddress(m_dll, name) : nullptr;
    }
};
