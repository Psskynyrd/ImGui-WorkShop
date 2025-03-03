#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <filesystem>

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
    DrawContent();
    DrawInfo();

    ImGui::End();
}


void WindowClass::DrawMenu()
{
    const auto ctrl_pressed = ImGui::GetIO().KeyCtrl;
    const auto s_presssed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S));
    const auto l_presssed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_L));

    if (ImGui::Button("Save") || (ctrl_pressed && s_presssed))
    {
        ImGui::OpenPopup("Save File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Load") || (ctrl_pressed && l_presssed))
    {
        ImGui::OpenPopup("Load File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
        std::memset(textBuffer, 0, bufferSize);
    }

    DrawSavePopup();
    DrawLoadPopup();
}

void WindowClass::DrawSavePopup()
{
    static char saveFilenameBuffer[256] = "text.txt";
    const auto esc_presssed =
        ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));

    ImGui::SetNextWindowSize(popupSize);
    ImGui::SetNextWindowPos(popupPos);

    if (ImGui::BeginPopupModal("Save File", nullptr, popupFlags))
    {
        ImGui::InputText("Filename",
                         saveFilenameBuffer,
                         sizeof(saveFilenameBuffer));

        if (ImGui::Button("Save", popupBtnSize))
        {
            SaveToFile(saveFilenameBuffer);
            currentFilename = saveFilenameBuffer;
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
    static char saveFilenameBuffer[256] = "text.txt";
    const auto esc_presssed =
        ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));

    ImGui::SetNextWindowSize(popupSize);
    ImGui::SetNextWindowPos(popupPos);

    if (ImGui::BeginPopupModal("Load File", nullptr, popupFlags))
    {
        ImGui::InputText("Filename",
                         saveFilenameBuffer,
                         sizeof(saveFilenameBuffer));

        if (ImGui::Button("Load", popupBtnSize))
        {
            LoadFromFile(saveFilenameBuffer);
            currentFilename = saveFilenameBuffer;
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

void WindowClass::DrawContent()
{
    static constexpr auto inputTextSize = ImVec2(1250.0F, 660.0F);
    static constexpr auto lineNumberSize = ImVec2(50.0F, inputTextSize.y);
    static constexpr auto inputTextFlags =
        ImGuiInputTextFlags_AllowTabInput |
        ImGuiInputTextFlags_NoHorizontalScroll;

    ImGui::BeginChild("LineNumbers",
                      lineNumberSize,
                      false,
                      ImGuiWindowFlags_NoScrollbar |
                          ImGuiWindowFlags_NoScrollWithMouse);
    const auto line_count =
        std::count(textBuffer, textBuffer + bufferSize, '\n') + 2;
    // ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
    //                     ImVec2(0, ImGui::GetTextLineHeightWithSpacing()));
    for (auto i = 1; i < line_count; ++i)
    {
        ImGui::Text("%d", i);
    }
    // ImGui::PopStyleVar();
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("TextEditor", inputTextSize, false);
    ImGui::InputTextMultiline("###inputField",
                              textBuffer,
                              bufferSize,
                              inputTextSize,
                              inputTextFlags);

    float scrollY = ImGui::GetScrollY();
    ImGui::SetScrollY(scrollY);

    ImGui::EndChild();
}
void WindowClass::DrawInfo()
{
    if (currentFilename.size() == 0)
    {
        ImGui::Text("No file opened!");
        return;
    }

    const auto file_extension = GetFileExtension(currentFilename);
    ImGui::Text("Opened file %s | File extension : %s",
                currentFilename.data(),
                file_extension.data());
}

void WindowClass::SaveToFile(std::string_view filename)
{
    auto out = std::ofstream{filename.data()};

    if (out.is_open())
    {
        out << textBuffer;
        out.close();
    }
}

void WindowClass::LoadFromFile(std::string_view filename)
{
    auto in = std::ofstream{filename.data()};

    if (in.is_open())
    {
        auto buffer = std::stringstream{};
        buffer << in.rdbuf();
        std::memcpy(textBuffer, buffer.str().data(), bufferSize);
        in.close();
    }
}

std::string WindowClass::GetFileExtension(std::string_view filename)
{
    const auto file_path = fs::path{filename};

    return file_path.extension().string();
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
