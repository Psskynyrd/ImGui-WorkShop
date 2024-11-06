#include <iostream>

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

    const auto currsor_pos = ImGui::GetCursorScreenPos();
    center = ImVec2(currsor_pos.x + circleRadius, currsor_pos.y + circleRadius);

    DrawCircle(circleRadius);

    GetTime();
    const auto [hour_theta, minute_theta, second_theta] = GetTheta();

    DrawClockHand(circleRadius * hrsClockHandLength,
                  hour_theta,
                  ImColor(1.0F, 0.0F, 0.0F, 1.0F));
    DrawClockHand(circleRadius * minsClockHandLength,
                  minute_theta,
                  ImColor(0.0F, 1.0F, 0.0F, 1.0F));
    DrawClockHand(circleRadius * secsClockHandLength,
                  second_theta,
                  ImColor(0.0F, 0.0F, 1.0F, 1.0F));
    
    DrawAllHourStrokes();
    DrawAllMinuteStrokes();

    DrawCircle(innerRadius);

    DrawDigitalClock();

    ImGui::End();
}

void WindowClass::DrawCircle(const float radius)
{
}
void WindowClass::DrawClockHand(const float radius,
                                const float theta,
                                const ImColor color)
{
}
void WindowClass::DrawAllHourStrokes()
{
}
void WindowClass::DrawAllMinuteStrokes()
{
}
void WindowClass::DrawDigitalClock()
{
}

void WindowClass::GetTime()
{
}
std::tuple<float, float, float> WindowClass::GetTheta()
{
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
