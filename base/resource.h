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

    phoxo::Image LoadSvgWithDpi(std::optional<phoxo::Color> fill_color = std::nullopt) const
    {
        using namespace phoxo;

        CSize   original = SVGSizeFastHack();
        float   scale = DPICalculator::GetDPIScaleFactor();
        CSize   target_size(std::lround(original.cx * scale), std::lround(original.cy * scale));
        auto   wicbmp = LoadSVG(target_size, scale);
        Image   bmp = ImageHandler::Make(wicbmp, WICPremultiplied32bpp);
        if (fill_color)
        {
            ImageFastPixel::FillRGBAndPremultiply(bmp, *fill_color);
        }
        return bmp;
    }

private:
    static int ParseIntAttr(const CStringA& xml, PCSTR search)
    {
        if (PCSTR ptr = StrStrIA(xml, search))
        {
            auto   begin = ptr + strlen(search);
            char*   end{};
            if (int v = strtol(begin, &end, 10))
                return v;
        }
        assert(false && "SVG resource is invalid");
        return 0;
    }

    SIZE SVGSizeFastHack() const
    {
        // Create a null-terminated CStringA from the resource memory
        CStringA   xml((const char*)m_ptr, m_size);
        int   x = ParseIntAttr(xml, R"(width=")");
        int   y = ParseIntAttr(xml, R"(height=")");
        return { x,y };
    }
};
