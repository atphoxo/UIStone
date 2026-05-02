#pragma once

// Build a 3-state image (normal / hover / pressed) from a single image
class Sequence3StateBuilder
{
public:
    // Returned image is PremultipliedAlpha
    static phoxo::Image LoadSvg(PCWSTR filepath, REFWICPixelFormatGUID output_format = WICPremultiplied32bpp)
    {
        using namespace phoxo;

        FCFileMapping   mem(filepath);
        if (!mem) return {};

        FCResource   loader(mem.m_data, (UINT)mem.m_size.QuadPart);
        Image   img = Build(ImageHandler::Make(loader.LoadSvgWithDpi(), WICNormal32bpp));
        if (output_format == WICPremultiplied32bpp)
        {
            ImageFastPixel::Premultiply(img);
        }
        return img;
    }

private:
    static void CoverState(auto& output, const auto& normal, auto& effect, int index)
    {
        auto   tmp = normal;
        tmp.ApplyEffect(effect);
        phoxo::ImageHandler::Cover(output, tmp, CPoint(tmp.Width() * index, 0));
    }

    static phoxo::Image Build(const phoxo::Image& normal)
    {
        using namespace phoxo;
        using namespace phoxo::effect;

        if (!normal)
            return {};

        Image   img;
        img.Create(normal.Width() * 3, normal.Height());
        ImageHandler::Cover(img, normal, { 0,0 }); // normal
        BrightnessContrast   hover(25, 3), pressed(-20, -3);
        CoverState(img, normal, hover, 1);
        CoverState(img, normal, pressed, 2);
        return img;
    }
};
