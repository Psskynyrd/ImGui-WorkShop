#pragma once

#include <cstdint>
#include <string_view>
#include <string>
#include <cstring>
#include <filesystem>

#include <imgui.h>

namespace fs = std::filesystem;

class WindowClass
{
public:
    static constexpr auto bufferSize = std::size_t{1024};
    static constexpr auto popupFlags =
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
    static constexpr auto popupSize = ImVec2(300.0F, 100.0F);
    static constexpr auto popupBtnSize = ImVec2(120.0F, 0.0F);
    static constexpr auto popupPos =
        ImVec2(1580.0F / 2.0F - popupSize.x / 2.0F - popupSize.x / 2.0F,
               820.0F / 2.0F - popupSize.y / 2.0F - popupSize.y / 2.0F);

    

public:
    WindowClass() : currentFilename({})
    {
        std::memset(textBuffer, 0, bufferSize);
    }

    void Draw(std::string_view label);

private:
    void DrawMenu();
    void DrawSavePopup();
    void DrawLoadPopup();
    void DrawContent();
    void DrawInfo();

    void SaveToFile(std::string_view filename);
    void LoadFromFile(std::string_view filename);
    std::string GetFileExtension(std::string_view filename);

private:
    char textBuffer[bufferSize];
    std::string currentFilename;
};

void render(WindowClass &window_obj);
