// Copyright (c) [2025] [Initial_Equationor]
// [AreaSegmentation] is licensed under Mulan PubL v2.
// You can use this software according to the terms and conditions of the Mulan PubL v2.
// You may obtain a copy of Mulan PubL v2 at:
// http://license.coscl.org.cn/MulanPubL-2.0
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
// EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
// MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See the Mulan PubL v2 for more details.
#pragma once
#include <SDL.h>

bool LoadTextureFromMemory(const void* data, size_t data_size, SDL_Surface *&surface);
bool LoadTextureFromFile(const char* file_name, SDL_Surface *&surface, Sint32& width, Sint32& height);