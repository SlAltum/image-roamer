// Copyright (c) [2025] [Initial_Equationor]
// [AreaSegmentation] is licensed under Mulan PubL v2.
// You can use this software according to the terms and conditions of the Mulan PubL v2.
// You may obtain a copy of Mulan PubL v2 at:
// http://license.coscl.org.cn/MulanPubL-2.0
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
// EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
// MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See the Mulan PubL v2 for more details.
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include <stdexcept>
#include <SDL.h>
#include "utils.hpp"
#include "image_roamer.hpp"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
//#define WINDOW_WIDTH 680
//#define WINDOW_HEIGHT 682

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

class ImgLayer : public ImageRoamer::ILayer {
protected:
    SDL_Texture* m_texture = nullptr;
    Sint32 m_width;
    Sint32 m_height;
    void RenderMiniMap(SDL_Renderer* renderer, ImageRoamer::Coordinate& windowCoord, ImageRoamer::Vector2d windowWH);
public:
    ImgLayer(SDL_Renderer* renderer, const char* path);
    ~ImgLayer() override;
    void Render(SDL_Renderer* renderer, ImageRoamer::Coordinate& windowCoord, ImageRoamer::Vector2d windowWH) override;
    void HandleEvent(SDL_Event& event, ImageRoamer::Coordinate& windowCoord, ImageRoamer::Vector2d windowWH) override;
};

ImgLayer::ImgLayer(SDL_Renderer* renderer, const char* path) {
    SDL_Surface *surface;
    LoadTextureFromFile(path, surface, m_width, m_height);
    if (!surface) {
        throw std::runtime_error("fail to load image");
    }
    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!m_texture) {
        throw std::runtime_error("fail to create texture");
    }
    SDL_FreeSurface(surface);
}

ImgLayer::~ImgLayer() {
    SDL_DestroyTexture(m_texture);
}

void ImgLayer::Render(SDL_Renderer* renderer, ImageRoamer::Coordinate& windowCoord, ImageRoamer::Vector2d windowWH) {
    float x = windowCoord.xy.x;
    float y = windowCoord.xy.y;
    float w = windowWH.x;
    float h = windowWH.y;
    SDL_Rect src; src.x = x; src.y = y; src.w = w; src.h = h;
    SDL_RenderCopy(renderer, m_texture, &src, nullptr);
    RenderMiniMap(renderer, windowCoord, windowWH);
} // void ImgLayer::Render

void ImgLayer::RenderMiniMap(SDL_Renderer* renderer, ImageRoamer::Coordinate& windowCoord, ImageRoamer::Vector2d windowWH) {
    SDL_Rect dst; dst.x = 0; dst.y = WINDOW_HEIGHT - 682 / 2; dst.w = 660 / 2; dst.h = 682 / 2;
    SDL_RenderCopy(renderer, m_texture, nullptr, &dst);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0xff, 0xff);
    SDL_RenderDrawRect(renderer, &dst);
    SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xff);
    dst.x += windowCoord.xy.x / 2;
    dst.y += windowCoord.xy.y / 2;
    dst.w = windowWH.x / 2;
    dst.h = windowWH.y / 2;
    SDL_RenderDrawRect(renderer, &dst);
} // void ImgLayer::RenderMiniMap

void ImgLayer::HandleEvent(SDL_Event& event, ImageRoamer::Coordinate& windowCoord, ImageRoamer::Vector2d windowWH) {

} // void ImgLayer::HandleEvent

// Main code
int main(int, char**)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("image roamer example",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return -1;
    }
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
    // Load Fonts
    ImFont* font = io.Fonts->AddFontFromFileTTF("assets/AlibabaPuHuiTi-3-55-Regular.ttf", 22.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.34f, 0.8f, 1.f, 1.0f);

//    Roamer(Coordinate windowCoord, Vector2d windowSize);
    auto roamer = new ImageRoamer::Roamer({
        { 660, 682 },
        { 0, 0 }, 1.f
    }, { WINDOW_WIDTH, WINDOW_HEIGHT });
    auto layer = new ImgLayer(renderer, "assets/example.jpg");
    roamer->PushLayer(layer);

    // Main loop
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                done = true;
            }
            roamer->HandleEvent(event);
        }
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }
        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_Always);
            ImGui::Begin("Coordinate Parameters");
            ImGui::BeginGroup();
            ImGui::SliderFloat("x", &roamer->m_windowCoord.xy.x, 0, roamer->m_windowCoord.wh.x);
            ImGui::SliderFloat("y", &roamer->m_windowCoord.xy.y, 0, roamer->m_windowCoord.wh.y);
            ImGui::SliderFloat("scale", &roamer->m_windowCoord.scale, 1.f, roamer->m_maxscale);
            ImGui::EndGroup();
            ImGui::End();
        }
        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        roamer->Render(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
