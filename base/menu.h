#pragma once

#ifdef _AFX
class FCMenu : public CMenu
{
public:
    void Translate()
    {
        std::deque<CMenu*>   vec{ this };
        while (!vec.empty())
        {
            CMenu*   pMenu = vec.back();
            vec.pop_back();

            for (int i = 0; i < pMenu->GetMenuItemCount(); i++)
            {
                CString   txt;
                pMenu->GetMenuString(i, txt, MF_BYPOSITION);
                if (txt.IsEmpty()) // separator
                    continue;

                UINT   item_id = pMenu->GetMenuItemID(i);
                auto   sub_menu = pMenu->GetSubMenu(i);
                if (sub_menu)
                    vec.push_back(sub_menu);

                if (OnTranslateMenuItem(sub_menu, item_id, txt))
                {
                    pMenu->ModifyMenu(i, MF_STRING | MF_BYPOSITION, item_id, txt);
                }
            }
        }
    }

private:
    // Returns true if txt contains the translated text, false if no translation
    virtual bool OnTranslateMenuItem(CMenu* sub_menu, UINT item_id, CString& txt) = 0;
};
#endif
