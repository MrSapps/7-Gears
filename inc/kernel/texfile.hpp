#pragma once

class TexFile
{
public:
    typedef unsigned int entry_t;

    struct BitData
    {
        entry_t color_min;
        entry_t color_max;
        entry_t alpha_min;
        entry_t alpha_max;
        entry_t pixel_min;
        entry_t pixel_max;
    };

    struct ImageData
    {
        entry_t bit_depth;
        entry_t width;
        entry_t height;
        entry_t pitch;
    };

    struct PaletteData
    {
        entry_t flag;
        entry_t index_bits;
        entry_t index_8bit;
        entry_t total_color_count;
        entry_t colors_per_palette;
    };

    struct RGBAData
    {
        entry_t red;
        entry_t green;
        entry_t blue;
        entry_t alpha;
    };

    struct PixelFormat
    {
        entry_t bits_per_pixel;
        entry_t bytes_per_pixel;
        RGBAData bit_count;
        RGBAData bit_mask;
        RGBAData bit_shift;
        RGBAData bit_count_unused;
        RGBAData shades;
    };

    struct Header
    {
        entry_t version;
        entry_t unknown_0x04;
        entry_t color_key_flag;
        entry_t unknown_0x0C;
        entry_t unknown_0x10;
        BitData bit_data;
        entry_t palette_type;
        entry_t palette_count;
        entry_t palette_total_color_count;
        ImageData image_data;
        entry_t unknown_0x48;
        PaletteData palette_data;
        entry_t runtime_data_ptr_palette_data;
        PixelFormat pixel_format;
        entry_t color_key_array_flag;
        entry_t runtime_data_ptr_color_key_array;
        entry_t reference_alpha;
        entry_t runtime_data_02;
        entry_t unknown_0xCC;
        entry_t runtime_data_palette_index;
        entry_t runtime_data_ptr_image_data;
        entry_t runtime_data_04;
        entry_t unknown_06;
        entry_t unknown_07;
        entry_t unknown_08;
        entry_t unknown_09;
    } m_header;

    struct rev_TextureFormat
    {
        entry_t width;
        entry_t height;
        entry_t pitch;
        entry_t unknown_0x48;
        entry_t palette_flag;
        entry_t bits_per_palette_index;
        entry_t palette_index_8bit;
        entry_t palette_size;
        entry_t palette_color_count;
        entry_t runtime_data_ptr_palette_data;
        PixelFormat pixel_format;
    };

};