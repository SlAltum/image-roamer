// Copyright (c) [2025] [Initial_Equationor]
// [AreaSegmentation] is licensed under Mulan PubL v2.
// You can use this software according to the terms and conditions of the Mulan PubL v2.
// You may obtain a copy of Mulan PubL v2 at:
// http://license.coscl.org.cn/MulanPubL-2.0
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
// EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
// MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See the Mulan PubL v2 for more details.
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"
#ifndef STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#endif
#include "utils.hpp"

bool LoadTextureFromMemory(const void* data, size_t data_size, SDL_Surface *&surface) {
    Sint32 width = 0;
    Sint32 height = 0;
    Sint32 bytes_per_pixel =
            0; // Since we work with bytes instead of bits here in C.

    // Now the good stuff:
    Uint8 *_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &width, &height, &bytes_per_pixel, 0);

    Sint32 dot_pitch = width * bytes_per_pixel;
    dot_pitch = (dot_pitch + 3) & ~3;

    Sint32 red_mask;
    Sint32 green_mask;
    Sint32 blue_mask;
    Sint32 alpha_mask;

    // This only works with little endian computer processors,
    // if you want support for big endian (if you even still use it), implement it
    // yourself.
    red_mask = 0x000000FF;
    green_mask = 0x0000FF00;
    blue_mask = 0x00FF0000;
    alpha_mask = (bytes_per_pixel == 4) ? 0xFF000000 : 0;

    // Now finally we can get that damn surface.
    surface = SDL_CreateRGBSurfaceFrom(
            _data, width, height, bytes_per_pixel * 8, dot_pitch, red_mask, green_mask,
            blue_mask, alpha_mask);
    if (!surface) {
        SDL_free(_data);
        return false;
    }

    return true;
} // bool LoadTextureFromMemory

bool LoadTextureFromFile(const char* file_name, SDL_Surface *&surface, Sint32& width, Sint32& height) {
    Sint32 bytes_per_pixel =
            0; // Since we work with bytes instead of bits here in C.

    // Now the good stuff:
    Uint8 *data = stbi_load(file_name, &width, &height, &bytes_per_pixel, 0);
    Sint32 dot_pitch = width * bytes_per_pixel;
    dot_pitch = (dot_pitch + 3) & ~3;

    Sint32 red_mask;
    Sint32 green_mask;
    Sint32 blue_mask;
    Sint32 alpha_mask;

    red_mask = 0x000000FF;
    green_mask = 0x0000FF00;
    blue_mask = 0x00FF0000;
    alpha_mask = (bytes_per_pixel == 4) ? 0xFF000000 : 0;

    surface = SDL_CreateRGBSurfaceFrom(
            data, width, height, bytes_per_pixel * 8, dot_pitch, red_mask, green_mask,
            blue_mask, alpha_mask);
    if (!surface) {
        SDL_free(data);
        return false;
    }

    return true;
} // bool LoadTextureFromFile