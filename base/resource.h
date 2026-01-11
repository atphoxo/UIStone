#pragma once

class FCResource
{
private:
    const void*   m_ptr;
    UINT   m_size;
    mutable IStreamPtr   m_stream;

public:
    /// Typical usage: mod = (HMODULE)&__ImageBase
    FCResource(UINT nID, PCWSTR resource_type, HMODULE mod = NULL)
    {
        auto   hres = FindResource(mod, MAKEINTRESOURCE(nID), resource_type);
        m_ptr = LockResource(::LoadResource(mod, hres));
        m_size = SizeofResource(mod, hres);
        assert(m_ptr);
    }

    operator IStream*() const
    {
        if (!m_stream)
        {
            m_stream = phoxo::Utils::CreateMemStream(m_ptr, m_size);
        }
        return m_stream;
    }

    /// Render the SVG resource into a premultiplied-alpha WIC bitmap
    IWICBitmapPtr LoadSVG(CSize output_size, float scale) const
    {
        IWICBitmapPtr   bmp = WIC::CreateBitmap(output_size, WICPremultiplied32bpp);
        D2D::SVGDrawer(m_ptr, m_size).DrawSvg(bmp, scale, D2D::g_factory);
        return bmp;
    }

    IWICBitmapPtr LoadSvgWithDpi() const
    {
        CSize   original = SVGSizeFastHack();
        float   scale = DPICalculator::GetDPIScaleFactor();
        CSize   target_size(std::lround(original.cx * scale), std::lround(original.cy * scale));
        return LoadSVG(target_size, scale);
    }

private:
    static int ParseIntAttr(const CStringA& xml, PCSTR search)
    {
        if (PCSTR ptr = StrStrIA(xml, search))
        {
            auto   begin = ptr + strlen(search);
            char*   end{};
            return strtol(begin, &end, 10);
        }
        return 0;
    }

    // width/height attributes must exist and are integer values
    SIZE SVGSizeFastHack() const
    {
        // Create a null-terminated CStringA from the resource memory
        CStringA   xml((const char*)m_ptr, m_size);
        int   x = ParseIntAttr(xml, R"( width=")"); assert(x);
        int   y = ParseIntAttr(xml, R"( height=")"); assert(y);
        return { x,y };
    }
};
