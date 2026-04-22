#pragma once

struct FCFileMapping
{
    HANDLE    m_file{};
    HANDLE    m_mapping{};
    LPCBYTE   m_data{};
    LARGE_INTEGER   m_size{};

    FCFileMapping(PCWSTR path)
    {
        m_file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        if (m_file == INVALID_HANDLE_VALUE)
            return;

        m_mapping = CreateFileMapping(m_file, NULL, PAGE_READONLY, 0, 0, NULL);
        if (m_mapping)
        {
            m_data = (LPCBYTE)MapViewOfFile(m_mapping, FILE_MAP_READ, 0, 0, 0);
            GetFileSizeEx(m_file, &m_size);
        }
    }

    ~FCFileMapping()
    {
        if (m_data) { UnmapViewOfFile(m_data); }
        if (m_mapping) { CloseHandle(m_mapping); }
        if (m_file != INVALID_HANDLE_VALUE) { CloseHandle(m_file); }
    }
};
