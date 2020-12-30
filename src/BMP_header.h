/* Copyright 2020, Sage Farrenholz */

#pragma pack(push,2)
struct BMP_Header {

    //File Header Structure
    char id[2] = {'B', 'M'};
    uint32_t size;
    uint32_t reserved = 0;
    uint32_t offset;

    //DIB Header Structure
    uint32_t header_size = 40;
    int32_t width;
    int32_t height;
    uint16_t color_planes = 1;
    uint16_t bit_depth;
    uint32_t compress = 0;
    uint32_t image_size = 0;
    int32_t x_res = 2835; //72dpi ~= 2835 ppm
    int32_t y_res = 2835; //72dpi ~= 2835 ppm
    uint32_t color_count = 0;
    uint32_t important_colors = 0;

};
#pragma pack(pop)
