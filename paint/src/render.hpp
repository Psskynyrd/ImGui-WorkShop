#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include <imgui.h>

class WindowClass
{
public:
    using PointData = std::tuple<ImVec2, ImColor, float>;
    static constexpr auto popupFlags =
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
    static constexpr auto popupSize = ImVec2(300.0F, 100.0F);
    static constexpr auto popupBtnSize = ImVec2(120.0F, 0.0F);
    static constexpr auto popupPos =
        ImVec2(1580.0F / 2.0F - popupSize.x / 2.0F - popupSize.x / 2.0F,
               820.0F / 2.0F - popupSize.y / 2.0F - popupSize.y / 2.0F);

public:
    WindowClass()
        : points({}), canvasPos({}), currentDrawColor(ImColor(255, 255, 255)),
          pointDrawSize(2.0F), filenameBuffer("test.bin")
    {
    }

    void Draw(std::string_view label);

private:
    void DrawMenu();
    void DrawCanvas();
    void DrawColorButtons();
    void DrawSizeSettings();

    void DrawSavePopup();
    void DrawLoadPopup();

    void SaveToImageFile(std::string_view filename);
    void LoadFromImageFile(std::string_view filename);
    void ClearCanvas();


private:
    std::uint32_t numRows = 800;
    std::uint32_t numCols = 600;
    std::uint32_t numChannels = 3;
    ImVec2 canvasSize =
        ImVec2(static_cast<float>(numRows), static_cast<float>(numCols));

    std::vector<PointData> points;
    ImVec2 canvasPos;

    ImColor currentDrawColor;
    float pointDrawSize;

    char filenameBuffer[256];

    // Add this member to your class definition
    ImVec2 prevMousePos = ImVec2(-1, -1); // Initialize with an invalid position
};

void render(WindowClass &window_obj);
