// Copyright (c) [2025] [Initial_Equationor]
// [AreaSegmentation] is licensed under Mulan PubL v2.
// You can use this software according to the terms and conditions of the Mulan PubL v2.
// You may obtain a copy of Mulan PubL v2 at:
// http://license.coscl.org.cn/MulanPubL-2.0
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
// EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
// MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See the Mulan PubL v2 for more details.
#include "image_roamer.hpp"

namespace ImageRoamer {
Roamer::Roamer(Coordinate windowCoord, Vector2d windowSize)
    : m_windowCoord(windowCoord),
      m_windowSize(windowSize) {
} // Roamer::Roamer

void Roamer::PushLayer(ILayer* layer) {
    m_layers.push_back(layer);
} // void Roamer::PushLayer

Vector2d Roamer::GetWindowCoordWH() {
    Vector2d wh; wh.x = m_windowCoord.wh.x; wh.y = m_windowCoord.wh.y;
    if (m_windowSize.x / m_windowSize.y < wh.x / wh.y) {
        wh.x = wh.y * m_windowSize.x / m_windowSize.y;
    } else if (m_windowSize.y / m_windowSize.x < wh.y / wh.x) {
        wh.y = wh.x * m_windowSize.y / m_windowSize.x;
    }
    wh.x /= m_windowCoord.scale;
    wh.y /= m_windowCoord.scale;
    return wh;
} // Vector2d Roamer::GetWindowCoordWH

void Roamer::coordCorrection() {
    Vector2d wh = GetWindowCoordWH();
    if (m_windowCoord.xy.x < 0) {
        m_windowCoord.xy.x = 0;
    } else if (m_windowCoord.xy.x > m_windowCoord.wh.x - wh.x) {
        m_windowCoord.xy.x = m_windowCoord.wh.x - wh.x;
    }
    if (m_windowCoord.xy.y < 0) {
        m_windowCoord.xy.y = 0;
    } else if (m_windowCoord.xy.y > m_windowCoord.wh.y - wh.y) {
        m_windowCoord.xy.y = m_windowCoord.wh.y - wh.y;
    }
} // void Roamer::coordCorrection

void Roamer::HandleEvent(SDL_Event& event) {
    static bool mousedown = false;
    switch (event.type) {
    case SDL_MOUSEBUTTONUP:
        mousedown = false;
        break;
    case SDL_MOUSEBUTTONDOWN:
        if(event.button.button == 1){
            mousedown = true;
        }
        break;
    case SDL_MOUSEWHEEL:
        {
            float offsetMult;
            if (event.wheel.y > 0) {
                m_windowCoord.scale *= 1.1;
                if (m_windowCoord.scale > m_maxscale) {
                    m_windowCoord.scale = m_maxscale;
                }
                offsetMult = 0.1;
            } else {
                m_windowCoord.scale /= 1.1;
                if (m_windowCoord.scale < 1.f) {
                    m_windowCoord.scale = 1.f;
                }
                offsetMult = - (1 - 1 / 1.1);
            }
            Vector2d wh = GetWindowCoordWH();
            int _x, _y;
            SDL_GetMouseState(&_x, &_y);
            float offsetX = offsetMult * wh.x * _x / m_windowSize.x;
            float offsetY = offsetMult * wh.y * _y / m_windowSize.y;
            m_windowCoord.xy.x += offsetX;
            m_windowCoord.xy.y += offsetY;
        }
        coordCorrection();
        break;
    case SDL_MOUSEMOTION:
        if(mousedown){
            Vector2d wh = GetWindowCoordWH();
            float offsetX = - wh.x * event.motion.xrel / m_windowSize.x;
            float offsetY = - wh.y * event.motion.yrel / m_windowSize.y;
            m_windowCoord.xy.x += offsetX;
            m_windowCoord.xy.y += offsetY;
            coordCorrection();
        }
        break;
    case SDL_WINDOWEVENT:
        switch(event.window.event){
            case SDL_WINDOWEVENT_RESIZED:
                m_windowSize.x = event.window.data1;
                m_windowSize.y = event.window.data2;
                coordCorrection();
                break;
        }
        break;
    }
    for (auto& layer : m_layers) {
        layer->HandleEvent(event, m_windowCoord, GetWindowCoordWH());
    }
} // void Roamer::HandleEvent

void Roamer::Render(SDL_Renderer* renderer) {
    for (auto& layer : m_layers) {
        layer->Render(renderer, m_windowCoord, GetWindowCoordWH());
    }
} // void Roamer::Render
} // namespace ImageRoamer
