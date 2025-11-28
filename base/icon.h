#pragma once

class FCIcon
{
public:
    HICON   p{};

public:
    FCIcon() = default;

    FCIcon(HICON ico) : p(ico)
    {
    }

    FCIcon(int res_id, int output_size)
    {
        ::LoadIconWithScaleDown((HINSTANCE)&__ImageBase, MAKEINTRESOURCE(res_id), output_size, output_size, &p);
    }

    ~FCIcon()
    {
        if (p) { ::DestroyIcon(p); }
    }

    operator HICON() const
    {
        return p;
    }

    IWICBitmapPtr CreateBitmap() const
    {
        IWICBitmapPtr   t;
        if (p) { WIC::g_factory->CreateBitmapFromHICON(p, &t); }
        return t;
    }

#ifdef _AFX
    static HICON CreateIcon(const phoxo::Image& img)
    {
        if (!img)
            return NULL;
        assert(!img.IsPremultiplied());
        CImageList   il;
        il.Create(img.Width(), img.Height(), ILC_COLOR32, 1, 1);
        il.Add(CBitmap::FromHandle(img), RGB(0, 0, 0));
        return il.ExtractIcon(0);
    }
#endif
};
