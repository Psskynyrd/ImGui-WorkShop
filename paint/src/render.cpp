#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin(label.data(), nullptr, window_flags);

    DrawMenu();
    DrawCanvas();

    ImGui::End();
}

void WindowClass::DrawMenu()
{
    const auto ctrl_pressed = ImGui::GetIO().KeyCtrl;
    const auto s_presssed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S));
    const auto l_presssed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_L));

    if (ImGui::Button("Save") || (ctrl_pressed && s_presssed))
    {
        ImGui::OpenPopup("Save Image");
    }

    ImGui::SameLine();

    if (ImGui::Button("Load") || (ctrl_pressed && l_presssed))
    {
        ImGui::OpenPopup("Load Image");
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
        ClearCanvas();
    }

    DrawColorButtons();
    DrawSizeSettings();

    DrawSavePopup();
    DrawLoadPopup();
}

void WindowClass::DrawSavePopup()
{
    const auto esc_presssed =
        ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));

    ImGui::SetNextWindowSize(popupSize);
    ImGui::SetNextWindowPos(popupPos);

    if (ImGui::BeginPopupModal("Save Image", nullptr, popupFlags))
    {
        ImGui::InputText("Filename", filenameBuffer, sizeof(filenameBuffer));

        if (ImGui::Button("Save", popupBtnSize))
        {
            SaveToImageFile(filenameBuffer);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", popupBtnSize) || esc_presssed)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void WindowClass::DrawLoadPopup()
{
    const auto esc_presssed =
        ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));

    ImGui::SetNextWindowSize(popupSize);
    ImGui::SetNextWindowPos(popupPos);

    if (ImGui::BeginPopupModal("Load Image", nullptr, popupFlags))
    {
        ImGui::InputText("Filename", filenameBuffer, sizeof(filenameBuffer));

        if (ImGui::Button("Load", popupBtnSize))
        {
            LoadFromImageFile(filenameBuffer);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", popupBtnSize) || esc_presssed)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}


void WindowClass::DrawCanvas()
{
    canvasPos = ImGui::GetCursorPos();
    const auto border_thickness = 1.5F;
    const auto button_size = ImVec2(canvasSize.x - 2.0F * border_thickness,
                                    canvasSize.y - 2.0F * border_thickness);
    ImGui::InvisibleButton("##canvas", button_size);

    const auto mouse_pos = ImGui::GetMousePos();
    const auto is_mouse_hovering = ImGui::IsItemHovered();

    if (is_mouse_hovering && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        const auto point = ImVec2{mouse_pos.x - canvasPos.x - border_thickness,
                                  mouse_pos.y - canvasPos.y - border_thickness};
        points.push_back(
            std::make_tuple(point, currentDrawColor, pointDrawSize));
    }

    auto *draw_list = ImGui::GetWindowDrawList();

    const auto background_min = ImVec2{canvasPos.x, canvasPos.y};
    const auto background_max =
        ImVec2{canvasPos.x + button_size.x, canvasPos.y + button_size.y};
    draw_list->AddRectFilled(background_min,
                             background_max,
                             IM_COL32(250, 255, 255, 255));

    for (const auto &[point, color, size] : points)
    {
        const auto pos = ImVec2{canvasPos.x + point.x + border_thickness,
                                canvasPos.y + point.y + border_thickness};
        draw_list->AddCircleFilled(pos, size, color);
    }

    const auto border_min = canvasPos;
    const auto border_max =
        ImVec2{canvasPos.x + button_size.x - border_thickness,
               canvasPos.y + button_size.y - border_thickness};
    draw_list->AddRect(border_min,
                       border_max,
                       IM_COL32(255, 0, 0, 255),
                       0.0F,
                       ImDrawFlags_RoundCornersAll,
                       border_thickness);
}
void WindowClass::DrawColorButtons()
{
    const auto selected_red = currentDrawColor == ImColor(255, 0, 0);
    const auto selected_green = currentDrawColor == ImColor(0, 255, 0);
    const auto selected_blue = currentDrawColor == ImColor(0, 0, 255);
    const auto selected_white = currentDrawColor == ImColor(255, 255, 255);
    const auto none_preset_color =
        !(selected_red || selected_green || selected_blue || selected_white);

    constexpr static auto orange = ImVec4(1.0F, 0.5F, 0.0F, 1.0F);

    // Red
    if (selected_red)
        ImGui::PushStyleColor(ImGuiCol_Button, orange);
    if (ImGui::Button("Red"))
        currentDrawColor = ImColor(255, 0, 0);
    if (selected_red)
        ImGui::PopStyleColor();

    ImGui::SameLine();

    // Green
    if (selected_green)
        ImGui::PushStyleColor(ImGuiCol_Button, orange);
    if (ImGui::Button("Green"))
        currentDrawColor = ImColor(0, 255, 0);
    if (selected_green)
        ImGui::PopStyleColor();

    ImGui::SameLine();

    // Blue
    if (selected_blue)
        ImGui::PushStyleColor(ImGuiCol_Button, orange);
    if (ImGui::Button("Blue"))
        currentDrawColor = ImColor(0, 0, 255);
    if (selected_blue)
        ImGui::PopStyleColor();

    ImGui::SameLine();

    // White
    if (selected_white)
        ImGui::PushStyleColor(ImGuiCol_Button, orange);
    if (ImGui::Button("White"))
        currentDrawColor = ImColor(255, 255, 255);
    if (selected_white)
        ImGui::PopStyleColor();

    ImGui::SameLine();

    // none preset color
    if (none_preset_color)
        ImGui::PushStyleColor(ImGuiCol_Button, orange);
    if (ImGui::Button("Choose"))
        ImGui::OpenPopup("Color Picker");
    if (ImGui::BeginPopup("Color Picker"))
    {
        ImGui::ColorPicker3("##colorPicker",
                            reinterpret_cast<float *>(&currentDrawColor));
        ImGui::EndPopup();
    }
    if (none_preset_color)
        ImGui::PopStyleColor();
}

void WindowClass::DrawSizeSettings()
{
    ImGui::Text("Draw Size");
    ImGui::SameLine();
    ImGui::PushItemWidth(canvasSize.x - ImGui::GetCursorPosX());
    ImGui::SliderFloat("##drawSize", &pointDrawSize, 1.0F, 10.0F);
    ImGui::PopItemWidth();
}

void WindowClass::SaveToImageFile(std::string_view filename)
{
}
void WindowClass::LoadFromImageFile(std::string_view filename)
{
}
void WindowClass::ClearCanvas()
{
    points.clear();
}


// void WindowClass::DrawContent()
// {
//     static constexpr auto inputTextSize = ImVec2(1250.0F, 660.0F);
//     static constexpr auto lineNumberSize = ImVec2(50.0F, inputTextSize.y);
//     static constexpr auto inputTextFlags =
//         ImGuiInputTextFlags_AllowTabInput |
//         ImGuiInputTextFlags_NoHorizontalScroll;

//     ImGui::BeginChild("LineNumbers",
//                       lineNumberSize,
//                       false,
//                       ImGuiWindowFlags_NoScrollbar |
//                           ImGuiWindowFlags_NoScrollWithMouse);
//     const auto line_count =
//         std::count(textBuffer, textBuffer + bufferSize, '\n') + 2;
//     // ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
//     //                     ImVec2(0, ImGui::GetTextLineHeightWithSpacing()));
//     for (auto i = 1; i < line_count; ++i)
//     {
//         ImGui::Text("%d", i);
//     }
//     // ImGui::PopStyleVar();
//     ImGui::EndChild();

//     ImGui::SameLine();

//     ImGui::BeginChild("TextEditor", inputTextSize, false);
//     ImGui::InputTextMultiline("###inputField",
//                               textBuffer,
//                               bufferSize,
//                               inputTextSize,
//                               inputTextFlags);

//     float scrollY = ImGui::GetScrollY();
//     ImGui::SetScrollY(scrollY);

//     ImGui::EndChild();
// }
// void WindowClass::DrawInfo()
// {
//     if (currentFilename.size() == 0)
//     {
//         ImGui::Text("No file opened!");
//         return;
//     }

//     const auto file_extension = GetFileExtension(currentFilename);
//     ImGui::Text("Opened file %s | File extension : %s",
//                 currentFilename.data(),
//                 file_extension.data());
// }

// void WindowClass::SaveToFile(std::string_view filename)
// {
//     auto out = std::ofstream{filename.data()};

//     if (out.is_open())
//     {
//         out << textBuffer;
//         out.close();
//     }
// }

// void WindowClass::LoadFromFile(std::string_view filename)
// {
//     auto in = std::ofstream{filename.data()};

//     if (in.is_open())
//     {
//         auto buffer = std::stringstream{};
//         buffer << in.rdbuf();
//         std::memcpy(textBuffer, buffer.str().data(), bufferSize);
//         in.close();
//     }
// }

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
