#pragma once

class FCHelper
{
public:
    template <typename T>
    static bool IsValidIndex(const T& vec, int index)
    {
        return index >= 0 && index < (int)vec.size();
    }

    template<typename T>
    static T& LazyCreateSingleton(std::unique_ptr<T>& obj)
    {
        if (!obj)
        {
            obj = std::make_unique<T>();
        }
        return *obj;
    }

    static bool IsExplorerInLightTheme()
    {
        DWORD   var = 1, len = sizeof(DWORD); // default: light mode
        RegGetValue(HKEY_CURRENT_USER, LR"(Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)", L"AppsUseLightTheme", RRF_RT_REG_DWORD, NULL, &var, &len);
        return var != 0;
    }

    static CString GetLastWin32ErrorMessage()
    {
        PWSTR   buffer = nullptr;
        ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
            , NULL
            , ::GetLastError()
            , 0
            , (PWSTR)&buffer
            , 0
            , NULL);

        CString   msg(buffer);
        msg.Trim();
        LocalFree(buffer);
        return msg;
    }
};
