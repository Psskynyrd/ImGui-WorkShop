#pragma once

#include <array>
#include <chrono>
#include <compare>
#include <cstdint>
#include <map>
#include <string_view>
#include <vector>

class WindowClass
{
public:
    static constexpr auto meetingWindowFlags =
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
    static constexpr auto popupSize = ImVec2(300.0F, 100.0F);
    static constexpr auto popupBtnSize = ImVec2(120.0F, 0.0F);
    static constexpr auto popupPos =
        ImVec2(1580.0F / 2.0F - popupSize.x / 2.0F - popupSize.x / 2.0F,
               820.0F / 2.0F - popupSize.y / 2.0F - popupSize.y / 2.0F);
    
    static constexpr auto monthNames =
        std::array<std::string_view, 12U>{"January",
                                          "February",
                                          "March",
                                          "April",
                                          "May",
                                          "June",
                                          "July",
                                          "August",
                                          "September",
                                          "October",
                                          "November",
                                          "December"};
    static constexpr auto minYear = 2000U;
    static constexpr auto maxYear = 2038U;

    struct Meeting
    {
        std::string name;

        void Serialize(std::ofstream &out);
        static Meeting Deserialize(std::ifstream &in);

        const bool operator==(const Meeting &rhs) const
        {
            return name == rhs.name;
        }
    };


public:
    WindowClass() : meetings({})
    {
    }

    void Draw(std::string_view label);

private:
    void DrawDateCombo();
    void DrawCalendar();
    void DrawAddMeetingWindow();
    void DrawMeetingList();

    void LoadMeetingFromFile(std::string_view fileName);
    void SaveMeetingToFile(std::string_view fileName);
    void UpdateSelectedDateVariables();

private:
    int selectedDay = 1;
    int selectedMonth = 1;
    int selectedYear = 2024;
    std::chrono::year_month_day selectedDate;

    bool addMeetingWindowOpen = false;

    float calendarFontSize = 2.0F;

    std::map<std::chrono::year_month_day, std::vector<Meeting>> meetings;
};

void render(WindowClass &window_obj);
