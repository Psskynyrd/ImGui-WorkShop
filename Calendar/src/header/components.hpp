#ifndef CALENDAR_HPP
#define CALENDAR_HPP

#include <chrono>
#include <map>
#include <string_view>

class WindowClass {
public:
    void LoadMeetingFromFile(std::string_view fileName);
    void SaveMeetingToFile(std::string_view fileName);
    void UpdateSelectedDateVariables();
};

#endif // CALENDAR_HPP
