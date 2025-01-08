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

#include <vector>
#include <SDL.h>

namespace ImageRoamer {
typedef struct Vector2d {
    float x;
    float y;
} Vector2d;
typedef struct Coordinate {
    Vector2d wh;
    Vector2d xy;
    float scale;
} Coordinate;

class ILayer {
public:
    virtual ~ILayer() = default;
    virtual void Render(SDL_Renderer* renderer, Coordinate& windowCoord, Vector2d windowWH) = 0;
    virtual void HandleEvent(SDL_Event& event, Coordinate& windowCoord, Vector2d windowWH) = 0;
};

class Roamer {
private:
    void coordCorrection();
protected:
    std::vector<ILayer*> m_layers;
    Vector2d m_windowSize;
public:
    float m_maxscale = 100.f;
    Coordinate m_windowCoord;
    Roamer(Coordinate windowCoord, Vector2d windowSize);
    ~Roamer() = default;
    void PushLayer(ILayer* layer);
    Vector2d GetWindowCoordWH();
    void HandleEvent(SDL_Event& event);
    void Render(SDL_Renderer* renderer);
}; // class Roamer
} // namespace ImageRoamer